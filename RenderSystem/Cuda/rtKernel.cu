/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include <cuda_fp16.h>
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <device_launch_parameters.h>
#include <cstdint>
#include <cstdio>
#include "RenderSystem/Cuda/rtCuMath.inl"
#include "RenderSystem/Cuda/rtCudaUtils.h"
#include "RenderSystem/Data/rtCameraTypes.h"
#include "RenderSystem/Data/rtLightTypes.h"
#include "RenderSystem/Data/rtMaterialTypes.h"
#include "RenderSystem/Math/rtColor.h"
#include "RenderSystem/rtRenderSystem.h"

RT_DEVICE_API bool rtCudaRayIntersectsObject(rtObjectType* obj, rtCudaRay* ray, const rtVector2& lim)
{
    switch (obj->type)
    {
    case RT_AO_SHAPE_CUBE:
    case RT_AO_BVO:
        return rtCuBoxTest(obj->bounds.bMin, obj->bounds.bMax, *ray, lim);
    case RT_AO_SHAPE_SPHERE:
    {
        rtSphereVolume* sphere = (rtSphereVolume*)obj->bounds.data;
        if (sphere)
            return rtCuSphereTest(sphere->center, sphere->radius, *ray, lim);
        return false;
    }
    default:
        return false;
    }
}

RT_DEVICE_API bool rtCudaRayIntersectsObject(rtCudaHitResult* nearest, rtObjectType* obj, rtCudaRay* ray, const rtVector2& lim)
{
    switch (obj->type)
    {
    case RT_AO_SHAPE_CUBE:
    case RT_AO_BVO:
        return rtCuBoxTest(nearest, obj->bounds.bMin, obj->bounds.bMax, *ray, lim);
    case RT_AO_SHAPE_SPHERE:
    {
        rtSphereVolume* sphere = (rtSphereVolume*)obj->bounds.data;
        if (sphere)
            return rtCuSphereTest(nearest, sphere->center, sphere->radius, *ray, lim);
        return false;
    }
    default:
        return false;
    }
}

RT_DEVICE_API bool rtCudaTestScene(const rtSceneType* sc, rtCudaHitResult* nearest, rtCudaRay* ray, bool first = false)
{
    //SK_ASSERT(sc && sc->camera);

    // copy the limits..
    rtVector2 lim = sc->camera->limits;

    nearest->object = nullptr;

    for (uint32_t i = 0; i < sc->objects.size; ++i)
    {
        rtObjectType* obj = (rtObjectType*)sc->objects.data[i];

        if (rtCudaRayIntersectsObject(nearest, obj, ray, lim))
        {
            if (first)
            {
                nearest->object = obj;
                return true;
            }

            if (nearest->distance < lim.y)
            {
                nearest->object = obj;
                lim.y           = nearest->distance;
            }
        }
    }
    return nearest->object != nullptr;
}

RT_DEVICE_API bool rtCudaTestScene(const rtSceneType* sc, rtCudaRay* ray, const rtVector2 lim)
{
    for (uint32_t i = 0; i < sc->objects.size; ++i)
    {
        rtObjectType* obj = (rtObjectType*)sc->objects.data[i];

        if (rtCudaRayIntersectsObject(obj, ray, lim))
            return true;
    }
    return false;
}

RT_DEVICE_API void rtCuGetRayDirection(rtCudaRay*       dest,
                                       const rtScalar&  x,
                                       const rtScalar&  y,
                                       const rtVector4& rotation,
                                       const rtVector4& offset)
{
    rtVector3 d;
    d.x = offset.w * (x + RT_EPSILON) * offset.y;
    d.y = offset.w * (y + RT_EPSILON) * offset.y;
    d.z = -1;
    rtCuMulQuat(dest->direction, d, rotation);
}

RT_DEVICE_API rtScalar rtCuGetIntensity(rtLightType* light, const rtVector3& point)
{
    const skScalar x2 = rtCuFloat3DistSqu(light->location, point);
    if (light->mode == 0)
    {
        // https://www.desmos.com/calculator/l2ipt7pc9k
        // this is meant to not have to modify x,
        // ie x = sqrt(x), ... then x*x,
        // because it needs to be done here per pixel...
        const skScalar x2R2 = 16 * x2 * light->elevation;
        return light->rr / (light->rre4 + x2R2);
    }
    return (light->elevation * light->elevation * light->energy) / (x2 + 10e-3f);

}

RT_DEVICE_API void rtCuTraceColorAndLight(rtVector3&             pixel,
                                          const rtSceneType*     sc,
                                          const rtCudaHitResult* nearest,
                                          const rtCudaRay&       ray,
                                          const rtScalar&        v)
{
    if (nearest && nearest->object)
    {
        rtMaterialType* ma = nearest->object->material;

        if (ma->flags != RT_MA_SHADELESS)
        {
            // apply lights
            if (sc->lights.size <= 0)
                return;

            rtVector3 world{};
            rtCuMix3(&world, &sc->horizon, &sc->zenith, 0.5f);

            rtScalar kd = 0;
            rtScalar nd = 0;
            rtScalar ks = 0;
            rtScalar sh = 1;
            rtScalar ia = 0;

            for (uint32_t l = 0; l < sc->lights.size; ++l)
            {
                rtLightType* li = sc->lights.data[l];

                rtVector3      lv = rtCuFloat3Norm(li->location, nearest->point);
                const rtScalar ln = rtCuFloat3Dot(lv, nearest->normal);

                const rtScalar i = rtCuGetIntensity(li, nearest->point);
                ia += ma->ambient;

                if (ln > 10e-4f)
                {
                    kd += ma->diffuse * ln * i;

                    if (ma->flags & RT_MA_SPECULAR && ma->specular > 0)
                    {
                        const rtVector3 eye = rtCuFloat3Norm(ray.origin, nearest->point);
                        const rtVector3 rfl = rtCuReflectFloat3(lv, nearest->normal);

                        const skScalar d = rtCuFloat3Dot(rfl, eye);
                        if (d > 10e-3f)
                            ks += ma->specular * i * powf(d, ma->hardness);
                    }

                    if (ma->flags & RT_MA_SHADOW)
                    {
                        rtCudaRay       r{nearest->point, lv};
                        const rtVector2 lim = {10e-3f, 1000};

                        if (rtCudaTestScene(sc, &r, lim))
                        {
                            const rtScalar fac = .5f * (1.f + i);
                            if (fac < 1.f)
                                sh *= fac;
                        }
                    }
                }
                else
                    nd += ma->diffuse * -ln * i;
            }

            ia /= rtScalar(sc->lights.size);

            world   = rtCuMulFloat3(world, ia);
            pixel.x = ma->color.x;
            pixel.y = ma->color.y;
            pixel.z = ma->color.z;
            pixel   = rtCuMulFloat3v(pixel, world);
            pixel   = rtCuMulFloat3(pixel, sh);
            pixel   = rtCuMulFloat3(pixel, kd);
            pixel   = rtCuAddFloat3(pixel, ks);

            rtCuMix(&pixel, &world, nd * 0.125f);

            rtCuSaturate(&pixel);
        }
        else
        {
            pixel.x = ma->color.x;
            pixel.y = ma->color.y;
            pixel.z = ma->color.z;
        }
    }
    else
    {
        rtScalar t = 1 - v;
        rtCuSaturate(&t);
        const rtScalar it = 1 - t;

        pixel.x = sc->horizon.x * t + sc->zenith.x * it;
        pixel.y = sc->horizon.y * t + sc->zenith.y * it;
        pixel.z = sc->horizon.z * t + sc->zenith.z * it;
    }
}

RT_DEVICE_API void rtCuSetPixel(rtCudaTarget*    target,
                                const int32_t    x,
                                const int32_t    y,
                                const rtVector3& bg)
{
    const int32_t loc = x * 4 + y * target->pitch;
    if (loc > 0 && loc < target->length)
    {
        uint8_t* pix = &target->pixels[loc];

        pix[0] = (uint8_t)(bg.z * 255.0f);
        pix[1] = (uint8_t)(bg.y * 255.0f);
        pix[2] = (uint8_t)(bg.x * 255.0f);
        pix[3] = 255;
    }
}

RT_DEVICE_API void rtCpuRenderSample(rtVector3&          pixel,
                                     rtCudaHitResult&    nearest,
                                     const rtCameraType* ca,
                                     const rtSceneType*  sc,
                                     const rtScalar&     kX,
                                     const rtScalar      kY,
                                     const int           y)
{
    rtCudaRay ray{};
    ray.origin = ca->location;
    rtCuGetRayDirection(&ray, kX, kY, ca->rotation, ca->offset);

    if (rtCudaTestScene(sc, &nearest, &ray))
    {
        // process hits
        if (sc->flags & RM_COLOR_AND_LIGHT)
            rtCuTraceColorAndLight(pixel, sc, &nearest, ray, 0);

        if (sc->flags & RM_COMPUTED_NORMAL)
        {
            if (sc->flags == RM_COMPUTED_NORMAL)
            {
                pixel = {
                    0.5f + nearest.normal.x * 0.5f,
                    0.5f + nearest.normal.y * 0.5f,
                    0.5f + nearest.normal.z * 0.5f,
                };
            }
            else
            {
                pixel.x = 0.125f + pixel.x * 0.5f + nearest.normal.x * 0.25f;
                pixel.y = 0.125f + pixel.y * 0.5f + nearest.normal.y * 0.25f;
                pixel.z = 0.125f + pixel.z * 0.5f + nearest.normal.z * 0.25f;
            }
        }

        if (sc->flags & RM_DISTANCE)
        {
            const skScalar df = skScalar(2.f) / (nearest.distance + skScalar(10e-3));
            if (sc->flags == RM_DISTANCE)
                pixel = {df, df, df};
            else
            {
                pixel.x = (pixel.x + df) * 0.5f;
                pixel.y = (pixel.y + df) * 0.5f;
                pixel.z = (pixel.z + df) * 0.5f;
            }
        }
    }
    else
    {
        // process misses
        if (sc->flags & RM_COLOR_AND_LIGHT)
            rtCuTraceColorAndLight(pixel, sc, &nearest, ray, rtScalar(y) * ca->offset.y);
    }

    // limit and invert...
    rtCuSaturate(&pixel);

    if (sc->flags & RM_INVERT)
    {
        pixel.x = 1.f - pixel.x;
        pixel.y = 1.f - pixel.y;
        pixel.z = 1.f - pixel.z;
    }
}

RT_KERNEL_API void rtCuRenderKernel(rtSceneType*  sc,
                                    rtCudaTarget* target)
{
    const int32_t start  = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    const rtCameraType* ca = sc->camera;
    rtCudaHitResult     nearest{};

    for (int32_t i = start; i < target->dimensions; i += stride)
    {
        const int32_t x = i / target->height;
        const int32_t y = i % target->height;

        if (!(sc->flags & RM_AA))
        {
            const skScalar kX = skScalar(target->width - 2 * x);
            const skScalar kY = skScalar(target->height - 2 * y);

            rtVector3 curPixel{};

            rtCpuRenderSample(curPixel, nearest, ca, sc, kX, kY, y);
            rtCuSetPixel(target, x, target->height - 1 - y, curPixel);
        }
        else
        {
            const skScalar kX = skScalar(target->width - 2 * x);
            const skScalar kY = skScalar(target->height - 2 * y);

            rtVector3      P[5]{};
            const skScalar h = 1.1625f;

            rtCpuRenderSample(P[0], nearest, ca, sc, kX, kY, y);
            rtCpuRenderSample(P[1], nearest, ca, sc, kX - h, kY - h, y);
            rtCpuRenderSample(P[2], nearest, ca, sc, kX + h, kY + h, y);
            rtCpuRenderSample(P[3], nearest, ca, sc, kX + h, kY - h, y);
            rtCpuRenderSample(P[4], nearest, ca, sc, kX - h, kY + h, y);

            rtVector3 curPixel{};
            curPixel.x = (P[0].x + P[1].x + P[2].x + P[3].x + P[4].x) * 0.2f;
            curPixel.y = (P[0].y + P[1].y + P[2].y + P[3].y + P[4].y) * 0.2f;
            curPixel.z = (P[0].z + P[1].z + P[2].z + P[3].z + P[4].z) * 0.2f;

            rtCuSetPixel(target, x, target->height - 1 - y, curPixel);
        }
    }
}

RT_CU_API void rtCudaKernelMain(rtCudaFrameBuffer* fb, rtCudaTarget* buffer, rtSceneType* scene)
{
    cudaError_t status;
    rtCuRenderKernel<<<fb->threads, fb->blocks>>>(scene, buffer);

    if ((status = cudaDeviceSynchronize()) != cudaSuccess)
        printf("Sync failed %d\n", status);
}
