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
#ifndef _skScalar_h_
#define _skScalar_h_

#ifdef __cplusplus
#include <cfloat>
#include <cmath>
// Header file only
#include "Utils/Config/skConfig.h"
#include "Utils/skMinMax.h"
#else
#include <float.h>
#include <math.h>
#endif 


#ifdef SK_DOUBLE
#define SK_EPSILON     DBL_EPSILON
#define SK_INFINITY    DBL_MAX
#define SK_NAN         NAN
typedef double skScalar;
#else
#define SK_EPSILON  FLT_EPSILON
#define SK_INFINITY FLT_MAX
#define SK_NAN      NAN
typedef float skScalar;
#endif


#endif//_skScalar_h_
