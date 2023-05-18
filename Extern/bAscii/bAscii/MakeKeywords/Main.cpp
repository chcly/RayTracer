#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Utils/skHash.h"

using namespace std;
using Pair = pair<string, SKhash>;

int Alpha(const Pair& a, const Pair& b)
{
    return a.first < b.first;
}

void writeToStream(vector<Pair> vec, size_t ms, std::ostream& os)
{
    os << "#pragma once" << endl
       << endl;
    os << "#include \"Utils/skHash.h\"" << endl;
    os << "struct bKey" << endl;
    os << "{" << endl;
    os << "    const char*  name;" << endl;
    os << "    const SKhash hash;" << endl;
    os << "};" << endl
       << endl;

    os << setfill(' ');
    for (const Pair& pair : vec)
    {
        os
            << "constexpr bKey KW_"
            << pair.first << setw(ms - pair.first.size()) << ' '
            << "= { \""
            << pair.first
            << "\", 0x"
            << uppercase
            << hex << pair.second
            << "};" << endl;
    }
}


void trim(string& str)
{
    str.erase(str.find_last_not_of("\r\n") + 1);
    str.erase(0, str.find_first_not_of("\r\n"));
    str.erase(str.find_last_not_of(" \t") + 1);
    str.erase(0, str.find_first_not_of(" \t"));
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " id\n";
        return 1;
    }

    ifstream                      fs;
    unordered_map<string, SKhash> sMap;
    unordered_set<SKhash>         hSet;
    vector<Pair>                  vec;

    fs.open(argv[1]);

    if (fs.is_open())
    {
        size_t ms = 0;
        while (!fs.eof())
        {
            string str;
            std::getline(fs, str);

            trim(str);
            if (str.find('#') != string::npos)
                continue;

            if (!str.empty())
            {
                if (sMap.find(str) == sMap.end())
                {
                    const SKhash hash = skHash(str.c_str());

                    if (hSet.find(hash) != hSet.end())
                        cout << "duplicate hash: " << str << hash << endl;
                    else
                    {
                        sMap.insert(make_pair(str, hash));
                        hSet.insert(hash);
                        ms = max(str.size() + 1, ms);

                        vec.emplace_back(str, hash);
                    }
                }
            }
        }
        sort(vec.begin(), vec.end(), Alpha);

        ofstream out;
        out.open("bAsciiKeywords.inl");

        if (out.is_open())
            writeToStream(vec, ms, out);
        else
            writeToStream(vec, ms, cout);
        return 0;
    }
    cout << "failed To open input file" << endl;
    return 1;
}
