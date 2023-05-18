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

#include "ftLogger.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "Utils/skHexPrint.h"
#include "Utils/skPlatformHeaders.h"
#include "ftMember.h"
#include "ftTable.h"

using namespace std;
using namespace ftFlags;

void ftLogger::log(int status)
{
    cout << "Exit status: ";
    switch (status)
    {
    case FS_INV_HEADER_STR:
        cout << "FS_INV_HEADER_STR";
        break;
    case FS_INV_LENGTH:
        cout << "FS_INV_LENGTH";
        break;
    case FS_INV_READ:
        cout << "FS_INV_READ";
        break;
    case FS_BAD_ALLOC:
        cout << "FS_BAD_ALLOC";
        break;
    case FS_INV_INSERT:
        cout << "FS_INV_INSERT";
        break;
    case FS_TABLE_INIT_FAILED:
        cout << "FS_TABLE_INIT_FAILED";
        break;
    case FS_OVERFLOW:
        cout << "FS_OVERFLOW";
        break;
    case FS_FAILED:
        cout << "FS_FAILED";
        break;
    case RS_INVALID_PTR:
        cout << "RS_INVALID_PTR";
        break;
    case RS_INVALID_CODE:
        cout << "RS_INVALID_CODE";
        break;
    case RS_LIMIT_REACHED:
        cout << "RS_LIMIT_REACHED";
        break;
    case RS_BAD_ALLOC:
        cout << "RS_BAD_ALLOC";
        break;
    case RS_MIS_ALIGNED:
        cout << "RS_MIS_ALIGNED";
        break;
    case FS_INV_VALUE:
        cout << "FS_INV_VALUE";
        break;
    case FS_CODE_ERROR:
        cout << "FS_CODE_ERROR";
        break;
    case FS_INTEGRITY_FAIL:
        cout << "FS_INTEGRITY_FAIL";
        break;
    case FS_DUPLICATE_BLOCK:
        cout << "FS_DUPLICATE_BLOCK";
        break;
    case FS_OK:
        cout << "FS_OK";
        break;
    case FS_STATUS_MIN:
        cout << "FS_STATUS_MIN";
        break;
    default: 
        cout << "Unknown";
        break;
    }
    cout << endl;
}

void ftLogger::log(int status, const char* format, ...)
{
    log(status);
    if (format)
    {
        char    buf[513];
        va_list lst;
        va_start(lst, format);
        int size = skp_printf(buf, 512, format, lst);
        va_end(lst);
        if (size < 0)
            size = 0;
        buf[size] = 0;
        cout << buf << endl;
    }
}

void ftLogger::logF(const char* format, ...)
{
    if (format)
    {
        char buf[513] = {};

        va_list lst;
        va_start(lst, format);
        int size = skp_printf(buf, 512, format, lst);
        va_end(lst);
        if (size < 0)
            size = 0;

        buf[size] = 0;
        cout << buf << endl;
    }
}

void ftLogger::separator()
{
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << setfill('-') << setw(FT_VOID_8 ? 87 : 79) << '-';
    cout << setfill(' ') << endl;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::divider()
{
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << setfill('-') << setw(FT_VOID_8 ? 47 : 39) << '-';
    cout << setfill(' ') << endl;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::log(const ftChunk& chunk)
{
    char* cp = (char*)&chunk.code;
    char  buf[5];
    memcpy(buf, cp, 4);
    buf[4] = '\0';

    skHexPrint::writeColor(CS_DARKYELLOW);
    cout << "Chunk" << endl;
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << "Code   : " << buf << endl;
    cout << "Len    : " << dec << chunk.length << endl;
    cout << "Old    : 0x" << hex << chunk.address << endl;
    cout << "TypeId : " << dec << chunk.structId << endl;
    cout << "Count  : " << dec << chunk.count << endl;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::color(skConsoleColorSpace cs)
{
    skHexPrint::writeColor(cs);
}

void ftLogger::log(const void* ptr, const SKsize& len)
{
    dumpHex(
        (char*)ptr,
        0,
        len,
        skHexPrint::PF_DEFAULT,
        -1);
}

void ftLogger::newline(int n)
{
    for (int i = 0; i < n; ++i)
        cout << endl;
}

void ftLogger::log(ftStruct* structure)
{
    skHexPrint::writeColor(CS_GREEN);
    cout << "Struct : " << structure->getName() << endl;
    skHexPrint::writeColor(CS_LIGHT_GREY);
    skHexPrint::writeColor(CS_WHITE);

    cout << "Type          : " << structure->getTypeIndex() << endl;
    cout << "Hash          : " << structure->getHashedType() << endl;
    cout << "Size In Bytes : " << structure->getSizeInBytes() << endl;
    cout << "Aligned 4     : " << (structure->getSizeInBytes() % 4 == 0 ? 1 : 0) << endl;
}

void ftLogger::log(ftMember* member)
{
    ftStruct* parent = member->getParent();
    if (parent != nullptr)
    {
        separator();
        skHexPrint::writeColor(CS_GREEN);
        cout << "Struct        : " << parent->getName() << endl;
        divider();
        cout << "Type          : " << member->getType() << endl;
        cout << "Name          : " << member->getName() << endl;
        cout << "Pointer Count : " << member->getPointerCount() << endl;
        cout << "Array Size    : " << member->getArraySize() << endl;
    }
}

void ftLogger_logStructTable(ftStruct* structure)
{
    cout << "Struct        : " << structure->getName() << endl;
    ftLogger::divider();

    int                         nr = 0;
    ftStruct::Members::Iterator it = structure->getMemberIterator();
    while (it.hasMoreElements())
    {
        ftMember* mbr = it.getNext();
        cout << setw(10) << nr;
        cout << setw(20) << mbr->getType();
        cout << setw(20) << mbr->getName();
        cout << endl;
        ++nr;
    }
}

string makeName(const char* name, size_t max)
{
    string rv = name;
    if (rv.size() > max - 2)
    {
        string nrv;
        for (size_t i = 0; i < max - 5; ++i)
            nrv.push_back(rv[i]);
        nrv += "...";
        rv = nrv;
    }
    return rv;
}

void ftLogger::log(ftStruct* fileStruct, ftStruct* memoryStruct)
{
    const int a = fileStruct->getMemberCount();
    const int b = memoryStruct->getMemberCount();
    const int c = skMax(a, b);
    separator();
    cout << left;
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << ' ';
    cout << setw(15) << "Type";
    cout << setw(15) << "Name";
    cout << setw(10) << "Offset";
    cout << setw(15) << "Type";
    cout << setw(15) << "Name";
    cout << setw(10) << "Offset";
    cout << endl;
    skHexPrint::writeColor(CS_WHITE);
    separator();

    for (int d = 0; d < c; ++d)
    {
        ftMember *fileMember = nullptr, *memoryMember = nullptr;
        if (d < a)
            fileMember = fileStruct->getMember(d);
        if (d < b)
            memoryMember = memoryStruct->getMember(d);

        cout << ' ';
        if (fileMember != nullptr)
        {
            cout << setw(15) << makeName(fileMember->getType(), 15);
            cout << setw(15) << makeName(fileMember->getName(), 15);
            cout << setw(10) << fileMember->getOffset();
        }
        else
            cout << setw(40) << ' ';

        if (memoryMember != nullptr)
        {
            cout << setw(15) << makeName(memoryMember->getType(), 15);
            cout << setw(15) << makeName(memoryMember->getName(), 15);
            cout << setw(10) << memoryMember->getOffset();
        }
        else
            cout << setw(40) << ' ';

        cout << endl;
    }
    separator();

    skHexPrint::writeColor(CS_GREEN);
    cout << "Size in bytes:" << setw(17) << ' ';
    cout << setw(10) << fileStruct->getSizeInBytes();
    cout << setw(30) << ' ';
    cout << setw(10) << memoryStruct->getSizeInBytes();
    cout << endl;
    cout << left;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::log(const ftName& name)
{
    separator();
    skHexPrint::writeColor(CS_GREEN);
    cout << "Name                 : " << name.name << endl;
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << "Hash                 : " << name.hash << endl;
    cout << "Pointer Count        : " << name.pointerCount << endl;
    cout << "Function pointer     : " << name.isFunctionPointer << endl;
    cout << "Number Of Dimensions : " << name.dimensionCount << endl;
    cout << "Dimension Size       : ";

    for (int i = 0; i < skMin<SKint8>(name.dimensionCount, FileTools_MaxArrayDim); ++i)
    {
        if (i > 0)
            cout << ',' << ' ';
        cout << name.dimensions[i];
    }
    cout << endl;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::log(const ftType& type)
{
    separator();
    skHexPrint::writeColor(CS_GREEN);
    cout << "Name                 : " << type.name << endl;
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << "Hash                 : " << type.hash << endl;
    cout << "Structure ID         : " << type.id << endl;
}

void ftLogger::log(const ftType& type, FTtype spacing)
{
    cout << right << setw(10) << spacing << ' ';
    skHexPrint::writeColor(CS_LIGHT_GREY);
    cout << left << type.name;

    cout << endl;
    skHexPrint::writeColor(CS_WHITE);
}

void ftLogger::logDiagnosticsCastHeader(const ftChunk& chunk,
                                        ftStruct*      fileStruct,
                                        ftStruct*      memoryStruct)
{
    newline(2);
    separator();
    log(chunk);
    separator();
    color(CS_GREEN);
    logF("Struct  : %s -> %s",
         fileStruct->getName(),
         memoryStruct->getName());
    log(fileStruct, memoryStruct);
}

void ftLogger::logDiagnosticsCastMemberHeader(ftMember* destMember,
                                              ftMember* sourceMember)
{
    newline();
    color(CS_DARKYELLOW);
    logF("%s %s (%d) ==> %s %s (%d)",
         sourceMember->getType(),
         sourceMember->getName(),
         sourceMember->getOffset(),
         destMember->getType(),
         destMember->getName(),
         sourceMember->getOffset());
}

void ftLogger::logReadChunk(const ftChunk& chunk, const void* block, const SKsize& len)
{
    newline();
    log(chunk);
    separator();
    log(block, len);
    separator();
}

void ftLogger::logSkipChunk(const ftChunk& chunk,
                            ftStruct*      fileStruct,
                            const void*    block,
                            const SKsize&  len)
{
    newline();
    color(CS_RED);
    logF("Skipping Chunk for structure %s", fileStruct->getName());
    newline();
    log(chunk);
    separator();
    log(block, len);
    separator();
}

void ftLogger::logMisCalculatedChunk(const ftChunk& chunk,
                                     SKsize         expected,
                                     SKsize         reported)
{
    log(chunk);
    color(CS_RED);
    logF(
        "Error the calculated chunk size does "
        "not match the reported size from the file:\n\texpected(%i) -> actual(%i)",
        expected,
        reported);
    color(CS_WHITE);
}

void ftLogger::logUnresolvedStructure(ftMemoryChunk* bin, ftStruct* fileStruct, ftStruct* memoryStruct)
{
    separator();
    logF("Failed to resolve both file and memory declarations for chunk:");
    log(bin->chunk);
    logF("File   : %s", fileStruct ? "Valid" : "Invalid");
    logF("Memory : %s", memoryStruct ? "Valid" : "Invalid");

    if (fileStruct)
    {
        log(fileStruct);
        separator();
        log(bin->fileBlock, fileStruct->getSizeInBytes());
    }
    if (memoryStruct)
    {
        log(memoryStruct);
        separator();
        log(bin->memoryBlock, memoryStruct->getSizeInBytes());
    }
    newline(2);
}

void ftLogger::logInvalidInsert(ftMemoryChunk* bin)
{
    separator();
    logF("Failed to insert chunk");
    log(bin->chunk);
    separator();
    log(bin->fileBlock, bin->chunk.length);
}

void ftLogger::logAlignment(const char* file,
                            SKsize      line,
                            SKsize      alignment,
                            const char* type,
                            const char* name,
                            SKsize      needed)
{
    if (type && name)
    {
        logF("%s error (%i): align %i: %s %s add %i bytes\n",
             file,
             line,
             alignment,
             type,
             name,
             needed);
    }
    else
    {
        logF("%s error (%i): align %i: add %i bytes\n",
             file,
             line,
             alignment,
             needed);
    }
}
