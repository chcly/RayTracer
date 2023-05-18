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
#include <cstring>

#include "ftCompiler.h"

void usage(const char* programName);
int  getBaseName(const char* input);

int main(int argc, char** argv)
{
    char* base = argv[0] + getBaseName(argv[0]);
    if (argc < 4)
    {
        usage(base);
        return 1;
    }

    printf("%s, Building table %s\n", base, argv[1]);
    ftCompiler tables;
    for (int i = 3; i < argc; ++i)
    {
        if (tables.parse(argv[i]) < 0)
        {
            printf("%s -> Parse Error: When compiling file %s\n", base, argv[i]);
            return 1;
        }
    }

    const int code = tables.compile();
    if (code != ftFlags::LNK_OK)
    {
        printf("%s -> Link Error(%08X): When compiling table %s\n", base, code, argv[1]);
        return code;
    }
    tables.writeFile(argv[1], argv[2]);
    return 0;
}

int getBaseName(const char* input)
{
    int offs = 0;
    if (input)
    {
        const int len = (int)strlen(input);
        for (int i = len - 1; i >= 0 && offs == 0; --i)
            if (input[i] == '/' || input[i] == '\\')
                offs = i + 1;
    }
    return offs;
}

void usage(const char* programName)
{
    if (programName)
    {
        printf("%s\n", programName);
        printf("       <tablename> <outfile> <infile>[0] ... <infile>[n]\n");
        printf("\n");
        printf("       tablename - A prefix on the generated table.  \n");
        printf("                   Eg: 'File' would be generated as:\n");
        printf("\n");
        printf("                      const unsigned char FileTable[]={...};\n");
        printf("                      const unsigned int  FileTableLen;\n");
        printf("\n");
        printf("       outfile   - The name of the output file that will be used to store the tables.\n");
        printf("       infile    - a white space separated list of file names to compile.\n");
    }
    else
        printf("Invalid program name supplied to usage.\n");
}
