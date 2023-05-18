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
/*! \addtogroup DataApi
 * @{
 */

#ifndef _rtArray_h_
#define _rtArray_h_

#include "Utils/skTraits.h"
#include "RenderSystem/Data/rtAllocator.h"

/// <summary>
///
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class rtArray
{
public:
    SK_DECLARE_TYPE(T)

    typedef rtArray<T> SelfType;
public:
    /// <summary>
    ///
    /// </summary>
    rtArray() :
        data(nullptr),
        size(0),
        capacity(0)
    {
    }

    ~rtArray()
    {
        clear();
    }
    

    /// <summary>
    ///
    /// </summary>
    void clear()
    {
        size     = 0;
        capacity = 0;
        rtAllocator::freeArray<T>(data);
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="_capacity"></param>
    void reserve(SKuint32 _capacity)
    {
        if (capacity < _capacity)
        {
            // This is using capacity plus one in order to reserve
            // and then push up to the max without expanding the
            // data. One more element after that will cause the expansion.
            _capacity = skMin<SKuint32>(_capacity + 1, 0xFFFF);
            if (data)
            {
                data = rtAllocator::reallocateArray<T>(data, _capacity, size);

                capacity = _capacity;
            }
            else
            {
                data = rtAllocator::allocateArray<T>(_capacity);

                capacity = _capacity;
            }
        }
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="v"></param>
    void push_back(ConstReferenceType v)
    {
        if (this->size + 1 <= 0xFFFF)
        {
            // If the size of the array is known ahead of time
            // and the data is reserved before pushing any elements.
            // The reserved size should be plus one.
            // This needs to know that the next element to
            // push will not overflow the array.
            if (this->size + 1 > this->capacity)
            {
                this->reserve(this->size == 0 ? SKInitalCap : this->size * 2);
                this->data[this->size++] = v;
            }
            else
                this->data[this->size++] = v;
        }
        else
            throw std::runtime_error("Array limit exceeded.");
    }

    /// <summary>
    /// 
    /// </summary>
    PointerType data;


    /// <summary>
    /// 
    /// </summary>
    SKuint32    size;

    /// <summary>
    /// 
    /// </summary>
    SKuint32    capacity;
};

/*! @} */
#endif  //_rtCuArray_h_
