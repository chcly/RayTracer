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
/*! \addtogroup FrontEnd
 * @{
 */
#ifndef _rtCommon_h_
#define _rtCommon_h_

#include "Utils/Config/skConfig.h"
#include "RenderSystem/rtConfig.h"


class rtLoader;
class rtRenderSystem;

class rtScene;
class rtMesh;
class rtMeshPrivate;

class rtObject;
class rtLight;
class rtCamera;


/// <summary>
/// 
/// </summary>
typedef struct rtPixelRGBA
{
#if SK_ENDIAN == SK_ENDIAN_BIG
    SKubyte r, g, b, a;
#else
    SKubyte b, g, r, a;
#endif
} rtPixelRGBA;

/*! @} */

#endif  //_rtCommon_h_
