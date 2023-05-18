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
#ifndef _ftConfig_h_
#define _ftConfig_h_

/// <summary>The upper limit for the size in bytes of a single member. </summary>
#define FileTools_MaxStructMember  65536

/// <summary>The maximum number of structures that are allowed in a table. </summary>
#define FileTools_MaxTableSize  5000

/// <summary>The maximum length of a character array. </summary>
#define FileTools_MaxCharArray 64

/// <summary>The maximum number of members in a struct or class. </summary>
#define FileTools_MaxMember 256

/// <summary>Hash table initial allocation size. </summary>
#define FileTools_DefaultAllocationSize 2048

/// <summary>Maximum allowed N-dimensional array. </summary>
#define FileTools_MaxArrayDim 3

/// <summary>Write a validation file. </summary>
#define FileTools_TypeLengthValidate 1

/// <summary>Swap from native endian when writing the table. (Test only) </summary>
/* #undef FileTools_SwapEndian */


/// <summary>
/// Defines the maximum number of data blocks that can be 
/// assigned to a chunk. 
/// This clamps the ftChunk::count field.
/// If this limit is reached, the status FS_INV_LENGTH will be returned.
/// </summary>
#define FileTools_MaxChunkArrayLength  65536


#endif  //_ftConfig_h_
