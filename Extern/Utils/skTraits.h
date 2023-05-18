/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
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
#ifndef _skTraits_h_
#define _skTraits_h_

#include "Config/skConfig.h"

#define SK_DECLARE_TYPE(T)             \
    typedef T        ValueType;        \
    typedef T&       ReferenceType;    \
    typedef T*       PointerType;      \
    typedef const T  ConstValueType;   \
    typedef const T* ConstPointerType; \
    typedef const T& ConstReferenceType;

#define SK_DECLARE_KEY_TYPE(T)            \
    typedef T        KeyType;             \
    typedef T&       ReferenceKeyType;    \
    typedef T*       PointerKeyType;      \
    typedef const T  ConstKeyType;        \
    typedef const T* ConstPointerKeyType; \
    typedef const T& ConstReferenceKeyType;

#define SK_DECLARE_REF_TYPE(T)                               \
    typedef typename T::ValueType          ValueType;        \
    typedef typename T::ReferenceType      ReferenceType;    \
    typedef typename T::PointerType        PointerType;      \
    typedef typename T::ConstValueType     ConstValueType;   \
    typedef typename T::ConstPointerType   ConstPointerType; \
    typedef typename T::ConstReferenceType ConstReferenceType;

#define SK_DECLARE_TYPE_NAME(T, N)          \
    typedef T        N##Type;               \
    typedef T&       N##ReferenceType;      \
    typedef T*       N##PointerType;        \
    typedef const T  Const##N##Type;        \
    typedef const T* ConstPointer##N##Type; \
    typedef const T& ConstReference##N##Type;

#define SK_IMPLEMENT_SORT(T, SELF, Size)                                \
public:                                                                 \
    typedef skSort<T, SELF, Size> SortType;                             \
    void                          sort(typename SortType::Function cmp) \
    {                                                                   \
        SortType srt(cmp);                                              \
        srt.sort((*this));                                              \
    }


#define SK_IMPLEMENT_QSORT(T, SELF, Size)                                \
public:                                                                  \
    typedef skQSort<T, SELF, Size> SortType;                             \
    void                           sort(typename SortType::Function cmp) \
    {                                                                    \
        SortType srt(cmp);                                               \
        srt.sort((*this));                                               \
    }


#endif  //_skTraits_h_
