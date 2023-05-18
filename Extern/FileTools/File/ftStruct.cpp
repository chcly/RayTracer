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
#include "ftStruct.h"
#include "ftMember.h"
#include "ftTable.h"

ftStruct::ftStruct(ftTable* parent) :
    m_type(0),
    m_hashedType(SK_NPOS),
    m_sizeInBytes(0),
    m_refs(0),
    m_lock(0),
    m_structureId(0),
    m_flag(0),
    m_table(parent),
    m_link(nullptr)
{
}

ftStruct::~ftStruct()
{
    for (ftMember* member : m_members)
        delete member;
}

ftMember* ftStruct::createMember()
{
    ftMember* mbr = new ftMember(this);
    m_members.push_back(mbr);
    return mbr;
}

ftMember* ftStruct::getMember(const Members::SizeType& index)
{
    if (index < m_members.size())
        return m_members[index];
    return nullptr;
}

ftMember* ftStruct::find(ftMember* other) const
{
    if (!other || other->m_searchKey == NoHash)
        return nullptr;

    ftMemberSearchKey msk = {other->m_searchKey, nullptr};
    if (m_memberSearch.findNonRecursive(msk, msk))
        return msk.member;
    return nullptr;
}

SKbyte* ftStruct::getChunk(void* base, SKsize idx, const SKsize max) const
{
    const SKsize next = m_sizeInBytes * idx;
    if (base && next <= max)
        return static_cast<SKbyte*>(base) + next;
    return nullptr;
}

const char* ftStruct::getName() const
{
    if (m_table && m_type < m_table->m_typeCount)
        return m_table->m_types[m_type].name;
    return "";
}
