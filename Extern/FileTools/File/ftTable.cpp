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
#include "ftTable.h"
#include "Utils/skPlatformHeaders.h"
#include "Utils/skString.h"
#include "ftEndianUtils.h"
#include "ftHashTypes.h"
#include "ftLogger.h"
#include "ftMember.h"
#include "ftStreams.h"

using namespace ftEndianUtils;
using namespace ftFlags;

const ftName ftTable::InvalidName = {};
const ftType ftTable::InvalidType = {};

ftTable::ftTable(const SKuint8 pointerLength) :
    m_names(nullptr),
    m_types(nullptr),
    m_sizes(nullptr),
    m_strcs(nullptr),
    m_nameCount(0),
    m_typeCount(0),
    m_strcCount(0),
    m_firstStruct(0),
    m_ptrLength(pointerLength)
{
}

ftTable::~ftTable()
{
    clearTables();
}

ftStruct* ftTable::findStructByName(const ftCharHashKey& key)
{
    const SKuint32 i = findTypeId(key);
    if (i != SK_NPOS32)
        return m_structures.at(i);
    return nullptr;
}

void ftTable::clearTables(void)
{
    if (!m_structures.empty())
    {
        for (ftStruct* structure : m_structures)
            delete structure;
    }

    m_typeFinder.clear();
    m_hashedNames.clear();

    if (m_names)
    {
        free(m_names);
        m_names = nullptr;
    }

    if (m_types)
    {
        free(m_types);
        m_types = nullptr;
    }

    if (m_sizes)
    {
        free(m_sizes);
        m_sizes = nullptr;
    }
    if (m_strcs)
    {
        free(m_strcs);
        m_strcs = nullptr;
    }
    m_nameCount = 0;
    m_typeCount = 0;
    m_strcCount = 0;
}

int ftTable::allocateTable(void**       destination,
                           const SKsize numberOfEntries,
                           const SKsize sizeOfEntry,
                           const int    fileFlags)
{
    if (!destination)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Invalid storage pointer.");
        return RS_LIMIT_REACHED;
    }

    if (numberOfEntries > FileTools_MaxTableSize || (int)numberOfEntries <= 0)
    {
        if (fileFlags != LF_NONE)
        {
            ftLogger::logF(
                "Invalid table entry count(%d): "
                "valid count should be [1, %d]",
                numberOfEntries,
                FileTools_MaxTableSize);
        }
        return RS_LIMIT_REACHED;
    }

    const SKsize allocLen = numberOfEntries * sizeOfEntry;
    if (allocLen > 0 && allocLen < SK_NPOS32)
    {
        *destination = (Names)malloc(allocLen);
        if (!*destination)
        {
            if (fileFlags != LF_NONE)
                ftLogger::logF("Failed to allocate table.");
            return RS_BAD_ALLOC;
        }

        // This doesn't explicitly need to be zeroed.
        // The memory will be initialized after this
        // call. This should only used be to
        // weed out any possible bugs.
        if (fileFlags & LF_DO_CHECKS)
            memset(*destination, 0, allocLen);
    }
    else
    {
        if (fileFlags != LF_NONE)
        {
            ftLogger::logF(
                "The sizeOfEntry(%d) parameter caused the "
                "allocation limit to be exceeded (%d)",
                sizeOfEntry,
                allocLen);
        }
        return RS_BAD_ALLOC;
    }
    return FS_OK;
}

int ftTable::read(const void*   tableSource,
                  const SKsize& tableLength,
                  const int     headerFlags,
                  const int     fileFlags)
{
    if (!tableSource)
        return RS_INVALID_PTR;

    ftMemoryStream stream;
    stream.open((const SKbyte*)tableSource, tableLength, 0, true);

    int status = readTableHeader(stream, ftIdNames::FT_SDNA, fileFlags);
    if (status != FS_OK)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Failed to read the table header.");
        return status;
    }

    status = readNameTable(stream, headerFlags, fileFlags);
    if (status != FS_OK)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Failed to read the name table.");
        clearTables();
        return status;
    }

    status = readTypeTable(stream, headerFlags, fileFlags);
    if (status != FS_OK)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Failed to read the type table.");
        clearTables();
        return status;
    }

    status = readSizeTable(stream, headerFlags, fileFlags);
    if (status != FS_OK)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Failed to read the size table.");
        clearTables();
        return status;
    }

    status = readStructureTable(stream, headerFlags, fileFlags);
    if (status != FS_OK)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Failed to read the structure table.");
        clearTables();
        return status;
    }

    status = compile(fileFlags);
    if (status == FS_OK)
    {
        if (fileFlags & LF_DIAGNOSTICS)
        {
            if (fileFlags & LF_DUMP_NAME_TABLE)
            {
                for (SKuint32 i = 0; i < m_nameCount; ++i)
                    ftLogger::log(m_names[i]);
            }

            if (fileFlags & LF_DUMP_TYPE_TABLE)
            {
                for (SKuint32 i = 0; i < m_typeCount; ++i)
                    ftLogger::log(m_types[i]);
            }

            if (fileFlags & LF_DUMP_SIZE_TABLE)
            {
                for (SKuint32 i = 0; i < m_typeCount; ++i)
                    ftLogger::log(m_types[i], m_sizes[i]);
            }
        }
    }
    return status;
}

int ftTable::readTableHeader(ftMemoryStream& stream, const char* headerName, const int fileFlags)
{
    char cp[5] = {};
    stream.read(cp, 4);

    if (skChar::equalsn(cp, headerName, 4) != 0)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Table is missing the %s code.", headerName);
        return RS_INVALID_CODE;
    }
    return FS_OK;
}

int ftTable::readNameTable(ftMemoryStream& stream, int headerFlags, int fileFlags)
{
    ftName   name{};
    SKuint32 count = 0;

    SKint32 status = readTableHeader(stream, ftIdNames::FT_NAME, fileFlags);
    if (status == FS_OK)
    {
        stream.readInt32(count);
        if (headerFlags & FH_ENDIAN_SWAP)
            count = swap32(count);

        status = allocateTable((void**)&m_names, count, sizeof(ftName), fileFlags);
        if (status == FS_OK)
        {
            m_hashedNames.reserve(count);

            for (m_nameCount = 0; m_nameCount < count && status == FS_OK; ++m_nameCount)
            {
                SKbyte* cp = stream.addressAtPosition();
                if (cp != nullptr)
                {
                    if ((status = convertName(name, cp)) != FS_OK)
                        return status;

                    m_names[m_nameCount] = name;
                    m_hashedNames.push_back(name.hash);
                }
                else
                    status = FS_INV_VALUE;

                if (stream.seekString() == SK_NPOS)
                    status = FS_INV_VALUE;
            }

            if (status == FS_OK)
            {
                count = (SKuint32)stream.getVaryingInt();
                count = (count + 3 & ~3) - count;
                if (count)
                    stream.seek(count, SEEK_CUR);
            }
        }
    }
    return status;
}

int ftTable::readTypeTable(ftMemoryStream& stream, const int headerFlags, const int fileFlags)
{
    SKuint32 count  = 0;
    SKuint32 status = readTableHeader(stream, ftIdNames::FT_TYPE, fileFlags);
    if (status == FS_OK)
    {
        stream.readInt32(count);
        if (headerFlags & FH_ENDIAN_SWAP)
            count = swap32(count);

        status = allocateTable((void**)&m_types, count, sizeof(ftType), fileFlags);
        if (status == FS_OK)
        {
            for (m_typeCount = 0; m_typeCount < count && status == FS_OK; ++m_typeCount)
            {
                SKbyte* cp = stream.addressAtPosition();
                if (cp != nullptr)
                {
                    m_types[m_typeCount].name = cp;
                    m_types[m_typeCount].hash = skHash(cp);
                    m_types[m_typeCount].id   = SK_NPOS32;
                }
                else
                    status = FS_INV_VALUE;

                if (stream.seekString() == SK_NPOS)
                    status = FS_INV_VALUE;
            }

            if (status == FS_OK)
            {
                count = (SKuint32)stream.getVaryingInt();
                count = (count + 3 & ~3) - count;
                if (count)
                    stream.seek(count, SEEK_CUR);
            }
        }
    }
    return status;
}

int ftTable::readSizeTable(ftMemoryStream& stream, const int headerFlags, const int fileFlags)
{
    SKuint32 status = readTableHeader(stream, ftIdNames::FT_TLEN, fileFlags);
    if (status == FS_OK)
    {
        status = allocateTable((void**)&m_sizes, m_typeCount, sizeof(FTtype), fileFlags);
        if (status == FS_OK)
        {
            SKuint16 type;

            for (SKuint32 i = 0; i < m_typeCount; ++i)
            {
                stream.readInt16(type);
                if (headerFlags & FH_ENDIAN_SWAP)
                    type = swap16(type);
                m_sizes[i] = type;
            }

            if (m_typeCount & 1)
                stream.seek(sizeof(SKuint16), SEEK_CUR);
        }
    }
    return status;
}

int ftTable::readStructureTable(ftMemoryStream& stream, const int headerFlags, const int fileFlags)
{
    SKuint32 count = 0;

    SKuint32 status = readTableHeader(stream, ftIdNames::FT_STRC, fileFlags);
    if (status == FS_OK)
    {
        stream.readInt32(count);
        if (headerFlags & FH_ENDIAN_SWAP)
            count = swap32(count);

        status = allocateTable((void**)&m_strcs,
                               count,
                               sizeof(FTtype) * FileTools_MaxMember,
                               fileFlags);

        if (status == FS_OK)
        {
            m_typeFinder.reserve(m_typeCount);
            SKuint16* tp = (SKuint16*)stream.addressAtPosition();

            m_strcCount = 0;
            while (m_strcCount < count && status == FS_OK)
            {
                status = buildStruct(tp, m_strcCount, headerFlags, fileFlags);
                m_strcCount++;
            }
        }
    }
    return status;
}

int ftTable::convertName(ftName& dest, char* cp) const
{
    dest = InvalidName;
    if (!cp)
        return FS_INV_VALUE;

    // All of the names are a reference to the block of data
    // that houses the tables. This is storing the address
    // of the current name's location in the buffer.
    dest.name      = cp;
    dest.hash      = skHash(dest.name);
    dest.arraySize = 1;

    const SKsize len = skChar::length(dest.name);

    // make sure that the name is in some sane limit.
    if (len > FileTools_MaxCharArray)
        return FS_INV_VALUE;

    int status = FS_OK;

    SKint8 i = 0;
    for (SKsize n = 0, k; n < len && status == FS_OK; ++n)
    {
        const SKint8 ch = cp[n];
        int          av = 0;
        switch (ch)
        {
        case ')':
        case ']':
            break;
        case '(':
            dest.isFunctionPointer = 1;
            break;
        case '*':
            dest.pointerCount++;
            break;
        case '[':
            for (k = n + 1; k < len && status == FS_OK; ++k, ++n)
            {
                const SKuint8 nch = cp[k];
                if (nch >= '0' && nch <= '9')
                    av = av * 10 + (nch - '0');
                else if (nch == ']')
                    break;
                else
                {
                    ftLogger::logF(
                        "Invalid character parsed when building "
                        "an array dimension '0x%02X'\n",
                        nch);
                    status = FS_INV_VALUE;
                }
            }
            if (status == FS_OK)
            {
                if (i < FileTools_MaxArrayDim)
                {
                    dest.dimensions[i] = av;
                    dest.arraySize *= dest.dimensions[i++];
                }
                else
                {
                    ftLogger::logF(
                        "Array dimension "
                        "overflow(%i:%i)\n",
                        i,
                        FileTools_MaxArrayDim);
                    status = FS_INV_VALUE;
                }
            }
            break;
        case '\0':
            break;
        default:
            // Filter over the range of valid characters
            // that can define a c/c++ identifier.
            bool valid = ch >= 'a' && ch <= 'z';
            valid      = valid || ch >= 'A' && ch <= 'Z';
            valid      = valid || ch >= '0' && ch <= '9';
            valid      = valid || ch >= '_';
            if (!valid)
            {
                ftLogger::logF(
                    "Read an invalid character in "
                    "the name table: '0x%02X'",
                    (SKuint8)ch);
                status = FS_INV_VALUE;
            }
            break;
        }
    }

    if (i > FileTools_MaxArrayDim)
        dest.dimensionCount = 0;
    else
        dest.dimensionCount = i;
    return status;
}

int ftTable::buildStruct(SKuint16*&     structure,
                         const SKuint16 current,
                         const int      headerFlags,
                         const int      fileFlags)
{
    int j, k, status = FS_OK;

    if (!m_strcs || !structure)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Invalid structure table");
        return RS_BAD_ALLOC;
    }

    m_strcs[current] = structure;

    if (headerFlags & FH_ENDIAN_SWAP)
    {
        structure[0] = swap16(structure[0]);
        structure[1] = swap16(structure[1]);

        status = isValidTypeName(structure[0], structure[1], fileFlags);
        if (status == FS_OK)
        {
            // Reassign the current id to the type
            // so it can be referenced later.
            m_types[structure[0]].id = current;

            const ftCharHashKey chk(m_types[structure[0]].name);

            if (!m_typeFinder.insert(chk, m_types[structure[0]]))
            {
                if (fileFlags != LF_NONE)
                    ftLogger::logF("Failed to insert the type name for structure(%d).", structure[0]);
                status = RS_LIMIT_REACHED;
            }
            else
            {
                k = structure[1];
                if (k < FileTools_MaxMember)
                {
                    j = 0;
                    structure += 2;
                    while (j < k && status == FS_OK)
                    {
                        structure[0] = swap16(structure[0]);
                        structure[1] = swap16(structure[1]);

                        status = isValidTypeName(structure[0], structure[1], fileFlags);
                        if (status == FS_OK)
                        {
                            ++j;
                            structure += 2;
                        }
                    }
                }
                else
                {
                    if (fileFlags != LF_NONE)
                        ftLogger::logF("Max members exceeded(%d).", FileTools_MaxMember);
                    status = RS_LIMIT_REACHED;
                }
            }
        }
    }
    else
    {
        if (structure[1] < FileTools_MaxMember)
        {
            // Reassign the current id to the type
            // so it can be referenced later.
            m_types[structure[0]].id = current;

            const ftCharHashKey chk(m_types[structure[0]].name);

            if (!m_typeFinder.insert(chk, m_types[structure[0]]))
            {
                if (fileFlags != LF_NONE)
                    ftLogger::logF("Failed to insert the type name for structure(%d) '%s'.",
                                   structure[0],
                                   m_types[structure[0]].name);
                status = RS_LIMIT_REACHED;
            }
            else if (fileFlags & LF_DO_CHECKS)
            {
                // Check all members for consistent values.

                k = structure[1];
                j = 0;
                structure += 2;
                while (j < k && status == FS_OK)
                {
                    status = isValidTypeName(structure[0], structure[1], fileFlags);
                    if (status == FS_OK)
                    {
                        ++j;
                        structure += 2;
                    }
                }
            }
            else
            {
                // skip past the members
                structure += 2 * structure[1] + 2;
            }
        }
        else
        {
            if (fileFlags != LF_NONE)
                ftLogger::logF("Max members exceeded(%d).", FileTools_MaxMember);
            status = RS_LIMIT_REACHED;
        }
    }
    return status;
}

int ftTable::isValidTypeName(const SKuint16& type, const SKuint16& name, const int flags) const
{
    if (type > m_typeCount)
    {
        if (flags != LF_NONE)
        {
            ftLogger::logF(
                "The parsed structure type (%d) exceeds the number of types(%d)",
                type,
                m_typeCount);
        }
        return RS_LIMIT_REACHED;
    }

    if (name > m_nameCount)
    {
        if (flags != LF_NONE)
        {
            ftLogger::logF(
                "The parsed structure name (%d) exceeds the number of names(%d)",
                name,
                m_nameCount);
        }
        return RS_LIMIT_REACHED;
    }
    return FS_OK;
}

bool ftTable::testDuplicateKeys() const
{
    bool testResult = true;

    SKuint32 i, j;
    for (i = 0; i < m_typeCount && testResult; ++i)
    {
        for (j = 0; j < m_typeCount && testResult; ++j)
            if (i != j)
                testResult = m_types[i].hash != m_types[j].hash;
    }

    if (testResult)
    {
        for (i = 0; i < m_nameCount && testResult; ++i)
        {
            for (j = 0; j < m_nameCount && testResult; ++j)
                if (i != j)
                    testResult = m_names[i].hash != m_names[j].hash;
        }
    }
    return testResult;
}

int ftTable::compile(const int fileFlags)
{
    if (!m_strcs || m_strcCount <= 0)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("No structures to compile.");
        return FS_TABLE_INIT_FAILED;
    }

    m_structures.reserve(m_strcCount);
    int status = FS_OK;

    // Save the first structure type index
    // So the isBuiltin test can determine
    // from a type index whether or not
    // the supplied type represents a struct.
    // Types in the type table are stored with
    // atomic types first.
    m_firstStruct = m_strcs[0][0];

    for (SKuint16 i = 0; i < m_strcCount && status == FS_OK; i++)
    {
        FTtype* structure = m_strcs[i];

        const FTtype type = structure[0];
        if (type > m_typeCount)
        {
            status = RS_LIMIT_REACHED;
            if (fileFlags != LF_NONE)
            {
                ftLogger::logF(
                    "The parsed structure type (%d) "
                    "exceeds the number of types(%d)",
                    type,
                    m_typeCount);
            }
        }
        else
        {
            if (m_types[type].hash == SK_NPOS)
            {
                status = RS_MIS_ALIGNED;
                if (fileFlags != LF_NONE)
                {
                    ftLogger::logF(
                        "The current structure (%d) is out "
                        "of alignment with the type table.",
                        type);
                }
            }
            else
            {
                const SKuint32 depth = 0;
                SKuint32       cof   = 0;

                ftStruct* newStruct;
                newStruct                = new ftStruct(this);
                newStruct->m_type        = type;
                newStruct->m_hashedType  = m_types[type].hash;
                newStruct->m_structureId = i;
                newStruct->m_sizeInBytes = m_sizes[type];
                newStruct->m_link        = nullptr;
                newStruct->m_flag        = ftStruct::CAN_LINK;
                m_structures.push_back(newStruct);

                const SKuint16 memberCount = structure[1];

                structure += 2;
                newStruct->m_members.reserve(memberCount);

                for (SKuint16 e = 0; e < memberCount && status == FS_OK; ++e, structure += 2)
                {
                    const short& subType = structure[0];
                    const short& name    = structure[1];

                    if (subType >= m_firstStruct && m_names[name].pointerCount == 0)
                    {
                        newStruct->addRef();
                        compile(m_types[subType].id,
                                &m_names[name],
                                m_names[name].arraySize,
                                newStruct,
                                cof,
                                depth + 1,
                                fileFlags,
                                status);

                        newStruct->m_flag |= ftStruct::HAS_DEPENDENT;
                    }
                    else
                    {
                        putMember(newStruct->m_type,
                                  nullptr,
                                  structure,
                                  newStruct,
                                  0,
                                  cof,
                                  0,
                                  fileFlags,
                                  status);
                    }
                }

                if (cof != newStruct->m_sizeInBytes)
                {
                    newStruct->m_flag |= ftStruct::MISALIGNED;

                    if (fileFlags != LF_NONE)
                    {
                        ftLogger::logF("Misaligned struct %s:%i:%i:%i\n",
                                       m_types[newStruct->m_type].name,
                                       i,
                                       cof,
                                       newStruct->m_sizeInBytes);
                    }
                    status = RS_MIS_ALIGNED;
                }
            }
        }
    }
    return status;
}

void ftTable::compile(FTtype    owningStructureType,
                      ftName*   owningStructureName,
                      FTtype    memberCount,
                      ftStruct* root,
                      SKuint32& currentOffset,
                      SKuint32  recursiveDepth,
                      int       fileFlags,
                      int&      status) const
{
    if (owningStructureType > m_strcCount)
    {
        if (fileFlags != LF_NONE)
            ftLogger::logF("Missing recursive type.");
        status = RS_LIMIT_REACHED;
    }
    else
    {
        for (SKuint32 a = 0; a < memberCount && status == FS_OK; ++a)
        {
            FTtype* structure = m_strcs[owningStructureType];

            const FTtype   structureType = structure[0];
            const SKuint32 oldOffset     = currentOffset;
            const SKuint32 origLen       = m_sizes[structure[0]];

            const SKuint32 length = structure[1];
            structure += 2;

            for (SKuint32 e = 0; e < length; e++, structure += 2)
            {
                const short& type = structure[0];
                const short& name = structure[1];

                if (type >= m_firstStruct && m_names[name].pointerCount == 0)
                {
                    root->addRef();

                    compile(m_types[type].id,
                            &m_names[name],
                            m_names[name].arraySize,
                            root,
                            currentOffset,
                            recursiveDepth + 1,
                            fileFlags,
                            status);
                }
                else
                {
                    putMember(structureType,
                              owningStructureName,
                              structure,
                              root,
                              a,
                              currentOffset,
                              recursiveDepth,
                              fileFlags,
                              status);
                }
            }

            if (currentOffset - oldOffset != origLen)
            {
                printf("Build ==> invalid offset (%i)(%i:%i)\n",
                       a,
                       currentOffset - oldOffset,
                       origLen);
                status = RS_MIS_ALIGNED;
            }
        }
    }
}

void ftTable::putMember(const FTtype   owningStructureType,
                        ftName*        owningStructureName,
                        const FTtype*  currentMember,
                        ftStruct*      root,
                        const FTtype   index,
                        SKuint32&      currentOffset,
                        const SKuint32 recursiveDepth,
                        const int      fileFlags,
                        int&           status) const
{
    const SKuint16& type = currentMember[0];
    const SKuint16& name = currentMember[1];

    bool limitTest = (SKint16)type < 0 ||
                     type >= m_typeCount ||
                     owningStructureType >= m_typeCount;
    if (!limitTest)
        limitTest = (SKint16)name < 0 || name >= m_nameCount;

    if (limitTest)
        status = RS_LIMIT_REACHED;
    else
    {
        if (type >= m_firstStruct)
            root->m_flag |= ftStruct::HAS_DEPENDENT;

        skString hash;

        if (!owningStructureName)
        {
            hashMember(hash,
                       root->getHashedType(),
                       SK_NPOS,
                       SK_NPOS,
                       m_types[type].hash,
                       m_names[name].hash);
        }
        else
        {
            hashMember(hash,
                       root->getHashedType(),
                       m_types[owningStructureType].hash,
                       owningStructureName->hash,
                       m_types[type].hash,
                       m_names[name].hash);
        }

        ftMember* member = root->createMember();
        member->setTypeIndex(type);
        member->setNameIndex(name);
        member->m_searchKey      = skHash(hash);
        member->m_offset         = currentOffset;
        member->m_location       = index;
        member->m_recursiveDepth = recursiveDepth;
        member->m_link           = nullptr;

        const ftMemberSearchKey msk = {member->m_searchKey, member};
        root->m_memberSearch.insert(msk);

        if (m_names[name].pointerCount > 0)
            member->m_sizeInBytes = m_ptrLength * m_names[name].arraySize;
        else
            member->m_sizeInBytes = m_sizes[type] * m_names[name].arraySize;

        currentOffset += member->m_sizeInBytes;

        if (fileFlags & LF_DUMP_MEMBER_HASH)
            ftLogger::logF("%s", hash.c_str());
    }
}

void ftTable::hashMember(skString&    name,
                         const SKhash parentStructName,
                         const SKhash owningStructType,
                         const SKhash owningStructMemberName,
                         const SKhash memberType,
                         const SKhash memberName) const
{
    // TODO: this needs to take into account types
    //       that may change but still may be usable
    //  EG;
    //   SomeStruct-SubStruct-int-val
    //   SomeStruct-SubStruct-long-val
    //
    // In this case the member currently will not be found.
    // But since the types are related enough to still be usable,
    // this needs to define the type as such:
    //
    //   SomeStruct-SubStruct-number_t-val
    //
    name.reserve(96);

    if (owningStructMemberName == SK_NPOS)
    {
        skString::format(name,
                         FT_MEMBER_HASH_FMT,
                         parentStructName,
                         memberType,
                         memberName,
                         memberType,
                         memberName);
    }
    else
    {
        skString::format(name,
                         FT_MEMBER_HASH_FMT,
                         parentStructName,
                         owningStructType,
                         owningStructMemberName,
                         memberType,
                         memberName);
    }
}

ftStruct* ftTable::findStructByType(const SKint32& typeIdx)
{
    const SKint32 size = (SKint32)m_structures.size();
    if (typeIdx > 0 && typeIdx < size && size > 0)
    {
        ftStruct* structure = m_structures.at(typeIdx);
        if (typeIdx != structure->m_structureId)
            ftLogger::logF("Type mismatch!");
        return structure;
    }
    return nullptr;
}

SKuint32 ftTable::findTypeId(const ftCharHashKey& type)
{
    const SKsize pos = m_typeFinder.find(type);
    if (pos != m_typeFinder.npos)
        return m_typeFinder.at(pos).id;
    return SK_NPOS32;
}
