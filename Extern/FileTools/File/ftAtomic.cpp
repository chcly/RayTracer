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
#include "Utils/skDisableWarnings.h"
#include "ftAtomic.h"
#include "Utils/skPlatformHeaders.h"
#include "ftHashTypes.h"
using namespace ftFlags;

SKhash makeHash(const char* name)
{
    if (!name || !*name)
        return -1;
    return ftCharHashKey(name).hash();
}

ftAtomic ftAtomicUtils::getPrimitiveType(const SKhash& typeKey)
{
    ftAtomic res = FT_ATOMIC_UNKNOWN;
    for (SKsize i = 0; i < NumberOfTypes && res == FT_ATOMIC_UNKNOWN; ++i)
    {
        if (Types[i].hash == typeKey)
            res = Types[i].type;
    }
    return res;
}

bool ftAtomicUtils::canCast(const SKhash& typeKeyA, const SKhash& typeKeyB)
{
    return isNumeric(typeKeyA) && isNumeric(typeKeyB);
}

ftAtomic ftAtomicUtils::getPrimitiveType(const char* typeName)
{
    if (!typeName || !*typeName)
        return FT_ATOMIC_UNKNOWN;
    return getPrimitiveType(ftCharHashKey(typeName).hash());
}

bool ftAtomicUtils::isInteger(const SKhash& typeKey)
{
    const ftAtomic tp = getPrimitiveType(typeKey);
    return tp < FT_ATOMIC_FLOAT;
}

bool ftAtomicUtils::isReal(const SKhash& typeKey)
{
    const ftAtomic tp = getPrimitiveType(typeKey);
    return tp == FT_ATOMIC_FLOAT || tp == FT_ATOMIC_DOUBLE || tp == FT_ATOMIC_SCALAR_T;
}

bool ftAtomicUtils::isNumeric(const SKhash& typeKey)
{
    const ftAtomic tp = getPrimitiveType(typeKey);
    return tp != FT_ATOMIC_VOID && tp != FT_ATOMIC_UNKNOWN;
}

template <typename T>
double ftAtomicUtils_getValue(char*& src)
{
    const double value = (double)(T) * (T*)src;
    src += sizeof(T);
    return value;
}

template <typename T>
void ftAtomicUtils_setValue(char*& dest, const double& value)
{
    *(T*)dest = (T)value;
    dest += sizeof(T);
}

void ftAtomicUtils_set(char*& destination, ftAtomic destinationType, const double& value)
{
    switch (destinationType)
    {
    case FT_ATOMIC_UCHAR:
    case FT_ATOMIC_CHAR:
        ftAtomicUtils_setValue<char>(destination, value);
        break;
    case FT_ATOMIC_SHORT:
        ftAtomicUtils_setValue<short>(destination, value);
        break;
    case FT_ATOMIC_USHORT:
        ftAtomicUtils_setValue<unsigned short>(destination, value);
        break;
    case FT_ATOMIC_INT:
        ftAtomicUtils_setValue<int>(destination, value);
        break;
    case FT_ATOMIC_ULONG:
    case FT_ATOMIC_LONG:
        ftAtomicUtils_setValue<long>(destination, value);
        break;
    case FT_ATOMIC_FLOAT:
        ftAtomicUtils_setValue<float>(destination, value);
        break;
    case FT_ATOMIC_DOUBLE:
        ftAtomicUtils_setValue<double>(destination, value);
        break;
    case FT_ATOMIC_INT64_T:
        ftAtomicUtils_setValue<SKint64>(destination, value);
        break;
    case FT_ATOMIC_UINT64_T:
        ftAtomicUtils_setValue<SKuint64>(destination, value);
        break;
    case FT_ATOMIC_SCALAR_T:
        ftAtomicUtils_setValue<scalar_t>(destination, value);
        break;
    case FT_ATOMIC_VOID:
    case FT_ATOMIC_UNKNOWN:
        break;
    }
}

void ftAtomicUtils::cast(char*    source,
                         char*    destination,
                         ftAtomic sourceType,
                         ftAtomic destinationType,
                         SKsize   length)
{
    if (!source || !destination)
    {
        printf("Invalid source and destination pointers!\n");
        return;
    }

    double value = 0;
    SKsize i     = 0;

    while (i < length)
    {
        switch (sourceType)
        {
        case FT_ATOMIC_UCHAR:
        case FT_ATOMIC_CHAR:
            value = ftAtomicUtils_getValue<char>(source);
            break;
        case FT_ATOMIC_SHORT:
            value = ftAtomicUtils_getValue<short>(source);
            break;
        case FT_ATOMIC_USHORT:
            value = ftAtomicUtils_getValue<unsigned short>(source);
            break;
        case FT_ATOMIC_INT:
            value = ftAtomicUtils_getValue<int>(source);
            break;
        case FT_ATOMIC_ULONG:
        case FT_ATOMIC_LONG:
            value = ftAtomicUtils_getValue<long>(source);
            break;
        case FT_ATOMIC_FLOAT:
            value = ftAtomicUtils_getValue<float>(source);
            break;
        case FT_ATOMIC_DOUBLE:
            value = ftAtomicUtils_getValue<double>(source);
            break;
        case FT_ATOMIC_INT64_T:
            value = ftAtomicUtils_getValue<SKint64>(source);
            break;
        case FT_ATOMIC_UINT64_T:
            value = ftAtomicUtils_getValue<SKuint64>(source);
            break;
        case FT_ATOMIC_SCALAR_T:
            value = ftAtomicUtils_getValue<scalar_t>(source);
            break;
        case FT_ATOMIC_VOID:
        case FT_ATOMIC_UNKNOWN:
            value = 0;
            break;
        }

        ftAtomicUtils_set(destination, destinationType, value);
        ++i;
    }
}

void ftAtomicUtils::cast(char*    source,
                         SKsize   srcOffs,
                         char*    destination,
                         SKsize   dstOffs,
                         ftAtomic sourceType,
                         ftAtomic destinationType,
                         SKsize   length)
{
    if (srcOffs != SK_NPOS && dstOffs != SK_NPOS)
    {
        cast(source + srcOffs,
             destination + dstOffs,
             sourceType,
             destinationType,
             length);
    }
}

#ifdef ftSCALAR_DOUBLE
#define scalar_t_size sizeof(double)
#else
#define scalar_t_size sizeof(float)
#endif

// clang-format off
const ftAtomicType ftAtomicUtils::Types[] = {
    {"char",     sizeof(char),     FT_ATOMIC_CHAR,     makeHash("char")},
    {"uchar",    sizeof(char),     FT_ATOMIC_UCHAR,    makeHash("uchar")},
    {"short",    sizeof(short),    FT_ATOMIC_SHORT,    makeHash("short")},
    {"ushort",   sizeof(short),    FT_ATOMIC_USHORT,   makeHash("ushort")},
    {"int",      sizeof(int),      FT_ATOMIC_INT,      makeHash("int")},
    {"long",     sizeof(long),     FT_ATOMIC_LONG,     makeHash("long")},
    {"ulong",    sizeof(long),     FT_ATOMIC_ULONG,    makeHash("ulong")},
    {"float",    sizeof(float),    FT_ATOMIC_FLOAT,    makeHash("float")},
    {"double",   sizeof(double),   FT_ATOMIC_DOUBLE,   makeHash("double")},
    {"int64_t",  sizeof(SKint64),  FT_ATOMIC_INT64_T,  makeHash("int64_t")},
    {"uint64_t", sizeof(SKuint64), FT_ATOMIC_UINT64_T, makeHash("uint64_t")},
    {"scalar_t", scalar_t_size,    FT_ATOMIC_SCALAR_T, makeHash("scalar_t")},
    {"void",     0,                FT_ATOMIC_VOID,     makeHash("void")},
};

const SKsize ftAtomicUtils::NumberOfTypes = sizeof Types / sizeof(ftAtomicType);
// clang-format on
