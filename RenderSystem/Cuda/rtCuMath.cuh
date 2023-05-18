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
#ifndef _rtCudaMath_cuh_
#define _rtCudaMath_cuh_

#include "rtCudaUtils.h"
#include "RenderSystem/Math/rtVectorTypes.h"

RT_CUD_API void   rtCuMix(rtVector3* a, rtVector3* b, rtScalar fac);
RT_CUD_API void   rtCuMix3(rtVector3* a, rtVector3* b, rtVector3* c, rtScalar fac);
RT_CUD_API void   iMix(rtVector3* a, rtVector3* b, rtScalar fac);
RT_CUD_API void   rtCuLimit3(rtVector3* a, rtScalar mi, rtScalar ma);
RT_CUD_API rtVector3 rtCuAddFloat3(rtVector3 a, rtVector3 b);
RT_CUD_API rtVector3 rtCuCrossFloat3(rtVector3 a, rtVector3 b);
RT_CUD_API rtVector3 rtCuMulFloat3(rtVector3 a, rtScalar s);
RT_CUD_API rtVector3 rtCuMulFloat3v(const rtVector3& a, const rtVector3& v);
RT_CUD_API rtScalar  rtCuFloat3Dot(rtVector3 a, rtVector3 b);
RT_CUD_API rtScalar  rtCuFloat3Dot(rtVector3 a);
RT_CUD_API rtVector3 rtCuFloat3Norm(rtVector3 a);
RT_CUD_API rtVector3 rtCuFloat3Sub(rtVector3 a, rtVector3 b);
RT_CUD_API rtVector3 rtCuMulQuat(rtVector3 v, float4 q);
RT_CUD_API rtVector3 rtCuReflectFloat3(const rtVector3& in,
                             const rtVector3& no);

#endif  //_rtCudaMath_cuh_
