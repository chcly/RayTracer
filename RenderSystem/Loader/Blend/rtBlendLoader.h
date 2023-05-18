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
#ifndef _rtBlendLoader_h_
#define _rtBlendLoader_h_

#include "RenderSystem/Loader/rtLoader.h"

/// <summary>
/// Blend file implementation of the rtLoader class
/// </summary>
class rtBlendLoader final : public rtLoader
{
private:
    friend class rtBlendLoaderPrivate;

public:
    rtBlendLoader()           = default;
    ~rtBlendLoader() override = default;

    /// <seealso cref="rtLoader"/>
    int load(const char* path) override;

    /// <seealso cref="rtLoader"/>
    void addScene(rtScene* sc)
    {
        m_scenes.push_back(sc);
    }
};

#endif  //_rtBlendLoader_h_
