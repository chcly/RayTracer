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
#ifndef _bAsciiBlock_h_
#define _bAsciiBlock_h_

#include "bAsciiCommon.h"
#include "bAsciiOpCodes.h"

class bAsciiBlock
{
protected:
    const bString     m_type;
    const int         m_code;
    bString           m_name;
    bInteger          m_len;
    bAsciiBlock*      m_parent;
    bAsciiBlockArray  m_children;
    bAsciiArgumentMap m_arguments;

public:
    bAsciiBlock(const bString& type, bInteger code);
    ~bAsciiBlock();

    void addArgument(bAsciiArgument* argument);

    bool getBool(const bCode& arg, const SKuint32& idx = 0, bool def = false);

    SKint16 getInt16(const bCode& arg, const SKuint32& idx = 0, SKint16 def = 0);

    SKint32 getInt32(const bCode& arg, const SKuint32& idx = 0, SKint32 def = 0);

    SKuint16 getUint16(const bCode& arg, const SKuint32& idx = 0, SKuint16 def = 0);

    SKuint32 getUint32(const bCode& arg, const SKuint32& idx = 0, SKuint32 def = 0);

    float getFloat(const bCode& arg, const SKuint32& idx = 0, float def = 0);

    void getFloatVector(float* dest, const bCode& arg, const SKuint32& idx, const SKuint32& size);

    const bStringVector& getStringVector(const bCode& arg, const bStringVector& def = bStringVector());

    const bNumberVector&  getNumberVector(const bCode& arg, const bNumberVector& def = bNumberVector());

    const bString& getString(const bCode& arg, const SKuint32& idx = 0, const bString& def = bString());

    bAsciiArgument* getArgument(const bCode& arg);

    SK_INLINE bool hasArgument(const bCode& arg) const
    {
        return m_arguments.find(arg) != m_arguments.npos;
    }

    void setParent(bAsciiBlock* par);

    SK_INLINE bAsciiBlock* getParent() const
    {
        return m_parent;
    }

    SK_INLINE bool hasParent() const
    {
        return m_parent != nullptr;
    }

    SK_INLINE bAsciiBlockArray& getChildren()
    {
        return m_children;
    }

    SK_INLINE void setName(const bString& v)
    {
        m_name = v;
    }

    SK_INLINE const bString& getName() const
    {
        return m_name;
    }

    SK_INLINE int getType() const
    {
        return m_code;
    }

    SK_INLINE const bString& getTypeName() const
    {
        return m_type;
    }

    SK_INLINE void setLength(bInteger v)
    {
        m_len = v;
    }

    SK_INLINE bInteger getLength() const
    {
        return m_len;
    }
};

#endif  //_bAsciiBlock_h_
