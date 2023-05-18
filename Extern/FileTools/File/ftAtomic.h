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
#ifndef _ftAtomic_h_
#define _ftAtomic_h_

#include "ftTypes.h"

/// <summary>
/// The ftAtomic enumeration is used to describe the primitive data types.
/// </summary>
/// <remarks>
/// The placement order of the enumerated types does matter. 
/// The type ids for structures start directly after FT_ATOMIC_VOID.
/// </remarks>
enum ftAtomic
{
    FT_ATOMIC_CHAR,
    FT_ATOMIC_UCHAR,
    FT_ATOMIC_SHORT,
    FT_ATOMIC_USHORT,
    FT_ATOMIC_INT,
    FT_ATOMIC_LONG,
    FT_ATOMIC_ULONG,
    FT_ATOMIC_FLOAT,
    FT_ATOMIC_DOUBLE,
    FT_ATOMIC_INT64_T,
    FT_ATOMIC_UINT64_T,
    FT_ATOMIC_SCALAR_T,
    FT_ATOMIC_VOID,
    FT_ATOMIC_UNKNOWN
};

/// <summary>
/// ftAtomicType is a structure that is used to manage the default data types.
/// </summary>
struct ftAtomicType
{
    /// <summary>
    /// Defines the data type name.
    /// </summary>
    const char* name;

    /// <summary>
    /// Defines the size in bytes of the data type.
    /// </summary>
    SKuint16 size;

    /// <summary>
    /// Is an enumerated type code of the data type.
    /// </summary>
    ftAtomic type;

    /// <summary>
    /// The computed hash of the data type.
    /// </summary>
    SKhash hash;
};

/// <summary>
/// ftAtomicUtils is a utility class to aid with casting struct members.
/// </summary>
class ftAtomicUtils
{
public:
    /// <summary>
    /// Performs a linear search of the built-in types to
    /// determine if the provided key is a match.
    /// </summary>
    /// <param name="typeKey">The key to test.</param>
    /// <returns>The corresponding ftAtomic enumeration, or FT_ATOMIC_UNKNOWN
    /// if it is not a primitive type defined in this system.
    /// </returns>
    static ftAtomic getPrimitiveType(const SKhash& typeKey);

    /// <summary>
    /// Hashes the supplied type name then calls ftAtomicUtils::getPrimitiveType with the result.
    /// </summary>
    /// <param name="typeName">The name to hash.</param>
    /// <returns>The corresponding ftAtomic enumeration or FT_ATOMIC_UNKNOWN
    /// if it is not a primitive type defined in this system.
    /// </returns>
    static ftAtomic getPrimitiveType(const char* typeName);

    /// <summary>
    /// Test to determine if the supplied key is an integer data type.
    /// </summary>
    /// <param name="typeKey">The key to test.</param>
    /// <returns>True if it is an integer type in the range of [FT_ATOMIC_CHAR,  FT_ATOMIC_SCALAR_T).</returns>
    static bool isInteger(const SKhash& typeKey);

    /// <summary>
    /// Test to determine if the supplied key is a floating-point data type.
    /// </summary>
    /// <param name="typeKey">The key to test.</param>
    /// <returns>True if it is a type in the range of [FT_ATOMIC_FLOAT,  FT_ATOMIC_SCALAR_T].</returns>
    static bool isReal(const SKhash& typeKey);

    /// <summary>
    /// Test to determine if it is an integer or a real number.
    /// </summary>
    /// <param name="typeKey">The key to test.</param>
    /// <returns>True if it is an integer or a real number otherwise returns false.</returns>
    static bool isNumeric(const SKhash& typeKey);

    /// <summary>
    /// Test to determine if the types can be cast together.
    /// </summary>
    /// <param name="typeKeyA">The first key to test.</param>
    /// <param name="typeKeyB">The second key to test. This should be compatible with the first key.</param>
    /// <returns>True if the types are compatible otherwise returns false.</returns>
    static bool canCast(const SKhash& typeKeyA, const SKhash& typeKeyB);

    /// <summary>
    /// Casts the source memory into the destination memory.
    /// </summary>
    /// <param name="source">The source memory.</param>
    /// <param name="destination">The destination memory.</param>
    /// <param name="sourceType">The atomic type for the source memory.</param>
    /// <param name="destinationType">The atomic type for the destination memory.</param>
    /// <param name="length">The total length of the memory.</param>
    static void cast(char*    source,
                     char*    destination,
                     ftAtomic sourceType,
                     ftAtomic destinationType,
                     SKsize   length);

    /// <summary>
    /// Casts the source memory into the destination memory with the option of separate offsets.
    /// This is used when the file structure and memory structure are different.
    /// </summary>
    /// <param name="source">The source memory.</param>
    /// <param name="srcOffs">The offset to the address in the source memory.</param>
    /// <param name="destination">The destination memory.</param>
    /// <param name="dstOffs">The offset to the address in the destination memory.</param>
    /// <param name="sourceType">The atomic type for the source memory.</param>
    /// <param name="destinationType">The atomic type for the destination memory.</param>
    /// <param name="length">The total length of the memory.</param>
    static void cast(char*    source,
                     SKsize   srcOffs,
                     char*    destination,
                     SKsize   dstOffs,
                     ftAtomic sourceType,
                     ftAtomic destinationType,
                     SKsize   length);

    /// <summary>
    /// Static type table for the built-in atomic types.
    /// </summary>
    static const ftAtomicType Types[];

    /// <summary>
    /// The total size of the type table.
    /// </summary>
    static const SKsize NumberOfTypes;
};

#endif  //_ftAtomic_h_
