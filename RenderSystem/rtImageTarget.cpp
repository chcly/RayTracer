/*
-------------------------------------------------------------------------------
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
#include "rtImageTarget.h"
#include <cstring>
#include "rtCommon.h"

rtImageTarget::rtImageTarget(SKuint32 w, SKuint32 h) :
    m_image(nullptr),
    m_frameBuffer()
{
    m_image = new skImage(w, h, SK_RGBA);
    m_image->setFlipY(true);
    m_frameBuffer.width   = (SKuint16)m_image->getWidth();
    m_frameBuffer.height  = (SKuint16)m_image->getHeight();
    m_frameBuffer.maxSize = (SKuint32)m_image->getSizeInBytes();
    m_frameBuffer.pitch   = (SKuint16)m_image->getPitch();
    m_frameBuffer.pixels  = new SKubyte[m_frameBuffer.maxSize];
}

rtImageTarget::~rtImageTarget()
{
    delete[] m_frameBuffer.pixels;
    delete m_image;
    m_image = nullptr;
}

void rtImageTarget::save(const char* path) const
{
    if (m_image)
    {
        for (SKuint32 y = 0; y < m_frameBuffer.height; ++y)
        {
            for (SKuint32 x = 0; x < m_frameBuffer.width; ++x)
            {
                rtPixelRGBA* rgba = (rtPixelRGBA*)&m_frameBuffer.pixels[x * 4 + y * m_frameBuffer.pitch];
                m_image->setPixel(x, y, {rgba->r, rgba->g, rgba->b, 255});
            }
        }
        m_image->save(path);
    }
}

SKuint32 rtImageTarget::getWidth()
{
    if (m_image)
        return m_image->getWidth();
    return 0;
}

SKuint32 rtImageTarget::getHeight()
{
    if (m_image)
        return m_image->getHeight();
    return 0;
}

SKuint32 rtImageTarget::getSizeInBytes()
{
    if (m_image)
    {
        const SKsize sz = m_image->getSizeInBytes();
        SK_ASSERT(sz < SK_NPOS32);
        return (SKuint32)sz;
    }
    return 0;
}

SKuint32 rtImageTarget::getPitch()
{
    if (m_image)
        return m_image->getPitch();
    return 0;
}
