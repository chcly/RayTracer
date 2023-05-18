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
#ifndef _ftChunk_h_
#define _ftChunk_h_

#include "ftTypes.h"

/// <summary>
/// ftChunkScan is a structure that contains the minimum amount of
/// information to jump around the file.
/// </summary>
/// <remarks>
/// This is used when scanning for a specific data block.
/// The code is the specific chunk id that is being searched for.
/// The length is used to seek over the current chunk's data so that
/// the next chunk can be found.
/// </remarks>
struct ftChunkScan
{
    SKuint32 code;
    SKuint32 length;
};

/// <summary>
/// ftChunk32 is a structure that represents
/// a chunk saved on a 32-bit platform.
/// </summary>
struct ftChunk32
{
    SKuint32 code;
    SKuint32 length;
    SKuint32 address;
    SKuint32 structId;
    SKuint32 count;
};

SK_ASSERTCOMP(ChunkLen32, sizeof(ftChunk32) == 20);

/// <summary>
/// ftChunk64 is a structure that represents
/// a chunk saved on a 64-bit platform.
/// </summary>
struct ftChunk64
{
    SKuint32 code;
    SKuint32 length;
    SKuint64 address;
    SKuint32 structId;
    SKuint32 count;
};
SK_ASSERTCOMP(ChunkLen64, sizeof(ftChunk64) == 24);

/// <summary>
/// ftChunk is a varying size structure that
/// represents a chunk saved on a 32-bit or a 64-bit platform.
/// </summary>
struct ftChunk
{
    SKuint32 code;
    SKuint32 length;
    SKsize   address;
    SKuint32 structId;
    SKuint32 count;
};

#if SK_ARCH == SK_ARCH_32
SK_ASSERTCOMP(ChunkLenNative, sizeof(ftChunk32) == sizeof(ftChunk));
#else
SK_ASSERTCOMP(ChunkLenNative, sizeof(ftChunk64) == sizeof(ftChunk));
#endif

/// <summary>
/// ftMemoryChunk is a structure that is used during the
/// loading process that contains the chunk read from the
/// file as well as its subsequent data.
/// </summary>
struct ftMemoryChunk
{
    enum Flag
    {
        /// <summary>
        /// Flag to indicate that this chunk was modified
        /// </summary>
        BLK_MODIFIED = 0x01,

        /// <summary>
        /// Flag to indicate that this chunk has been reconstructed.
        /// </summary>
        BLK_LINKED = 0x02,
    };

    /// <summary>
    /// Default constructor to initialize fields.
    /// </summary>
    ftMemoryChunk() :
        next(nullptr),
        prev(nullptr),
        chunk{},
        fileBlock(nullptr),
        fileBlockLen(0),
        memoryBlock(nullptr),
        memoryBlockLen(0),
        pointerBlock(nullptr),
        pointerBlockLen(0),
        flag(0),
        newTypeId(0),
        fileStruct(nullptr),
        memoryStruct(nullptr)
    {
    }

    /// <summary>
    /// Pointer to the next chunk in the list.
    /// </summary>
    ftMemoryChunk* next;

    /// <summary>
    /// Pointer to the previous chunk in the list.
    /// </summary>
    ftMemoryChunk* prev;

    /// <summary>
    /// Contains chunk information that was read from the file.
    /// </summary>
    ftChunk chunk;

    /// <summary>
    /// Is the block of memory that was allocated and read
    /// from the file. It contains the data of the structure at
    /// the time of saving.
    /// Its length is the size of the corresponding structure in the file table.
    /// </summary>
    void* fileBlock;

    /// <summary>
    /// Contains the total length of allocated memory for memoryBlock.
    /// </summary>
    SKuint32 fileBlockLen;

    /// <summary>
    /// Is the block of memory allocated for conversion.
    /// Its length is the size of the corresponding structure in the memory table.
    /// If the file and memory structures are different, the data from fileBlock gets cast into memoryBlock one member at a time.
    /// </summary>
    void* memoryBlock;

    /// <summary>
    /// Contains the total length of allocated memory for memoryBlock.
    /// </summary>
    SKuint32 memoryBlockLen;

    /// <summary>
    /// Is the storage location for pointers to pointers.
    /// When casting fileBlock into memoryBlock, if the current member is
    /// a pointer to a pointer, this block provides the storage location
    /// for each of the pointers. The address of pointerBlock is assigned to
    /// memoryBlock at the offset to the specific pointer to pointer member.
    /// </summary>
    void* pointerBlock;

    /// <summary>
    /// Contains the total length of allocated memory for pointerBlock.
    /// </summary>
    SKuint32 pointerBlockLen;

    /// <summary>
    /// Contains the Flags that have been set for this chunk.
    /// </summary>
    SKuint8 flag;

    /// <summary>
    /// Contains the chunk's type index into the memory table's type array.
    /// </summary>
    FTtype newTypeId;

    /// <summary>
    /// A reference to the current structure from the file table.
    /// </summary>
    ftStruct* fileStruct;

    /// <summary>
    /// A reference to the current structure from the memory table.
    /// </summary>
    ftStruct* memoryStruct;
};


/// <summary>
/// ftChunkUtils is a utility class to read, write, and scan chunks.
/// </summary>
struct ftChunkUtils
{
    enum Size
    {
        BlockSize = sizeof(ftChunk),
        Block32   = sizeof(ftChunk32),
        Block64   = sizeof(ftChunk64),
        BlockScan = sizeof(ftChunkScan),
    };

    /// <summary>
    /// Reads a chunk from the supplied stream.
    /// </summary>
    /// <param name="dest">The destination memory.</param>
    /// <param name="stream">The stream to read from.</param>
    /// <param name="headerFlags">A copy of the file's header flags.</param>
    /// <param name="totalSize">Should be the size in bytes of the file.</param>
    /// <returns>The total number of bytes that were read from the stream.</returns>
    /// <seealso cref="ftFlags::FileHeader"/>
    static SKsize read(ftChunk* dest, skStream* stream, int headerFlags, SKsize totalSize);

    /// <summary>
    /// Utility to write a chunk to the supplied stream.
    /// </summary>
    /// <param name="source">The source memory to write</param>
    /// <param name="stream">The stream to write to.</param>
    /// <returns>The total number of bytes that were written. </returns>
    static SKsize write(ftChunk* source, skStream* stream);

    /// <summary>
    /// Utility to scan over chunks with the supplied stream.
    /// </summary>
    /// <param name="dest">The destination memory.</param>
    /// <param name="stream">The stream to read from.</param>
    /// <param name="headerFlags">A copy of the file's header flags.</param>
    /// <param name="totalSize">Should be the size in bytes of the file.</param>
    /// <returns>The total number of bytes that were read from the stream. </returns>
    static SKsize scan(ftChunkScan* dest, skStream* stream, int headerFlags, SKsize totalSize);


    /// <summary>
    /// This tests to determine if the supplied code meets the criteria for a valid code.
    /// </summary>
    /// <param name="code">The sequence to test.</param>
    /// <returns>
    /// True if every byte in the code is a mixture of:
    /// ```
    /// 1. upper case characters
    /// 2. lower case characters
    /// 3. ASCII digits [0,9]
    /// 4. underscore character '_'
    /// 5. space character ' '
    /// ```
    /// Otherwise this will return false.
    /// </returns>
    static bool isValidCode(const SKuint32& code);


    static const ftChunk BlankChunk;
};

#endif  //_ftChunk_h_
