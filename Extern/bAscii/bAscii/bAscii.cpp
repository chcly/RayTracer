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
#include "bAscii.h"
#include <cstdio>
#include "bAsciiLinker.h"
#include "bAsciiBlock.h"
#include "bAsciiCompiler.h"
#include "bAsciiMain.h"
#include "bAsciiScanner.h"

bAscii::bAscii() :
    m_main(nullptr),
    m_compiled(false)
{
}

bAscii::~bAscii()
{
    clearParseTree();
    delete m_main;
}

bAsciiBlock* bAscii::findBlock(const bString& name)
{
    SKsize pos;
    if ((pos = m_findBlocks.find(name)) != SK_NPOS)
        return m_findBlocks.at(pos);
    return nullptr;
}

void bAscii::clearParseTree(void)
{
    // frees the text based memory

    for (bAsciiBlock* block : m_blocks)
        delete block;
    m_blocks.clear();
    m_findBlocks.clear();
}

void bAscii::addBlock(bAsciiBlock* block)
{
    if (block)
        m_blocks.push_back(block);
}

void bAscii::addBlock(const bString& name, bAsciiBlock* block)
{
    if (block)
    {
        if (m_findBlocks.find(name) == SK_NPOS)
            m_findBlocks.insert(name, block);
    }
}

void bAscii::addBlock(const bCode code, bAsciiBlock* block)
{
    if (code >= LIB_GLOBAL && code < LIB_MAX)
    {
        if (m_codeMapBlocks.find(code) == SK_NPOS)
            m_codeMapBlocks.insert(code, block);
    }
}

bAsciiBlock* bAscii::findBlock(bCode code)
{
    SKsize pos;
    if ((pos = m_codeMapBlocks.find(code)) != SK_NPOS)
        return m_codeMapBlocks.at(pos);
    return nullptr;
}

int bAscii::load(const char* pathname, bool verbose)
{
    if (verbose)
        printf("Parsing file: %s\n", pathname);

    bAsciiScanner sc(pathname);
    if (sc.getError() < 0)
    {
        printf("Parsing file: %s failed\n", pathname);
        return -1;
    }

    if (verbose)
        printf("Starting scan...\n");

    const bAsciiCompiler compiler(this, verbose);
    if (!compiler.compile(sc))
        return -1;

    if (verbose)
        printf("Scan complete...\n");

    m_compiled = true;
    return 0;
}

bAsciiMain* bAscii::linkMain(bool verbose)
{
    if (m_compiled && !m_main)
    {
        bAsciiMain* mainPtr = new bAsciiMain();

        bAsciiLinker lnk;
        const int    status = lnk.link(this, mainPtr, verbose);
        if (status < 0)
            delete mainPtr;
        else
            m_main = mainPtr;

        clearParseTree();
    }
    return m_main;
}

bAsciiMain* bAscii::loadApi(const char* pathname, bool verbose)
{
    if (load(pathname, verbose) == 0)
        return linkMain(verbose);
    return nullptr;
}
