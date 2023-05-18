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
#include "Templates.h"
#include "Utils/skHexPrint.h"
#include "Utils/skMemoryStream.h"
#include "catch/Macro.h"
#include "ftAtomic.h"
#include "ftCompiler.h"
#include "ftLogger.h"
#include "ftScanner.h"



TEST_CASE("LinkTest")
{
    // TODO: Create a testing file format.
    //
    // 1) Be able to setup the test by first saving 
    //    the tables to the file format.
    // 2) Load the newly saved file as the 
    //    file tables, then use the tables generated from the API as the 
    //    memory tables.
    //
    // 3) Test the cross link function.
    // 4) Add new types to the API and save sequential versions of the file.
    // 5) Test the cross link function on the different versions of the file.
    //
    // SUCCESS - Means that the members are correctly linked against multiple 
    //           versions of the file.
    //
}
