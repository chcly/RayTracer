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
#ifndef _ftCompiler_h_
#define _ftCompiler_h_

#include "ftTypes.h"
#include "Utils/skArray.h"

struct ftBuildMember;
class ftBuildStruct;
typedef skArray<ftBuildStruct> ftBuildStructArray;

/// <summary>
/// ftCompiler is a utility class to aid in building
/// the memory tables.
/// </summary>
class ftCompiler
{
public:
    ftCompiler();
    ~ftCompiler();

    /// <summary>
    /// Parses the source file at the supplied path.
    /// </summary>
    /// <param name="path">Path to the source that should be parsed</param>
    /// <returns>Negative one on an error, otherwise returns last token id that was read.</returns>
    int parse(const char* path);

    /// <summary>
    /// Parses the supplied memory buffer.
    /// </summary>
    /// <param name="path">
    /// A pathname for the memory buffer.
    /// It is used to track a list of needed include files that will be included
    /// in the validation source file.
    /// </param>
    /// <param name="data">The memory that should be read.</param>
    /// <param name="len">The size in bytes of the supplied memory.</param>
    /// <returns>Negative one on an error otherwise returns last token id that was read.</returns>
    int parse(const char* path, const char* data, SKsize len);

    /// <summary>
    /// Compiles the tables.
    /// </summary>
    /// <remarks>This should be called after a successful parse.</remarks>
    int compile();

    /// <summary>
    /// Writes the table to the supplied stream.
    /// </summary>
    /// <param name="tableName">The name of the table to use when writing the memory table.</param>
    /// <param name="fp">An output stream to use to write to.</param>
    void writeFile(const ftId& tableName, class skStream* fp);

    /// <summary>
    /// Writes the table to the supplied file path.
    /// </summary>
    /// <param name="tableName">The name of the table to use when writing the memory table.</param>
    /// <param name="path">The file path of the file to write.</param>
    void writeFile(const ftId& tableName, const ftPath& path);

    /// <summary>
    /// Writes the table directly to the stream without any extra formatting.
    /// </summary>
    /// <param name="stream">An output stream that is used to write.</param>
    void writeStream(class skStream* stream);

    /// <summary>
    /// Sets the internal write mode
    /// </summary>
    /// <param name="mode">A value of ftFlags::WriteMode</param>
    void setWriteMode(const int mode)
    {
        m_writeMode = mode;
    }

private:
    int  parse();
    void parseClass(int& token, ftToken& tokenPtr);
    void parseIdentifier(int& token, ftToken& tokenPtr, ftBuildStruct& buildStruct);

    static void handleConstant(int&           token,
                               ftToken&       tokenPtr,
                               ftBuildMember& member);

    static void handleStatementClosure(int&           token,
                                       ftBuildStruct& buildStruct,
                                       ftBuildMember& member,
                                       bool           forceArray,
                                       bool           isIdentifier);

    void errorUnknown(int& token, ftToken& tokenPtr);

    void writeBinPtr(skStream* fp, void* ptr, int len);

    void writeCharPtr(skStream* fp, const ftStringPtrArray& pointers);

    void writeValidationProgram(const ftPath& path);

    static void makeName(ftBuildMember&, bool);

    ftTableBuilder*    m_build;
    ftPathArray        m_includes;
    ftStringPtrArray   m_namespaces, m_skip;
    ftBuildStructArray m_builders;
    int                m_curBuf, m_writeMode;
    ftScanner*         m_scanner;
};

#endif  //_ftCompiler_h_
