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
#include <string>
#include <vector>
#include "templates/LargeStringArray.h"
#include "Utils/skMap.h"
#include "ftHashTypes.h"
#include "gtest/gtest.h"

typedef skHashTable<ftCharHashKey, SKuint32> TypeFinder;
typedef skHashTable<SKuintPtr, SKuintPtr>    PointerFinder;

GTEST_TEST(ftCharHash, HashTest)
{
    TypeFinder findType;
    size_t     i;
    for (i = 0; i < random_names_len; ++i)
    {
        ftCharHashKey chk(random_names[i]);

        SKsize lookup = findType.find(chk);
        EXPECT_EQ(SK_NPOS, lookup);

        bool result = findType.insert(chk, (SKuint32)i + 1);
        EXPECT_EQ(true, result);
    }

    EXPECT_EQ(random_names_len, findType.size());
    i = 0;
    
    TypeFinder::Iterator it = findType.iterator();
    while (it.hasMoreElements() && i < random_names_len)
    {
        ftCharHashKey chk = it.getNext().first;

        size_t cmp = strcmp(chk.key(), random_names[i]);
        EXPECT_EQ(0, cmp);
        ++i;
    }
}

GTEST_TEST(ftCharHash, PointerHash)
{
    int *ptrArray = new int[101];

    size_t i;
    for (i = 0; i < 100; ++i)
        ptrArray[i] = (int)i + 1;


    PointerFinder findPtr;
    int *it = ptrArray;

    for (i = 0; i < 100; ++i)
    {
        SKuintPtr ptr = (SKuintPtr)(it + i);

        SKsize lookup = findPtr.find(ptr);
        EXPECT_EQ(SK_NPOS, lookup);


        bool result = findPtr.insert(ptr, it[i]);
        EXPECT_EQ(true, result);
    }

    EXPECT_EQ(100, findPtr.size());

    i = 0;

    PointerFinder::Iterator pit = findPtr.iterator();
    while (pit.hasMoreElements() && i < 100)
    {
        SKuintPtr chk = pit.getNext().first;
        SKuintPtr ptr = (SKuintPtr)(it + i);
        EXPECT_EQ(ptr, chk);

        ++i;
    }

    delete[] ptrArray;
}
