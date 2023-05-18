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
#include "ftStreams.h"
#include "Utils/skPlatformHeaders.h"

#if FT_USE_ZLIB == 1
#include "zconf.h"
#include "zlib.h"
#endif

#if FT_USE_ZLIB == 1

ftGzStream::ftGzStream() :
    m_handle(nullptr)
{
}

ftGzStream::~ftGzStream()
{
    ftGzStream::close();
}

void ftGzStream::open(const char* path, int mode)
{
    if (path)
    {
        close();
        m_mode = mode;
        if (m_mode == READ || m_mode == READ_TEXT)
            m_handle = gzopen(path, "rb");
        else if (mode == WRITE || m_mode == WRITE_TEXT)
            m_handle = gzopen(path, "wb");
        else
            m_mode = SK_NPOS32;
    }
    else
        printf("Invalid path name.\n");
}

void ftGzStream::close(void)
{
    if (m_handle != nullptr)
    {
        gzclose((gzFile)m_handle);
        m_handle = nullptr;
    }
}

SKsize ftGzStream::read(void* dest, SKsize nr) const
{
    if (!canRead() || !isOpen())
        return SK_NPOS;

    return gzread((gzFile)m_handle, dest, (unsigned int)nr);
}

SKsize ftGzStream::write(const void* src, SKsize nr)
{
    if (!canWrite() || !isOpen() || !src)
        return SK_NPOS;

    if (nr > 0 && nr < SK_NPOS)
        return gzwrite((gzFile)m_handle, src, (unsigned int)nr);
    return 0;
}

bool ftGzStream::eof(void) const
{
    if (!m_handle)
        return true;
    return gzeof((gzFile)m_handle) != 0;
}

SKsize ftGzStream::position(void) const
{
    return gztell((gzFile)m_handle);
}

SKsize ftGzStream::size(void) const
{
    return 0;
}

bool ftGzStream::seek(SKint64 offset, SKsize dir)
{
    if (!isOpen())
        return false;

    long way;
    if (dir == SEEK_END)
        way = SEEK_SET;  // supported ?
    else if (dir == SEEK_CUR)
        way = SEEK_CUR;
    else
        way = SEEK_SET;

    if (gzseek(static_cast<gzFile>(m_handle), (long)offset, way) != -1)
        return true;
    return false;
}

#endif
