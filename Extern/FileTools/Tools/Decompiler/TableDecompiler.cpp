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
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Utils/skList.h"
#include "Utils/skStack.h"
#include "Utils/skString.h"
#include "Utils/skTimer.h"
#include "ftMember.h"
#include "Utils/skPlatformHeaders.h"
#include "ftScanDNA.h"
#include "ftStruct.h"
#include "ftTable.h"

typedef skArray<ftStruct*> StructArray;
typedef skList<ftStruct*>  StructDeque;

using namespace std;

struct ProgramInfo
{
    const char* programName;
    skString    inFile;
    skString    outFile;
    skStream*   stream;
    void*       dna;
    ftTable*   tables;
    char        header[13];
    bool        useNamespace;
    string      nameSpaceName;
    bool        fixBlendNaming;
    bool        initTypes;
    bool        writeHashCode;
    string      outName;  // only valid in extract to file
};

void writeFileHeader(ProgramInfo& ctx, ostream& out)
{
    char           buf[32];
    const SKuint32 br = skGetTimeString(buf, 31, "%a %b %d %r");

    string       name = ctx.inFile.c_str();
    const size_t last = name.find_last_of('/') + 1;
    name              = name.substr(last, name.size());

    out << "/*" << endl
        << endl;
    out << "    This file was automatically generated." << endl;
    out << "    https://github.com/CharlesCarley/FileTools" << endl;
    out << endl;
    out << "    By    : " << ctx.programName << endl;
    out << "    From  : " << name << "(" << ctx.header << ")" << endl;
    if (br != SK_NPOS32)
        out << "    On    : " << buf << endl
            << endl;
    out << "*/" << endl;

    name = name.substr(0, name.find('.'));

    ctx.outName = name.c_str();
    out << "#ifndef _" << name << "_h_" << endl;
    out << "#define _" << name << "_h_" << endl;
    out << endl;

    if (ctx.initTypes)
    {
        out << "#include <cstdint>" << endl;
        out << endl;
    }

    if (ctx.fixBlendNaming)
    {
        out << "#ifdef near" << endl;
        out << "#undef near" << endl;
        out << "#endif//near" << endl;

        out << "#ifdef far" << endl;
        out << "#undef far" << endl;
        out << "#endif//far" << endl;
        out << endl;
    }
}

void writeFileFooter(ProgramInfo& ctx, ostream& out)
{
    out << "#endif//_" << ctx.outName << "_h_" << endl;
}

void writeIndent(ProgramInfo& ctx, ostream& out, int nr, int spacePerIndent = 4)
{
    if (!ctx.useNamespace)
        nr -= 1;

    spacePerIndent = skMax(spacePerIndent, 0);
    int i;
    for (i = 0; i < nr; ++i)
        out << setw(spacePerIndent) << ' ';
}

void writeForward(ProgramInfo& ctx, ostream& out, ftStruct* ftStrc)
{
    writeIndent(ctx, out, 1);
    out << "struct " << ftStrc->getName() << ';' << endl;
}

void writeStructure(ProgramInfo& ctx, ostream& out, ftStruct* structure)
{
    writeIndent(ctx, out, 1);
    out << "struct " << structure->getName() << endl;

    writeIndent(ctx, out, 1);
    out << "{" << endl;

    FTtype* cs = ctx.tables->getStructAt(structure->getStructIndex());

    int maxLeft = -1;
    if (cs)
    {
        const FTtype count = cs[1];
        cs += 2;
        for (FTtype i = 0; i < count; ++i, cs += 2)
        {
            const ftType& type = ctx.tables->getTypeAt(cs[0]);

            maxLeft = skMax(maxLeft, (int)strlen(type.name));
        }
    }

    cs = ctx.tables->getStructAt(structure->getStructIndex());
    if (cs)
    {
        const FTtype cnt = cs[1];
        cs += 2;
        for (FTtype i = 0; i < cnt; ++i, cs += 2)
        {
            const ftType& type = ctx.tables->getTypeAt(cs[0]);
            const ftName& name = ctx.tables->getNameAt(cs[1]);

            writeIndent(ctx, out, 2);

            if (ctx.fixBlendNaming)
            {
                if (string(type.name) == "anim")
                    out << left << setw(maxLeft) << "Anim" << ' ' << name.name << ';' << endl;
                else
                    out << left << setw(maxLeft) << type.name << ' ' << name.name << ';' << endl;
            }
            else
                out << left << setw(maxLeft) << type.name << ' ' << name.name << ';' << endl;
        }
    }
    writeIndent(ctx, out, 1);
    out << "};" << endl;
    out << endl;
}

void writeHashCodes(ProgramInfo& ctx, ostream& out, const StructArray& structures)
{
    writeIndent(ctx, out, 1);
    out << "namespace StructCodes" << endl;

    writeIndent(ctx, out, 1);
    out << "{" << endl;

    writeIndent(ctx, out, 2);
    out << "typedef unsigned long long HashCode;" << endl;
    out << endl;

    StructArray::ConstIterator cit = structures.iterator();
    while (cit.hasMoreElements())
    {
        const ftStruct* structure = cit.getNext();
        out << setw(ctx.useNamespace ? 8 : 4) << ' ';
        out << "const HashCode SDNA_" << uppercase << structure->getName() << "= 0x" << hex << structure->getHashedType() << ';' << endl;
    }
    writeIndent(ctx, out, 1);
    out << "}" << endl;
    out << endl;
    out << endl;
}

void writeUnresolved(ProgramInfo& ctx, ostream& out, FTtype* typeNotFound)
{
    char*         typeName = ctx.tables->getTypeNameAt(typeNotFound[0]);
    const ftName& name     = ctx.tables->getNameAt(typeNotFound[1]);

    if (name.pointerCount > 0)
    {
        if (ctx.fixBlendNaming && string(typeName) == "anim")
        {
            writeIndent(ctx, out, 1);
            out << "struct Anim" << endl;

            writeIndent(ctx, out, 1);
            out << '{' << endl;

            writeIndent(ctx, out, 2);
            out << "int missing;" << endl;

            writeIndent(ctx, out, 1);
            out << "};" << endl;
            out << endl;
        }
        else
        {
            writeIndent(ctx, out, 1);
            out << "struct " << typeName << endl;
            writeIndent(ctx, out, 1);
            out << '{' << endl;

            writeIndent(ctx, out, 2);
            out << "int missing;" << endl;

            writeIndent(ctx, out, 1);
            out << "};" << endl;
            out << endl;
        }
    }
}

bool hasUnResolved(skArray<FTtype*>& unresolved, const FTtype* independent)
{
    bool found = false;

    skArray<FTtype*>::Iterator it = unresolved.iterator();
    while (!found && it.hasMoreElements())
    {
        FTtype* type = it.getNext();
        found        = type[0] == independent[0];
    }
    return found;
}

bool structContains(ftStruct*    a,
                    ftStruct*    b,
                    StructArray& searchList,
                    StructArray& independent)
{
    ftTable* tables = a->getOwner();
    bool      result = false;
    if (tables)
    {
        FTtype* structure = tables->getStructAt(a->getStructIndex());
        if (structure)
        {
            const FTtype count = structure[1];
            structure += 2;
            for (FTtype i = 0; i < count && !result; ++i, structure += 2)
            {
                const ftType& type = tables->getTypeAt(structure[0]);
                const ftName& name = tables->getNameAt(structure[1]);

                if (type.id != SK_NPOS32)
                {
                    if (name.pointerCount <= 0)
                    {
                        ftCharHashKey chk(type.name);
                        ftStruct* foundStruct = tables->findStructByName(chk);

                        if (foundStruct && foundStruct == b)
                        {
                            if (independent.find(b) == independent.npos)
                            {
                                if (searchList.find(foundStruct) != searchList.npos)
                                {
                                    if (foundStruct->getHashedType() == b->getHashedType())
                                        result = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

void findMemberStructs(ftStruct*    fStructure,
                       StructArray& searchList,
                       StructArray& independent,
                       StructArray& out)
{
    StructArray::Iterator it = searchList.iterator();
    while (it.hasMoreElements())
    {
        ftStruct* tStructure = it.getNext();
        if (tStructure != fStructure)
        {
            if (structContains(fStructure, tStructure, searchList, independent))
                out.push_back(tStructure);
        }
    }
}

void organizeDependentStructs(StructArray& dependent, StructArray& independent)
{
    StructDeque structs, topLevel;
    if (!dependent.empty())
    {
        StructArray::Iterator it = dependent.iterator();

        while (it.hasMoreElements())
        {
            ftStruct*   structure = it.getNext();
            StructArray dependencies;

            findMemberStructs(structure, dependent, independent, dependencies);

            if (dependencies.empty())
            {
                structure->lock();
                topLevel.push_front(structure);
            }
            else
                structs.push_back(structure);
        }

        it = dependent.iterator();
        while (it.hasMoreElements())
        {
            ftStruct* structure = it.getNext();

            StructArray dependencies;
            findMemberStructs(structure, dependent, independent, dependencies);

            if (!dependencies.empty())
            {
                StructDeque::Link* link = structs.find(structure);
                if (link)
                {
                    StructArray           erase;
                    StructArray::Iterator sit = dependencies.iterator();
                    while (sit.hasMoreElements())
                    {
                        ftStruct* ls = sit.getNext();
                        if (!ls->isLocked())
                        {
                            erase.push_back(ls);
                            structs.erase(ls);
                        }
                    }

                    sit = erase.iterator();
                    while (sit.hasMoreElements())
                    {
                        ftStruct* ls = sit.getNext();
                        if (!ls->isLocked())
                        {
                            ls->lock();
                            structs.insert_front(link, ls);
                        }
                    }
                }
            }
            else if (!structure->isLocked())
            {
                StructDeque::Link* link = structs.find(structure);
                if (link)
                {
                    structure->lock();
                    structs.erase(link);
                    topLevel.push_front(structure);
                }
            }
        }
    }

    dependent.clear();
    StructDeque::Iterator it = topLevel.iterator();
    while (it.hasMoreElements())
        dependent.push_back(it.getNext());

    it = structs.iterator();
    while (it.hasMoreElements())
        dependent.push_back(it.getNext());
}

void sortStructs(ProgramInfo&        ctx,
                 skArray<FTtype*>&   unresolved,
                 skArray<ftStruct*>& independent,
                 skArray<ftStruct*>& dependent)
{
    skArray<ftStruct*> main = ctx.tables->getStructureArray();

    skArray<ftStruct*>::Iterator it = main.iterator();
    while (it.hasMoreElements())
    {
        ftStruct* cs = it.getNext();

        FTtype* structure = ctx.tables->getStructAt(cs->getStructIndex());
        if (structure)
        {
            const FTtype count = structure[1];
            structure += 2;

            bool hasStructs           = false;
            bool hasNonPointerStructs = false;

            for (FTtype i = 0; i < count; ++i, structure += 2)
            {
                const ftType& type = ctx.tables->getTypeAt(structure[0]);
                const ftName& name = ctx.tables->getNameAt(structure[1]);

                if (type.id != SK_NPOS32 && name.pointerCount <= 0)
                    hasNonPointerStructs = true;

                if (type.id != SK_NPOS32)  // it's a valid struct
                    hasStructs = true;
                else if (name.pointerCount > 0)
                {
                    // if it's not a struct or an atomic type then it's unresolved.
                    if (string(type.name) != "bool")
                    {
                        const ftAtomic atomic = ftAtomicUtils::getPrimitiveType(
                            ftCharHashKey(type.name).hash());
                        if (atomic == ftAtomic::FT_ATOMIC_UNKNOWN)
                        {
                            if (!hasUnResolved(unresolved, structure))
                                unresolved.push_back(structure);
                        }
                    }
                }
            }
            if (!hasStructs || !hasNonPointerStructs)
                independent.push_back(cs);
            else
                dependent.push_back(cs);
        }
    }

    organizeDependentStructs(dependent, independent);
}

int extractToFile(ProgramInfo& ctx)
{
    int       status = 0;
    ftTable* tables = ctx.tables;

    skArray<FTtype*>   unresolved;
    skArray<ftStruct*> independent, dependent;
    sortStructs(ctx, unresolved, independent, dependent);

    ofstream outFile;
    ostream& outStream = outFile;

    outFile.open(ctx.outFile.c_str());
    if (outFile.is_open())
    {
        writeFileHeader(ctx, outStream);

        if (ctx.useNamespace)
        {
            outStream << "namespace " << ctx.nameSpaceName << endl;
            outStream << "{" << endl;
            outStream << endl;
        }

        if (ctx.writeHashCode)
            writeHashCodes(ctx, outStream, tables->getStructureArray());

        skArray<ftStruct*>::Iterator it = tables->getStructIterator();
        while (it.hasMoreElements())
        {
            ftStruct* structure = it.getNext();
            writeForward(ctx, outStream, structure);
        }

        skArray<FTtype*>::Iterator uit = unresolved.iterator();
        while (uit.hasMoreElements())
        {
            FTtype* cur = uit.getNext();
            writeUnresolved(ctx, outStream, cur);
        }
        outStream << endl;

        it = independent.iterator();
        while (it.hasMoreElements())
            writeStructure(ctx, outStream, it.getNext());
        outStream << endl;

        it = dependent.iterator();
        while (it.hasMoreElements())
            writeStructure(ctx, outStream, it.getNext());
        outStream << endl;
        if (ctx.useNamespace)
            outStream << "}" << endl;
        writeFileFooter(ctx, outStream);
    }
    else
    {
        status = -1;
        cout << "Failed to open the output file " << ctx.outFile.c_str() << endl;
    }
    return status;
}

void usage(const char* programName)
{
    if (programName)
    {
        printf("%s\n", programName);
        printf("       <options> -i <infile> -o <outfile>\n\n");
        printf("       <options>\n");
        printf("                 -n  Use namespace. Puts all declarations inside a namespace.\n");
        printf("                 -b  Fix .blend  nonstandard naming that would require -fpermissive.\n");
        printf("                 -s  include <stdint.h>.\n");
        printf("                 -c  Generate structure hash codes.\n");
        printf("                 -h  Display this message.\n");
        printf("\n");
    }
    else
        printf("Invalid program name supplied to usage.\n");
}

int parseCommandLine(ProgramInfo& ctx, int argc, char** argv)
{
    int status = 0;
    for (int i = 0; i < argc && status == 0; ++i)
    {
        char* curArg = argv[i];
        if (*curArg == '-')
        {
            ++curArg;
            if (*curArg == 'i')
            {
                if (i + 1 < argc)
                    ctx.inFile = argv[++i];
                else
                {
                    printf(
                        "Parsed the end of the command line while "
                        "searching for the next input.\n");
                    status = -1;
                }
            }
            else if (*curArg == 'h')
                usage(ctx.programName);
            else if (*curArg == 'n')
            {
                ctx.useNamespace  = true;
                ctx.nameSpaceName = argv[++i];
            }
            else if (*curArg == 'b')
                ctx.fixBlendNaming = true;
            else if (*curArg == 's')
                ctx.initTypes = true;
            else if (*curArg == 'c')
                ctx.writeHashCode = true;
            else if (*curArg == 'o')
            {
                if (i + 1 < argc)
                    ctx.outFile = argv[++i];
                else
                {
                    printf(
                        "Parsed the end of the command line while "
                        "searching for the next input.\n");
                    status = -1;
                }
            }
        }
    }

    if (ctx.inFile.empty())
    {
        status = -2;
        printf("The input file option (-i <infile>) is required.\n");
    }
    else if (ctx.outFile.empty())
    {
        status = -2;
        printf("The output file option (-o <outfile>) is required.\n");
    }

    return status;
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

int main(int argc, char** argv)
{
    // The main idea of this program is
    // to load any type of file and scan for a DNA1 chunk
    // then extract the tables.

    char* base = argv[0] + getBaseName(argv[0]);

    ProgramInfo ctx  = {base, "", "", nullptr, nullptr, nullptr, ""};
    ctx.useNamespace = false;

    if (parseCommandLine(ctx, argc, argv) < 0)
    {
        usage(base);
        return 1;
    }

    int status;

    ftGzStream fp;
    fp.open(ctx.inFile.c_str(), skStream::READ);
    if (fp.isOpen())
    {
        ftScanDna scanner;
        status = scanner.findHeaderFlags(&fp);
        if (status != ftFlags::FS_OK)
        {
            printf(
                "Failed to extract the appropriate header "
                "flags from the supplied file %s.\n",
                ctx.inFile.c_str());
        }
        else
        {
            fp.seek(0, SEEK_SET);
            fp.read(ctx.header, 12);
            ctx.header[12] = 0;

            status = scanner.scan(&fp);
            if (status == ftFlags::FS_OK)
            {
                ctx.dna    = scanner.getDNA();
                ctx.tables = new ftTable(scanner.is64Bit() ? 8 : 4);

                status = ctx.tables->read(ctx.dna,
                                          scanner.getLength(),
                                          scanner.getFlags(),
                                          ftFlags::LF_ONLY_ERR);
                if (status == ftFlags::FS_OK)
                {
                    status = extractToFile(ctx);
                }
                else
                {
                    printf(
                        "Failed to rebuild the DNA table"
                        "from the supplied file %s.\n",
                        ctx.inFile.c_str());
                }
            }
            else
            {
                printf(
                    "Failed to properly scan to the DNA1 block"
                    "in the supplied file %s.\n",
                    ctx.inFile.c_str());
            }
        }
    }
    else
    {
        status = -1;
        printf("Failed to open the input file for reading.\n");
    }

    delete ctx.tables;
    if (ctx.dna)
        free(ctx.dna);
    return status;
}
