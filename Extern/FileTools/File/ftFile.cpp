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
#include "ftFile.h"
#include "Utils/skPlatformHeaders.h"
#include "ftAtomic.h"
#include "ftEndianUtils.h"
#include "ftHashTypes.h"
#include "ftLogger.h"
#include "ftMember.h"
#include "ftScanDNA.h"
#include "ftStreams.h"
#include "ftTable.h"

using namespace ftEndianUtils;
using namespace ftFlags;

ftFile::ftFile(const char* header) :
    m_headerFlags(0),
    m_fileFlags(LF_ONLY_ERR),
    m_headerId(header),
    m_fileSizeInBytes(0),
    m_curFile(nullptr),
    m_fileTableData(nullptr),
    m_filterList(nullptr),
    m_filterListLen(0),
    m_castFilter(nullptr),
    m_castFilterLen(0),
    m_inclusive(false),
    m_memoryVersion(-1),
    m_fileVersion(0),
    m_memory(nullptr),
    m_file(nullptr)
{
    m_chunks.clear();
}

ftFile::~ftFile()
{
    clearStorage();
}

int ftFile::load(const char* path, int mode)
{
    if (path == nullptr || !*path)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Path name must not be null.");
        return FS_FAILED;
    }

    if (mode == RM_COMPRESSED)
    {
        skFileStream fs;
        fs.open(path, skStream::READ);

        if (fs.isOpen())
        {
            // Test the file for magic numbers ID1, ID2
            // https://www.ietf.org/rfc/rfc1952.txt ( 2.3.1.)

            unsigned char magic[3];
            fs.read(magic, 2);
            magic[2] = 0;

            if (magic[0] != 0x1F && magic[1] != 0x8B)
            {
                // Assuming it is uncompressed or any other type of file.
                mode = RM_UNCOMPRESSED;
            }
        }
    }

    skStream* stream = openStream(path, mode);
    if (!stream->isOpen())
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("File '%s' loading failed.", path);
        return FS_FAILED;
    }

    m_curFile = path;

    const int result = parseStreamImpl(stream);
    delete stream;
    return result;
}

skStream* ftFile::openStream(const char* path, int mode)
{
    skStream* stream;
    if (mode == RM_UNCOMPRESSED || mode == RM_COMPRESSED)
    {
        if (mode == RM_COMPRESSED)
            stream = new ftGzStream();
        else
            stream = new skFileStream();
    }
    else
        stream = new skMemoryStream();

    stream->open(path, skStream::READ);

    m_fileSizeInBytes = stream->size();
    return stream;
}

int ftFile::load(const void* memory, SKsize sizeInBytes)  //, int mode)
{
    // Fix this, perhaps move zlib to utils
    // if the gzStream ever materializes..
    // and if there is really ever enough use for it.

    skMemoryStream ms;
    ms.open(memory, sizeInBytes, skStream::READ);  //, mode == PM_COMPRESSED);

    if (!ms.isOpen())
    {
        if (m_fileFlags != LF_NONE)
        {
            ftLogger::logF("Memory %p(%d) loading failed!\n",
                           memory,
                           sizeInBytes);
        }
        return FS_FAILED;
    }

    return parseStreamImpl(&ms);
}

int ftFile::parseHeader(skStream* stream)
{
    m_headerFlags = 0;
    m_fileVersion = 0;

    SKbyte* hp = m_header.ptr();
    stream->read(hp, HEADER_OFFSET);
    m_header.resize(HEADER_OFFSET);

    if (skChar::equalsn(hp, m_headerId, 7) != 0)
        return FS_INV_HEADER_STR;

    SKbyte* magic = hp + 7;
    if (*magic++ == FM_64_BIT)
    {
        m_headerFlags |= FH_CHUNK_64;
        if (FT_VOID_4)
            m_headerFlags |= FH_VAR_BITS;
    }
    else if (FT_VOID_8)
        m_headerFlags |= FH_VAR_BITS;

    const int  current = (int)getEndian();
    const char endian  = *magic++;

    if (endian == FM_BIG_ENDIAN)
    {
        if (current == FT_ENDIAN_IS_LITTLE)
            m_headerFlags |= FH_ENDIAN_SWAP;
    }
    else if (endian == FM_LITTLE_ENDIAN)
    {
        if (current == FT_ENDIAN_IS_BIG)
            m_headerFlags |= FH_ENDIAN_SWAP;
    }

    m_fileVersion = skChar::toInt32(magic);
    return FS_OK;
}

int ftFile::preScan(skStream* stream)
{
    ftScanDna scanner;
    scanner.setFlags(m_headerFlags);

    int status = scanner.scan(stream);
    if (status == FS_OK)
    {
        m_fileTableData = scanner.getDNA();
        if (m_fileTableData && scanner.getLength() > 0)
        {
            m_file = new ftTable((m_headerFlags & FH_CHUNK_64) != 0 ? 8 : 4);
            status = m_file->read(m_fileTableData, scanner.getLength(), m_headerFlags, m_fileFlags);
            if (status == FS_OK)
            {
                if (m_fileFlags & LF_DO_CHECKS)
                    status = runTableChecks(m_file);
            }
            else
            {
                if (m_fileFlags != LF_NONE)
                    ftLogger::logF("File table initialization failed.");
            }
        }
        else
            status = FS_TABLE_INIT_FAILED;
    }

    return status;
}

int ftFile::runTableChecks(ftTable* check) const
{
    if (check)
    {
        if (!check->testDuplicateKeys())
        {
            if (m_fileFlags != LF_NONE)
                ftLogger::logF("There are duplicate names in the table.");

            return FS_FAILED;
        }
    }
    return FS_OK;
}

int ftFile::parseStreamImpl(skStream* stream)
{
    ftChunk chunk = ftChunkUtils::BlankChunk;

    // Ensure that any memory from a previous
    // call has been freed.
    clearStorage();

    int status = parseHeader(stream);
    if (status != FS_OK)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::log(status, "Failed to extract the file header.");
        return status;
    }

    status = initializeMemory();
    if (status != FS_OK)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::log(status, "Failed to initialize the memory tables.");
        return status;
    }

    status = preScan(stream);
    if (status != FS_OK)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::log(status, "Failed to pre-scan the file.");
        return status;
    }

    if (!stream->seek(HEADER_OFFSET, SEEK_SET))
    {
        status = FS_INV_READ;
        ftLogger::log(status, "Failed to seek back to the header.");
    }

    m_map.reserve(skClamp(FileTools_DefaultAllocationSize, 0, 4096));

    while (chunk.code != ftIdNames::ENDB &&
           chunk.code != ftIdNames::DNA1 &&
           status == FS_OK && !stream->eof())
    {
        if ((int)ftChunkUtils::read(&chunk, stream, m_headerFlags, m_fileSizeInBytes) <= 0)
            status = FS_INV_READ;
        else if (chunk.code == ftIdNames::TEST)
        {
            if (!stream->seek(chunk.length, SEEK_CUR))
            {
                if (m_fileFlags != LF_NONE)
                    ftLogger::logF("Failed to skip over a TEST chunk.");
                status = FS_INV_READ;
            }
        }
        else if (chunk.code != ftIdNames::ENDB && chunk.code != ftIdNames::DNA1)
        {
            if ((int)chunk.length > 0 && chunk.length < m_fileSizeInBytes)
            {
                void* curPtr = malloc(chunk.length);
                if (!curPtr)
                    status = FS_BAD_ALLOC;
                else
                {
                    if (stream->read(curPtr, chunk.length) <= 0)
                        status = FS_INV_READ;
                    else
                        handleChunk(curPtr, chunk.length, chunk, status);
                }
            }
            else
                status = FS_INV_LENGTH;
        }
    }
    if (status == FS_OK)
        status = rebuildStructures();

    if (m_fileFlags != LF_NONE)
    {
        if (status != FS_OK)
            ftLogger::log(status, "File read failed.");
    }

    if (chunk.code != ftIdNames::DNA1)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Failed to reach the end byte.");

        if (status == FS_OK)
            status = FS_INV_READ;
    }
    return status;
}

void ftFile::handleChunk(void*          block,
                         SKsize         allocLen,
                         const ftChunk& chunk,
                         int&           status)
{
    ftMemoryChunk* bin = (ftMemoryChunk*)malloc(sizeof(ftMemoryChunk));
    if (bin)
    {
        memset(bin, 0, sizeof(ftMemoryChunk));
        memcpy(&bin->chunk, &chunk, sizeof(ftChunk));

        // This is saved here to recalculate the total
        // number of elements in a pointer array.
        bin->pointerBlockLen = (SKuint32)allocLen;

        const ftPointerHashKey phk(chunk.address);
        if (m_map.find(phk) != m_map.npos)
        {
            free(block);
            freeChunk(bin);

            // This should be an error if it's properly linked,
            // in theory there should be no duplicates.
            status = FS_DUPLICATE_BLOCK;
        }
        else
        {
            bin->fileBlock    = block;
            bin->fileBlockLen = (SKuint32)allocLen;

            ftStruct* memoryStruct = nullptr;

            if ((bin->chunk.code == ftIdNames::DATA ||
                 bin->chunk.code == ftIdNames::REND) &&
                bin->chunk.structId <= m_file->getFirstStructType())
            {
                status = allocateMBlock(phk,
                                        bin,
                                        (SKsize)chunk.length * (SKsize)chunk.count,
                                        false);
            }
            else
            {
                ftStruct* fileStruct = m_file->findStructByType(bin->chunk.structId);
                if (fileStruct)
                {
                    memoryStruct = findInMemoryTable(fileStruct);

                    if (memoryStruct)
                    {
                        const SKuint32 expected = fileStruct->getSizeInBytes() * chunk.count;
                        if (expected != chunk.length)
                        {
                            if (m_fileFlags & LF_MIS_REPORTED)
                                ftLogger::logMisCalculatedChunk(chunk, expected, bin->fileBlockLen);

                            status = FS_INV_LENGTH;
                        }
                    }
                }

                if (fileStruct && memoryStruct && status == FS_OK)
                {
                    bin->fileStruct   = fileStruct;
                    bin->memoryStruct = memoryStruct;
                    bin->newTypeId    = (FTtype)bin->memoryStruct->getStructIndex();

                    if (!skip(fileStruct->getHashedType()))
                    {
                        const SKsize totalAlloc = (SKsize)bin->chunk.count * (SKsize)bin->memoryStruct->getSizeInBytes();

                        status = allocateMBlock(phk,
                                                bin,
                                                totalAlloc,
                                                true);
                    }
                    else
                    {
                        if (m_fileFlags & LF_DIAGNOSTICS && m_fileFlags & LF_DUMP_SKIP)
                            ftLogger::logSkipChunk(bin->chunk, fileStruct, bin->fileBlock, bin->chunk.length);
                        freeChunk(bin);
                    }
                }
                else
                {
                    if (m_fileFlags & LF_DIAGNOSTICS && m_fileFlags & LF_UNRESOLVED)
                        ftLogger::logUnresolvedStructure(bin, fileStruct, memoryStruct);
                    freeChunk(bin);
                }
            }
        }
    }
    else
        status = FS_BAD_ALLOC;
}

int ftFile::allocateMBlock(const ftPointerHashKey& phk, ftMemoryChunk* bin, const SKsize& len, bool zero)
{
    int status = FS_OK;

    // Change the length of the file structure's memory
    // to account for the memory structures size.
    const SKuint32 totSize = (SKuint32)len;
    if (totSize > 0 && totSize < m_fileSizeInBytes)
    {
        bin->memoryBlockLen = totSize;
        bin->memoryBlock    = malloc(totSize);
        if (!bin->memoryBlock)
            status = FS_BAD_ALLOC;
        else
        {
            if (zero)
                memset(bin->memoryBlock, 0, totSize);
            else
            {
                // This is for the case when the chunk.code is saved
                // as DATA, and the structure ID is less than the first
                // user-defined type. I.E. it's an atomic pointer type so it's
                // safe to just copy this block

                SKsize cpSize = bin->memoryBlockLen;
                if (cpSize > bin->fileBlockLen)
                    cpSize = bin->fileBlockLen;

                memcpy(bin->memoryBlock, bin->fileBlock, cpSize);
            }
        }

        if (status == FS_OK)
        {
            insertChunk(phk, bin, zero, status);

            if (m_fileFlags & LF_READ_CHUNKS)
                ftLogger::logReadChunk(bin->chunk, bin->fileBlock, bin->chunk.length);
        }
    }
    else
        status = FS_BAD_ALLOC;
    return status;
}

void ftFile::insertChunk(const ftPointerHashKey& phk, ftMemoryChunk*& chunk, bool addToRebuildList, int& status)
{
    if (!m_map.insert(phk, chunk))
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logInvalidInsert(chunk);
        freeChunk(chunk);
        status = FS_INV_INSERT;
    }
    else if (addToRebuildList)
    {
        m_chunks.pushBack(chunk);
    }
}

void ftFile::freeChunk(ftMemoryChunk*& chunk)
{
    if (chunk)
    {
        if (chunk->pointerBlock)
            free(chunk->pointerBlock);
        if (chunk->fileBlock)
            free(chunk->fileBlock);
        if (chunk->memoryBlock)
            free(chunk->memoryBlock);
        free(chunk);
        chunk = nullptr;
    }
}

int ftFile::rebuildStructures()
{
    int status = FS_OK;

    SKsize* dstPtr;

    const bool diagnosticFlag = (m_fileFlags & LF_DIAGNOSTICS) != 0 && (m_fileFlags & LF_DUMP_CAST) != 0;
    bool       diagnostics    = false;

    for (ftMemoryChunk* node = (ftMemoryChunk*)m_chunks.first; node && status == FS_OK; node = node->next)
    {
        const ftChunk& chunk        = node->chunk;
        ftStruct*      fileStruct   = node->fileStruct;
        ftStruct*      memoryStruct = node->memoryStruct;

        if (diagnosticFlag && fileStruct && memoryStruct)
        {
            diagnostics = m_castFilter ? searchFilter(m_castFilter, fileStruct->getHashedType(), m_castFilterLen) : true;
            if (diagnostics)
                ftLogger::logDiagnosticsCastHeader(chunk, fileStruct, memoryStruct);
        }

        for (SKuint32 n = 0;
             n < chunk.count && status == FS_OK &&
             fileStruct && memoryStruct &&
             (node->flag & ftMemoryChunk::BLK_LINKED) == 0;
             ++n)
        {
            SKbyte* dst = memoryStruct->getChunk(node->memoryBlock, n, node->memoryBlockLen);
            SKbyte* src = fileStruct->getChunk(node->fileBlock, n, node->fileBlockLen);

            ftStruct::Members::Iterator it = memoryStruct->getMemberIterator();
            while (it.hasMoreElements())
            {
                ftMember* dstMember = it.getNext();
                ftMember* srcMember = fileStruct->find(dstMember);

                if (srcMember)
                {
                    dstPtr = dstMember->jumpToOffset(dst, node->memoryBlockLen);

                    SKsize* srcPtr = srcMember->jumpToOffset(src, node->fileBlockLen);
                    if (dstPtr && srcPtr)
                    {
                        if (diagnostics)
                        {
                            ftLogger::newline();
                            ftLogger::color(CS_DARKYELLOW);
                            ftLogger::logF("%s %s (%d) ==> %s %s (%d)",
                                           srcMember->getType(),
                                           srcMember->getName(),
                                           srcMember->getOffset(),
                                           dstMember->getType(),
                                           dstMember->getName(),
                                           srcMember->getOffset());
                        }

                        castMember(dstMember,
                                   dstPtr,
                                   srcMember,
                                   srcPtr,
                                   status);

                        if (diagnostics)
                        {
                            ftLogger::separator();
                            ftLogger::log(srcPtr, srcMember->getSizeInBytes());
                            ftLogger::newline();
                            ftLogger::log(dstPtr, dstMember->getSizeInBytes());
                            ftLogger::separator();
                            ftLogger::newline();
                        }

                        if (status != FS_OK)
                        {
                            if (m_fileFlags != LF_NONE)
                                ftLogger::logF("Cast member failed.");
                        }
                    }
                    else if (m_fileFlags != LF_NONE)
                    {
                        ftLogger::separator();
                        ftLogger::logF("Failed to offset to the member location:");
                        if (!dstPtr)
                        {
                            ftLogger::logF("Destination : %s offset (%d).",
                                           dstMember->getName(),
                                           dstMember->getOffset());
                        }
                        if (!srcPtr)
                        {
                            ftLogger::logF("Source      : %s offset (%d).",
                                           srcMember->getName(),
                                           srcMember->getOffset());
                        }
                        ftLogger::newline();
                    }
                }
                else
                {
                    // The memory in destination pointer is already zeroed, and
                    // the data at that offset should already be initialized.
                    // But if it's not it should be zeroed, and its probably
                    // a bug somewhere because something overflowed into it.
                    dstPtr = dstMember->jumpToOffset(dst, node->memoryBlockLen);

                    if (m_fileFlags & LF_DO_CHECKS)
                    {
                        void* zeroedMemoryCmp = malloc(dstMember->getSizeInBytes());
                        if (!zeroedMemoryCmp)
                            status = FS_BAD_ALLOC;
                        else
                        {
                            memset(zeroedMemoryCmp, 0, dstMember->getSizeInBytes());

                            if (memcmp(dstPtr, zeroedMemoryCmp, dstMember->getSizeInBytes()) != 0)
                                status = FS_OVERFLOW;

                            free(zeroedMemoryCmp);
                        }
                    }

                    if (status == FS_OK)
                    {
                        if (diagnosticFlag)
                        {
                            ftLogger::newline();
                            ftLogger::logF("MISSING %s %s",
                                           dstMember->getType(),
                                           dstMember->getName());
                            ftLogger::separator();
                            ftLogger::log(dstPtr, dstMember->getSizeInBytes());
                            ftLogger::newline();
                        }
                    }
                }
            }
        }

        if (node->memoryBlock && status == FS_OK)
        {
            node->flag |= ftMemoryChunk::BLK_LINKED;
            status = notifyDataRead(node->memoryBlock, node->memoryBlockLen, node->chunk);
        }
    }

    return status;
}

void ftFile::castMember(ftMember* dst,
                        SKsize*&  dstPtr,
                        ftMember* src,
                        SKsize*&  srcPtr,
                        int&      status)
{
    if (dst->isPointer())
        castMemberPointer(dst, dstPtr, src, srcPtr, status);
    else
        castMemberVariable(dst, dstPtr, src, srcPtr, status);
}

void ftFile::castMemberPointer(ftMember* dst,
                               SKsize*&  dstPtr,
                               ftMember* src,
                               SKsize*&  srcPtr,
                               int&      status)
{
    if (dst->getPointerCount() > 1)
        castPointerToPointer(dst, dstPtr, src, srcPtr, status);
    else
        castPointer(dst, dstPtr, src, srcPtr, status);
}

template <typename BaseType>
void ftFile::castPointer(SKsize*& dstPtr, SKsize* srcPtr, SKsize arrayLen)
{
    BaseType* ptr = (BaseType*)srcPtr;
    for (SKsize i = 0; i < arrayLen; ++i)
    {
        void* vp  = (void*)(SKsize)*ptr++;
        dstPtr[i] = (SKsize)findPointer(ftPointerHashKey(vp));
    }
}

void ftFile::castPointerToPointer(ftMember* dst,
                                  SKsize*&  dstPtr,
                                  ftMember* src,
                                  SKsize*&  srcPtr,
                                  int&      status)
{
    ftMemoryChunk* bin = findBlock((SKsize)*srcPtr);
    if (bin)
    {
        if (bin->flag & ftMemoryChunk::BLK_MODIFIED && bin->pointerBlock)
        {
            *dstPtr = (SKsize)bin->pointerBlock;

            if (m_fileFlags != LF_NONE)
                ftLogger::logF("Reusing block %p", bin->pointerBlock);
        }
        else
        {
            const SKsize fps = m_file->getSizeofPointer();
            if (fps == 4 || fps == 8)
            {
                const SKsize total = bin->pointerBlockLen / fps;

                SKsize* newBlock = (SKsize*)calloc(total, sizeof(SKsize));
                if (newBlock != nullptr)
                {
                    SKsize* oldPtr = (SKsize*)bin->fileBlock;
                    if (fps == 4)
                        castPointer<SKuint32>(newBlock, oldPtr, total);
                    else
                        castPointer<SKuint64>(newBlock, oldPtr, total);

                    free(bin->pointerBlock);
                    bin->pointerBlock = newBlock;
                    bin->flag |= ftMemoryChunk::BLK_MODIFIED;

                    *dstPtr = (SKsize)newBlock;
                }
                else
                    status = FS_BAD_ALLOC;
            }
            else
            {
                if (m_fileFlags != LF_NONE)
                    ftLogger::logF("Unknown pointer length(%d). Pointers should be either 4 or 8 bytes", fps);
                status = FS_INV_VALUE;
            }
        }
    }
    else if (m_fileFlags & LF_MISSING_PTR_PTR)
    {
        if ((SKsize)*srcPtr != 0)
        {
            ftLogger::logF("Failed to find corresponding chunk for address (0x%08X)",
                           (SKsize)*srcPtr);
            ftLogger::logF("Source");
            ftLogger::log(src);
            ftLogger::logF("Destination");
            ftLogger::log(dst);
            ftLogger::log(srcPtr, src->getSizeInBytes());
            ftLogger::newline();
            ftLogger::log(dstPtr, dst->getSizeInBytes());
        }
    }
}

void ftFile::castPointer(ftMember* dst,
                         SKsize*&  dstPtr,
                         ftMember* src,
                         SKsize*&  srcPtr,
                         int&      status)
{
    const SKsize arrayLen = skMin(dst->getArraySize(), src->getArraySize());

    const SKsize filePointerSize = m_file->getSizeofPointer();

    if (filePointerSize == 4 || filePointerSize == 8)
    {
        if (filePointerSize == 4)
            castPointer<SKuint32>(dstPtr, srcPtr, arrayLen);
        else
            castPointer<SKuint64>(dstPtr, srcPtr, arrayLen);
    }
    else
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Unknown pointer length(%d). Pointers should be either 4 or 8 bytes", filePointerSize);
        status = FS_INV_VALUE;
    }
}

void ftFile::castMemberVariable(ftMember* dst,
                                SKsize*&  dstPtr,
                                ftMember* src,
                                SKsize*&  srcPtr,
                                int&      status) const
{
    const SKsize dstElmSize   = dst->getSizeInBytes();
    const SKsize srcElmSize   = src->getSizeInBytes();
    const SKsize maxAvailable = skMin(srcElmSize, dstElmSize);
    const SKsize arrayLen     = skMax(skMin(dst->getArraySize(), src->getArraySize()), 1);

    // FT_MAX_MBR_RANGE
    // Provides an upper boundary for the number of array
    // elements that can be used
    // for instance: int member_variable[FT_MAX_MBR_RANGE];

    if (maxAvailable <= 0 || arrayLen > FileTools_MaxStructMember)
    {
        // re examine this, it should already be ruled out
        // when the table is compiled

        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Element size is out of range src(%d), dst(%d), max(%d)",
                           srcElmSize,
                           dstElmSize,
                           FileTools_MaxStructMember);
        status = FS_INV_LENGTH;
    }
    else if (!src->isBuiltinType() || !dst->isBuiltinType())
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Invalid atomic type src(%d), dst(%d)",
                           src->getAtomicType(),
                           dst->getAtomicType());
        status = FS_INV_LENGTH;
    }
    else
    {
        bool needsSwapped = (m_headerFlags & FH_ENDIAN_SWAP) != 0 && srcElmSize > 1;
        if (needsSwapped)
            needsSwapped = !src->isCharacterArray();

        const bool needsCast = src->getHashedType() != dst->getHashedType();

        const bool canCopy = !needsCast && !needsSwapped;
        if (canCopy)
        {
            if (m_fileFlags & LF_DIAGNOSTICS)
            {
                if (srcElmSize > dstElmSize)
                {
                    ftLogger::logF("The source member is larger than the destination member.");
                    ftLogger::logF("    %d bytes of data will be truncated.", srcElmSize - dstElmSize);
                }
            }

            if (dst->isCharacter())
            {
                // Allow for null terminated strings.
                // This will handle the case when there is extra information in the
                // source buffer. strncpy will copy up to the first null terminator
                // and the extra info in the buffer will be left untouched.
                strncpy((SKbyte*)dstPtr, (SKbyte*)srcPtr, maxAvailable);
            }
            else
                memcpy(dstPtr, srcPtr, maxAvailable);
        }
        else
        {
            SKbyte* dstBPtr = reinterpret_cast<SKbyte*>(dstPtr);
            SKbyte* srcBPtr = reinterpret_cast<SKbyte*>(srcPtr);

            const ftAtomic stp = src->getAtomicType();
            const ftAtomic dtp = dst->getAtomicType();

            SKbyte tmpBuf[MaxSwapSpace + 1] = {};

            const SKsize srcLen = skMin(MaxSwapSpace, srcElmSize / arrayLen);
            const SKsize dstLen = skMin(MaxSwapSpace, maxAvailable / arrayLen);

            int sc;
            if (src->isInteger16())
                sc = 1;
            else if (src->isInteger32())
                sc = 2;
            else if (src->isInteger64())
                sc = 3;
            else
                sc = 0;

            for (SKsize i = 0; i < arrayLen; i++)
            {
                if (needsSwapped)
                {
                    memcpy(tmpBuf, srcBPtr, srcLen);

                    switch (sc)
                    {
                    case 1:
                        swap16((SKuint16*)tmpBuf, 1);
                        break;
                    case 2:
                        swap32((SKuint32*)tmpBuf, 1);
                        break;
                    case 3:
                        swap64((SKuint64*)tmpBuf, 1);
                        break;
                    default:
                        memset(tmpBuf, 0, dstLen);
                        break;
                    }
                }
                else
                {
                    ftAtomicUtils::cast((char*)srcBPtr, (char*)dstBPtr, stp, dtp, 1);
                }

                dstBPtr += dstElmSize;
                srcBPtr += srcElmSize;
            }
        }
    }
}

void ftFile::castAtomicMemberArray(ftMember* dst,
                                   SKbyte*&  dstPtr,
                                   ftMember* src,
                                   SKbyte*&  srcPtr,
                                   int&      status)
{
    // TODO
}

void ftFile::castAtomicMember(ftMember* dst,
                              SKbyte*&  dstPtr,
                              ftMember* src,
                              SKbyte*&  srcPtr,
                              int&      status)
{
    // TODO
}

void ftFile::setFilter(SKhash*& dest, SKint32& destLen, SKhash* filter, const SKint32 length)
{
    if (!filter)
        return;

    dest  = filter;
    int i = 0;
    while (i < length && dest[i] != 0)
        i++;

    destLen = i;
    for (i = 0; i < destLen - 2; i++)
    {
        int k = i;
        for (int j = i + 1; j < destLen - 1; ++j)
            if (dest[j] < dest[k])
                k = j;
        if (k != i)
            skSwap(dest[i], dest[k]);
    }
}

bool ftFile::searchFilter(const SKhash* searchIn, const SKhash& searchFor, const SKint32& len)
{
    if (!searchIn)
        return false;

    int f = 0, l = len - 1;
    while (f <= l)
    {
        const int m = (f + l) >> 1;
        if (searchIn[m] == searchFor)
            return true;
        if (searchIn[m] > searchFor)
            l = m - 1;
        else
            f = m + 1;
    }
    return false;
}

bool ftFile::skip(const SKhash& id) const
{
    if (!m_filterList)
        return false;

    const bool res = searchFilter(m_filterList, id, m_filterListLen);
    return m_inclusive ? !res : res;
}

void ftFile::setFilterList(SKhash* filter, const SKuint32 length, const bool inclusive)
{
    m_inclusive = inclusive;
    setFilter(m_filterList, m_filterListLen, filter, length);
}

void ftFile::setCastFilter(SKhash* filter, const SKuint32 length)
{
    setFilter(m_castFilter, m_castFilterLen, filter, length);
}

ftStruct* ftFile::findInTable(ftStruct* findStruct, ftTable* sourceTable, ftTable* findInTable)
{
    ftStruct* found = nullptr;

    if (findStruct != nullptr)
    {
        const SKuint16 structType = findStruct->getTypeIndex();
        SKbyte*        searchKey  = sourceTable->getTypeNameAt(structType);

        if (searchKey != nullptr)
            found = findInTable->findStructByName(ftCharHashKey(searchKey));
    }
    return found;
}

ftStruct* ftFile::findInMemoryTable(ftStruct* fileStruct) const
{
    return findInTable(fileStruct, m_file, m_memory);
}

ftStruct* ftFile::findInFileTable(ftStruct* memoryStruct) const
{
    return findInTable(memoryStruct, m_memory, m_file);
}

void* ftFile::findPointer(const SKsize& iPtr)
{
    SKsize i;
    if ((i = m_map.find(ftPointerHashKey(iPtr))) != m_map.npos)
        return m_map.at(i)->memoryBlock;
    return nullptr;
}

void* ftFile::findPointer(const ftPointerHashKey& iPtr)
{
    SKsize i;
    if ((i = m_map.find(iPtr)) != m_map.npos)
        return m_map.at(i)->memoryBlock;
    return nullptr;
}

ftMemoryChunk* ftFile::findBlock(const SKsize& iPtr)
{
    SKsize i;
    if ((i = m_map.find(ftPointerHashKey(iPtr))) != m_map.npos)
        return m_map.at(i);
    return nullptr;
}

ftTable* ftFile::getMemoryTable(void)
{
    if (!m_memory)
        initializeMemory();
    return m_memory;
}

int ftFile::initializeMemory(void)
{
    int status = m_memory == nullptr ? (int)FS_FAILED : (int)FS_OK;
    if (!m_memory)
    {
        m_memory = new ftTable(sizeof(void*));

        status = initializeTables(m_memory);
        if (status == FS_OK)
        {
            if (m_fileFlags & LF_DO_CHECKS)
                status = runTableChecks(m_memory);
        }
    }
    return status;
}

int ftFile::initializeTables(ftTable* tables)
{
    // This calls down into derived classes
    // to gain access to the memory table.
    void*        tableData = getTables();
    const SKsize tableSize = getTableSize();

    if (tableData != nullptr && tableSize > 0 && tableSize != SK_NPOS)
        return tables->read(tableData, tableSize, 0, m_fileFlags);
    return FS_FAILED;
}

void ftFile::clearStorage(void)
{
    if (!m_map.empty())
    {
        ChunkMap::Iterator it = m_map.iterator();
        while (it.hasMoreElements())
        {
            ftMemoryChunk* mc = it.getNext().second;
            freeChunk(mc);
        }
    }
    m_map.clear();
    m_chunks.clear();

    if (m_fileTableData)
    {
        free(m_fileTableData);
        m_fileTableData = nullptr;
    }

    if (m_file)
    {
        delete m_file;
        m_file = nullptr;
    }

    if (m_memory)
    {
        delete m_memory;
        m_memory = nullptr;
    }
}

int ftFile::save(const char* path, const int mode)
{
    skStream* fs;

    if (mode == RM_COMPRESSED)
        fs = new ftGzStream();
    else
        fs = new skFileStream();

    fs->open(path, skStream::WRITE);

    if (!fs->isOpen())
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Failed to open output file for saving.");
        return FS_FAILED;
    }

    const SKuint8 cp = FT_VOID_8 ? FM_64_BIT : FM_32_BIT;
    SKuint8       ce = getEndian();
    if (ce == FT_ENDIAN_IS_BIG)
        ce = FM_BIG_ENDIAN;
    else
        ce = FM_LITTLE_ENDIAN;

    ftHeader header;
    char     version[4];
    sprintf(version, "%i", m_memoryVersion);
    header.resize(12);

    strncpy(header.ptr(), m_headerId, 7);  // The first 7 bytes of the header
    header[7] = cp;                        // The 8th byte is the pointer size
    header[8] = ce;                        // The 9th byte is the endian
    strncpy(&header[9], version, 3);       // The last 3 bytes are the version string.
    fs->write(header.ptr(), ftHeader::capacity());

    // the main bulk of saving chunks
    // happens in the  derived classes.
    serializeData(fs);

    ftChunk ch = {
        ftIdNames::DNA1,
        (SKuint32)getTableSize(),
        0,
        0,
        1,
    };
    fs->write(&ch, ftChunkUtils::BlockSize);
    fs->write(getTables(), ch.length);

    ch.code     = ftIdNames::ENDB;
    ch.length   = 0;
    ch.count    = 0;
    ch.address  = 0;
    ch.structId = 0;
    fs->write(&ch, ftChunkUtils::BlockSize);
    delete fs;
    return FS_OK;
}

void ftFile::serialize(skStream*      stream,
                       const char*    id,
                       const SKuint32 code,
                       const SKsize   len,
                       void*          writeData)
{
    if (m_memory == nullptr)
        getMemoryTable();

    const SKuint32 ft = m_memory->findTypeId(ftCharHashKey(id));

    if (ft == SK_NPOS32)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("writeStruct: %s - not found", id);
        return;
    }

    serialize(stream, ft, code, len, writeData);
}

void ftFile::serialize(skStream*      stream,
                       const char*    id,
                       const SKuint32 code,
                       const SKsize   len,
                       void*          writeData,
                       const int      nr)
{
    if (m_memory == nullptr)
        getMemoryTable();

    const SKuint32 ft = m_memory->findTypeId(ftCharHashKey(id));
    if (ft == SK_NPOS32)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("writeStruct: %s - not found", id);
        return;
    }

    serializeChunk(stream, code, nr, ft, len, writeData);
}

bool ftFile::isValidWriteData(void* writeData, const SKsize& len) const
{
    if (writeData == nullptr || len == SK_NPOS || len == 0)
    {
        if (m_fileFlags != LF_NONE)
            ftLogger::logF("Invalid write data\n");
        return false;
    }
    return true;
}

void ftFile::serializeChunk(skStream*      stream,
                            const SKuint32 code,
                            const SKuint32 nr,
                            const SKuint32 typeIndex,
                            const SKsize   len,
                            void*          writeData) const
{
    if (isValidWriteData(writeData, len))
    {
        ftChunk ch{
            code,
            (SKuint32)len,
            (SKsize)writeData,
            typeIndex,
            nr,
        };
        ftChunkUtils::write(&ch, stream);

        if (m_fileFlags & LF_WRITE_CHUNKS)
        {
            ftLogger::newline();
            ftLogger::log(ch);
            ftLogger::separator();
            ftLogger::log(writeData, len);
            ftLogger::separator();
        }
    }
}

void ftFile::serialize(skStream*      stream,
                       const FTtype   index,
                       const SKuint32 code,
                       const SKsize   len,
                       void*          writeData) const
{
    serializeChunk(stream, code, 1, index, len, writeData);
}

void ftFile::serialize(skStream* stream, const SKsize len, void* writeData) const
{
    serializeChunk(stream, ftIdNames::DATA, 1, 0, len, writeData);
}
