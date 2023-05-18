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
#ifndef _ftLogger_h_
#define _ftLogger_h_

#include "Utils/skDebugger.h"
#include "ftFile.h"
#include "ftTable.h"

namespace ftLogger
{
    /// <summary>
    /// Logs the enumerated name a predetermined status return code.
    /// </summary>
    /// <param name="status">The ftFlags::FileStatus code</param>
    extern void log(int status);

    /// <summary>
    /// Logs the supplied status code along with a printf style message.
    /// </summary>
    /// <param name="status">The ftFlags::FileStatus code</param>
    /// <param name="format">A message format string.</param>
    /// <param name="...">Variable arguments.</param>
    extern void log(int status, const char* format, ...);

    /// <summary>
    /// General printf style message.
    /// </summary>
    /// <param name="format">A message format string.</param>
    /// <param name="...">Variable arguments.</param>
    extern void logF(const char* format, ...);

    /// <summary>
    /// Logs a line to the console.
    /// </summary>
    extern void separator();

    /// <summary>
    /// Logs a smaller line to the console.
    /// </summary>
    extern void divider();

    /// <summary>
    /// Adds n number of line breaks to the console.
    /// </summary>
    extern void newline(int n = 1);

    /// <summary>
    /// Sets the current text foreground color.
    /// </summary>
    /// <param name="cs"></param>
    extern void color(skConsoleColorSpace cs);

    /// <summary>
    /// Logs the chunk's members to the console.
    /// </summary>
    /// <param name="chunk"></param>
    extern void log(const ftChunk& chunk);

    /// <summary>
    /// Logs a hex dump of the supplied memory to the console.
    /// </summary>
    /// <param name="ptr">The memory to output.</param>
    /// <param name="len">The length of the supplied memory.</param>
    extern void log(const void* ptr, const SKsize& len);

    /// <summary>
    /// Logs basic information about a structure.
    /// </summary>
    /// <example>
    /// <code>
    /// Struct : Object
    /// Type          : 125
    /// Hash          : 12108760437263257774
    /// Size In Bytes : 1432
    /// Aligned 4     : 1
    /// </code>
    /// </example>
    extern void log(ftStruct* structure);

    /// <summary>
    /// Logs basic information about a specific structure member.
    /// </summary>
    /// <example>
    /// <code>
    /// Struct        : Object
    /// Type          : char
    /// Name          : name[66]
    /// Pointer Count : 0
    /// Array Size    : 66
    /// </code>
    /// </example>
    extern void log(ftMember* member);

    /// <summary>
    /// Logs the file memory structures side by side.
    /// </summary>
    extern void log(ftStruct* fileStruct, ftStruct* memoryStruct);

    /// <summary>
    /// Logs the ftName structure.
    /// </summary>
    extern void log(const ftName& name);

    /// <summary>
    /// Logs the ftType structure.
    /// </summary>
    extern void log(const ftType& type);

    /// <summary>
    /// Logs the ftType structure.
    /// </summary>
    extern void log(const ftType& type, FTtype spacing);

    /// <summary>
    /// Logs the chunk as well as a side-by-side log of the file and memory structures.
    /// </summary>
    extern void logDiagnosticsCastHeader(const ftChunk& chunk,
                                         ftStruct*      fileStruct,
                                         ftStruct*      memoryStruct);

    /// <summary>
    /// Logs side by side information about the supplied members.
    /// </summary>
    extern void logDiagnosticsCastMemberHeader(ftMember* destMember,
                                               ftMember* sourceMember);

    /// <summary>
    /// Logs the chunk header as well as a hex dump of the chunk's data.
    /// </summary>
    extern void logReadChunk(const ftChunk& chunk,
                             const void*    block,
                             const SKsize&  len);

    /// <summary>
    /// Prints a message stating that the chunk has been skipped.
    /// Then logs the chunk header as well as a hex dump of the chunk's data.
    /// </summary>
    extern void logSkipChunk(const ftChunk& chunk,
                             ftStruct*      fileStruct,
                             const void*    block,
                             const SKsize&  len);

    extern void logMisCalculatedChunk(const ftChunk& chunk,
                                 SKsize         expected,
                                 SKsize         reported);

    extern void logUnresolvedStructure(ftMemoryChunk* bin, ftStruct* fileStruct, ftStruct* memoryStruct);

    /// <summary>
    /// Logs detailed information on a chunk that was not inserted into the hash table.
    /// </summary>
    extern void logInvalidInsert(ftMemoryChunk* bin);

    void logAlignment(const char* file,
                      SKsize      line,
                      SKsize      alignment,
                      const char* type,
                      const char* name,
                      SKsize      needed);

}  // namespace ftLogger

#endif  //_ftLogger_h_
