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
#include "Blender.h"
#include "ExtraFiles.h"
#include "ftBlend.h"
#include "ftLogger.h"
#include "ftTable.h"
#include "gtest/gtest.h"

using namespace Blender;
using namespace ftFlags;

SKhash reportFilter[]{
    ftCharHashKey("Object").hash(),
    ftCharHashKey("Mesh").hash(),
    ftCharHashKey("Camera").hash(),
    ftCharHashKey("CollectionChild").hash(),
    ftCharHashKey("CollectionObject").hash(),
    ftCharHashKey("Collection").hash(),
    0};

GTEST_TEST(BlendFile, BasicLoad)
{
    ftBlend fp;
    //fp.setFileFlags(LF_DIAGNOSTICS | LF_DO_CHECKS | LF_DUMP_CAST);
    fp.setCastFilter(reportFilter, sizeof(reportFilter) / sizeof(SKhash));
    const int status = fp.load("Test.blend");
    EXPECT_EQ(FS_OK, status);
    EXPECT_EQ(0, status);
}

GTEST_TEST(BlendFile, ExtractScene)
{
    ftBlend   fp;
    const int status = fp.load("Test.blend");
    EXPECT_EQ(FS_OK, status);

    EXPECT_NE(fp.m_fg, nullptr);
    EXPECT_NE(fp.m_fg->curscene, nullptr);
    EXPECT_TRUE(strcmp(fp.m_fg->curscene->id.name, "SCScene") == 0);
}



GTEST_TEST(BlendFile, LargeChunkCount)
{
    ftBlend fp;
    // This file has a chunk.count larger than 256, which was the
    // old value for it. ( Currently the max for chunk.count is 65536 )
    // Limiting the count is only a precaution to prevent possible
    // invalid values from creeping in.  
    const int status = fp.load("Test1.blend");
    EXPECT_EQ(FS_OK, status);
}


GTEST_TEST(BlendFile, MissingDNA1)
{
    ftBlend fp;
    // This file has had it's DNA1 code deliberately removed.
    const int status = fp.load("MissingDNA1.blend");
    EXPECT_EQ(FS_CODE_ERROR, status);
}

GTEST_TEST(BlendFile, InvalidDNASize)
{
    ftBlend fp;
    // This file has had it's DNA1 length field zeroed.
    const int status = fp.load("InvalidDNASize.blend");
    EXPECT_EQ(FS_INV_READ, status);
}

GTEST_TEST(BlendFile, DNA1WrongSize)
{
    ftBlend fp;
    // This file has had it's DNA1 length field divided by 2.
    const int status = fp.load("DNA1WrongSize.blend");
    EXPECT_EQ(FS_INV_VALUE, status);
}

GTEST_TEST(BlendFile, RandomJunk)
{
    // This file has had the chunks filled with random bytes.
    // The expected result is that it should be able to read it,
    // but whatever data that was read is expected to be nonsense.
    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked2.blend"));
    EXPECT_EQ(FS_OK, status);

    // This should be null because, the original address at the time of saving
    // a chunk should not match any know address in a data block that is completely randomized.
    // If it does match, or link to something, it would mean that 4|8 random bytes matched
    // a chunk address from the file.
    Scene* sc = fp.m_fg->curscene;
    EXPECT_EQ(sc, nullptr);
}

GTEST_TEST(BlendFile, RandomDatablockSwap)
{
    // This file has had the data blocks swapped with other 'valid'
    // blocks, but has left the reported chunk length the same.
    // The expected result is a code error because chunks will not be
    // aligned. When it goes to the next chunk that misaligned data
    // will be read as a chunk. 'Chances' are that whatever data is read
    // will not meet the criteria for ftChunkUtils::isValidCode.
    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked1.blend"));
    EXPECT_EQ(FS_CODE_ERROR, status);
}

GTEST_TEST(BlendFile, RandomChunkSwap)
{
    // This file has had the chunk codes swapped with other 'valid'
    // codes. If the code alone is used to determine what type of data
    // is supplied for structure memory then casting to that memory will
    // corrupt the heap.

    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load("Test_Junked0.blend");
    EXPECT_EQ(FS_INTEGRITY_FAIL, status);
}

GTEST_TEST(BlendFile, Randomized)
{
    // This keeps the DNA1, chunk codes, and lengths intact
    // but randomized everything else.

    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked4.blend"));
    EXPECT_NE(FS_OK, status);
}

GTEST_TEST(BlendFile, Zeroed)
{
    // This test zeros every data block in the file but leaves the
    // chunk headers intact.
    // The DNA1 block is kept intact.

    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked5.blend"));
    EXPECT_EQ(FS_OK, status);
}

GTEST_TEST(BlendFile, AllZeroed)
{
    //  This test zeros everything in the chunk header except it's code, and
    //  drops the data blocks with them.
    //  The DNA1 block is kept intact.

    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked6.blend"));
    EXPECT_NE(FS_OK, status);
}

GTEST_TEST(BlendFile, ChunkHeadersMaxed)
{
    /// This test sets the chunk header's members to their data-type's maximum value.
    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked7.blend"));
    EXPECT_NE(FS_OK, status);
}

GTEST_TEST(BlendFile, FileMaxed)
{
    /// This test sets the chunk header's address members the data-type's maximum value.
    /// Then fills the data block with the same value.
    /// The expected return is FS_DUPLICATE_BLOCK, because in theory the saved address
    /// for a chunk should be unique.

    ftBlend fp;
    fp.setFileFlags(LF_DIAGNOSTICS | LF_UNRESOLVED | LF_DO_CHECKS);
    const int status = fp.load(GetFilePathCString("Test_Junked8.blend"));
    EXPECT_EQ(FS_DUPLICATE_BLOCK, status);
}

void BlendFile_TestCommonScene(Scene* sc)
{
    EXPECT_NE(sc, nullptr);
    EXPECT_NE(sc->master_collection, nullptr);

    ListBase         lb = sc->master_collection->children;
    CollectionChild* cc = (CollectionChild*)lb.first;
    EXPECT_NE(cc, nullptr);

    if (cc)
    {
        EXPECT_NE(cc->collection, nullptr);
        EXPECT_TRUE(strcmp(cc->collection->id.name, "GRCollection") == 0);

        ListBase collections = cc->collection->gobject;

        CollectionObject* co = (CollectionObject*)collections.first;

        int i = 0;
        while (co)
        {
            EXPECT_NE(co->ob, nullptr);
            Object* obj = co->ob;

            switch (i)
            {
            case 0:
            {
                EXPECT_TRUE(strcmp(obj->id.name, "OBCube") == 0);
                EXPECT_TRUE(obj->type == 1);
                EXPECT_TRUE(obj->data != nullptr);

                Mesh* me = (Mesh*)obj->data;
                EXPECT_TRUE(strcmp(me->id.name, "MECube") == 0);

                break;
            }
            case 1:
            {
                EXPECT_TRUE(strcmp(obj->id.name, "OBLight") == 0);
                EXPECT_TRUE(obj->data != nullptr);

                Lamp* ca = (Lamp*)obj->data;
                EXPECT_TRUE(strcmp(ca->id.name, "LALight") == 0);
                break;
            }
            case 2:
            {
                EXPECT_TRUE(strcmp(obj->id.name, "OBCamera") == 0);
                EXPECT_TRUE(obj->type == 11);
                EXPECT_TRUE(obj->data != nullptr);

                Camera* ca = (Camera*)obj->data;
                EXPECT_TRUE(strcmp(ca->id.name, "CACamera") == 0);
                break;
            }
            default:
                break;
            }

            ++i;
            co = co->next;
        }
        EXPECT_EQ(cc->next, nullptr);
    }
}

GTEST_TEST(BlendFile, AssertReflection)
{
    ftBlend   fp;
    const int status = fp.load(GetFilePathCString("Test_Junked3.blend"));
    EXPECT_EQ(FS_OK, status);

    Scene* sc = fp.m_fg->curscene;
    BlendFile_TestCommonScene(sc);
}

GTEST_TEST(BlendFile, IterateObjects)
{
    ftBlend   fp;
    const int status = fp.load("Test.blend");
    EXPECT_EQ(FS_OK, status);

    Scene* sc = fp.m_fg->curscene;
    BlendFile_TestCommonScene(sc);
}

GTEST_TEST(BlendFile, AssertTypes)
{
    ftBlend   fp;
    const int status = fp.load("Test.blend");
    EXPECT_EQ(FS_OK, status);

    ftTable* table = fp.getFileTable();
    EXPECT_NE(table, nullptr);

    const ftTable::Types& types = table->getTypes();
    const SKuint32        nr    = table->getNumberOfTypes();

    for (SKuint32 i = 0; i < nr; ++i)
    {
        const ftType& type = types[i];
        if (i < table->getFirstStructType())
            EXPECT_EQ(SK_NPOS32, type.id);
        else
        {
            if (type.id != SK_NPOS32)
            {
                EXPECT_TRUE(type.id < table->getNumberOfStructs());
            }
        }
    }
}
