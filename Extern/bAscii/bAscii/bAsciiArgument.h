/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _bAsciiArgument_h_
#define _bAsciiArgument_h_

#include "bAsciiCommon.h"

class bAsciiArgument
{
public:
    bAsciiArgument(const bCode& name, const bCode& type, const int nrArg) :
        name(name),
        type(type),
        nrArg(nrArg)
    {
    }

    ~bAsciiArgument() = default;

    const bCode  name;
    const bCode  type;
    const int    nrArg;
    bNumberVector  numbers;
    bBooleanVector booleans;
    bStringVector strings;
};

#endif  //_bAsciiScanner_h_
