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
#ifndef _skMatrix4_h_
#define _skMatrix4_h_

#include "skVector3.h"
#include "skTransform2D.h"

class skMatrix4
{
public:
    union
    {
        skScalar m[4][4];
        skScalar p[16]{};
    };

public:
    skMatrix4()
    {
    }

    skMatrix4(const skMatrix4& v);
    skMatrix4(skScalar m00,
              skScalar m01,
              skScalar m02,
              skScalar m03,
              skScalar m10,
              skScalar m11,
              skScalar m12,
              skScalar m13,
              skScalar m20,
              skScalar m21,
              skScalar m22,
              skScalar m23,
              skScalar m30,
              skScalar m31,
              skScalar m32,
              skScalar m33);

    skMatrix4(const skTransform2D& v);
    skMatrix4(const skScalar* v);

    skMatrix4& operator=(const skMatrix4& v);
    skMatrix4  operator*(const skMatrix4& v) const;
    skMatrix4& transpose();
    skMatrix4  transposed() const;

    void      setTrans(const skVector3& v);
    void      setTrans(skScalar x, skScalar y, skScalar z);
    void      setScale(const skVector3& v);
    void      setScale(skScalar x, skScalar y, skScalar z);
    skVector3 getScale() const;
    skVector3 getTrans() const;
    void      makeIdentity();
    skScalar  det() const;
    skMatrix4 inverted() const;

    void multAssign(const skMatrix4& a, const skMatrix4& b);

    void        makeTransform(const skVector3& loc, const skVector3& scale, const skQuaternion& rot);
    void        makeTransform(const skVector3& loc, const skVector3& scale, const skMatrix3& rot);
    void        makeInverseTransform(const skVector3& loc, const skVector3& scale, const skQuaternion& rot);
    void        makeInverseTransform(const skVector3& loc, const skVector3& scale, const skMatrix3& rot);
    static void merge(skMatrix4& d, const skMatrix4& a, const skMatrix4& b);

    void print() const;

public:
    static const skMatrix4 Identity;
    static const skMatrix4 Zero;
};

#endif  //_skMatrix4_h_
