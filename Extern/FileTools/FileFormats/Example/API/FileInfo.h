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
#ifndef _FileInfo_h_
#define _FileInfo_h_

#define FileVersionMajor    1
#define FileVersionMinor    0
#define FileVersionBuild    0
#define FileVersionRevision 1


typedef struct FileInfo
{
    short major;
    short minor;
    short build;
    short revision;
    char versionString[32];
} FileInfo;


typedef struct Link
{
    void* data;
} Link;

#endif//_FileInfo_h_
