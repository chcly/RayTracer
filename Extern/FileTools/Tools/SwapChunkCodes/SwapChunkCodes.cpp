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
#include <cstdio>
#include <cstdlib>
#include "Utils/CommandLine/skCommandLineParser.h"
#include "Utils/skLogger.h"
#include "Utils/skPlatformHeaders.h"
#include "Utils/skRandom.h"
#include "ftEndianUtils.h"
#include "ftLogger.h"
#include "ftTypes.h"

using namespace ftFlags;
using skStringArray = skCommandLine::Parser::StringArray;

enum SwapCodesIds
{
    SWP_SWAP,
    SWP_OUTPUT,
    SWP_MAX,
};

const skCommandLine::Switch Switches[SWP_MAX] = {
    {
        SWP_SWAP,
        's',
        "swap",
        "Specify a swap type .\n"
        "  - Arguments: [0, 8]\n"
        "    - 0: Swaps only the chunk's code leaving the data block the same.\n"
        "    - 1: Swaps the chunks data block leaving the length untouched so that data is corrupted.\n"
        "    - 2: Randomly fills the chunks data block with junk.\n"
        "    - 3: Leaves the data untouched, and reflects what is read to the output file.\n"
        "    - 4: Randomizes both chunk and chunk data .\n"
        "    - 5: Zeros all chunk data .\n"
        "    - 6: Zeros chunks and data .\n"
        "    - 7: Maxes out all bytes in the chunk headers.\n"
        "    - 8: Maxes out all bytes in the chunk headers and data blocks.\n",
        false,
        1,
    },
    {
        SWP_OUTPUT,
        'o',
        "output",
        "Specify the output file.",
        false,
        1,
    },
};

class ftSwapChunks
{
private:
    skArray<ftMemoryChunk> m_readChunks;
    skArray<SKuint32>      m_readChunkCodes;
    int                    m_headerFlags;
    SKsize                 m_totalSize;

public:
    ftSwapChunks() :
        m_headerFlags(0),
        m_totalSize(0)
    {
        skRandInit();
    }

    ~ftSwapChunks()
    {
        for (ftMemoryChunk& element : m_readChunks)
        {
            if (element.fileBlock)
                free(element.fileBlock);
        }
    }

    int findHeaderFlags(skStream* stream)
    {
        int status = FS_OK;

        char buf[13] = {};

        m_totalSize = stream->size();

        const SKsize read = stream->read(buf, 12);
        if (read != SK_NPOS && read > 0)
        {
            char* magic = &buf[7];

            if (*magic == FM_64_BIT || *magic == FM_32_BIT)
            {
                if (*magic == FM_64_BIT)
                {
                    m_headerFlags |= FH_CHUNK_64;
                    if (FT_VOID_4)
                        m_headerFlags |= FH_VAR_BITS;
                }
                else if (FT_VOID_8)
                    m_headerFlags |= FH_VAR_BITS;

                const char endian = *++magic;
                if (endian == FM_BIG_ENDIAN || endian == FM_LITTLE_ENDIAN)
                {
                    const int current = (int)ftEndianUtils::getEndian();

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
                    else
                    {
                        ftLogger::logF("The file header is missing the endian magic.");
                        status = FS_INV_HEADER_STR;
                    }
                }
            }
            else
            {
                ftLogger::logF("The file header is missing the chunk size magic.");
                status = FS_INV_HEADER_STR;
            }
        }
        else
            status = FS_INV_READ;
        return status;
    }

    int readAllChunks(skStream* stream)
    {
        int     status = FS_OK;
        ftChunk chunk  = {};

        while (chunk.code != ftIdNames::ENDB &&
               chunk.code != ftIdNames::DNA1 &&
               status == FS_OK && !stream->eof())
        {
            const SKsize bytesRead = ftChunkUtils::read(&chunk, stream, m_headerFlags, m_totalSize);
            if ((int)bytesRead <= 0 || bytesRead == SK_NPOS)
                status = FS_INV_READ;
            else if (chunk.code != ftIdNames::ENDB)
            {
                if (chunk.length > 0 && chunk.length != SK_NPOS32)
                {
                    ftMemoryChunk mc;
                    mc.chunk     = chunk;
                    mc.fileBlock = malloc(chunk.length);
                    if (!mc.fileBlock)
                        status = FS_BAD_ALLOC;
                    else
                    {
                        if (!stream->read(mc.fileBlock, chunk.length))
                        {
                            status = FS_INV_READ;
                            free(mc.fileBlock);
                        }
                        else
                        {
                            m_readChunks.push_back(mc);
                            if (chunk.code != ftIdNames::DNA1)
                            {
                                // linear
                                if (m_readChunkCodes.find(chunk.code) == m_readChunkCodes.npos)
                                    m_readChunkCodes.push_back(chunk.code);
                            }
                        }
                    }
                }
                else
                    status = FS_INV_LENGTH;
            }
        }
        return status;
    }

    int getRandomCode() const
    {
        return (int)skRandomUnsignedInt(m_readChunkCodes.size());
    }

    int getRandomChunkIndex() const
    {
        return (int)skRandomUnsignedInt(m_readChunks.size());
    }

    void swapCode(skFileStream& of, ftMemoryChunk& element)
    {
        if (element.chunk.code != ftIdNames::DNA1)
        {
            const int code = getRandomCode();
            ftChunk   nc   = element.chunk;
            nc.code        = m_readChunkCodes.at(code);
            of.write(&nc, sizeof(ftChunk));
        }
        else
            of.write(&element.chunk, sizeof(ftChunk));

        if (element.fileBlock)
            of.write(element.fileBlock, element.chunk.length);
    }

    void swapData(skFileStream& of, ftMemoryChunk& element) const
    {
        of.write(&element.chunk, sizeof(ftChunk));
        if (element.fileBlock)
        {
            const SKint32        idx = getRandomChunkIndex();
            const ftMemoryChunk& mc  = m_readChunks[idx];

            if (mc.chunk.code != ftIdNames::DNA1)
                of.write(mc.fileBlock, mc.chunk.length);
            else
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    static void randDataBlock(skFileStream& of, ftMemoryChunk& element)
    {
        of.write(&element.chunk, sizeof(ftChunk));
        if (element.fileBlock)
        {
            if (element.chunk.code != ftIdNames::DNA1)
            {
                SKuint8* ptr = (SKuint8*)element.fileBlock;
                for (SKuint32 i = 0; i < element.chunk.length; ++i)
                    ptr[i] = (SKuint8)skRandomUnsignedInt(0xFF);
                of.write(ptr, element.chunk.length);
            }
            else
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    static void zeroData(skFileStream& of, ftMemoryChunk& element)
    {
        of.write(&element.chunk, sizeof(ftChunk));

        if (element.fileBlock)
        {
            if (element.chunk.code != ftIdNames::DNA1)
            {
                SKuint8* ptr = (SKuint8*)element.fileBlock;
                for (SKuint32 i = 0; i < element.chunk.length; ++i)
                    ptr[i] = 0x00;
                of.write(ptr, element.chunk.length);
            }
            else
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    static void zeroAllData(skFileStream& of, ftMemoryChunk& element)
    {
        if (element.chunk.code != ftIdNames::DNA1)
        {
            ftChunk nc  = element.chunk;
            nc.address  = 0;
            nc.count    = 0;
            nc.length   = 0;
            nc.structId = 0;
            of.write(&nc, sizeof(ftChunk));
        }
        else
        {
            of.write(&element.chunk, sizeof(ftChunk));
            if (element.fileBlock)
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    static void reflectData(skFileStream& of, ftMemoryChunk& element)
    {
        of.write(&element.chunk, sizeof(ftChunk));
        if (element.fileBlock)
            of.write(element.fileBlock, element.chunk.length);
    }

    static void randData(skFileStream& of, ftMemoryChunk& element)
    {
        ftChunk nc = element.chunk;

        nc.address  = (SKsize)(skUnitRandom() * (double)SK_NPOS);
        nc.structId = (SKuint32)(skUnitRandom() * (double)SK_NPOS32);
        nc.count    = (SKuint32)(skUnitRandom() * (double)SK_NPOS32);
        of.write(&nc, sizeof(ftChunk));

        if (element.fileBlock)
        {
            if (element.chunk.code != ftIdNames::DNA1)
            {
                SKuint8* ptr = (SKuint8*)element.fileBlock;
                for (SKuint32 i = 0; i < element.chunk.length; ++i)
                    ptr[i] = (SKuint8)skRandomUnsignedInt(0xFF);
                of.write(ptr, element.chunk.length);
            }
            else
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    static void maxChunkHeaders(skFileStream& of, ftMemoryChunk& element)
    {
        ftChunk nc  = element.chunk;
        nc.address  = SK_MAX;
        nc.structId = SK_MAX32;
        nc.count    = SK_MAX32;
        of.write(&nc, sizeof(ftChunk));

        if (element.fileBlock)
            of.write(element.fileBlock, element.chunk.length);
    }

    static void maxChunkHeadersAndData(skFileStream& of, ftMemoryChunk& element)
    {
        ftChunk nc = element.chunk;

        ftByteInteger bi{};
        bi.int64   = SK_MAX;
        nc.address = bi.int64;

        of.write(&nc, sizeof(ftChunk));

        if (element.fileBlock)
        {
            if (element.chunk.code != ftIdNames::DNA1)
            {
                SKuint8* ptr = (SKuint8*)element.fileBlock;
                for (SKuint32 i = 0; i < element.chunk.length; ++i)
                    ptr[i] = bi.int8[i % 8];
                of.write(ptr, element.chunk.length);
            }
            else
                of.write(element.fileBlock, element.chunk.length);
        }
    }

    int writeShuffledChunks(const skString& output, skStream* input, int shuffle)
    {
        skFileStream of(output.c_str(), skStream::WRITE);
        if (!of.isOpen())
        {
            printf("Failed to open the output file.\n");
            return 1;
        }

        char buffer[12]{};
        input->seek(0, SEEK_SET);
        input->read(buffer, 12);

        of.write(buffer, 12);

        for (ftMemoryChunk& element : m_readChunks)
        {
            switch (shuffle)
            {
            case 0:
                swapCode(of, element);
                break;
            case 1:
                swapData(of, element);
                break;
            case 2:
                randDataBlock(of, element);
                break;
            case 4:
                randData(of, element);
                break;
            case 5:
                zeroData(of, element);
                break;
            case 6:
                zeroAllData(of, element);
                break;
            case 7:
                maxChunkHeaders(of, element);
                break;
            case 8:
                maxChunkHeadersAndData(of, element);
                break;
            case 3:
            default:
                reflectData(of, element);
                break;
            }
        }

        ftChunk end = {ftIdNames::ENDB, 0, 0, 0, 0};
        of.write(&end, sizeof(ftChunk));
        return 0;
    }
};

class Application
{
private:
    skString m_input;
    skString m_output;
    int      m_swap;

public:
    Application() :
        m_swap(0)
    {
    }

    bool parse(int argc, char** argv)
    {
        skCommandLine::Parser psr;
        if (psr.parse(argc, argv, Switches, sizeof Switches / sizeof Switches[0]) < 0)
            return false;

        const skCommandLine::Parser::StringArray array = psr.getArgList();

        if (array.empty())
        {
            printf("No input files\n");
            return false;
        }

        m_input  = array[0];
        m_output = psr.getValueString(SWP_OUTPUT, 0);
        m_swap   = psr.getValueInt(SWP_SWAP, 0, 0);

        return true;
    }

    int run() const
    {
        skFileStream fs;
        fs.open(m_input.c_str(), skStream::READ);
        if (!fs.isOpen())
        {
            printf("Failed to open the input file.\n");
            return 1;
        }

        ftSwapChunks sc;
        if (sc.findHeaderFlags(&fs) != FS_OK)
        {
            printf("Failed to extract the header flags.\n");
            return 1;
        }

        if (sc.readAllChunks(&fs) != FS_OK)
        {
            printf("Failed to read chunks.\n");
            return 1;
        }

        return sc.writeShuffledChunks(m_output, &fs, m_swap);
    }
};

int main(int argc, char** argv)
{
    try
    {
        skLogger    log;
        Application app;
        if (!app.parse(argc, argv))
            return 1;

        return app.run();
    }
    catch (...)
    {
        printf("An exception occurred\n");
    }
    return 1;
}
