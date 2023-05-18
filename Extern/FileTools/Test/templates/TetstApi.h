/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

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
#ifndef _TestAPI_h_
#define _TestAPI_h_


struct StructTest1
{
    char name[64]; // 1d array []
    int  id1, id2;
};



struct StructTest2
{
    // test recursive structures
    StructTest1 nested;
};

struct StructTest3
{
    // Pointer
    StructTest2* nested;
};


struct StructTest4
{
    // Pointer -> Pointer
    StructTest2** nested;
};


struct StructTest5
{
    // function pointer.. 
    void (*fp)();
};


struct StructTest6
{
    // multi dimensional 
    double multi[4][4];
};



struct StructTest7
{
    // multi dimensional  -> FT_ARR_DIM_MAX
    double multi[4][4][4];
};


struct StructTest8
{
    // multi dimensional  -> FT_ARR_DIM_MAX
    double multi[4][4][4];
};


#endif  //_TestAPI_h_