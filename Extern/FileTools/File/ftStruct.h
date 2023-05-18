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
#ifndef _ftStruct_h_
#define _ftStruct_h_

#include "Utils/skBinarySearchTree.h"
#include "ftAtomic.h"
#include "ftTypes.h"

class ftTable;
class ftStruct;
class ftMember;

/// <summary>
/// ftName is a structure to hold information about a c/c++ member.
/// </summary>
/// <remarks>
/// The name is defined with the following semantics:
/// <code>
/// (type) [(pointerCount)(name)(arraySize:dimensions)]
/// </code>
/// </remarks>
struct ftName
{
    /// <summary>
    /// Default constructor.
    /// </summary>
    ftName() :
        name(nullptr),
        hash(SK_NPOS),
        pointerCount(0),
        dimensionCount(0),
        isFunctionPointer(0),
        arraySize(0),
        dimensions{}
    {
    }

    /// <summary>
    /// Contains the name of the c/c++ member.
    /// </summary>
    char* name;

    /// <summary>
    /// Contains the hashed version of the 'char* name' member
    /// </summary>
    SKhash hash;

    /// <summary>
    /// Contains the number of pointers that are attached to the name.
    /// </summary>
    SKint32 pointerCount;

    /// <summary>
    /// Contains the number of array dimensions that are attached to the name.
    /// </summary>
    SKint8 dimensionCount;

    /// <summary>
    /// Contains a true or false value if the member is defined to be a function pointer.
    /// </summary>
    SKint8 isFunctionPointer;

    /// <summary>
    /// Contains the total number of array dimensions.
    /// </summary>
    SKint16 arraySize;

    /// <summary>
    /// Is an array that holds the individual dimension for multi-dimensional arrays.
    /// </summary>
    SKint32 dimensions[FileTools_MaxArrayDim];
};

/// <summary>
/// ftType is a structure that is used to store information about a c/c++ type.
/// </summary>
struct ftType
{
    /// <summary>
    /// Default constructor.
    /// </summary>
    ftType() :
        name(nullptr),
        hash(SK_NPOS),
        id(SK_NPOS32)
    {
    }

    /// <summary>
    /// Contains the data type name.
    /// </summary>
    char* name;  // note: This memory is only a reference
                 //       to the allocated memory in the
                 //       main table.

    /// <summary>
    /// Contains the hashed version of the 'char* name' member
    /// </summary>
    SKhash hash;

    /// <summary>
    /// Contains the structure index if the type is a structure. Otherwise, this field is
    /// unused and should be set to data type limit for an unsigned int.
    /// </summary>
    /// <remarks>
    /// Caution must be used here. All data types are stored in a single array of structures.
    /// This is the index into that array with an offset of the total number of atomic types.
    /// </remarks>
    SKuint32 id;
};

/// <summary>
/// ftMemberSearchKey is a paired type name hash to ftMember search key.
/// </summary>
/// <remarks>
/// It is used with the ftStruct::MemberLookup binary search tree to speed up
/// searching for differences between file and memory members.
/// </remarks>
struct ftMemberSearchKey
{
    SKhash    hash;
    ftMember* member;
};

inline bool operator<(const ftMemberSearchKey& a, const ftMemberSearchKey& b)
{
    return a.hash < b.hash;
}

inline bool operator==(const ftMemberSearchKey& a, const ftMemberSearchKey& b)
{
    return a.hash == b.hash;
}

/// <summary>
/// ftStruct is a class that is used to store information about a c/c++ class or struct.
/// </summary>
class ftStruct final
{
public:
    typedef skArray<ftMember*>                    Members;
    typedef skBinarySearchTree<ftMemberSearchKey> MemberLookup;

    enum Flag
    {
        CAN_LINK = 0,
        /// <summary>
        /// This flag indicates that this structure was not found
        /// in either the file or memory table.
        /// </summary>
        MISSING = 0x001,
        /// <summary>
        /// This flag indicates a misalignment between loaded and calculated
        /// offsets to member data. If this is reported the loader cannot jump
        /// directly to the member after the first misaligned offset.
        /// Which means that something is corrupted.
        /// </summary>
        MISALIGNED = 0x002,
        /// <summary>
        /// This flag is used to determine whether or not the file an memory versions of this
        /// structure are in sync. If they are out of sync, then each member in the file structure
        /// needs to be cast into each member of the memory structure.
        /// </summary>
        NEED_CAST = 0x004,

        /// <summary>
        /// Internal flag.
        /// If this is set, then this structure has other structures for members.
        /// </summary>
        HAS_DEPENDENT = 0x008,
    };

private:
    friend class ftTable;
    friend class ftMember;

    ftMember* createMember();

    SKuint16     m_type;
    SKhash       m_hashedType;
    SKint32      m_sizeInBytes;
    SKint32      m_refs, m_lock;
    SKint32      m_structureId;
    SKint32      m_flag;
    Members      m_members;
    ftTable*     m_table;
    ftStruct*    m_link;
    MemberLookup m_memberSearch;

public:
    /// <summary>
    /// Main constructor.
    /// </summary>
    /// <param name="parent">A pointer to the owing table for this structure.</param>
    explicit ftStruct(ftTable* parent);

    ~ftStruct();

    // String type name

    /// <summary>
    /// Gets the structure's user-defined name.
    /// </summary>
    /// <returns>A reference to the character memory in the NAME table for this structure.</returns>
    const char* getName() const;

    /// <summary>
    /// Gets a member defined within the struct.
    /// </summary>
    /// <param name="index">The index for the member in the array.</param>
    /// <returns>The member at the index or a null pointer if the index is out of bounds.</returns>
    ftMember* getMember(const Members::SizeType& index);

    /// <summary>
    /// Performs a non-recursive binary search for the supplied member.
    /// </summary>
    /// <param name="other">
    /// The corresponding member to find.
    /// </param>
    /// <returns>
    /// Null if other is null, other's search key is not defined, or if other is not found.
    /// Otherwise, it will return the found corresponding member.
    /// </returns>
    /// <remarks>
    /// This method is meant to be used to find a previously declared member in another table.
    /// The member may have changed slightly between now and the time it was originally saved.
    /// It is not guaranteed that this will return the exact member in question.
    /// The accuracy is dependent on how well the search key is built.
    /// </remarks>
    ftMember* find(ftMember* other) const;

    /// <summary>
    /// Gets the total number of members in this structure
    /// </summary>
    /// <returns>The count.</returns>
    /// <remarks>The count includes recursive structures.
    /// That is structures inside of structures that are not declared as a pointer.</remarks>
    Members::SizeType getMemberCount() const
    {
        return m_members.size();
    }

    /// <summary>
    /// Returns an iterator for members
    /// </summary>
    /// <returns>skPointerIncrementIterator</returns>
    Members::Iterator getMemberIterator()
    {
        return m_members.iterator();
    }

    /// <summary>
    /// Gets the table that this structure belongs to.
    /// </summary>
    /// <returns>The owning table.</returns>
    ftTable* getOwner() const
    {
        return m_table;
    }

    /// <summary>
    /// Returns the base address as a byte pointer to the nth block of base.
    /// </summary>
    /// <param name="base">The start address</param>
    /// <param name="idx">The offset to the address.</param>
    /// <param name="max">The maximum offset</param>
    /// <returns>Null if the base parameter is null or if the idx parameter is greater than max.</returns>
    /// <remarks>
    /// This is only used when there are chunks back to back.
    /// It simply offsets to the next chunk.
    /// </remarks>
    SKbyte* getChunk(void* base, SKsize idx, SKsize max) const;

    /// <summary>
    /// Gets this structures type index in the owning table's type table.
    /// </summary>
    SKint16 getTypeIndex() const
    {
        return m_type;
    }

    /// <summary>
    /// Gets the hashed type name of this structure.
    /// </summary>
    /// <returns></returns>
    const SKhash& getHashedType() const
    {
        return m_hashedType;
    }

    /// <summary>
    /// Gets this structures index in the owning table's structure table.
    /// </summary>
    SKint32 getStructIndex() const
    {
        return m_structureId;
    }

    /// <summary>
    /// Gets the total size in bytes of this structure.
    /// </summary>
    const SKint32& getSizeInBytes() const
    {
        return m_sizeInBytes;
    }

    /// <returns>Returns the currently set flags.</returns>
    SKint32 getFlag() const
    {
        return m_flag;
    }

    /// <summary>
    /// Sets the current state of all flags.
    /// </summary>
    /// <param name="bits"></param>
    void setFlag(const SKint32& bits)
    {
        m_flag = bits;
    }

    /// <summary>
    /// Appends a flag to the current flags.
    /// </summary>
    /// <param name="bit">The bit flag to add.</param>
    void addFlag(const SKint32& bit)
    {
        m_flag |= bit;
    }

    /// <summary>
    /// Tests to see if a bit flag is set.
    /// </summary>
    /// <param name="bit">The bit flag to test.</param>
    /// <returns>True if the flag is set otherwise returns false.</returns>
    bool hasFlag(const SKint32& bit) const
    {
        return (m_flag & bit) != 0;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    void setLink(ftStruct* structure)
    {
        m_link = structure;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    ftStruct* getLink()
    {
        return m_link;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    const ftStruct* getLink() const
    {
        return m_link;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    bool hasLink() const
    {
        return m_link != nullptr;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// If this bit is set, it means that the struct contains
    /// references to other structures that are not pointers.
    /// </remarks>
    bool hasDependentTypes() const
    {
        return (m_flag & HAS_DEPENDENT) != 0;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// Returns the number of dependent structures
    /// </remarks>
    SKint32 getReferences() const
    {
        return m_refs;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    void addRef()
    {
        m_refs++;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    void lock()
    {
        m_lock = 1;
    }

    /// <remarks>
    /// Internal use only.
    /// It is used to separate and sort structures based on the declaration order of its dependencies.
    /// </remarks>
    bool isLocked() const
    {
        return m_lock != 0;
    }
};

#endif  //_ftStruct_h_
