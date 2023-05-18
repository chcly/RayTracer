/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _bAsciiScanner_h_
#define _bAsciiScanner_h_

#include "bAsciiCommon.h"

class skStream;

class bAsciiToken
{
public:
    bAsciiToken() :
        tok(OP_UNKNOWN),
        line(0)
    {
    }

    bAsciiToken(const bString& b, int op, int line) :
        string(b),
        tok(op),
        line(line)
    {
    }

    bAsciiToken& operator=(const bAsciiToken& o) = default;

    bString string;
    int     tok;
    int     line;
};

class bAsciiScanner
{
protected:
    int         m_line;
    char*       m_buffer;
    SKuint32    m_bufSize;
    SKuint32    m_curTok;
    int         m_error;
    const char* m_source;

    void loadStream(skStream& stream);

public:
    bAsciiScanner(const char* file);

    bAsciiScanner(const char* source, const char* memBuf, SKsize size);

    ~bAsciiScanner();

    int getError() const
    {
        return m_error;
    }

    const char* getSource() const
    {
        return m_source;
    }

    int scan(bAsciiToken& tok, bool verbose);
};

#endif  //_bAsciiScanner_h_
