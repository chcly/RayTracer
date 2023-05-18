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
/*! \addtogroup CpuKernel
 * @{
 */

#ifndef _rtCpuKernel_h_
#define _rtCpuKernel_h_
#include "RenderSystem/rtScene.h"

struct rtFrameBufferInfo;

/// <summary>
/// The CPU frame buffer is split into separate threads, where the number of threads is
/// limited by the maximum number of hardware concurrent threads.
/// The frame buffer is subdivided as:
/// \f$
/// \frac{1}{N}
/// \f$
///
/// Then for each thread the parameters are defined as:
/// \f$
/// \begin{bmatrix}
/// 0
/// \frac{y}{N} 
/// w 
/// \frac{1}{N}
/// \end{bmatrix}
/// \f$ 
/// </summary>
struct rtTileParams
{
    const SKint32 x;
    const SKint32 y;
    const SKint32 w;
    const SKint32 h;
};

/// <summary>
/// Threaded call back per pixel
/// </summary>
/// <param name="fb">Writable reference to the rtFrameBufferInfo structure.</param>
/// <param name="sc">The scene to render.</param>
/// <param name="tile">The current tile to render.</param>
extern void rtCpuKernelMain(rtFrameBufferInfo&  fb,
                            const rtSceneType*  sc,
                            const rtTileParams* tile);
/*!
 * @}
 */
#endif  //_rtCpuKernel_h_
