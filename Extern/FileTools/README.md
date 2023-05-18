# FileTools

The FileTools project is a small collection of tools that are centered around loading file formats which are similar in structure to the [Blender](https://blender.org) .blend file. The main idea of the file format is to take a C or C++ API and compile it into a set of tables that can be used to rebuild the API on load.

## Contents

1. [FileTools](#filetools)
    1. [Contents](#contents)
    2. [File Structure](#file-structure)
        1. [File Header](#file-header)
        2. [Chunk Header](#chunk-header)
        3. [Reserved Codes](#reserved-codes)
    3. [Table structure](#table-structure)
        1. [Type table(TYPE)](#type-tabletype)
        2. [Name table(NAME)](#name-tablename)
        3. [Type size table(TLEN)](#type-size-tabletlen)
        4. [Structure table(STRC)](#structure-tablestrc)
        5. [Building the tables](#building-the-tables)
            1. [Scanner](#scanner)
            2. [Usage](#usage)
            3. [Output](#output)
            4. [CMake Utility](#cmake-utility)
        6. [Reversing a file's tables](#reversing-a-files-tables)
    4. [Sub-modules](#sub-modules)
    5. [Testing](#testing)
    6. [Building](#building)
    7. [Optional defines](#optional-defines)
    8. [Extra documentation](#extra-documentation)

## File Structure

The file structure is a simple chunk based format. It consists of a 12-byte file header followed by n-number of chunks then closed with an blank chunk header. The closing header should contain all zeros except for the code which should be set to the ENDB identifier.

### File Header

The file header determines the file type, the architecture of the saving platform, and the API version.

| Byte(s) | Data Type | Description                                                      |
|:-------:|:----------|:-----------------------------------------------------------------|
|  [0,6]  | char[7]   | Used to determine the type of file.                              |
|    7    | char      | Byte 7 is used to determine whether to load 32 or 64 bit chunks. |
|    8    | char      | Byte 8 identifies the byte-order of the file.                    |
| [9,11]  | int       | Is a three-digit version code. (EG: 1.5.0 equals 150)            |

The following ASCII codes are reserved for bytes 7 and 8:

| ASCII | Hex  | Description                                                   |
|:------|:-----|:--------------------------------------------------------------|
| '-'   | 0x2D | Indicates that the file was saved with 64-bit chunks.         |
| '_'   | 0x5F | Indicates that the file was saved with 32-bit chunks.         |
| 'V'   | 0x56 | Indicates that the file was saved on a big-endian machine.    |
| 'v'   | 0x76 | Indicates that the file was saved on a little-endian machine. |

### [Chunk Header](File/ftChunk.h#30)

The chunk header is a varying sized structure that is 20 or 24 bytes. Its size is dependent on the platform architecture at the time of saving because it stores the heap address of its data block.

```cpp
struct ChunkNative
{
    unsigned int code;      // 4 bytes
    unsigned int length;    // 4 bytes
    size_t       address;   // 4|8 bytes
    unsigned int structId;  // 4 bytes
    unsigned int count;     // 4 bytes
}; // 20 or 24 bytes total

struct Chunk32
{
    unsigned int code;      // 4 bytes
    unsigned int length;    // 4 bytes
    unsigned int address;   // 4 bytes
    unsigned int structId;  // 4 bytes
    unsigned int count;     // 4 bytes
}; // 20 bytes total

struct Chunk64
{
    unsigned int      code;      // 4 bytes
    unsigned int      length;    // 4 bytes
    unsigned int64_t  address;   // 8 bytes
    unsigned int      structId;  // 4 bytes
    unsigned int      count;     // 4 bytes
}; // 24 bytes total
```

| Member   | Description                                                              |
|:---------|:-------------------------------------------------------------------------|
| code     | Is a unique identifier for determining how this block should be handled. |
| length   | Is the size in bytes of the data block.                                  |
| address  | Is the heap address of the data block at the time of saving.             |
| structId | Is the type index found in the SDNA->TYPE table.                         |
| count    | Is the number of subsequent blocks being saved in this chunk.            |

A complete chunk includes a header and block of memory directly following the header. The block of memory should be the same length as described in the header.

The following is an example of a 32-bit chunk:

```txt
Chunk
Code   : OLIB
Len    : 152
Addr   : 0x15170d0
StrcId : 0
Count  : 1
-------------------------------------------------------------------------------
00000000   43 75 62 65 00 00 00 00  00 00 00 00 00 00 00 00  |Cube............|
00000010   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000020   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000030   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000040   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000050   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000060   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000070   00 00 00 00 63 58 E2 3F  00 00 00 00 63 58 E2 3F  |....cX.?....cX.?|
00000080   00 00 00 00 63 58 E2 3F  02 00 00 00 50 41 44 42  |....cX.?....PADB|
00000090   68 09 B9 01 CD CD CD CD                           |h.......        |
-------------------------------------------------------------------------------
```

### [Reserved Codes](File/ftTables.h#L38)

The following are reserved codes that the loader internally uses when reading a file.  

| CODE | Description                                                                                                                      |
|:-----|:---------------------------------------------------------------------------------------------------------------------------------|
| DNA1 | Lets the loader know that it needs to load the API tables.                                                                       |
| SDNA | API  Table header. This is unused outside of testing for the existence of the tables.                                            |
| NAME | Indicates that a NAME table follows.                                                                                             |
| TYPE | Indicates that a TYPE table follows..                                                                                            |
| TLEN | Indicates that a TLEN table follows.                                                                                             |
| STRC | Indicates that a STRC table follows.                                                                                             |
| DATA | Indicates a block of data that may or may not have a structure associated with it but still needs to be relinked by its address. |
| ENDB | Indicates that there are no more chunks to read.                                                                                 |

## [Table structure](File/ftTables.h)

### Type table(TYPE)

This table starts with a 4-byte TYPE code. Directly following the code is a 4-byte integer containing the number of strings in the type table. The table itself is a NULL-terminated string array. Built-in or atomic types are declared first, which is then followed by user-defined class/struct names.

### Name table(NAME)

The name table is identical in structure as the type table. Its string array contains the member names of all user-defined types. Any extra information about a specific member other than its atomic type comes from the name. Which would be, for instance, determining whether or not it is a pointer, a pointer to a pointer, an array or multidimensional array, etc.. By default, FileTools supports only three-dimensional arrays. The preprocessor definition FT_ARR_DIM_MAX 3, may be changed to support larger n-dimensional arrays.

### Type size table(TLEN)

The TLEN table is an array of 2-byte integers. It contains the same number of elements as the type table. The index for the type table directly corresponds to the same element in the size table. The values in this table contain the computed size for the atomic types as well as all user-defined types.

For example:

```c
struct vec3f
{
    float x; // size[type[float]] * name[x].array_len  = 4 bytes  
    float y; // size[type[float]] * name[y].array_len  = 4 bytes
    float z; // size[type[float]] * name[z].array_len  = 4 bytes
}; // size[type[vec3f]] = 12 bytes  
```

### Structure table(STRC)

The STRC table is an array of 2-byte integers that hold the indices of the other tables. The table starts with the 4-byte identifier STRC, then another 4-byte integer that contains the number of structure definitions in the table. Then each structure in the table is stored by a 2-byte integer that holds the type table index and another 2-byte integer that hold the total number of members in the structure. Then from zero to the total number of members, a pair of 2-byte integers hold the member's type index and the member's name index.

### [Building the tables](Tools/Compiler/TableCompiler.cpp)

The TableCompiler program compiles everything into the tables. It reads the files via the command line.

#### [Scanner](File/ftScanner.h)

The goal of the scanner is to extract only member variables from classes and structures. It does parse namespaces but only in as much to store its name. Every time the scanner reads an 'n', 'c', or an 's', it tests for a namespace class or struct keyword. If any of the keywords match, it attempts to store the keyword identifier. If it matches a struct or a class keyword, the scanner changes its state and attempts to extract and store member variables. The scanner will not parse C++ outside the scope of member variable declaration and does not calculate the size of an object with base types. If any API specific methods are needed, the scanner tests for the comment keyword:

```txt
// @makeft_ignore
```

If the scanner finds a match, it switches its state to ignore everything up to the end of the file or until the next reoccurrence of the keyword. One thing to note is that any member variable declarations must be visible to the scanner for it to calculate the correct size of the class or structure. Members can be public or private from the perspective of C++, but no member variables should be inside an ignore block.

#### Usage

```txt
Uasge: TableCompiler.exe <tablename> <ofilename> <ifile[0]> ... <ifile[n]>
```

| Argument    | Description                                       |
|:------------|:--------------------------------------------------|
| tablename   | A unique name for the output table array.         |
| ofilename   | Output file and path name for the generated code. |
| ifile[0..n] | Input API declaration file(s) to parse.           |

#### Output

The output should be included in the library that contains the rest of the file implementation.

```c
const unsigned char DocsExampleTables[]={
    0x53,0x44,0x4E,0x41,0x4E,0x41,0x4D,0x45,0x03,0x00,0x00,0x00,
    0x78,0x00,0x79,0x00,0x7A,0x00,0x62,0x79,0x54,0x59,0x50,0x45,
    0x0B,0x00,0x00,0x00,0x63,0x68,0x61,0x72,0x00,0x75,0x63,0x68,
    0x61,0x72,0x00,0x73,0x68,0x6F,0x72,0x74,0x00,0x75,0x73,0x68,
    0x6F,0x72,0x74,0x00,0x69,0x6E,0x74,0x00,0x6C,0x6F,0x6E,0x67,
    0x00,0x75,0x6C,0x6F,0x6E,0x67,0x00,0x66,0x6C,0x6F,0x61,0x74,
    0x00,0x64,0x6F,0x75,0x62,0x6C,0x65,0x00,0x76,0x6F,0x69,0x64,
    0x00,0x76,0x65,0x63,0x33,0x66,0x00,0x62,0x54,0x4C,0x45,0x4E,
    0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x04,0x00,
    0x04,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x0C,0x00,0x40,0x40,
    0x53,0x54,0x52,0x43,0x01,0x00,0x00,0x00,0x0A,0x00,0x03,0x00,
    0x07,0x00,0x00,0x00,0x07,0x00,0x01,0x00,0x07,0x00,0x02,0x00,
};
const unsigned int DocsExampleLen=sizeof(DocsExampleTables);
```

The pure virtual methods in [ftFile](File/ftFile.h) provide access for the loader to get the compiler output.

```c
virtual void*       getTables(void) = 0;
virtual FBTsize     getTableSize(void) = 0;
```

```c
#include "DocsExample.inl"

void* DocsExample::getTables(void)
{
    return (void*)DocsExampleTables;
}

FBTsize DocsExample::getTableSize(void)
{
    return DocsExampleLen;
}
```

#### [CMake Utility](Tools/Compiler/CMake/Readme.md)

The CMake utility can be used to attach table generation to a build. This macro will output the needed include file to the current build directory and then adds the build directory to the list of include paths.

### [Reversing a file's tables](Tools/Decompiler/TableDecompiler.cpp)

The TableDecompiler tool is for converting a file's tables back into usable C++. This program will scan the supplied input file for a DNA1 chunk and attempt to read the tables back. Then it will output a single header file that contains the sorted structure definitions. 

Example output can be found in the [BlendFile](FileFormats/Blend/Blender.h) loader.  

## Sub-modules

The files [gitupdate.py](gitupdate.py) or [gitupdate.bat](gitupdate.bat) help automate initial cloning and with keeping the modules up to date.

Once this project has been cloned. The following command will initialize any external modules.

```txt
python gitupdate.py 
...
gitupdate.bat 
```

## Testing

The testing directory is setup to work with [googletest](https://github.com/google/googletest). As well as the initial setup for testing the standalone module either using GitHub actions or with Jenkins.

## Building

Building with CMake and Make

```sh
mkdir build
cd build
cmake ..

make
```

## Optional defines

| Option                         | Description                         | Default |
|:-------------------------------|:------------------------------------|:--------|
| FileTools_BUILD_TEST           | Build the unit test program.        | OFF     |
| FileTools_AUTO_RUN_TEST        | Automatically run the test program. | OFF     |
| FileTools_BUILD_COMPILER       | Build the table compiler.           | ON      |
| FileTools_BUILD_DECOMPILER     | Build the table decompiler.         | ON      |
| FileTools_BUILD_RECOMPILE_TEST | Build the Decompile/Recompile test. | OFF     |
| FileTools_BLEND_FILE           | Build the Blend file loader.        | OFF     |
| FileTools_BLEND_TEST           | Builds a test .blend program.       | OFF     |

## Extra documentation

Extra documentation can be found [here](https://charlescarley.github.io/FileTools/).
