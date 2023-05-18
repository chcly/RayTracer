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
#include <iostream>
#include "Templates.h"
#include "Utils/skMemoryStream.h"
#include "ftAtomic.h"
#include "ftCompiler.h"
#include "ftLogger.h"
#include "ftMember.h"
#include "gtest/gtest.h"

GTEST_TEST(ftCompiler, Basic)
{
    int        status;
    ftCompiler compiler;
    status = compiler.parse("TestGen", (const char*)TETSTAPI, TETSTAPI_SIZE);
    EXPECT_GE(status, 0);

    status = compiler.compile();

    EXPECT_EQ(ftFlags::LNK_OK, status);

    compiler.setWriteMode(ftFlags::WRITE_STREAM);
    skMemoryStream stream;
    compiler.writeStream(&stream);

    ftLogger::log(stream.ptr(), stream.size());

    ftTable tbl(sizeof(void*));
    tbl.read(stream.ptr(), stream.size(), 0, 0);

    SKuint32 nr, i;

    nr = tbl.getNumberOfTypes();
    i  = 0;

    while (i < nr)
    {
        const ftType& type = tbl.getTypeAt(i);

        EXPECT_NE(nullptr, type.name);
        EXPECT_NE(SK_NPOS, type.hash);

        // builtin types do not point to a structure
        if (i < ftAtomicUtils::NumberOfTypes)
            EXPECT_EQ(type.id, SK_NPOS32);
        else
            EXPECT_LT(type.id, tbl.getNumberOfStructs());

        ++i;
    }

    nr = tbl.getNumberOfNames();
    i  = 0;
    while (i < nr)
    {
        const ftName& name = tbl.getNameAt(i);

        EXPECT_NE(nullptr, name.name);

        ftCharHashKey hk(name.name);
        EXPECT_EQ(name.hash, hk.hash());
        ++i;
    }
}

using namespace std;

GTEST_TEST(ftCompiler, RebuildTest)
{
    ftCompiler compiler;

    int status = compiler.parse("TestGen", (const char*)TETSTAPI, TETSTAPI_SIZE);
    EXPECT_GE(status, 0);

    status = compiler.compile();

    EXPECT_EQ(ftFlags::LNK_OK, status);

    compiler.setWriteMode(ftFlags::WRITE_STREAM);
    skMemoryStream stream(skStream::WRITE);

    compiler.writeStream(&stream);
    ftLogger::log(stream.ptr(), stream.size());

    ftTable tbl(sizeof(void*));
    status = tbl.read(stream.ptr(), stream.size(), 0, ftFlags::LF_DIAGNOSTICS);
    EXPECT_EQ(ftFlags::FS_OK, status);

    ftTable::StructureArray::Iterator it = tbl.getStructIterator();
    while (it.hasMoreElements())
    {
        ftStruct* structure = it.getNext();

        cout << "struct " << structure->getName() << endl;
        cout << "{" << endl;

        ftStruct::Members::Iterator mit = structure->getMemberIterator();
        while (mit.hasMoreElements())
        {
            ftMember* mbr = mit.getNext();
            cout << "    ";
            cout << mbr->getType() << " " << mbr->getName() << endl;
        }
        cout << "}" << endl;
    }
}
