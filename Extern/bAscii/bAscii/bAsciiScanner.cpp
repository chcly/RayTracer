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
#include "bAsciiScanner.h"
#include <cstdlib>
#include <cstring>
#include "Utils/skFileStream.h"
#include "Utils/skMemoryStream.h"
#include "bAsciiOpCodes.h"

#define CurrentToken ((m_curTok < m_bufSize && m_curTok >= 0) ? m_buffer[m_curTok] : OP_END)
#define PreviousToken ((m_curTok < m_bufSize && m_curTok >= 1) ? m_buffer[m_curTok - 1] : OP_END)
#define NextToken (((m_curTok + 1) < m_bufSize && m_curTok >= 0) ? m_buffer[m_curTok + 1] : OP_END)
#define Advance (m_curTok += 1)
#define Rewind (m_curTok -= 1)
#define StartComment '#'
#define StartString '"'

#define IsLetter(c) (((c) >= 'a' && (c) <= 'z' || (c) >= 'A' && (c) <= 'Z'))
#define IsDigit(c) (((c) >= '0' && (c) <= '9') || (c) == '.')
#define IsValidIdentifier(c) (IsLetter(c) || ((c) >= '0' && (c) <= '9') || (c) == '_')

bAsciiScanner::bAsciiScanner(const char* file) :
    m_line(0),
    m_buffer(nullptr),
    m_bufSize(0),
    m_curTok(0),
    m_error(-1),
    m_source(file)
{
    skFileStream fs;
    fs.open(file, skStream::READ_TEXT);
    if (fs.isOpen())
        loadStream(fs);
}

bAsciiScanner::bAsciiScanner(const char* source, const char* memBuf, SKsize size) :
    m_line(0),
    m_buffer(nullptr),
    m_bufSize(0),
    m_curTok(0),
    m_error(-1),
    m_source(source)
{
    skMemoryStream ms;
    ms.open(memBuf, size, skStream::READ_TEXT);
    if (ms.isOpen())
        loadStream(ms);
}

bAsciiScanner::~bAsciiScanner()
{
    delete[] m_buffer;
    m_buffer = nullptr;
}

void bAsciiScanner::loadStream(skStream& stream)
{
    if (stream.isOpen())
    {
        m_bufSize = (SKuint32)stream.size();
        if (m_bufSize > 0)
        {
            m_buffer = new char[(SKsize)m_bufSize + 1];

            const SKsize br = stream.read(m_buffer, m_bufSize);
            if (br <= m_bufSize)
            {
                m_bufSize    = (SKuint32)br;
                m_buffer[br] = 0;

                m_curTok = 0;
                m_error  = 0;
                m_line   = 1;
            }
        }
    }
}

int bAsciiScanner::scan(bAsciiToken& tok, bool verbose)
{
    tok.string.clear();
    tok.tok  = OP_UNKNOWN;
    tok.line = -1;

    for (;;)
    {
        const char c = CurrentToken;
        if (c == OP_END)
        {
            tok.tok    = OP_END;
            tok.line   = m_line;
            tok.string = "EOF";
            return OP_END;
        }

        if (c == '\n' || c == '\r')
        {
            if (NextToken == '\r')
                Advance;
            m_line += 1;
            Advance;
        }
        else if (c == ' ' || c == '\t')
            Advance;
        else if (c == StartComment)
        {
            do
            {
                Advance;
            } while (CurrentToken != '\n' && CurrentToken != '\r' && CurrentToken != OP_END);

            if (CurrentToken == '\n')
            {
                if (NextToken == '\r')
                    Advance;
                m_line += 1;
                Advance;
            }
        }
        else if (c == ':')
        {
            Advance;
            tok.string.append(':');
            tok.tok  = OP_SECTION;
            tok.line = m_line;

            return OP_SECTION;
        }
        else if (c == ',')
        {
            Advance;
            tok.string.append(',');
            tok.tok  = OP_SEPARATOR;
            tok.line = m_line;
            return OP_SEPARATOR;
        }
        else if (c == '=')
        {
            Advance;
            tok.string.append('=');
            tok.tok  = OP_ASSIGNMENT;
            tok.line = m_line;
            return OP_ASSIGNMENT;
        }
        else if (c == ';')
        {
            Advance;
            tok.string.append(';');
            tok.tok  = OP_TERM;
            tok.line = m_line;
            return OP_TERM;
        }
        else if (c == '{')
        {
            Advance;
            tok.string.append('{');
            tok.tok  = OP_BRACKET_OPEN;
            tok.line = m_line;
            return OP_BRACKET_OPEN;
        }
        else if (c == '}')
        {
            Advance;
            tok.string.append('}');
            tok.tok  = OP_BRACKET_CLOSE;
            tok.line = m_line;
            return OP_BRACKET_CLOSE;
        }
        else if (c == StartString)
        {
            Advance;
            do
            {
                tok.string.append(CurrentToken);
                Advance;
            } while (CurrentToken != StartString && CurrentToken != OP_END);

            if (CurrentToken == OP_END)
            {
                // eof scan
                tok.string = "end of file";
                tok.tok    = OP_UNKNOWN;
                tok.line   = m_line;
                return OP_UNKNOWN;
            }
            Advance;
            tok.tok  = OP_STRING;
            tok.line = m_line;
            return OP_STRING;
        }
        else if (IsLetter(c))
        {
            do
            {
                tok.string.append(CurrentToken);
                Advance;
            } while (IsLetter(CurrentToken) && CurrentToken != OP_END);

            tok.tok  = OP_ALPHA;
            tok.line = m_line;
            return OP_ALPHA;
        }
        else if (c == '-' || IsDigit(c))
        {
            if (c == '-' && !IsDigit(NextToken))
            {
                tok.tok  = OP_UNKNOWN;
                tok.line = m_line;
                return OP_UNKNOWN;
            }
            do
            {
                tok.string.append(CurrentToken);
                Advance;
            } while (IsDigit(CurrentToken) && CurrentToken != OP_END);

            tok.tok  = OP_NUMBER;
            tok.line = m_line;
            return OP_NUMBER;
        }
        else
        {
            tok.tok  = OP_UNKNOWN;
            tok.line = m_line;
            return OP_UNKNOWN;
        }
    }
}
