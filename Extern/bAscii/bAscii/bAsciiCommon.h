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
#ifndef _bAsciiCommon_h_
#define _bAsciiCommon_h_

#include "Utils/skMap.h"
#include "Utils/skString.h"

struct bBlockDecl;
class bAscii;
class bAsciiMain;
class bStatement;
class bAsciiBlock;
class bAsciiCompiler;
class bAsciiScanner;
class bAsciiArgument;

#define OP_UNKNOWN SK_NPOS32
#define OP_MIN 0
#define OP_ERROR (-1)
#define OP_UNDEFINED (-1)

typedef SKuint32 bCode;

// ---------------------------------------------------------------------------
using bString        = skString;
using bNumber        = float;
using bInteger       = int;
using bNumberVector  = skArray<bNumber>;
using bStringVector  = skArray<bString>;
using bBooleanVector = skArray<bool>;

// ---------------------------------------------------------------------------
typedef skArray<bAsciiBlock*>                  bAsciiBlockArray;
typedef skHashTable<SKuint32, bAsciiArgument*> bAsciiArgumentMap;
typedef skHashTable<skString, bAsciiBlock*>    bAsciiBlockMap;
typedef skHashTable<SKuint32, bAsciiBlock*>    bAsciiBlockMapI;

#endif  //_bAsciiCommon_h_
