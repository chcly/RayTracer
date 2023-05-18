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
#ifndef _rtCpuRenderSystem_h_
#define _rtCpuRenderSystem_h_

#include "RenderSystem/rtRenderSystem.h"
#include "RenderSystem/rtScene.h"

class rtTileManager;
class rtTarget;
class skRay;
class skImage;

class rtCpuRenderSystem : public rtRenderSystem
{
private:
    rtTileManager* m_tiles;

    void initialize(rtScene* scene);

public:
    rtCpuRenderSystem();
    ~rtCpuRenderSystem() override;

    void render(rtScene* scene) override;
};

#endif  //_rtCpuRenderSystem_h_
