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
#include "ftScanner.h"
#include <cstddef>

using namespace ftFlags;

ftScanner::ftScanner(const char* ptr, SKsize length) :
    m_buffer(nullptr),
    m_pos(0),
    m_len(0),
    m_state(0),
    m_lineNo(1)
{
    if (length < 0x7FFFFFFF)
    {
        m_len    = (int)length;
        m_buffer = ptr;
    }
}

int ftScanner::lex(ftToken& token)
{
    if (!m_buffer)
        return FT_NULL_TOKEN;

    int decision;
    while (!isEof())
    {
        switch (m_state)
        {
        case FT_IN_START:
            decision = handleStartState(token);
            break;
        case FT_IN_NAMESPACE:
            decision = handleNamespaceState(token);
            break;
        case FT_INSIDE:
            decision = handleInsideState(token);
            break;
        case FT_IN_CLASS:
        case FT_IN_STRUCT:
            decision = handleClassState(token);
            break;
        case FT_IN_SKIP:
            decision = handleToggleState();
            break;
        default:
            decision = FT_KEEP_GOING;
            m_pos++;
            break;
        }

        if (decision == FT_EOF)
            return FT_EOF;
        if (decision != FT_KEEP_GOING)
            return token.getTokenId();
    }
    return FT_EOF;
}

int ftScanner::handleStartState(ftToken& token)
{
    ignoreUntilNcs();
    if (isEof())
        return FT_EOF;

    if (m_buffer[m_pos] == 'n')
    {
        m_pos += isKeyword("namespace", 9, FT_IN_NAMESPACE);
        if (m_state == FT_IN_NAMESPACE)
        {
            makeKeyword(token, "namespace", FT_NAMESPACE);
            return token.getTokenId();
        }
    }
    else if (m_buffer[m_pos] == 'c')
    {
        m_pos += isKeyword("class", 5, FT_IN_CLASS);
        if (m_state == FT_IN_CLASS)
        {
            makeKeyword(token, "class", FT_CLASS);
            return token.getTokenId();
        }
    }
    else if (m_buffer[m_pos] == 's')
    {
        m_pos += isKeyword("struct", 6, FT_IN_STRUCT);
        if (m_state == FT_IN_STRUCT)
        {
            makeKeyword(token, "struct", FT_STRUCT);
            return token.getTokenId();
        }
    }
    else
        m_pos++;

    return FT_KEEP_GOING;
}

int ftScanner::handleNamespaceState(ftToken& token)
{
    ignoreWhiteSpace();
    if (isEof())
        return FT_EOF;

    if (isAlpha(m_buffer[m_pos]) || m_buffer[m_pos] == '_')
    {
        makeIdentifier(token);
        m_state = FT_IN_START;
        return token.getTokenId();
    }

    if (m_buffer[m_pos] == '/')
        return handleLineComment();

    ++m_pos;
    m_state = FT_IN_START;
    return FT_KEEP_GOING;
}

int ftScanner::handleClassState(ftToken& token)
{
    ignoreWhiteSpace();
    if (isEof())
        return FT_EOF;

    if (isAlpha(m_buffer[m_pos]) || m_buffer[m_pos] == '_')
    {
        makeIdentifier(token);
        return token.getTokenId();
    }

    if (m_buffer[m_pos] == '/')
        return handleLineComment();

    switch (m_buffer[m_pos])
    {
    case '{':
        makeLeftBracket(token);
        m_state = FT_INSIDE;
        return token.getTokenId();
    case '}':
        makeRightBracket(token);
        return token.getTokenId();
    case ';':
        makeSemicolon(token);
        m_state = FT_IN_START;
        return token.getTokenId();
    default:
        ++m_pos;
        break;
    }
    return FT_KEEP_GOING;
}

int ftScanner::handleInsideState(ftToken& token)
{
    ignoreWhiteSpace();
    if (isEof())
        return FT_EOF;

    const char cp = m_buffer[m_pos];
    if (isAlpha(cp) || cp == '_')
    {
        bool keepGoing = false;

        if (isPotentialKeyword(cp))
        {
            for (size_t i = 0; i < KeywordTableSize && !keepGoing; ++i)
            {
                if (KeywordTable[i].name[0] == cp &&
                    isKeyword(KeywordTable[i].name, KeywordTable[i].len, m_state) > 1)
                {
                    const int tz = KeywordTable[i].token;
                    if (tz != FT_PUBLIC && tz != FT_PRIVATE && tz != FT_PROTECTED)
                    {
                        m_pos += KeywordTable[i].len;
                        makeKeyword(token, KeywordTable[i].name, KeywordTable[i].token);
                        return token.getTokenId();
                    }

                    m_pos += KeywordTable[i].len;
                    keepGoing = true;
                }
            }
        }
        if (!keepGoing)
        {
            makeIdentifier(token);
            return token.getTokenId();
        }
    }
    else if (isDigit(cp))
    {
        makeDigit(token);
        return token.getTokenId();
    }
    else if (cp == '/')
        return handleLineComment();
    else
    {
        switch (m_buffer[m_pos])
        {
        case '*':
            makePointer(token);
            return token.getTokenId();
        case ',':
            makeComma(token);
            return token.getTokenId();
        case ':':
            m_pos++;
            break;
        case '[':
            makeLeftBrace(token);
            return token.getTokenId();
        case ']':
            makeRightBrace(token);
            return token.getTokenId();
        case '(':
            makeLeftParen(token);
            return token.getTokenId();
        case ')':
            makeRightParen(token);
            return token.getTokenId();
        case '}':
            makeRightBracket(token);
            m_state = FT_IN_START;
            return token.getTokenId();
        case ';':
            makeSemicolon(token);
            return token.getTokenId();
        default:
            ++m_pos;
            break;
        }
    }
    return FT_KEEP_GOING;
}

int ftScanner::handleLineComment()
{
    m_pos++;
    if (isEof())
        return FT_EOF;

    const char cp = m_buffer[m_pos];
    if (cp == '/')
    {
        while (m_pos < m_len &&
               m_buffer[m_pos] != '@' &&
               !isNewLine(m_buffer[m_pos]))
            m_pos++;

        if (isEof())
            return FT_EOF;

        if (!isNewLine(m_buffer[m_pos]))
        {
            m_pos++;
            if (isKeyword("makeft_ignore", 13, FT_IN_SKIP))
                m_pos += 13;
            else
            {
                while (m_pos < m_len && !isNewLine(m_buffer[m_pos]))
                    m_pos++;

                if (isEof())
                    return FT_EOF;

                if (isNewLine(m_buffer[m_pos + 1]))
                    m_pos++;
            }
        }
    }
    return FT_KEEP_GOING;
}

int ftScanner::handleToggleState()
{
    while (m_pos < m_len && m_buffer[m_pos] != '@')
        m_pos++;

    if (isEof())
        return FT_EOF;

    m_pos++;
    if (isKeyword("makeft_ignore", 13, FT_INSIDE))
        m_pos += 13;

    return FT_KEEP_GOING;
}

bool ftScanner::newlineTest()
{
    const bool skp = m_pos < m_len && isNewLine(m_buffer[m_pos]);
    while (m_pos < m_len && isNewLine(m_buffer[m_pos]))
    {
        m_lineNo++;
        if (m_pos + 1 < m_len && isNewLine(m_buffer[m_pos + 1]))
            m_pos++;
        m_pos++;
    }
    return skp;
}

void ftScanner::ignoreUntilNcs()
{
    while (m_pos < m_len && !isNcs(m_buffer[m_pos]))
    {
        if (!newlineTest())
            ++m_pos;
    }
}

void ftScanner::ignoreWhiteSpace()
{
    while (m_pos < m_len && isWhiteSpace(m_buffer[m_pos]))
    {
        if (newlineTest() == 0)
            ++m_pos;
    }
}

void ftScanner::makeIdentifier(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();

    while (m_pos < m_len &&
           isIdentifier(m_buffer[m_pos]))
    {
        ref.push_back(m_buffer[m_pos]);
        m_pos++;
    }
    token.setTokenId(FT_ID);
}

void ftScanner::makeLeftBracket(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back('{');
    token.setTokenId(FT_L_BRACKET);
    m_pos++;
}

void ftScanner::makeRightBracket(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back('}');
    token.setTokenId(FT_R_BRACKET);
    m_pos++;
}

void ftScanner::makeSemicolon(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back(';');
    token.setTokenId(FT_TERM);
    m_pos++;
}

void ftScanner::makeColon(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back(':');
    token.setTokenId(FT_COLON);
    m_pos++;
}

void ftScanner::makePointer(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back('*');
    token.setTokenId(FT_POINTER);
    m_pos++;
}

void ftScanner::makeLeftBrace(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back('[');
    token.setTokenId(FT_L_BRACE);
    m_pos++;
}

void ftScanner::makeRightBrace(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back(']');
    token.setTokenId(FT_RBRACE);
    m_pos++;
}

void ftScanner::makeComma(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back(',');
    token.setTokenId(FT_COMMA);
    m_pos++;
}

void ftScanner::makeLeftParen(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back('(');
    token.setTokenId(FT_L_PARENTHESIS);
    m_pos++;
}

void ftScanner::makeRightParen(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref.push_back(')');
    token.setTokenId(FT_R_PARENTHESIS);
    m_pos++;

    // Scan till the first statement terminator.
    // If this ever needs to serialize the parameter list,
    // then copy this to an extra storage string in the token.
    // ... For now just skip over it.
    while (m_pos < m_len && m_buffer[m_pos] != ';')
        ++m_pos;
}

void ftScanner::makeDigit(ftToken& token)
{
    ftToken::String& ref = token.getRef();
    ref.clear();

    while (m_pos < m_len &&
           isDigit(m_buffer[m_pos]))
    {
        ref.push_back(m_buffer[m_pos]);
        m_pos++;
    }

    token.setArrayLen(skChar::toInt32(ref.c_str()));
    token.setTokenId(FT_CONSTANT);
}

void ftScanner::makeKeyword(ftToken& token, const char* kw, const SKint32& id)
{
    ftToken::String& ref = token.getRef();
    ref.clear();
    ref = kw;
    token.setTokenId(id);
}

const ftKeywordTable ftScanner::KeywordTable[] = {
    {"public", 6, FT_PUBLIC},
    {"private", 7, FT_PRIVATE},
    {"protected", 9, FT_PROTECTED},
    {"struct", 6, FT_STRUCT},
    {"class", 5, FT_CLASS},
    {"char", 4, FT_CHAR},
    {"uchar", 5, FT_CHAR},
    {"short", 5, FT_SHORT},
    {"ushort", 6, FT_SHORT},
    {"int", 3, FT_INTEGER},
    {"uint", 4, FT_INTEGER},
    {"long", 4, FT_LONG},
    {"ulong", 5, FT_LONG},
    {"float", 5, FT_FLOAT},
    {"double", 6, FT_DOUBLE},
    {"void", 4, FT_VOID},
};

const SKsize ftScanner::KeywordTableSize = sizeof KeywordTable / sizeof(ftKeywordTable);

bool ftScanner::isEof() const
{
    return m_pos >= m_len || m_buffer[m_pos] == '\0';
}

bool ftScanner::isNewLine(const char& ch)
{
    return ch == '\n' || ch == '\r';
}

bool ftScanner::isNcs(const char& ch)
{
    return ch == 'n' || ch == 'c' || ch == 's';
}

bool ftScanner::isAlpha(const char& ch)
{
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}

bool ftScanner::isPotentialKeyword(const char& ch)
{
    return ch == 'c' ||
           ch == 'u' ||
           ch == 's' ||
           ch == 'i' ||
           ch == 'l' ||
           ch == 'p' ||
           ch == 'f' ||
           ch == 'd' ||
           ch == 'v';
}

bool ftScanner::isDigit(const char& ch)
{
    return ch != '\0' && (ch >= '0' && ch <= '9');
}

bool ftScanner::isAlphaNumeric(const char& ch)
{
    return ch != '\0' && (isAlpha(ch) || isDigit(ch));
}

bool ftScanner::isIdentifier(const char& ch)
{
    return isAlphaNumeric(ch) || ch == '_';
}

bool ftScanner::isWhiteSpace(const char& ch)
{
    return ch == ' ' || ch == '\t' || isNewLine(ch);
}

int ftScanner::isKeyword(const char* kw, const SKint32& len, const SKint32& stateIfTrue)
{
    const char* tp = &m_buffer[m_pos];
    if (m_pos + (len - 1) < m_len &&
        !isIdentifier(m_buffer[m_pos + len]) &&
        skChar::equalsn(tp, kw, len) == 0)
    {
        m_state = stateIfTrue;
        return len;
    }
    return 1;
}
