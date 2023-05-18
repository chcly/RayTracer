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
#ifndef _bAsciiKeywords_h_
#define _bAsciiKeywords_h_

#include "bAsciiCommon.h"
#define BASCII_VERSION 1

struct bAsciiLocal
{
    const char*        name;
    SKhash             hash;
    bCode              op;
    bCode              ctor;
    bCode              arg;
    bCode              type;
    int                argCount;
    int                maxArgCount;
    const bAsciiLocal* types;
};

extern const bAsciiLocal* bAsciiFindGlobal(SKhash code);
extern const bAsciiLocal* bAsciiFindLocal(const bAsciiLocal* st, SKhash code);


#endif  //_bAsciiKeywords_h_
