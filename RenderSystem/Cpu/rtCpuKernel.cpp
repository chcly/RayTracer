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
#include "RenderSystem/Cpu/rtCpuKernel.h"
#include "Math/skRectangle.h"
#include "RenderSystem/Cpu/rtCpuMath.h"
#include "RenderSystem/Cpu/rtCpuRenderSystem.h"
#include "RenderSystem/Data/rtMaterialTypes.h"
#include "RenderSystem/Math/rtColor.h"
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtLight.h"

bool rtCpuRayIntersectsObject(rtObjectType* obj, rtCpuRay* ray, const rtVector2& lim)
{
    switch (obj->type)
    {
    case RT_AO_SHAPE_MESH:
    case RT_AO_SHAPE_CUBE:
    case RT_AO_BVO:
        return rtCpuBoxTest(obj->bounds.bMin, obj->bounds.bMax, *ray, lim);
    case RT_AO_SHAPE_SPHERE:
    {
        rtSphereVolume* sphere = (rtSphereVolume*)obj->bounds.data;
        if (sphere)
            return rtCpuSphereTest(sphere->center, sphere->radius, *ray, lim);
        return false;
    }
    default:
        return false;
    }
}

bool rtCpuRayIntersectsObject(rtCpuHitResult* nearest, rtObjectType* obj, rtCpuRay* ray, const rtVector2& lim)
{
    switch (obj->type)
    {
    case RT_AO_SHAPE_MESH:
    case RT_AO_SHAPE_CUBE:
    case RT_AO_BVO:
        return rtCpuBoxTest(nearest, obj->bounds.bMin, obj->bounds.bMax, *ray, lim);
    case RT_AO_SHAPE_SPHERE:
    {
        rtSphereVolume* sphere = (rtSphereVolume*)obj->bounds.data;
        if (sphere)
            return rtCpuSphereTest(nearest, sphere->center, sphere->radius, *ray, lim);
        return false;
    }
    default:
        return false;
    }
}

bool rtCpuTestScene(const rtSceneType* sc, rtCpuHitResult* nearest, rtCpuRay* ray, bool first = false)
{
    SK_ASSERT(sc && sc->camera);

    // copy the limits..
    rtVector2 lim = sc->camera->limits;

    for (uint32_t i = 0; i < sc->objects.size; ++i)
    {
        rtObjectType* obj = (rtObjectType*)sc->objects.data[i];
        if (rtCpuRayIntersectsObject(nearest, obj, ray, lim))
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

bool rtCpuTestScene(const rtSceneType* sc, rtCpuRay* ray)
{
    const rtVector2& lim = sc->camera->limits;

    for (uint32_t i = 0; i < sc->objects.size; ++i)
    {
        rtObjectType* obj = (rtObjectType*)sc->objects.data[i];

        if (rtCpuRayIntersectsObject(obj, ray, lim))
            return true;
    }
    return false;
}

static void rtSetPixel(rtFrameBufferInfo& fb,
                       const SKuint32&    x,
                       const SKuint32&    y,
                       const rtColor&     c)
{
    const SKuint32 loc = x * 4 + y * fb.pitch;
    if (loc < fb.maxSize)
    {
        rtPixelRGBA* col = (rtPixelRGBA*)&fb.pixels[loc];
        c.toBytes(col->r, col->g, col->b);
    }
}

static skScalar rtCpuGetIntensity(const rtLightType* light,
                                  const rtVector3&   point)
{
    const skScalar x2 = rtCpuVec3DistSqu(light->location, point);
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

static void rtCpuTraceSilhouette(rtColor&              pixel,
                                 const rtVector4&      ori,
                                 const rtCpuHitResult& nearest,
                                 const rtCpuRay&       rayCenter,
                                 const rtScalar&       kx,
                                 const rtScalar&       ky,
                                 const rtVector4&      offset,
                                 const rtVector2&      limit)
{
    pixel.set(1);

    const skScalar h = 2 + 10e-3f;
    SK_ASSERT(nearest.object);

    rtCpuRay r[4];
    r[0].origin = rayCenter.origin;
    r[1].origin = rayCenter.origin;
    r[2].origin = rayCenter.origin;
    r[3].origin = rayCenter.origin;

    rtCpuComputeRayDirection(r[0].direction, ori, kx, ky, h, h, offset);
    rtCpuComputeRayDirection(r[1].direction, ori, kx, ky, -h, h, offset);
    rtCpuComputeRayDirection(r[2].direction, ori, kx, ky, h, -h, offset);
    rtCpuComputeRayDirection(r[3].direction, ori, kx, ky, -h, -h, offset);

    bool sum =
        rtCpuRayIntersectsObject(nearest.object, &r[0], limit);

    sum = sum && rtCpuRayIntersectsObject(nearest.object, &r[1], limit);
    sum = sum && rtCpuRayIntersectsObject(nearest.object, &r[2], limit);
    sum = sum && rtCpuRayIntersectsObject(nearest.object, &r[3], limit);
    if (!sum)
        pixel.zero();
}

static void rtCpuTraceColorAndLight(rtColor&              pixel,
                                    const rtSceneType*    sc,
                                    const rtCpuHitResult& nearest,
                                    const rtCpuRay&       ray,
                                    const skScalar&       v)
{
    if (nearest.object)
    {
        rtMaterialType& ma = *nearest.object->material;

        if (ma.flags != RT_MA_SHADELESS)
        {
            // apply lights
            const rtLightArray& lights = sc->lights;
            if (lights.size <= 0)
                return;

            rtColor world;
            world.mix(rtColor(sc->horizon),
                      rtColor(sc->zenith),
                      0.5f);

            rtColor material;
            material.setF(ma.color.x,
                          ma.color.y,
                          ma.color.z);
            skScalar kd = 0;
            skScalar nd = 0;
            skScalar ks = 0;
            skScalar sh = 1;
            skScalar ia = 0;

            for (SKuint32 l = 0; l < lights.size; ++l)
            {
                rtLightType*   lt = lights.data[l];
                rtVector3      lv = rtCpuVec3Norm(lt->location, nearest.point);
                const skScalar ln = rtCpuVec3Dot(lv, nearest.normal);

                const skScalar i = rtCpuGetIntensity(lt, nearest.point);
                ia += ma.ambient;

                if (ln > 10e-4f)
                {
                    kd += ma.diffuse * ln * i;

                    if (ma.flags & RT_MA_SPECULAR && ma.specular > 0)
                    {
                        rtVector3 eye = rtCpuVec3Norm(ray.origin, nearest.point);

                        const rtVector3 rfl = rtCpuReflect(lv, nearest.normal);

                        const skScalar d = rtCpuVec3Dot(rfl, eye);

                        if (d > 10e-3f)
                            ks += ma.specular * i * powf(d, ma.hardness);
                    }

                    if (ma.flags & RT_MA_SHADOW)
                    {
                        rtCpuRay r{nearest.point, lv};
                        if (rtCpuTestScene(sc, &r))
                        {
                            const rtScalar fac = .5f * (1.f + i);
                            if (fac < 1.f)
                                sh *= fac;
                        }
                    }
                }
                else
                    nd += ma.diffuse * -ln * i;
            }

            ia /= skScalar(lights.size);

            world.mul(ia);
            material.mul(world);
            material.mul(sh);
            material.mul(kd);
            material.add(ks);
            material.mix(world, material, nd * 0.125f);
            pixel.setLimit(material, 0, 1);
        }
        else
        {
            pixel.setF(ma.color.x,
                       ma.color.y,
                       ma.color.z);
        }
    }
    else
    {
        pixel.mix(rtColor(sc->horizon),
                  rtColor(sc->zenith),
                  skClampf(1 - v, 0, 1));
    }
}

static void rtCpuRenderSample(rtColor&            curPixel,
                              rtCpuHitResult&     nearest,
                              const rtCameraType* ca,
                              const rtSceneType*  sc,
                              const rtScalar&     kX,
                              const rtScalar      kY,
                              const int           y)
{
    rtCpuRay ray{};
    ray.origin = ca->location;
    rtCpuComputeRayDirection(ray.direction,
                             ca->rotation,
                             kX,
                             kY,
                             ca->offset);

    // cache the first ray cast...
    nearest.object = nullptr;
    if (rtCpuTestScene(sc, &nearest, &ray))
    {
        // process modes when hit
        if (sc->flags & RM_COLOR_AND_LIGHT)
            rtCpuTraceColorAndLight(curPixel, sc, nearest, ray, 0);

        if (sc->flags & RM_COMPUTED_NORMAL)
        {
            if (sc->flags == RM_COMPUTED_NORMAL)
            {
                curPixel.setF(
                    skScalar(0.5) + nearest.normal.x * skScalar(0.5),
                    skScalar(0.5) + nearest.normal.y * skScalar(0.5),
                    skScalar(0.5) + nearest.normal.z * skScalar(0.5));
            }
            else
            {
                rtColor n;
                n.setF(
                    skScalar(0.5) + nearest.normal.x * skScalar(0.5),
                    skScalar(0.5) + nearest.normal.y * skScalar(0.5),
                    skScalar(0.5) + nearest.normal.z * skScalar(0.5));
                curPixel.mix(n, curPixel, 0.5f);
            }
        }

        if (sc->flags & RM_DISTANCE)
        {
            skScalar df = skScalar(2.f) / (nearest.distance + skScalar(10e-3));
            if (sc->flags == RM_DISTANCE)
                curPixel.set(skClampf(df, 0, 1));
            else
            {
                rtColor d;
                d.set(skClampf(df, 0, 1));
                curPixel.mix(d, curPixel, 0.5f);
            }
        }

        if (sc->flags & RM_OUTLINE)
        {
            if (sc->flags == RM_OUTLINE)
                rtCpuTraceSilhouette(curPixel, ca->rotation, nearest, ray, kX, kY, ca->offset, {10e-4f, 1000});
            else
            {
                rtColor pixel;
                rtCpuTraceSilhouette(pixel, ca->rotation, nearest, ray, kX, kY, ca->offset, {10e-4f, 1000});
                curPixel.mul(pixel);
            }
        }
    }
    else
    {
        // process modes when not hit
        if (sc->flags & RM_COLOR_AND_LIGHT)
            rtCpuTraceColorAndLight(curPixel, sc, nearest, ray, skScalar(y) * ca->offset.y);
        else if (sc->flags == RM_OUTLINE)
            curPixel.set(1);
    }

    curPixel.saturate();
    if (sc->flags & RM_INVERT)
        curPixel.invert();
}

void rtCpuKernelMain(rtFrameBufferInfo&  fb,
                     const rtSceneType*  sc,
                     const rtTileParams* tile)
{
    const rtCameraType* ca      = sc->camera;
    rtCpuHitResult      nearest = {};

    for (SKint32 y = tile->y; y < tile->h; ++y)
    {
        for (SKint32 x = tile->x; x < tile->w; x++)
        {
            if (!(sc->flags & RM_AA))
            {
                const skScalar kX = skScalar(fb.width - 2 * x);
                const skScalar kY = skScalar(fb.height - 2 * y);

                rtColor curPixel;

                rtCpuRenderSample(curPixel, nearest, ca, sc, kX, kY, y);
                rtSetPixel(fb, x, fb.height - 1 - y, curPixel);
            }
            else
            {
                const skScalar kX = skScalar(fb.width - 2 * x);
                const skScalar kY = skScalar(fb.height - 2 * y);

                rtColor        P[5];
                const skScalar h = 1.1625f;

                rtCpuRenderSample(P[0], nearest, ca, sc, kX, kY, y);
                rtCpuRenderSample(P[1], nearest, ca, sc, kX - h, kY - h, y);
                rtCpuRenderSample(P[2], nearest, ca, sc, kX + h, kY + h, y);
                rtCpuRenderSample(P[3], nearest, ca, sc, kX + h, kY - h, y);
                rtCpuRenderSample(P[4], nearest, ca, sc, kX - h, kY + h, y);

                rtColor curPixel;
                curPixel.set(P[0]);
                curPixel.add(P[1]);
                curPixel.add(P[2]);
                curPixel.add(P[3]);
                curPixel.add(P[4]);
                curPixel.mul(0.2f);

                rtSetPixel(fb, x, fb.height - 1 - y, curPixel);
            }
        }
    }
}
