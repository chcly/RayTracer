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
#ifndef _ftTables_h_
#define _ftTables_h_

#include "Utils/skArray.h"
#include "Utils/skMap.h"
#include "ftHashTypes.h"
#include "ftStreams.h"
#include "ftStruct.h"
#include "ftTypes.h"

/// <summary>
/// Grouping of common type codes.
/// </summary>
namespace ftIdNames
{
    const char FT_SDNA[5] = {'S', 'D', 'N', 'A', '\0'};
    const char FT_NAME[5] = {'N', 'A', 'M', 'E', '\0'};  // Name array
    const char FT_TYPE[5] = {'T', 'Y', 'P', 'E', '\0'};  // Type Array
    const char FT_TLEN[5] = {'T', 'L', 'E', 'N', '\0'};  // Type length array
    const char FT_STRC[5] = {'S', 'T', 'R', 'C', '\0'};  // Struct/Class Array
    const char FT_OFFS[5] = {'O', 'F', 'F', 'S', '\0'};  // Offset map (Optional & TODO)

    const SKuint32 SDNA = FT_TYPEID('S', 'D', 'N', 'A');
    const SKuint32 DNA1 = FT_TYPEID('D', 'N', 'A', '1');
    const SKuint32 ENDB = FT_TYPEID('E', 'N', 'D', 'B');
    const SKuint32 DATA = FT_TYPEID('D', 'A', 'T', 'A');
    const SKuint32 TEST = FT_TYPEID('T', 'E', 'S', 'T');
    const SKuint32 REND = FT_TYPEID('R', 'E', 'N', 'D');
}  // namespace ftIdNames

/// <summary>
/// ftTable is the main class for handling the structure tables.
/// </summary>
class ftTable
{
public:
    /// <summary>
    /// Definition for the NAME table.
    /// </summary>
    typedef ftName* Names;

    /// <summary>
    /// Definition for the TYPE table.
    /// </summary>
    typedef ftType* Types;

    /// <summary>
    /// Definition for the TLEN table.
    /// </summary>
    typedef FTtype* TypeLengths;

    /// <summary>
    /// Definition for the STRC table.
    /// </summary>
    typedef FTtype** StructurePointers;

    typedef skArray<SKsize> NameHash;

    typedef skArray<ftStruct*>                 StructureArray;
    typedef skHashTable<ftCharHashKey, ftType> TypeFinder;

    static const ftName InvalidName;
    static const ftType InvalidType;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    /// <param name="pointerLength">
    /// Should be the sizeof(void*) from the perspective of the table.
    /// </param>
    explicit ftTable(SKuint8 pointerLength);

    ~ftTable();

    /// <summary>
    /// Rebuilds the table memory.
    /// </summary>
    /// <param name="tableSource">The compiled memory table.</param>
    /// <param name="tableLength">The length of the supplied memory.</param>
    /// <param name="headerFlags">Are the ftFlags::FileMagic flags that were extracted from the file header.</param>
    /// <param name="fileFlags">A copy of the logging flags that are set in ftFile::setFileFlags.</param>
    /// <returns>One of the status codes that are found in ftFlags::FileStatus.</returns>
    int read(const void*   tableSource,
             const SKsize& tableLength,
             int           headerFlags,
             int           fileFlags);

    /// <summary>
    /// Searches for a named data type in the TYPE table.
    /// </summary>
    /// <param name="type">A hashed version of the data type name.</param>
    /// <returns>
    /// The data types index into the type table, if it is
    /// found, otherwise returns SK_NPOS32.
    /// </returns>
    SKuint32 findTypeId(const ftCharHashKey& type);


    /// <summary>
    /// Returns an array of ftStruct types.
    /// </summary>
    const StructureArray& getStructureArray() const
    {
        return m_structures;
    }

    /// <summary>
    /// Returns an array Iterator for ftStruct types.
    /// </summary>
    StructureArray::Iterator getStructIterator()
    {
        return m_structures.iterator();
    }

    /// <summary>
    /// Returns the size of a pointer from the perspective of the table. 
    /// </summary>
    SKuint8 getSizeofPointer() const
    {
         // FIXME: It depends on the correct flag being set when loaded it's not
         // actually being computed, so this needs fixed.
        return m_ptrLength;
    }


    /// <summary>
    /// Performs a hash table lookup for the supplied key
    /// </summary>
    /// <param name="key">Should be the structure's defined name.</param>
    /// <returns>
    /// If found, an instance of the structure that is stored by
    /// the supplied name otherwise returns null.
    /// </returns>
    ftStruct* findStructByName(const ftCharHashKey& key);

    /// <summary>
    /// Attempts to find the structure at the supplied index.
    /// </summary>
    /// <param name="typeIdx">If the type index is known in advance then
    /// this method directly accesses the structure array at the supplied index.
    /// </param>
    /// <returns>
    /// The structure at the supplied index or null
    /// if the supplied index is out of bounds.
    /// </returns>
    ftStruct* findStructByType(const SKint32& typeIdx);


    /// <summary>
    /// Returns the total number of names in the NAME table.
    /// </summary>
    SKuint32 getNumberOfNames() const
    {
        return m_nameCount;
    }

    /// <summary>
    /// Returns a name record at the supplied index.
    /// </summary>
    const ftName& getNameAt(const SKuint32 idx) const
    {
        if (idx < m_nameCount)
            return m_names[idx];
        return InvalidName;
    }

    /// <summary>
    /// Returns the total number of types in the TYPE table.
    /// </summary>
    SKuint32 getNumberOfTypes() const
    {
        return m_typeCount;
    }

    /// <summary>
    /// Returns a type record at the supplied index.
    /// </summary>
    const ftType& getTypeAt(const SKuint32 idx) const
    {
        if (idx < m_typeCount)
            return m_types[idx];
        return InvalidType;
    }

    /// <summary>
    /// Returns a type name at the supplied index or null if the index is out of bounds.
    /// </summary>
    char* getTypeNameAt(const SKuint32 idx) const
    {
        if (idx < m_typeCount)
            return m_types[idx].name;
        return nullptr;
    }

    /// <summary>
    /// Returns the total number of structures in the STRC table.
    /// </summary>
    SKuint32 getNumberOfStructs() const
    {
        return m_strcCount;
    }

    /// <summary>
    /// Returns a record of a structure at the supplied index or null if the index is out of bounds.
    /// </summary>
    FTtype* getStructAt(SKuint32 idx) const
    {
        if (idx < m_strcCount)
            return m_strcs[idx];
        return nullptr;
    }

    /// <summary>
    /// Returns the offset in the type table that defines where atomic types stop
    /// and user-defined structures begin.
    /// </summary>
    SKuint16 getFirstStructType() const
    {
        return m_firstStruct;
    }


    /// <summary>
    /// Returns a const reference to the TYPE table.
    /// </summary>
    const Types& getTypes() const
    {
        return m_types;
    }

    /// <summary>
    /// Checks the validity of the computed hashed names.
    /// </summary>
    /// <returns>True if they are unique.</returns>
    bool testDuplicateKeys() const;

private:
    friend class ftStruct;
    friend class ftMember;

    Names             m_names;
    Types             m_types;
    TypeLengths       m_sizes;
    StructurePointers m_strcs;
    NameHash          m_hashedNames;
    SKuint16          m_nameCount;
    SKuint16          m_typeCount;
    SKuint16          m_strcCount;
    SKuint16          m_firstStruct;
    StructureArray    m_structures;
    SKuint8           m_ptrLength;
    TypeFinder        m_typeFinder;

    static int readTableHeader(
        ftMemoryStream& stream,
        const char*     headerName,
        int             fileFlags);

    int readNameTable(
        ftMemoryStream& stream,
        int             headerFlags,
        int             fileFlags);

    int readTypeTable(
        ftMemoryStream& stream,
        int             headerFlags,
        int             fileFlags);

    int readSizeTable(
        ftMemoryStream& stream,
        int             headerFlags,
        int             fileFlags);

    int readStructureTable(
        ftMemoryStream& stream,
        int             headerFlags,
        int             fileFlags);

    static int allocateTable(
        void** destination,
        SKsize numberOfEntries,
        SKsize sizeOfEntry,
        int    fileFlags);

    void clearTables(void);

    int convertName(ftName& dest, char* cp) const;

    int buildStruct(SKuint16*& structure, SKuint16 current, int headerFlags, int fileFlags);

    int isValidTypeName(const SKuint16& type, const SKuint16& name, int flags) const;

    void putMember(FTtype        owningStructureType,
                   ftName*       owningStructureName,
                   const FTtype* currentMember,
                   ftStruct*     root,
                   FTtype        index,
                   SKuint32&     currentOffset,
                   SKuint32      recursiveDepth,
                   int           fileFlags,
                   int&          status) const;

    void compile(FTtype    owningStructureType,
                 ftName*   owningStructureName,
                 FTtype    memberCount,
                 ftStruct* root,
                 SKuint32& currentOffset,
                 SKuint32  recursiveDepth,
                 int       fileFlags,
                 int&      status) const;

    int compile(int fileFlags);

    void hashMember(class skString& name,
                    SKhash          parentStructName,
                    SKhash          owningStructType,
                    SKhash          owningStructMemberName,
                    SKhash          memberType,
                    SKhash          memberName) const;
};

#endif  //_ftTables_h_
