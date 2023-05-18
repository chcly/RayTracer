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

#include "ftCompiler.h"
#include <cstdio>
#include "Utils/skArray.h"
#include "ftConfig.h"
#include "ftLogger.h"
#include "ftScanner.h"
#include "ftStreams.h"

#define FT_IS_VALID_TOKEN(x) ((x) > 0)

using namespace ftFlags;

/// <summary>
/// Holds the count for all elements in the table.
/// </summary>
struct ftMaxAllocSize
{
    SKuint32 names;
    SKuint32 types;
    SKuint32 lengths;
    SKuint32 structTable;
    SKuint32 structures;
};

/// <summary>
/// Utility struct to aid in building the tables.
/// </summary>
struct ftBuildMember
{
    ftBuildMember() :
        typeId(-1),
        nameId(-1),
        ptrCount(0),
        numDimensions(0),
        isFunctionPointer(0),
        undefined(0),
        isDependentType(false),
        arrays(),
        arraySize(1),
        line(-1)
    {
    }

    ftId         type;
    ftId         name;
    FTtype       typeId;
    FTtype       nameId;
    SKuint16     ptrCount;
    SKuint16     numDimensions;
    int          isFunctionPointer;
    int          undefined;
    bool         isDependentType;
    ftArraySlots arrays;
    SKsize       arraySize;
    ftPath       path;
    SKsize       line;
};

/// <summary>
/// Utility struct to aid in building the tables.
/// </summary>
class ftBuildStruct
{
public:
    typedef skArray<ftBuildMember> Members;

public:
    ftBuildStruct() :
        structureId(SK_NPOS),
        nrDependentTypes(0),
        line(-1)
    {
    }

    SKsize  structureId;
    ftId    name;
    Members members;
    SKsize  nrDependentTypes;
    ftPath  path;
    SKsize  line;
};

/// <summary>
/// Utility class to build a table
/// </summary>
class ftTableBuilder
{
public:
    typedef skArray<FTtype> IntPtrArray;
    typedef skArray<FTtype> TypeArray;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    ftTableBuilder();
    ~ftTableBuilder() = default;

    /// <summary>
    /// Builds the information required to write the table.
    /// </summary>
    /// <param name="structBuilders">
    /// Is a reference to a ftBuildStructArray that will hold the type information
    /// until it is written.
    /// </param>
    /// <returns>
    /// LNK_OK on success otherwise returns a bit flag containing one
    /// or more flags from ftFlags::LinkerIssues.
    /// </returns>
    int compile(ftBuildStructArray& structBuilders);

    /// <summary>
    /// Computes the type length table and reports any alignment issues.
    /// </summary>
    /// <param name="structBuilders">
    /// Is a reference to a ftBuildStructArray that will hold the TYPE information
    /// until it is written.
    /// </param>
    /// <returns>
    /// LNK_OK on success otherwise returns a bit flag containing one
    /// or more flags from ftFlags::LinkerIssues.
    /// </returns>
    int buildTypeLengthTable(ftBuildStructArray& structBuilders);

    /// <summary>
    /// Adds the builtin atomic types found in ftAtomicUtils::Types
    /// </summary>
    /// <returns>
    /// LNK_OK on success otherwise returns a bit flag containing one
    /// or more flags from ftFlags::LinkerIssues.
    /// </returns>
    int buildBuiltinTypes();

    /// <summary>
    /// Adds a type to the type table.
    /// </summary>
    /// <param name="type">The name of the type.</param>
    /// <param name="len">The length of the type if it is known.</param>
    /// <returns>The index of the element in the TYPE array or SK_NPOS16 if the total types exceed 0xFFFF.</returns>
    FTtype addType(const ftId& type, const SKuint16& len = 0);

    /// <summary>
    /// Performs a linear search for the name and if it is not found.
    /// it is appended to the name array.
    /// </summary>
    /// <param name="lookup">The name to add</param>
    /// <returns>The index of the element in the name array.</returns>
    ftStringPtrArray::SizeType addName(const ftId& lookup);

    ftMaxAllocSize   allocationSizes;
    ftStringPtrArray name;
    ftStringPtrArray typeLookup;
    IntPtrArray      typeLengths;
    IntPtrArray      typeLengths64Bit;
    TypeArray        structures;
    ftStringPtrArray undefined;
    SKuint32         numberOfBuiltIn;
};

ftTableBuilder::ftTableBuilder() :
    allocationSizes{},
    numberOfBuiltIn(0)
{
    allocationSizes.names       = 0;
    allocationSizes.types       = 0;
    allocationSizes.lengths     = 0;
    allocationSizes.structTable = 0;
    allocationSizes.structures  = 0;
}

int ftTableBuilder::buildBuiltinTypes()
{
    int status = LNK_OK;
    for (size_t i = 0; i < ftAtomicUtils::NumberOfTypes && status == LNK_OK; ++i)
    {
        const FTtype type = addType(ftAtomicUtils::Types[i].name, ftAtomicUtils::Types[i].size);
        if (type == SK_NPOS16)
            status = LNK_ASSERT;
    }

    numberOfBuiltIn = typeLookup.size();
    if (numberOfBuiltIn != ftAtomicUtils::NumberOfTypes)
    {
        ftLogger::logF("Failed to create builtin types.\n");
        status = LNK_ASSERT;
    }
    return status;
}

FTtype ftTableBuilder::addType(const ftId& type, const SKuint16& len)
{
    SKsize loc = typeLookup.find(type);
    if (loc == typeLookup.npos)
    {
        allocationSizes.types += type.size() + 1;
        allocationSizes.lengths += sizeof(FTtype);
        loc = typeLookup.size();

        if (loc > FileTools_MaxTableSize)
        {
            ftLogger::logF("Type limit exceeded\n");
            return SK_NPOS16;
        }

        typeLookup.push_back(type);
        typeLengths.push_back(len);
        typeLengths64Bit.push_back(len);
    }
    return (FTtype)loc;
}

ftStringPtrArray::SizeType ftTableBuilder::addName(const ftId& lookup)
{
    ftStringPtrArray::SizeType loc;
    if ((loc = name.find(lookup)) == name.npos)
    {
        allocationSizes.names += name.size() + 1;
        loc = name.size();
        name.push_back(lookup);
    }
    return loc;
}

int ftTableBuilder::compile(ftBuildStructArray& structBuilders)
{
    int status;
    if ((status = buildBuiltinTypes()) != LNK_OK)
        return status;

    for (ftBuildStruct& bs : structBuilders)
    {
        bs.structureId = addType(bs.name, 0);
        if (bs.structureId == SK_NPOS16)
            return LNK_ASSERT;

        structures.push_back((SKuint16)bs.structureId);
        structures.push_back((SKuint16)bs.members.size());

        allocationSizes.structures += sizeof(FTtype) * 2;

        for (ftBuildMember& member : bs.members)
        {
            member.typeId = addType(member.type, 0);
            if (member.typeId == SK_NPOS16)
                return LNK_ASSERT;

            const ftStringPtrArray::SizeType idx = addName(member.name);
            if (idx >= SK_NPOS16)
            {
                ftLogger::logF("Name limit exceeded\n");
                return LNK_ASSERT;
            }
            member.nameId = (FTtype)idx;

            structures.push_back(member.typeId);
            structures.push_back(member.nameId);

            allocationSizes.structures += sizeof(FTtype) * 2;
        }
    }

    return buildTypeLengthTable(structBuilders);
}

int ftTableBuilder::buildTypeLengthTable(ftBuildStructArray& structBuilders)
{
    ftBuildStruct* structs = structBuilders.ptr();
    const SKsize   total   = structBuilders.size();

    SKsize next = total, prev = 0;

    FTtype* tln64 = typeLengths64Bit.ptr();
    FTtype* tLens = typeLengths.ptr();

    int              status = LNK_OK;
    ftStringPtrArray missingReport, missing;

    FTtype firstNonAtomic = 0;
    if (structs)
        firstNonAtomic = (FTtype)structs[0].structureId;

    while (next != prev && structs)
    {
        prev = next;
        next = 0;

        for (SKsize i = 0; i < total; ++i)
        {
            ftBuildStruct& cur = structs[i];

            if (tLens[cur.structureId] != 0)
            {
                SKuint32 pos;
                if ((pos = missingReport.find(cur.name)) != missingReport.npos)
                    missingReport.remove(pos);
            }
            else
            {
                ftBuildMember* member = cur.members.ptr();
                const SKsize   nrEle  = cur.members.size();

                SKsize len    = 0;
                SKsize fake64 = 0;
                bool   hasPtr = false;

                for (SKsize e = 0; e < nrEle; ++e)
                {
                    ftBuildMember& v  = member[e];
                    const SKsize   ct = v.typeId;

                    if (v.ptrCount > 0)
                    {
                        hasPtr = true;
                        if (len % FT_VOID_P)
                        {
                            ftLogger::logAlignment(cur.path.c_str(),
                                                   v.line,
                                                   FT_VOID_P,
                                                   v.type.c_str(),
                                                   v.name.c_str(),
                                                   FT_VOID_P - len % FT_VOID_P);

                            status |= LNK_ALIGNMENT_P;
                        }
                        if (fake64 % 8)
                        {
                            ftLogger::logAlignment(cur.path.c_str(),
                                                   v.line,
                                                   8,
                                                   v.type.c_str(),
                                                   v.name.c_str(),
                                                   8 - fake64 % 8);

                            status |= LNK_ALIGNMENT_P;
                        }
                        len += FT_VOID_P * v.arraySize;
                        fake64 += 8 * v.arraySize;
                    }
                    else if (tLens[ct])
                    {
                        if (ct >= firstNonAtomic)
                        {
                            if (FT_VOID_8 && len % 8)
                            {
                                ftLogger::logAlignment(cur.path.c_str(),
                                                       v.line,
                                                       8,
                                                       v.type.c_str(),
                                                       v.name.c_str(),
                                                       8 - len % 8);
                                status |= LNK_ALIGNMENT_S;
                            }
                        }

                        if (tLens[ct] > 3 && len % 4)
                        {
                            ftLogger::logAlignment(cur.path.c_str(),
                                                   v.line,
                                                   4,
                                                   v.type.c_str(),
                                                   v.name.c_str(),
                                                   4 - len % 4);

                            status |= LNK_ALIGNMENT_4;
                        }
                        else if (tLens[ct] == 2 && len % 2)
                        {
                            ftLogger::logAlignment(cur.path.c_str(),
                                                   v.line,
                                                   2,
                                                   v.type.c_str(),
                                                   v.name.c_str(),
                                                   2 - len % 2);

                            status |= LNK_ALIGNMENT_2;
                        }

                        len += tLens[ct] * v.arraySize;
                        fake64 += tln64[ct] * v.arraySize;
                    }
                    else
                    {
                        next++;
                        len = 0;
                        if (missingReport.find(cur.name) == missingReport.npos)
                            missingReport.push_back(cur.name);

                        tln64[cur.structureId] = 0;
                        tLens[cur.structureId] = 0;
                        break;
                    }
                }

                tln64[cur.structureId] = (FTtype)fake64;
                tLens[cur.structureId] = (FTtype)len;

                if (len != 0)
                {
                    if (hasPtr || fake64 != len)
                    {
                        if (fake64 % 8)
                        {
                            ftLogger::logAlignment(cur.path.c_str(),
                                                   cur.line,
                                                   8,
                                                   nullptr,
                                                   nullptr,
                                                   8 - fake64 % 8);
                            status |= LNK_ALIGNMENT_8;
                        }
                    }

                    if (len % 4)
                    {
                        ftLogger::logAlignment(cur.path.c_str(),
                                               cur.line,
                                               4,
                                               nullptr,
                                               nullptr,
                                               4 - len % 4);
                        status |= LNK_ALIGNMENT_4;
                    }
                }
            }
        }
    }

    if (!missingReport.empty())
    {
        status |= LNK_UNDEFINED_TYPES;
        for (ftId& id : missingReport)
            ftLogger::logF("Missing reference to type '%s'\n", id.c_str());
    }

    return status;
}

ftCompiler::ftCompiler() :
    m_build(new ftTableBuilder()),
    m_curBuf(0),
    m_writeMode(WRITE_ARRAY),
    m_scanner(nullptr)
{
}

ftCompiler::~ftCompiler()
{
    delete m_build;
}

void ftCompiler::makeName(ftBuildMember& v, bool forceArray)
{
    ftId     newName;
    SKuint16 i;
    if (v.isFunctionPointer)
        newName.push_back('(');

    if (v.ptrCount > 0)
    {
        for (i = 0; i < v.ptrCount; ++i)
            newName.push_back('*');
    }

    for (i = 0; i < v.name.size(); ++i)
        newName.push_back(v.name[i]);

    if (v.isFunctionPointer)
    {
        newName.push_back(')');
        newName.push_back('(');
        newName.push_back(')');
    }
    if (v.arraySize > 1 || forceArray)
    {
        if (v.numDimensions > FileTools_MaxArrayDim)
            printf("The number of array dimensions exceeded. Max FT_ARR_DIM_MAX(%i) needed (%i)\n", FileTools_MaxArrayDim, v.numDimensions);
        else
        {
            for (i = 0; i < v.numDimensions; ++i)
            {
                ftId dest;
                newName.push_back('[');
                skSprintf(dest.ptr(), ftId::capacity() - 1, "%i", v.arrays[i]);

                char* cp = dest.ptr();
                for (int j = 0; cp[j]; ++j)
                    newName.push_back(cp[j]);

                newName.push_back(']');
            }
        }
    }
    v.name = newName;
}

int ftCompiler::parse(const char* path, const char* data, SKsize len)
{
    if (!path)
        return -1;

    ftScanner scanner(data, (SKsize)len);

    // Only enable the scanner
    // in the scope of parsing.
    m_scanner = &scanner;
    m_includes.push_back(path);

    const int ret = parse();

    m_scanner = nullptr;
    return ret;
}

int ftCompiler::parse(const char* path)
{
    skFileStream fp;
    fp.open(path, skStream::READ);
    if (!fp.isOpen())
    {
        printf("Error: File loading failed: %s\n", path);
        return -1;
    }

    const SKsize len = fp.size();
    int          rc  = -1;

    if (len == SK_NPOS)
        printf("Error: Failed to determine the length of the file: %s\n", path);
    else
    {
        char* buffer = new char[len + 1];

        const SKsize br = fp.read(buffer, len);
        if (br != SK_NPOS)
        {
            buffer[br] = 0;

            rc = parse(path, buffer, len + 1);
        }
        else
            printf("Error: Failed to read from the file: %s\n", path);

        delete[] buffer;
    }
    return rc;
}

int ftCompiler::parse()
{
    int     token;
    ftToken tokenPtr;

    do
    {
        token = m_scanner->lex(tokenPtr);
        if (token == FT_NAMESPACE)
        {
            token = m_scanner->lex(tokenPtr);
            if (token == FT_ID)
            {
                if (m_namespaces.find(tokenPtr.getValue().c_str()) == m_namespaces.npos)
                    m_namespaces.push_back(tokenPtr.getValue().c_str());
            }
        }
        else if (token == FT_STRUCT || token == FT_CLASS)
            parseClass(token, tokenPtr);

    } while (FT_IS_VALID_TOKEN(token));

    return token;
}

void ftCompiler::parseClass(int& token, ftToken& tokenPtr)
{
    do
    {
        token = m_scanner->lex(tokenPtr);
        if (token == FT_ID)
        {
            ftBuildStruct bs;
            bs.name = tokenPtr.getValue();

            token = m_scanner->lex(tokenPtr);
            if (token == FT_L_BRACKET)
            {
                do
                {
                    token = m_scanner->lex(tokenPtr);
                    if (token != FT_R_BRACKET)
                    {
                        if (token == FT_CLASS || token == FT_STRUCT)
                            token = m_scanner->lex(tokenPtr);

                        if (token >= FT_ID && token <= FT_VOID)
                            parseIdentifier(token, tokenPtr, bs);
                        else
                            errorUnknown(token, tokenPtr);
                    }
                } while (token != FT_R_BRACKET &&
                         FT_IS_VALID_TOKEN(token));

                m_builders.push_back(bs);
            }
        }

    } while (token != FT_R_BRACKET && token != FT_TERM &&
             FT_IS_VALID_TOKEN(token));
}

void ftCompiler::parseIdentifier(int& token, ftToken& tokenPtr, ftBuildStruct& buildStruct)
{
    bool forceArray = false;

    const ftToken::String& typeId = tokenPtr.getValue();

    ftBuildMember cur;
    cur.type      = typeId;
    cur.undefined = 0;

    const bool isId = token == FT_ID;

    do
    {
        token = m_scanner->lex(tokenPtr);
        switch (token)
        {
        case FT_RBRACE:
        case FT_L_BRACE:
            forceArray = true;
            break;
        case FT_CONSTANT:
            handleConstant(token, tokenPtr, cur);
            break;
        case FT_POINTER:
            cur.ptrCount++;
            break;
        case FT_ID:
            cur.name = tokenPtr.getValue();
            break;
        case FT_L_PARENTHESIS:
            cur.name              = tokenPtr.getValue();
            cur.isFunctionPointer = 1;
            cur.ptrCount          = 0;
            break;
        case FT_R_PARENTHESIS:
        case FT_PRIVATE:
        case FT_PUBLIC:
        case FT_PROTECTED:
        case FT_COLON:
            break;
        case FT_TERM:
        case FT_COMMA:
            handleStatementClosure(token,
                                   buildStruct,
                                   cur,
                                   forceArray,
                                   isId);
            break;
        default:
            errorUnknown(token, tokenPtr);
            break;
        }

    } while (token != FT_TERM && FT_IS_VALID_TOKEN(token));
}

void ftCompiler::handleConstant(int& token, ftToken& tokenPtr, ftBuildMember& member)
{
    if (member.numDimensions + 1 > FileTools_MaxArrayDim)
    {
        printf("Maximum number of array slots exceeded!\n");
        printf("define FT_ARR_DIM_MAX to expand.\nCurrent = [] * %i\n", FileTools_MaxArrayDim);
        token = FT_NULL_TOKEN;
    }
    else
    {
        member.arrays[member.numDimensions] = tokenPtr.getArrayLen();
        member.numDimensions++;
        member.arraySize *= tokenPtr.getArrayLen();
    }
}

void ftCompiler::handleStatementClosure(int&           token,
                                        ftBuildStruct& buildStruct,
                                        ftBuildMember& member,
                                        bool           forceArray,
                                        bool           isIdentifier)
{
    makeName(member, forceArray);

    if (isIdentifier && member.ptrCount == 0)
    {
        if (buildStruct.nrDependentTypes > 0)
            buildStruct.nrDependentTypes = buildStruct.nrDependentTypes * 2;
        else
            buildStruct.nrDependentTypes++;

        // Flag it as dependent
        member.isDependentType = true;
    }

    buildStruct.members.push_back(member);

    // reset it for the next iteration
    member.ptrCount  = 0;
    member.arraySize = 1;
    if (token == FT_COMMA)
        member.numDimensions = 0;
}

void ftCompiler::errorUnknown(int& token, ftToken& tokenPtr)
{
    printf("%s(%i): error : Unknown character parsed! '%s'\n",
           m_includes.back().c_str(),
           m_scanner->getLine(),
           tokenPtr.getValue().c_str());
    token = FT_NULL_TOKEN;
}

int ftCompiler::compile()
{
    return m_build->compile(m_builders);
}

void ftCompiler::writeFile(const ftId& tableName, skStream* fp)
{
    if (fp)
    {
        fp->writef("const unsigned char %sTable[]={\n", tableName.c_str());
        m_writeMode = WRITE_ARRAY;
        writeStream(fp);
        fp->writef("\n};\n");
        fp->writef("const int %sLen=sizeof(%sTable);\n", tableName.c_str(), tableName.c_str());
    }
    else
    {
        ftLogger::logF("Invalid write stream");
    }
}

void ftCompiler::writeFile(const ftId& tableName, const ftPath& path)
{
    skFileStream fp;
    fp.open(path.c_str(), skStream::WRITE);
    if (!fp.isOpen())
    {
        printf("Failed to open data file: %s\n", path.c_str());
        return;
    }

    fp.writef("const unsigned char %sTable[]={\n", tableName.c_str());

    m_writeMode = WRITE_ARRAY;
    writeStream(&fp);

    fp.writef("\n};\n");
    fp.writef("const int %sLen=sizeof(%sTable);\n", tableName.c_str(), tableName.c_str());

#if FileTools_TypeLengthValidate == 1
    writeValidationProgram(path.c_str());
#endif
}

void ftCompiler::writeStream(skStream* stream)
{
    int i;
    m_curBuf = -1;

    writeBinPtr(stream, (void*)&ftIdNames::FT_SDNA[0], 4);
    writeBinPtr(stream, (void*)&ftIdNames::FT_NAME[0], 4);
    i = m_build->name.size();

#ifdef FileTools_SwapEndian
    i = ftSwap32(i);
#endif

    writeBinPtr(stream, &i, 4);
    writeCharPtr(stream, m_build->name);

    writeBinPtr(stream, (void*)&ftIdNames::FT_TYPE[0], 4);
    i = m_build->typeLookup.size();

#ifdef FileTools_SwapEndian
    i = ftSwap32(i);
#endif

    writeBinPtr(stream, &i, 4);
    writeCharPtr(stream, m_build->typeLookup);
    writeBinPtr(stream, (void*)&ftIdNames::FT_TLEN[0], 4);

#ifdef FileTools_SwapEndian
    for (i = 0; i < (int)m_build->m_tlen.size(); i++)
        m_build->m_tlen.at(i) = ftSwap16(m_build->m_tlen.at(i));
#endif

    writeBinPtr(stream, m_build->typeLengths.ptr(), m_build->allocationSizes.lengths);
    if (m_build->typeLengths.size() & 1)
    {
        char pad[2] = {'@', '@'};
        writeBinPtr(stream, (void*)&pad[0], 2);
    }

    writeBinPtr(stream, (void*)&ftIdNames::FT_STRC[0], 4);
    i = m_builders.size();

#ifdef FileTools_SwapEndian
    i = ftSwap32(i);
#endif
    writeBinPtr(stream, &i, 4);

#ifdef FileTools_SwapEndian
    for (i = 0; i < (int)m_build->m_strc.size(); i++)
        m_build->m_strc.at(i) = ftSwap16(m_build->m_strc.at(i));
#endif

    writeBinPtr(stream, m_build->structures.ptr(), m_build->allocationSizes.structures);
}

void ftCompiler::writeCharPtr(skStream* fp, const ftStringPtrArray& pointers)
{
    char           pad[4] = {'b', 'y', 't', 'e'};
    SKuint32       i      = 0;
    const SKuint32 s      = pointers.size();
    SKuint32       t      = 0;

    while (i < s)
    {
        ftId id = pointers[i++];
        id.push_back('\0');
        writeBinPtr(fp, (void*)id.c_str(), id.size());
        t += id.size();
    }

    int len = t;

    len = len + 3 & ~3;
    if (len - t)
    {
        ftId id;
        for (SKuint32 p = 0; p < len - t; p++)
            id.push_back(pad[p % 4]);

        writeBinPtr(fp, (void*)id.c_str(), id.size());
    }
}

void ftCompiler::writeBinPtr(skStream* fp, void* ptr, int len)
{
    if (m_writeMode == WRITE_ARRAY)
    {
        unsigned char* cb = (unsigned char*)ptr;

        for (int i = 0; i < len; ++i, ++m_curBuf)
        {
            if (m_curBuf % 16 == 15)
                fp->write("\n", 1);
            fp->writef("0x%02X,", (unsigned char)cb[i]);
        }
    }
    else
        fp->write(ptr, len);
}

#define ToString(x) #x

void ftCompiler::writeValidationProgram(const ftPath& path)
{
#if FileTools_TypeLengthValidate == 1

    ftPath      string;
    ftPathArray split;
    path.split(split, '/', '\\');

    int last = 0;
    for (SKuint16 i = path.size() - 1; i > 0; --i)
    {
        if (path[i] == '.')
        {
            last = i;
            break;
        }
    }

    for (SKuint16 i = 0; i < path.size(); ++i)
    {
        if (i >= last)
            break;

        string.push_back(path[i]);
    }

    string += "Validator.cpp";

    skFileStream fp;
    fp.open(string.c_str(), skStream::WRITE);
    if (!fp.isOpen())
    {
        printf("Failed to open validation file %s\n", string.c_str());
        return;
    }

    for (const ftPath& include : m_includes)
        fp.writef("#include \"%s\"\n", include.c_str());

    fp.writef("#include <cstdlib>\n");
    fp.writef("#include <cstdio>\n\n");
    fp.writef("#define ToString(x) #x\n");
    fp.writef("#define AssertFailed(typeName, expected, actual) ");
    const char* extra = ToString(
        fprintf(stderr,
                "%s(%i): error : Validation failed with ( %i = sizeof(%s) ) != %i\n",
                __FILE__,
                __LINE__,
                actual,
                typeName,
                expected););
    fp.writef("%s\n", extra);

    if (!m_namespaces.empty())
    {
        for (const ftId& ns : m_namespaces)
            fp.writef("using namespace %s;\n\n\n", ns.c_str());
    }

    fp.writef("int main()\n{\n\tint errors=0;\n");

    ftBuildStructArray::Iterator it = m_builders.iterator();
    while (it.hasMoreElements())
    {
        ftBuildStruct& bs = it.getNext();

        ftId&        cur = m_build->typeLookup.at((SKuint32)bs.structureId);
        const FTtype len = m_build->typeLengths.at((SKuint32)bs.structureId);

        if (m_skip.find(cur) != m_skip.npos)
            continue;
#ifdef FileTools_SwapEndian
        len = ftSwap16(len);
#endif
        fp.writef("\tif (sizeof(%s) != %i)\n\t{\n\t\terrors ++;\n", cur.c_str(), len);
        fp.writef("\t\t");
        fp.writef("AssertFailed(ToString(%s), %i, (int)sizeof(%s));\n", cur.c_str(), len, cur.c_str());
        fp.writef("\t}\n\n");
    }

    fp.writef("\t");
    fp.writef(
        "if (errors > 0) fprintf(stderr, \"%%s(%%i): error : "
        "there are %%i misaligned types.\\n\", __FILE__, __LINE__, errors);\n");

    fp.writef("\treturn errors == 0 ? 0 : 1;\n}\n");

#endif
}
