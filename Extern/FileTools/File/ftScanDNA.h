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
#ifndef _ftScanDNA_h_
#define _ftScanDNA_h_

#include "ftTypes.h"

/// <summary>
/// ftScanDna is a utility class to read minimal information from the file by
/// skipping from chunk to chunk.
/// </summary>
/// <remarks>
/// The idea of the DNA scan is to jump to the
/// DNA1 block first, extract the table data,
/// then seek back to the file header offset
/// and read the chunks up to the DNA1 block.
/// Then, create associations with the structure
/// and member declarations along with every chunk.
/// </remarks>
class ftScanDna
{
private:
    void*  m_foundBlock;
    SKsize m_foundLen, m_totalLen;
    int    m_headerFlags;

public:
    ftScanDna();
    ~ftScanDna();

    /// <summary>
    /// Extracts the needed flags from the file header
    /// or an error status if the flags are not found.
    /// </summary>
    /// <param name="stream">The stream to use for reading.</param>
    /// <returns>ftFlags::FileHeader</returns>
    int findHeaderFlags(skStream* stream);

    /// <summary>
    /// This is here to provide an option to set the required flags if they have
    /// already been extracted from the file
    /// </summary>
    /// <param name="headerFlags">ftFlags::FileHeader</param>
    void setFlags(int headerFlags)
    {
        m_headerFlags = headerFlags;
    }

    /// <summary>
    /// Returns a copy of the internal ftFlags::FileHeader flags>
    /// </summary>
    int getFlags() const
    {
        return m_headerFlags;
    }

    /// <summary>
    /// Preforms the scan of the file. Execution stops on any error, or when the DNA1 chunk has been read.
    /// </summary>
    /// <param name="stream">The stream to use for reading.</param>
    /// <returns>ftFlags::FileStatus</returns>
    int scan(skStream* stream);


    /// <summary>
    /// If the DNA1 block is found, this will contain the memory that was extracted.
    /// </summary>
    /// <returns>Null if it was not found or not read yet.</returns>
    /// <remarks>
    /// `Note`: that this class does not manage the
    ///       memory allocated for the DNA1 block.
    ///       It was allocated with malloc and the memory should be released
    ///       with a call to free.
    /// </remarks>
    void* getDNA() const
    {
        return m_foundBlock;
    }

    /// <summary>
    /// Returns the length of the DNA1 block.
    /// </summary>
    SKsize getLength() const
    {
        return m_foundLen;
    }


    /// <summary>
    /// A test to see if the file is a 64-bit file. 
    /// </summary>
    bool is64Bit() const;
};

#endif  //_ftScanDNA_h_
