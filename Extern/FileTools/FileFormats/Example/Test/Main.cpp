#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Example.h"
#include "ftStreams.h"
#include "ftTables.h"
#include "ftTypes.h"

#define TestFile "test.ex"

void writeFile(int version)
{
    Example e;
    e.setFileFlags(ftFlags::LF_WRITE_CHUNKS);

    e.getInfo().major = version;


    Data1* d1 = new Data1();
    d1->arr1[0][0] = 1;
    d1->arr1[0][1] = 0;
    d1->arr1[1][0] = 0;
    d1->arr1[1][1] = 1;

    d1->var1 = 1;
    d1->var2 = 2;
    d1->var3 = 4;
    d1->var5 = 1;

    e.getData1().push_back(d1);

    Data2 d2;

    d2.ptr = d1;
    d2.val1 = (int64_t)d2.ptr;
    e.getData2().push_back(d2);

    e.save(TestFile);

    delete d1;
}


void readFile(void)
{
    Example e;

    e.setFileFlags(ftFlags::LF_DIAGNOSTICS|ftFlags::LF_DUMP_CAST);
    e.load(TestFile);

    Example::Data2Array::Iterator d2a = e.getData2().iterator();
    while (d2a.hasMoreElements())
    {
        Data2 dt = d2a.getNext();

        if (dt.ptr)
        {
            Data1* dp = dt.ptr;
            printf("------------ Data1* :%p -------------\n", dp);
            printf("{{%f, %f}, {%f, %f}}\n",
                   dp->arr1[0][0],
                   dp->arr1[0][1],
                   dp->arr1[1][0],
                   dp->arr1[1][1]
                  );
        }

    }

}


int main()
{
    writeFile(200);
    readFile();
    return 0;
}