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
#ifndef _Example_h_
#define _Example_h_

#include "ftFile.h"
#include "API/FileInfo.h"
#include "API/Data1.h"
#include "API/Data2.h"



class Example : public ftFile
{
public:
    typedef skArray<Data1*> Data1Array;
    typedef skArray<Data2>  Data2Array;

private:
    FileInfo  m_info;
    Data1Array m_data;
    Data2Array m_data2;

public:
    Example();
    virtual ~Example();
    inline FileInfo&     getInfo(void)  { return m_info; }
    inline Data1Array&   getData1(void) { return m_data; }
    inline Data2Array&   getData2(void) { return m_data2; }

protected:
    virtual void*       getTables(void);
    virtual FBTsize     getTableSize(void);


    virtual int notifyDataRead(void* p, const ftChunk& id);
    virtual int serializeData(skStream* stream);
};


#endif//_Example_h_
