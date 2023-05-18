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
#ifndef _ftToken_h_
#define _ftToken_h_

#include "ftTypes.h"

/// <summary>
/// ftKeywordTable is a lookup table for common c/c++
/// keywords and built-in data types. 
/// </summary>
struct ftKeywordTable
{
    /// <summary>
    /// Defines the name of the keyword.
    /// </summary>
    const char* name;

    /// <summary>
    /// Defines the length of the keyword name.
    /// </summary>
    int len;

    /// <summary>
    /// Defines the enumerated ftFlags::TokenID for the keyword.
    /// </summary>
    int token;
};

/// <summary>
/// ftToken is a temporary data storage class to store a c/c++ language semantic.
/// It is used when extracting member data with ftScanner.
/// </summary>
class ftToken
{
public:
    typedef skFixedString<FileTools_MaxCharArray> String;
    friend class ftScanner;

private:
    int    m_id;
    String m_value;
    int    m_arrayConstant;

    void setTokenId(int token)
    {
        m_id = token;
    }

    void setArrayLen(int arrayLength)
    {
        m_arrayConstant = arrayLength;
    }

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    ftToken();

    /// <summary>
    /// Constructs a token from an id and the string representation of the id.
    /// </summary>
    /// <param name="id">Should be one of the TokenID values.</param>
    /// <param name="value">Should be a character array with a size less than FT_MAX_ID.</param>
    ftToken(int id, const String& value);

    /// <summary>
    /// Definition for a copy constructor.
    /// </summary>
    /// <param name="token">A const reference to the token that will be copied into class members.</param>
    ftToken(const ftToken& token) = default;

    /// <summary>
    /// Gets the token identifier.
    /// </summary>
    /// <returns>The integer value of the token. The value should be one of ftFlags::TokenID enumerated values.</returns>
    int getTokenId() const
    {
        return m_id;
    }

    /// <summary>
    /// Gets a const reference to the string data for this token.
    /// </summary>
    const String& getValue() const
    {
        return m_value;
    }

    /// <summary>
    /// Gets a writable reference to the string data for this token.
    /// </summary>
    String& getRef()
    {
        return m_value;
    }

    /// <summary>
    /// Gets the defined array size of c/c++ data type.
    /// </summary>
    /// <remarks>
    /// If the type is not defined as an array in c/c++, the array length
    /// is defined to 1. Otherwise, the value that is returned should be less
    /// than or equal to the FT_ARRAY_DIM_MAX value.
    /// </remarks>
    int getArrayLen() const
    {
        return m_arrayConstant;
    }
};

#endif  //_ftToken_h_
