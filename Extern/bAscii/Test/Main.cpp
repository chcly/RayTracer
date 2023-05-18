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
#include <gtest/gtest.h>
#include "TestDirectory.h"
#include "bAscii/bAscii.h"
#include "bAscii/bAsciiBlock.h"
#include "bAscii/bAsciiOpCodes.h"

#define VERBOSE false

GTEST_TEST(Global, T1)
{
    bAscii    file;
    const int status = file.load(GetTestFilePath("Global/T1.bascii"), VERBOSE);
    EXPECT_EQ(0, status);

    bAsciiBlock* blk = file.findBlock(LIB_GLOBAL);
    EXPECT_NE(nullptr, blk);

    const bString creator = blk->getString(OP_GLOBAL_CREATOR);
    EXPECT_TRUE(creator.empty());
    const bString scene = blk->getString(OP_GLOBAL_SCENE);
    EXPECT_TRUE(scene.empty());
}

GTEST_TEST(Global, T2)
{
    bAscii    file;
    const int status = file.load(GetTestFilePath("Global/T2.bascii"), VERBOSE);
    EXPECT_EQ(0, status);

    bAsciiBlock* blk = file.findBlock(LIB_GLOBAL);
    EXPECT_NE(nullptr, blk);

    const bString creator = blk->getString(OP_GLOBAL_CREATOR);
    EXPECT_TRUE(creator.equals("Some Valid String"));
    const bString scene = blk->getString(OP_GLOBAL_SCENE);
    EXPECT_TRUE(scene.equals("Active Scene In the scene library"));
    const SKuint32 version = blk->getInt32(OP_GLOBAL_VERSION);
    EXPECT_EQ(123, version);
}

void TestBlockProperties(bAsciiBlock*    block,
                         const bString&  expectedName,
                         const bCode&    expectedCode,
                         const bString&  expectedUserName,
                         bAsciiBlock*    expectedParent,
                         const bInteger& expectedLength)
{
    ASSERT_NE(nullptr, block);
    EXPECT_TRUE(block->getTypeName().equals(expectedName));
    EXPECT_EQ(expectedCode, block->getType());

    if (expectedUserName.empty())
        EXPECT_TRUE(block->getName().empty());
    else
        EXPECT_TRUE(block->getName().equals(expectedUserName));

    EXPECT_EQ(expectedParent, block->getParent());
    EXPECT_EQ(expectedLength, block->getLength());
}

GTEST_TEST(SceneLibrary, T1)
{
    bAscii    file;
    const int status = file.load(GetTestFilePath("SceneLibrary/T1.bascii"), true);
    EXPECT_EQ(0, status);

    bAsciiBlock* blk = file.findBlock(LIB_SCENE);
    TestBlockProperties(blk,
                        "SceneLibrary",
                        LIB_SCENE,
                        "",
                        nullptr,
                        OP_UNDEFINED);

    bAsciiBlockArray& children = blk->getChildren();
    EXPECT_EQ(1, children.size());

    bAsciiBlock* ch = children[0];
    TestBlockProperties(ch,
                        "Scene",
                        LIB_BLOCK_SCENE,
                        "T1_scene",
                        blk,
                        OP_UNDEFINED);

    const bStringVector& objects = ch->getStringVector(OP_SCENE_OBJECTS);
    EXPECT_EQ(1, objects.size());
    EXPECT_TRUE(objects[0].equals("A"));

    const bNumberVector& horizon = ch->getNumberVector(OP_SCENE_HORIZON);
    EXPECT_EQ(3, horizon.size());
    EXPECT_FLOAT_EQ(1, horizon[0]);
    EXPECT_FLOAT_EQ(2, horizon[1]);
    EXPECT_FLOAT_EQ(3, horizon[2]);

    const bNumberVector& zenith = ch->getNumberVector(OP_SCENE_ZENITH);
    EXPECT_EQ(3, zenith.size());
    EXPECT_FLOAT_EQ(1, zenith[0]);
    EXPECT_FLOAT_EQ(2, zenith[1]);
    EXPECT_FLOAT_EQ(3, zenith[2]);


    const bNumber& flags = ch->getFloat(OP_SCENE_FLAGS);
    EXPECT_EQ(1, flags);


    bAsciiBlock *block = file.findBlock("T1_scene");
    ASSERT_EQ(block, ch);
    TestBlockProperties(block,
                        "Scene",
                        LIB_BLOCK_SCENE,
                        "T1_scene",
                        blk,
                        OP_UNDEFINED);


}

#if 1
GTEST_TEST(bAscii, BigEndian)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/BigEndian.bascii"),
        VERBOSE);

    EXPECT_EQ(0, status);
}

GTEST_TEST(bAscii, momo_ogreSmallAnim)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/momo_ogreSmallAnim.bascii"),
        VERBOSE);

    EXPECT_EQ(0, status);
}

GTEST_TEST(bAscii, MultiMat249)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/MultiMat249.bascii"),
        VERBOSE);

    EXPECT_EQ(0, status);
}

GTEST_TEST(bAscii, UVLayers)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/UVLayers.bascii"),
        VERBOSE);

    EXPECT_EQ(0, status);
}

GTEST_TEST(bAscii, VertexColors)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/VertexColors.bascii"),
        VERBOSE);

    EXPECT_EQ(0, status);
}

GTEST_TEST(bAscii, CompileError)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/CompileError.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, InvalidSyntax)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/InvalidSyntax.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, InvalidKeyword)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/InvalidKeyword.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, InvalidArg)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/InvalidArg.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, InvalidArgCount)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/InvalidArgCount.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, InvalidStride)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/InvalidStride.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, MaxArg)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/MaxArg.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}

GTEST_TEST(bAscii, MissingConstructor)
{
    bAscii    file;
    const int status = file.load(
        GetTestFilePath("Original/MissingConstructor.bascii"),
        VERBOSE);

    EXPECT_EQ(-1, status);
}
#endif
