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
#ifndef _bAscii_h_
#define _bAscii_h_

#include "bAsciiCommon.h"

/// <summary>
/// Parser for the .bascii file format.
/// </summary>
/// <remarks>
/// There are two data access methods for this class. The first access method is through load.
/// load will only parse and store the tree. bAsciiBlock and bAsciiArgument should be used to
/// access the underlying data. The second access method is through loadApi.
/// loadApi will use the bAsciiBlock and bAsciiArgument classes to construct a direct data access
/// API (bAsciiMain).
/// </remarks>
class bAscii
{
protected:
    bAsciiMain*      m_main;
    bAsciiBlockArray m_blocks;
    bAsciiBlockMap   m_findBlocks;
    bAsciiBlockMapI  m_codeMapBlocks;
    bool             m_compiled;

    friend class bAsciiCompiler;

    bAsciiMain* linkMain(bool verbose);

    void clearParseTree();

    void addBlock(bAsciiBlock* block);
    void addBlock(const bString& name, bAsciiBlock* block);
    void addBlock(bCode code, bAsciiBlock* block);

public:
    bAscii();
    ~bAscii();

    /// <summary>
    /// Loads the supplied pathname. 
    /// </summary>
    /// <param name="pathname">The file system path to the file.</param>
    /// <param name="verbose">If true, this will enable extra debug information.</param>
    /// <returns>Returns 0 on success, otherwise returns -1.</returns>
    int load(const char* pathname, bool verbose = false);

    /// <summary>
    /// Loads the supplied pathname.
    /// </summary>
    /// <param name="pathname">The file system path to the file.</param>
    /// <param name="verbose">If true, this will enable extra debug information.</param>
    /// <returns>Returns a valid bAsciiMain instance on success, otherwise returns null.</returns>
    bAsciiMain* loadApi(const char* pathname, bool verbose = false);

    /// <summary>
    /// Searches for a block with the supplied name.
    /// </summary>
    /// <param name="name">
    /// The user supplied lookup name.
    /// </param>
    /// <returns>A valid bAsciiBlock instance on success, otherwise returns null.</returns>
    /// <example>
    /// <code>
    /// SceneLibrary: {
    ///     Scene: "name" {}
    /// }
    /// </code>
    ///
    /// bAsciiBlock *block = findBlock("name");
    /// </example>
    bAsciiBlock* findBlock(const bString& name);

    /// <summary>
    /// Searches for the library block by its predefined code.
    /// </summary>
    /// <param name="code">One of the bAsciiLibraries values.</param>
    /// <returns>The library associated with the supplied code or null if it is not found.</returns>
    bAsciiBlock* findBlock(bCode code);

    SK_INLINE bAsciiBlockArray& getBlocks()
    {
        return m_blocks;
    }
};

#endif  //_bAscii_h_
