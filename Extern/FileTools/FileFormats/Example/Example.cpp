/*
-------------------------------------------------------------------------------
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
#include "Example.h"
#include "ftTables.h"
#include "ftStreams.h"

#define FT_IN_SOURCE_FILE
#include "ftPlatformHeaders.h"
#include "ExampleTables.inl"



namespace ExampleCodes
{
    const FBTuint32 INFO = FT_TYPEID('I', 'N', 'F', 'O');
    const FBTuint32 DAT1 = FT_TYPEID('D', 'A', 'T', '1');
    const FBTuint32 DAT2 = FT_TYPEID('D', 'A', 'T', '2');
}


Example::Example() :
    ftFile("Example")
{
    m_info.major    = FileVersionMajor;
    m_info.minor    = FileVersionMinor;
    m_info.build    = FileVersionBuild;

    m_info.revision = FileVersionRevision;

    ftp_sprintf(m_info.versionString, 32, "%i%i%i",
                m_info.major, m_info.minor, m_info.build);

    m_memoryVersion = atoi(m_info.versionString);
}

Example::~Example()
{
}

void* Example::getTables(void)
{
    return (void*)ExampleTablesTable;
}

FBTsize Example::getTableSize(void)
{
    return ExampleTablesLen;
}

int Example::notifyDataRead(void* p, const ftChunk& id)
{
    if (id.m_code == ExampleCodes::INFO)
        m_info = *((FileInfo*)p);
    else if (id.m_code == ExampleCodes::DAT1)
        m_data.push_back((Data1*)p);
    else if (id.m_code == ExampleCodes::DAT2)
        m_data2.push_back(*(Data2*)p);

    return ftFlags::FS_OK;
}

int Example::serializeData(skStream* stream)
{
    serialize(stream, "FileInfo", ExampleCodes::INFO, sizeof(FileInfo), &m_info);

    Data1Array::Iterator d1 = m_data.iterator();
    while (d1.hasMoreElements())
    {
        Data1* d1v = d1.getNext();
        serialize(stream, "Data1", ExampleCodes::DAT1, sizeof(Data1), &d1v);
        serialize(stream, sizeof(Data1), d1v);
    }

    Data2Array::Iterator d2 = m_data2.iterator();
    while (d2.hasMoreElements())
    {
        Data2& d2v = d2.getNext();
        serialize(stream, "Data2", ExampleCodes::DAT2, sizeof(Data2), &d2v);
    }

    return ftFlags::FS_OK;
}
