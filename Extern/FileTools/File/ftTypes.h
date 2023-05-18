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
#ifndef _ftTypes_h_
#define _ftTypes_h_

#ifndef ftHAVE_CONFIG
#include "ftConfig.h"
#endif

#include "Utils/skArray.h"
#include "Utils/skFixedString.h"
#include "Utils/skMap.h"
#include "Utils/skString.h"

#define FT_TYPEID(a, b, c, d) SK_ID(a, b, c, d)
#define FT_TYPEID2(a, b) SK_ID2(a, b)

#define FT_INLINE SK_INLINE

using FTtype = SKuint16;

// Most code is dependent on this being a short
SK_ASSERTCOMP(FTtypeSize, sizeof(FTtype) == 2);

const SKhash NoHash = SK_MKMX(SKhash);

#ifdef ftSCALAR_DOUBLE
#define scalar_t double
#else
#define scalar_t float
#endif

/// <summary>
/// Utility to test the platform pointer length.
/// </summary>
enum ftPointerLen
{
    /// <summary>
    /// Defines the size of void* for the current platform.
    /// </summary>
    FT_VOID_P = sizeof(void*),

    /// <summary>
    /// Defined to 1 if the current platform is 32 bits.
    /// </summary>
    FT_VOID_4 = FT_VOID_P == 4,
    /// <summary>
    /// Defined to 1 if the current platform is 64 bits.
    /// </summary>
    FT_VOID_8 = FT_VOID_P == 8,
};

/// <summary>
/// ftList is a utility list class
/// </summary>
class ftList
{
public:

    struct Link
    {
        Link* next;
        Link* prev;
    };

    Link* first;
    Link* last;

public:
    ftList() :
        first(nullptr),
        last(nullptr)
    {
    }

    ~ftList()
    {
        clear();
    }

    /// <summary>
    /// Sets the first and last pointers in the list to null.
    /// </summary>
    void clear()
    {
        first = last = nullptr;
    }


    /// <summary>
    /// Appends an element to the back of the list.
    /// </summary>
    /// <param name="pointer">
    /// Needs to be a block of memory that
    /// starts with the Link signature.
    /// </param>
    void pushBack(void* pointer)
    {
        Link* link = (Link*)pointer;
        if (!link)
            return;
        link->prev = last;

        if (last)
            last->next = link;

        if (!first)
            first = link;
        last = link;
    }

    /// <summary>
    /// Disconnects the supplied pointer form the list. 
    /// </summary>
    /// <param name="pointer">
    /// Needs to be a block of memory that
    /// starts with the Link signature.
    /// </param>
    /// <returns>True on success otherwise false.</returns>
    bool erase(void* pointer)
    {
        Link* link = (Link*)pointer;
        if (!link)
            return false;

        if (link->next)
            link->next->prev = link->prev;
        if (link->prev)
            link->prev->next = link->next;
        if (last == link)
            last = link->prev;
        if (first == link)
            first = link->next;
        return true;
    }
};

/// <summary>
/// ftHeader is a fixed character array for the file header.
/// </summary>
typedef skFixedString<12> ftHeader;

/// <summary>
/// ftByteInteger is a utility union to split a 64-bit integer into smaller bytes.
/// </summary>
union ftByteInteger
{
    SKuint64 int64;
    SKuint32 int32[2];
    SKuint16 int16[4];
    SKuint8  int8[8];
};

/// <summary>
/// ftByteInteger32 is a utility union to split a 32-bit integer into smaller bytes.
/// </summary>
union ftByteInteger32
{
    SKuint32 int32;
    SKuint16 int16[2];
    SKuint8  int8[4];
};

class skStream;
class ftTable;
class ftStruct;
struct ftName;
class ftMember;
class ftTable;
class ftStruct;

/// <summary>
/// Grouping namespace for flags
/// </summary>
namespace ftFlags
{
    /// <summary>
    /// FileMagic contains values that have been extracted from
    /// file's header string.
    /// </summary>
    enum FileMagic
    {
        /// <summary>
        /// This indicates that the file was saved on a big-endian platform.
        /// </summary>
        FM_BIG_ENDIAN = 0x56,
        /// <summary>
        /// This indicates that the file was saved on a little-endian platform.
        /// </summary>
        FM_LITTLE_ENDIAN = 0x76,
        /// <summary>
        /// This indicates that the file was saved on a 32-bit platform.
        /// </summary>
        FM_32_BIT = 0x5F,
        /// <summary>
        /// This indicates that the file was saved on a 64-bit platform.
        /// </summary>
        FM_64_BIT = 0x2D,

        /// <summary>
        /// Is the offset to the end of the file's header.
        /// </summary>
        HEADER_OFFSET = 0x0C,
    };

    /// <summary>
    /// Return status codes
    /// </summary>
    enum FileStatus
    {
        /// <summary>
        /// Start code for enumerated values.
        /// </summary>
        FS_STATUS_MIN = -18,

        /// <summary>
        /// Invalid insert.
        /// This is set when inserting a chunk into the pointer lookup table.
        /// </summary>
        FS_INV_INSERT,

        /// <summary>
        /// Invalid allocation.
        /// Used to catch null values that are returned by malloc.
        /// </summary>
        FS_BAD_ALLOC,

        /// <summary>
        /// Invalid Value.
        /// </summary>
        FS_INV_VALUE,

        /// <summary>
        /// Invalid read.
        /// A stream read failed.
        /// </summary>
        FS_INV_READ,

        /// <summary>
        /// Invalid size.
        /// An expected size constraint was not met.
        /// </summary>
        FS_INV_LENGTH,

        /// <summary>
        /// Invalid header.
        /// An unexpected header was read.
        /// </summary>
        FS_INV_HEADER_STR,

        /// <summary>
        /// Table read error.
        /// Failed to read any structures from the table.
        /// </summary>
        FS_TABLE_INIT_FAILED,

        /// <summary>
        /// A diagnostics check error. A zeroed block of memory contains something other than zeros.
        /// </summary>
        FS_OVERFLOW,

        /// <summary>
        /// A general fail status.
        /// </summary>
        FS_FAILED,

        /// <summary>
        /// If this status is returned, it means that every byte in the code
        /// did not match the following criteria:
        /// <remarks>
        /// <code>
        /// upper case characters
        /// lower case characters
        /// ASCII digits [0,9]
        /// underscore character '_'
        /// space character ' '
        /// </code>
        /// Assuming that the code was constructed properly, then this most likely
        /// means that some data is out of alignment with what was computed and or reported.
        /// </remarks>
        /// </summary>
        FS_CODE_ERROR,

        /// <summary>
        /// If this status is returned, it means that the size of the data block that was provided
        /// did not match the size of the block it was provided for.
        /// </summary>
        FS_INTEGRITY_FAIL,

        /// <summary>
        /// If this status is returned, it means that the address
        /// stored in a chunk header was already processed.
        /// </summary>
        FS_DUPLICATE_BLOCK,

        // Table codes
        RS_INVALID_PTR,

        RS_INVALID_CODE,
        RS_LIMIT_REACHED,
        RS_BAD_ALLOC,
        RS_MIS_ALIGNED,


        // This should always be zero
        // until < 0 tests are removed
        // and replaced with != FS_OK
        FS_OK,
    };

    enum ReadMode
    {
        RM_UNCOMPRESSED = 0,
        RM_COMPRESSED,
        RM_READ_FROM_MEMORY,
    };

    /// <summary>
    /// FileHeader defines conditional flags that are set based
    /// on the file's header string.
    /// </summary>
    enum FileHeader
    {
        /// <summary>
        /// Is a flag that indicates that the byte order in the file
        /// is different than the byte order of the platform that is
        /// loading the file.
        /// </summary>
        FH_ENDIAN_SWAP = 0x01,

        /// <summary>
        /// Is a flag that indicates that the chunk size in the file
        /// was saved on a 64 bit platform.
        /// </summary>
        FH_CHUNK_64 = 0x02,

        /// <summary>
        /// Is a flag that indicates that the chunk size in the file
        /// does not match the chunk size on the current platform.
        /// </summary>
        FH_VAR_BITS = 0x04,
    };

    enum LogFlags
    {
        LF_NONE             = 0x0000,
        LF_ONLY_ERR         = 0x0001,
        LF_READ_CHUNKS      = 0x0002,  // Logs the chunks as they are read
        LF_WRITE_CHUNKS     = 0x0004,  // Logs the chunks as they are written
        LF_DO_CHECKS        = 0x0008,
        LF_DIAGNOSTICS      = 0x0010,  // Enable diagnostics printout
        LF_DUMP_NAME_TABLE  = 0x0020,  // Output the contents of the name table.
        LF_DUMP_TYPE_TABLE  = 0x0040,  // Output the contents of the type table.
        LF_DUMP_SIZE_TABLE  = 0x0080,  // Output the contents of the size table.
        LF_DUMP_MEMBER_HASH = 0x0100,  // Output member search key
        LF_DUMP_SKIP        = 0x0200,  // Log the blocks being skipped.
        LF_DUMP_CAST        = 0x0400,  // Log the before and after buffers while casting.
        LF_UNRESOLVED       = 0x0800,  // Log cases where there is not a memory structure associated with a file structure.
        LF_MISSING_PTR_PTR  = 0x1000,  // Log cases where the pointer to pointer lookup address is valid but was not found.
        LF_MIS_REPORTED     = 0x2000,  // Log cases where the calculated size does not match the reported size.
    };

    enum LinkerIssues
    {
        LNK_OK              = 0x000,
        LNK_ASSERT          = 0x001,
        LNK_ALIGNMENT_2     = 0x002,
        LNK_ALIGNMENT_4     = 0x004,
        LNK_ALIGNMENT_8     = 0x008,
        LNK_ALIGNMENT_S     = 0x010,
        LNK_ALIGNMENT_P     = 0x020,
        LNK_UNKNOWN         = 0x040,
        LNK_UNDEFINED_TYPES = 0x080,
        LNK_DUPLICATE_TYPES = 0x100
    };

    /// <summary>
    /// WriteMode is a flag that is used to control
    /// how ftCompiler formats the output in write methods.
    /// </summary>
    enum WriteMode
    {
        /// <summary>
        /// Writes the table as a c/c++ array.
        /// </summary>
        /// <remarks>
        /// The table is written back to c/c++ to be compiled as a source file.
        ///  <code>
        ///  const unsigned char ${TableName}Table[] = {
        ///     ...
        ///  }
        ///  const int ${TableName}Len=sizeof(${TableName}Table);
        ///  </code>
        /// </remarks>
        WRITE_ARRAY = 0,
        /// <summary>
        /// Writes the table as binary memory.
        /// </summary>
        WRITE_STREAM,
    };

    enum TokenID
    {
        FT_KEEP_GOING = -2,
        FT_NULL_TOKEN = -1,
        FT_EOF,
        FT_COMMA         = ',',
        FT_POINTER       = '*',
        FT_L_BRACE       = '[',
        FT_COLON         = ':',
        FT_RBRACE        = ']',
        FT_L_PARENTHESIS = '(',
        FT_R_PARENTHESIS = ')',
        FT_L_BRACKET     = '{',
        FT_R_BRACKET     = '}',
        FT_TERM          = ';',
        FT_ID            = 256,
        FT_CHAR,
        FT_SHORT,
        FT_INTEGER,
        FT_LONG,
        FT_FLOAT,
        FT_DOUBLE,
        FT_INT64,
        FT_SCALAR,
        FT_PUBLIC,
        FT_PRIVATE,
        FT_PROTECTED,
        FT_VOID,
        FT_CLASS,
        FT_NAMESPACE,
        FT_STRUCT,
        FT_CONSTANT,
        // FT_UNION, ?
    };

    enum LexState
    {
        FT_IN_START = 0,
        FT_IN_NAMESPACE,
        FT_IN_CLASS,
        FT_IN_STRUCT,
        FT_INSIDE,
        FT_IN_SKIP,
    };

}  // namespace ftFlags

typedef skFixedString<272>                    ftPath;
typedef skFixedString<FileTools_MaxCharArray> ftId;
typedef int                                   ftArraySlots[FileTools_MaxArrayDim];
typedef void*                                 ftParser;
typedef skArray<ftId>                         ftStringPtrArray;
typedef skHashTable<ftId, ftId>               ftStringPtrTable;
typedef skArray<ftPath>                       ftPathArray;

class ftTableBuilder;
class ftScanner;
class ftToken;

#if SK_ARCH == SK_ARCH_64
#define FT_MEMBER_HASH_FMT "%016llX-%016llX-%016llX-%016llX-%016llX"
#else
#define FT_MEMBER_HASH_FMT "%08X-%08X-%08X-%08X-%08X"
#endif

#endif  //_ftTypes_h_
