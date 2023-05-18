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
#ifndef _ftFile_h_
#define _ftFile_h_

#include "Utils/skMap.h"
#include "ftChunk.h"
#include "ftHashTypes.h"
#include "ftTypes.h"

/// <summary>
/// ftFile is the base class for a file in this system.
/// Derived classes should override the table methods and supply them to this class.
/// </summary>
class ftFile
{
public:
    typedef skHashTable<ftPointerHashKey, ftMemoryChunk*> ChunkMap;

    typedef ftList MemoryChunks;

private:
    int         m_headerFlags;
    int         m_fileFlags;
    const char* m_headerId;
    ftHeader    m_header;
    SKsize      m_fileSizeInBytes;
    skString    m_curFile;
    void*       m_fileTableData;
    SKhash*     m_filterList;
    SKint32     m_filterListLen;
    SKhash*     m_castFilter;
    SKint32     m_castFilterLen;
    bool        m_inclusive;

protected:
    int          m_memoryVersion;
    int          m_fileVersion;
    MemoryChunks m_chunks;
    ChunkMap     m_map;
    ftTable*     m_memory;
    ftTable*     m_file;

public:
    /// <summary>
    /// Default constructor
    /// </summary>
    /// <param name="header">
    /// This should be the first seven bytes of the file header.
    /// </param>
    explicit ftFile(const char* header);
    virtual ~ftFile();

    /// <summary>
    /// Attempts to load the file from the supplied file path.
    /// </summary>
    /// <param name="path">File system path to the file's location.</param>
    /// <param name="mode">
    /// Mode to determine how the file should be read.
    /// It should be one of the ftFlags::ReadMode values.
    /// </param>
    /// <returns>
    /// A status code indicating the result.
    /// It should be one of the ftFlags::FileStatus values.
    /// </returns>
    int load(const char* path, int mode = 0);

    /// <summary>
    /// Attempts to load the file from the supplied memory.
    /// </summary>
    /// <param name="memory">A pointer to a block of memory that holds the file.</param>
    /// <param name="sizeInBytes">The total size in bytes of the supplied block of memory.</param>
    /// <returns>
    /// A status code indicating the result.
    /// It should be one of the ftFlags::FileStatus values.
    /// </returns>
    int load(const void* memory, SKsize sizeInBytes);

    /// <summary>
    /// Attempts to save the file to the supplied path.
    /// </summary>
    /// <param name="path">The output file system path.</param>
    /// <param name="mode">Mode to determine how the file should be saved. It should be one of the ftFlags::ReadMode codes.</param>
    /// <returns>A status code indicating the result. It should be one of the ftFlags::FileStatus codes.</returns>
    virtual int save(const char* path, int mode = 0);

    /// <summary>
    /// Returns a pointer to the current memory tables.
    /// </summary>
    ftTable* getMemoryTable();

    /// <summary>Returns the file's complete header string</summary>
    const ftHeader& getHeader() const
    {
        return m_header;
    }

    /// <summary>
    /// Returns the file's integer version that
    /// was extracted from the header.
    /// </summary>
    const int& getVersion() const
    {
        return m_fileVersion;
    }

    /// <summary>
    /// Returns the file path that was supplied during the call to load.
    /// </summary>
    const skString& getPath() const
    {
        return m_curFile;
    }

    /// <summary>
    /// Returns the table that was saved in the file.
    /// </summary>
    ftTable* getFileTable() const
    {
        return m_file;
    }

    /// <summary>
    /// Returns a linked list of all chunks that
    /// have been extracted from the file.
    /// </summary>
    MemoryChunks& getChunks()
    {
        return m_chunks;
    }

    /// <summary>
    /// Gets the flags that are currently set for the file.
    /// </summary>
    /// <returns>
    /// An integer that contains one or more of the bit flags found in
    /// ftFlags::LogFlags.
    /// </returns>
    int getFileFlags() const
    {
        return m_fileFlags;
    }

    /// <summary>
    /// Sets the flags for the file.
    /// </summary>
    /// <param name="flags">The flags value should contain one or more
    /// of the bit flags found in ftFlags::LogFlags.</param>
    void setFileFlags(const int flags)
    {
        m_fileFlags = flags;
    }

    /// <summary>
    /// Attaches a flag to the current flag.
    /// </summary>
    /// <param name="flag">The flag value should contain one or more
    /// of the bit flags found in ftFlags::LogFlags</param>
    void addFileFlag(const int flag)
    {
        m_fileFlags |= flag;
    }

    /// <summary>
    /// This enables a filter for structures.
    /// </summary>
    /// <param name="filter">The input filter.</param>
    /// <param name="length">The number of structures in the filter</param>
    /// <param name="inclusive">Sets whether the filter should include or exclude the supplied filter.</param>
    /// <remarks>
    /// Internally struct types are stored by a hash of their unique type name.
    /// The filter list needs to be a null terminated SKhash [] array.
    ///
    /// The hash function resides in the ftCharHashKey class
    /// Usage:
    /// <code>
    /// FBThash filterList[] = {
    ///      ftCharHashKey("StructToFilter1").hash(),
    ///      ftCharHashKey("StructToFilter2").hash(),
    ///      nullptr
    /// };
    /// </code>
    /// </remarks>
    void setFilterList(SKhash* filter, SKuint32 length, bool inclusive = false);

    /// <summary>
    /// When the diagnostic flag is set, this sets an inclusive output filter for supplied structures.
    /// </summary>
    /// <param name="filter">The structures that should be logged.</param>
    /// <param name="length">The number of hash codes in the filter.</param>
    void setCastFilter(SKhash* filter, SKuint32 length);

    void serialize(skStream* stream, const char* id, SKuint32 code, SKsize len, void* writeData, int nr);

    void serialize(skStream* stream, const char* id, SKuint32 code, SKsize len, void* writeData);

    void serialize(skStream* stream, FTtype index, SKuint32 code, SKsize len, void* writeData) const;

    void serialize(skStream* stream, SKsize len, void* writeData) const;

protected:
    /// <summary>
    /// This is an abstract method that allows this class to gain access
    /// to the memory tables.
    /// </summary>
    /// <returns>
    /// The return value should be the character array that was generated
    /// by the TableCompiler.
    /// </returns>
    virtual void* getTables() = 0;

    /// <summary>
    /// Allows this class to access the size of the table output.
    /// </summary>
    /// <returns>
    /// The return value should be the total size in bytes of the character array that
    /// was generated by the TableCompiler.
    /// </returns>
    virtual SKsize getTableSize() = 0;

    /// <summary>
    /// This is a notification callback that allows derived classes
    /// a chance to handle a chunk further. This method is invoked after
    /// each successful chunk read.
    /// </summary>
    /// <param name="pointer">Contains the memory of the reconstructed data.</param>
    /// <param name="sizeInBytes">Holds the size in bytes of the memory</param>
    /// <param name="chunk">Is the chunk that was read from the file for the reconstructed memory.</param>
    /// <returns>
    /// This method should return one of the status codes found in ftFlags::FileStatus.
    /// A return code of FS_OK signals to this class that it should keep reading.
    /// Any other code will force an exit.
    /// </returns>
    virtual int notifyDataRead(void* pointer, SKsize sizeInBytes, const ftChunk& chunk) = 0;

    virtual int serializeData(skStream* stream) = 0;

    bool isValidWriteData(void* writeData, const SKsize& len) const;

    int initializeTables(ftTable* tables);

    int initializeMemory();

private:

    static bool searchFilter(const SKhash*  searchIn,
                             const SKhash&  searchFor,
                             const SKint32& len);

    static void setFilter(SKhash*& dest, SKint32& destLen, SKhash* filter, SKint32 length);

    template <typename BaseType>
    void castPointer(SKsize*& dstPtr, SKsize* srcPtr, SKsize arrayLen);

    void* findPointer(const ftPointerHashKey& iPtr);

    void* findPointer(const SKsize& iPtr);

    ftMemoryChunk* findBlock(const SKsize& iPtr);

    skStream* openStream(const char* path, int mode);

    bool skip(const SKhash& id) const;

    void serializeChunk(skStream* stream,
                        SKuint32  code,
                        SKuint32  nr,
                        SKuint32  typeIndex,
                        SKsize    len,
                        void*     writeData) const;

    void handleChunk(
        void*          block,
        SKsize         allocLen,
        const ftChunk& chunk,
        int&           status);

    void insertChunk(const ftPointerHashKey& phk,
                     ftMemoryChunk*&         chunk,
                     bool                    addToRebuildList,
                     int&                    status);

    static void freeChunk(ftMemoryChunk*& chunk);

    int runTableChecks(ftTable* check) const;

    void clearStorage();

    int parseHeader(skStream* stream);

    int parseStreamImpl(skStream* stream);

    int preScan(skStream* stream);

    int rebuildStructures();

    int allocateMBlock(const ftPointerHashKey& phk, 
        ftMemoryChunk* bin, 
        const SKsize& len, 
        bool zero);

    void castMember(
        ftMember* dst,
        SKsize*&  dstPtr,
        ftMember* src,
        SKsize*&  srcPtr,
        int&      status);

    void castMemberPointer(
        ftMember* dst,
        SKsize*&  dstPtr,
        ftMember* src,
        SKsize*&  srcPtr,
        int&      status);

    void castPointer(
        ftMember* dst,
        SKsize*&  dstPtr,
        ftMember* src,
        SKsize*&  srcPtr,
        int&      status);

    void castPointerToPointer(
        ftMember* dst,
        SKsize*&  dstPtr,
        ftMember* src,
        SKsize*&  srcPtr,
        int&      status);

    void castMemberVariable(
        ftMember* dst,
        SKsize*&  dstPtr,
        ftMember* src,
        SKsize*&  srcPtr,
        int&      status) const;

    void castAtomicMemberArray(
        ftMember* dst,
        SKbyte*&  dstPtr,
        ftMember* src,
        SKbyte*&  srcPtr,
        int&      status);

    void castAtomicMember(
        ftMember* dst,
        SKbyte*&  dstPtr,
        ftMember* src,
        SKbyte*&  srcPtr,
        int&      status);

    static ftStruct* findInTable(ftStruct* findStruct, ftTable* sourceTable, ftTable* findInTable);

    ftStruct* findInMemoryTable(ftStruct* fileStruct) const;

    ftStruct* findInFileTable(ftStruct* memoryStruct) const;
};

#endif  //_ftFile_h_
