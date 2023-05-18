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

#ifndef _ftMember_h_
#define _ftMember_h_

#include "ftAtomic.h"
#include "ftTypes.h"

/// <summary>
/// ftMember is a utility class to access member
/// data from the ftStruct class.
/// </summary>
class ftMember
{
private:
    friend class ftTable;
    friend class ftStruct;

    void setNameIndex(const SKuint16& idx);

    void setTypeIndex(const SKuint16& idx);

    ftStruct* m_parent;
    ftMember* m_link;

    // TODO: Look at this these should all be unsigned.
    // Find all references, and look for specific reasons
    // as to why they are declared signed.
    SKint32 m_location;
    SKint32 m_offset;
    SKint32 m_recursiveDepth;
    SKint32 m_sizeInBytes;

    // Keep this signed.
    SKint32 m_atomic;

    SKint16 m_type;
    SKint16 m_name;
    SKhash  m_hashedType;
    SKhash  m_hashedName;
    SKhash  m_searchKey;

public:
    /// <summary>
    /// Default constructor
    /// </summary>
    /// <param name="owner">A handle to the parent structure.</param>
    explicit ftMember(ftStruct* owner);

    ~ftMember() = default;

    /// <summary>
    /// Returns the name of the member.
    /// </summary>
    const char* getName() const;

    /// <summary>
    /// Returns the data type name of the member.
    /// </summary>
    const char* getType() const;

    /// <summary>
    /// Test to determine if the data type of this member
    /// is a primitive type or a structure.
    /// </summary>
    bool isBuiltinType() const;

    /// <summary>
    /// Test to determine if the data type of this member
    /// is a structure.
    /// </summary>
    bool isStructure() const;

    /// <summary>
    /// Test to determine if this type is a pointer to
    /// a memory location.
    /// </summary>
    bool isPointer() const;

    /// <summary>
    /// Test to determine if this type has more than one subscript.
    /// </summary>
    bool isArray() const;

    /// <summary>
    /// Returns true if this type is defined as 'char' or 'uchar'.
    /// </summary>
    bool isCharacter();

    /// <summary>
    /// Returns true if this member is defined as a character and has an array subscript.
    /// </summary>
    bool isCharacterArray()
    {
        return isCharacter() && isArray();
    }

    /// <summary>
    /// Returns true if this type is defined as 'short' or 'ushort'.
    /// </summary>
    bool isInteger16();

    /// <summary>
    /// Returns true if this type is defined as 'int' or 'uint'.
    /// </summary>
    bool isInteger32();

    /// <summary>
    /// Returns true if this type is defined as 'int64_t' or 'uint64_t'.
    /// </summary>
    bool isInteger64();

    /// <summary>
    /// Returns the total size of the array or 1 if the type is not defined as an array.
    /// </summary>
    int getArraySize() const;

    /// <summary>
    /// Returns the total number asterisks that are attached to the name.
    /// </summary>
    int getPointerCount() const;

    /// <summary>
    /// Returns the underlying data type length.
    /// </summary>
    int getArrayElementSize() const;

    /// <summary>
    /// Returns the underlying enumeration for this type.
    /// </summary>
    /// <returns>
    /// If this member is a primitive type then the return code will be less than
    /// FT_ATOMIC_UNKNOWN otherwise, it will be FT_ATOMIC_UNKNOWN
    /// </returns>
    ftAtomic getAtomicType();

    /// <summary>
    /// Used to test semi-equal members so that an offset to the correct
    /// member in both file and memory buffers can be matched.
    /// </summary>
    /// <param name="rhs">
    /// The other member to test against.
    /// Regardless of what 'this' points to (fileMember or memoryMember)
    /// the rhs parameter needs to be in the opposite table.
    /// </param>
    /// <returns>True if the internal keys match.</returns>
    /// <remarks>
    /// The data may change over time:
    /// <example><code>
    /// int foo;    => short foo;
    /// int bar[3]; => int bar[8];
    /// </code></example>
    /// Ideally this is meant to weed out those subtle changes.
    /// In the current state, this is not 100 percent.
    /// </remarks>
    bool compare(ftMember* rhs) const;

    /// <summary>
    /// Offsets to this member's memory location.
    /// </summary>
    /// <param name="base">Is the memory that was allocated from the chunk.</param>
    /// <param name="maxAlloc">Is the amount of memory allocated from the chunk.</param>
    /// <returns>
    /// The start address of this member, or null if the
    /// precomputed offset falls outside the range [0, maxAlloc).
    /// </returns>
    SKsize* jumpToOffset(void* base, SKsize maxAlloc) const;

    /// <summary>
    /// Returns the size in bytes for this member.
    /// </summary>
    SKsize getSizeInBytes() const
    {
        return m_sizeInBytes;
    }

    /// <summary>
    /// Returns the direct offset to this member from the start address of the parent structure.
    /// </summary>
    SKsize getOffset() const
    {
        return m_offset;
    }

    /// <summary>
    /// Returns the data type name as a hash code.
    /// </summary>
    const SKhash& getHashedType() const
    {
        return m_hashedType;
    }

    /// <summary>
    /// Returns the member name as a hash code.
    /// </summary>
    const SKhash& getHashedName() const
    {
        return m_hashedName;
    }

    /// <summary>
    /// Returns the structure that this member belongs to.
    /// </summary>
    ftStruct* getParent() const
    {
        return m_parent;
    }

    // internal
    ftMember* getLink() const
    {
        return m_link;
    }

    // internal
    bool hasLink() const
    {
        return m_link != nullptr;
    }

    // internal
    void setLink(ftMember* member)
    {
        m_link = member;
    }
};

#endif  //_ftMember_h_
