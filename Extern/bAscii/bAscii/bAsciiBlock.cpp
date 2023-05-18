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
#include "bAsciiBlock.h"
#include "bAsciiArgument.h"

bAsciiBlock::bAsciiBlock(const bString& type, const bInteger code) :
    m_type(type),
    m_code(code),
    m_len(OP_UNDEFINED),
    m_parent(nullptr)
{
}

bAsciiBlock::~bAsciiBlock()
{
    for (SKsize i = 0; i < m_arguments.size(); ++i)
        delete m_arguments[i];
}

bAsciiArgument* bAsciiBlock::getArgument(const bCode& arg)
{
    SKsize pos;
    if ((pos = m_arguments.find(arg)) != SK_NPOS)
        return m_arguments.at(pos);

    return nullptr;
}

void bAsciiBlock::addArgument(bAsciiArgument* argument)
{
    if (m_arguments.find(argument->name) == SK_NPOS)
        m_arguments.insert(argument->name, argument);
}

bool bAsciiBlock::getBool(const bCode& arg, const SKuint32& idx, bool def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->booleans.size())
        return obj->booleans[idx];
    return def;
}

SKint16 bAsciiBlock::getInt16(const bCode& arg, const SKuint32& idx, SKint16 def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->numbers.size())
        return (SKint16)obj->numbers[idx];
    return def;
}

SKint32 bAsciiBlock::getInt32(const bCode& arg, const SKuint32& idx, SKint32 def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->numbers.size())
        return (SKint32)obj->numbers[idx];
    return def;
}

SKuint16 bAsciiBlock::getUint16(const bCode& arg, const SKuint32& idx, SKuint16 def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->numbers.size())
        return (SKuint16)obj->numbers[idx];
    return def;
}

SKuint32 bAsciiBlock::getUint32(const bCode& arg, const SKuint32& idx, SKuint32 def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->numbers.size())
        return (SKuint32)obj->numbers[idx];
    return def;
}

float bAsciiBlock::getFloat(const bCode& arg, const SKuint32& idx, float def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->numbers.size())
        return obj->numbers[idx];
    return def;
}

void bAsciiBlock::getFloatVector(float* dest, const bCode& arg, const SKuint32& idx, const SKuint32& size)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx + size <= obj->numbers.size())
    {
        for (SKuint32 i = 0; i < size; ++i)
            dest[i] = obj->numbers[idx + i];
    }
}

const bStringVector& bAsciiBlock::getStringVector(const bCode& arg, const bStringVector& def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj)
        return obj->strings;
    return def;
}

const bNumberVector& bAsciiBlock::getNumberVector(const bCode& arg, const bNumberVector& def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj)
        return obj->numbers;
    return def;
}


const bString& bAsciiBlock::getString(const bCode& arg, const SKuint32& idx, const bString& def)
{
    bAsciiArgument* obj = getArgument(arg);
    if (obj && idx < obj->strings.size())
        return obj->strings[idx];
    return def;
}

void bAsciiBlock::setParent(bAsciiBlock* par)
{
    if (par)
    {
        m_parent = par;
        m_parent->m_children.push_back(this);
    }
}
