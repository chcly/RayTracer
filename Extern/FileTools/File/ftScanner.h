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
#ifndef _ftScanner_h_
#define _ftScanner_h_

#include "ftToken.h"
#include "ftTypes.h"

/// <summary>
/// ftScanner is a basic c/c++ scanner that is used to extract class members.
/// </summary>
class ftScanner
{
private:
    const char* m_buffer;
    SKint32     m_pos;
    SKint32     m_len;
    SKint32     m_state;
    SKint32     m_lineNo;

    static const ftKeywordTable KeywordTable[];
    static const SKsize         KeywordTableSize;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    /// <param name="ptr">Is a memory buffer that contains the source that should be parsed.</param>
    /// <param name="length">The size in bytes of the memory buffer.</param>
    ftScanner(const char* ptr, SKsize length);

    /// <summary>
    /// Scans the source for a token.
    /// </summary>
    /// <param name="token">Is a reference to a ftToken class that will temporally hold the result.</param>
    /// <returns>The enumerated TokenID of the result.</returns>
    int lex(ftToken& token);

    /// <summary>
    /// Used to retrieve the line number for error reporting.
    /// </summary>
    /// <returns>Returns the last read line.</returns>
    int getLine() const
    {
        return m_lineNo;
    }

private:
    int isKeyword(const char* kw, const SKint32& len, const SKint32& stateIfTrue);

    bool isEof() const;

    static bool isNewLine(const char& ch);

    static bool isNcs(const char& ch);

    static bool isAlpha(const char& ch);

    static bool isPotentialKeyword(const char& ch);

    static bool isDigit(const char& ch);

    static bool isAlphaNumeric(const char& ch);

    static bool isIdentifier(const char& ch);

    static bool isWhiteSpace(const char& ch);

    void ignoreWhiteSpace();

    void ignoreUntilNcs();

    bool newlineTest();

    static void makeKeyword(ftToken& token, const char* kw, const SKint32& id);

    void makeIdentifier(ftToken& token);

    void makeLeftBracket(ftToken& token);

    void makeRightBracket(ftToken& token);

    void makeColon(ftToken& token);

    void makeSemicolon(ftToken& token);

    void makePointer(ftToken& token);

    void makeLeftBrace(ftToken& token);

    void makeRightBrace(ftToken& token);

    void makeLeftParen(ftToken& token);

    void makeRightParen(ftToken& token);

    void makeComma(ftToken& token);

    void makeDigit(ftToken& token);

    int handleStartState(ftToken& token);

    int handleNamespaceState(ftToken& token);

    int handleClassState(ftToken& token);

    int handleInsideState(ftToken& token);

    int handleToggleState();

    int handleLineComment();
};

#endif  // !_ftScanner_h_
