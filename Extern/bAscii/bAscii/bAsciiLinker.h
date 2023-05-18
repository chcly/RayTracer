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
#ifndef _bAsciiLinker_h_
#define _bAsciiLinker_h_

#include "bApi.h"
#include "bAsciiCommon.h"

class bAsciiLinker
{
protected:
    bAscii*     m_parent;
    bAsciiMain* m_main;
    bool        m_verbose;

    void linkCamera(bAsciiBlock* lib) const;
    void linkLight(bAsciiBlock* lib) const;
    void linkShape(bAsciiBlock* lib) const;

    void linkMesh(bAsciiBlock* lib) const;
    void resolveData(const bString& string, bObject* ob) const;

    void linkObject(bAsciiBlock* lib) const;
    void linkScene(bAsciiBlock* lib) const;
    void linkMaterial(bAsciiBlock* lib) const;

public:
    bAsciiLinker();
    ~bAsciiLinker() = default;

    int link(bAscii* parent, bAsciiMain* main, bool verbose);
};

#endif  //_bAsciiLinker_h_
