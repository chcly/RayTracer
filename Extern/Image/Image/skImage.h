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
#ifndef _skImage_h_
#define _skImage_h_

#include "Image/skPixel.h"
#include "Utils/Config/skConfig.h"
#include "Utils/skDisableWarnings.h"

class skImage
{
private:
    SKuint32      m_width;
    SKuint32      m_height;
    SKuint32      m_pitch;
    SKuint32      m_bpp;
    SKsize        m_size;
    SKubyte*      m_bytes;
    bool          m_flip;
    skPixelFormat m_format;
    FIBITMAP*     m_bitmap;

    void unloadAndReset();

    void allocateBytes();

    void calculateBitsPerPixel();

    void calculateFormat();


    SKuint32 getBufferPos(const SKuint32& x, const SKuint32& y) const
    {
        if (m_flip)
            return getBufferPosFlipped(x, y);
        return getBufferPosNormal(x, y);
    }


    SKuint32 getBufferPosNormal(const SKuint32& x, const SKuint32& y) const
    {
        return (SKuint32)y * m_pitch + x * m_bpp;
    }

    SKuint32 getBufferPosFlipped(const SKuint32& x, const SKuint32& y) const
    {
        return (SKuint32)(m_height - 1 - y) * m_pitch + x * m_bpp;
    }

    void _updateFromBitmap();

public:
    skImage();
    skImage(SKuint32 width, SKuint32 height, skPixelFormat format);
    ~skImage();

    SKuint32 getWidth() const
    {
        return m_width;
    }

    SKuint32 getHeight() const
    {
        return m_height;
    }

    SKuint32 getPitch() const
    {
        return m_pitch;
    }

    SKuint32 getBPP() const
    {
        return m_bpp;
    }

    SKubyte* getBytes() const
    {
        return m_bytes;
    }

    SKsize getSizeInBytes() const
    {
        return m_size;
    }

    skPixelFormat getFormat() const
    {
        return m_format;
    }

    void setFlipY(bool v)
    {
        m_flip = v;
    }


    void clear(const skPixel& pixel) const;

    void setPixel(const SKuint32& x, const SKuint32& y, const skPixel& pixel) const;

    void getPixel(const SKuint32& x, const SKuint32& y, skPixel& pixel) const;

    void fillRect(SKuint32       x,
                  SKuint32       y,
                  SKuint32       width,
                  SKuint32       height,
                  const skPixel& col) const;

    void strokeRect(SKuint32       x,
                    SKuint32       y,
                    SKuint32       width,
                    SKuint32       height,
                    const skPixel& col) const;

    void lineTo(SKint32        x1,
                SKint32        y1,
                SKint32        x2,
                SKint32        y2,
                const skPixel& col) const;

    skImage* convertToFormat(const skPixelFormat& format) const;

    void save(const char* file) const;

    bool load(const char* file);

    bool loadFromMemory(void *mem, const SKsize& size);


    static void initialize();

    static void finalize();


    static void copy(SKubyte*       dst,
                     const SKubyte* src,
                     SKuint32       w,
                     SKuint32       h,
                     skPixelFormat  dstFmt,
                     skPixelFormat  srcFmt);

    static SKuint32 getSize(const skPixelFormat& format);

    static skPixelFormat getFormat(SKuint32 bpp);

    static void getPixel(skPixel& dest, const SKubyte* src, skPixelFormat format);

    static void setPixel(SKubyte* dst, const skPixel& src, skPixelFormat format);
};

#endif  //_skImage_h_
