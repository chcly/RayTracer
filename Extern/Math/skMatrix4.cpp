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
#include "skMatrix4.h"
#include "skMatrix3.h"
#include <cstdio>


const skMatrix4 skMatrix4::Identity = skMatrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const skMatrix4 skMatrix4::Zero     = skMatrix4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

void skMatrix4::print() const
{
    printf("[ %3.3f, %3.3f, %3.3f, %3.3f ]\n", (double)m[0][0], (double)m[0][1], (double)m[0][2], (double)m[0][3]);
    printf("[ %3.3f, %3.3f, %3.3f, %3.3f ]\n", (double)m[1][0], (double)m[1][1], (double)m[1][2], (double)m[1][3]);
    printf("[ %3.3f, %3.3f, %3.3f, %3.3f ]\n", (double)m[2][0], (double)m[2][1], (double)m[2][2], (double)m[2][3]);
    printf("[ %3.3f, %3.3f, %3.3f, %3.3f ]\n", (double)m[3][0], (double)m[3][1], (double)m[3][2], (double)m[3][3]);
}

skMatrix4::skMatrix4(const skMatrix4& v)
{
    *this = v;
}

skMatrix4::skMatrix4(
    skScalar m00,
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
    skScalar m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

skMatrix4::skMatrix4(const skTransform2D& v)
{
    m[0][0] = v.m[0][0];
    m[0][1] = v.m[0][1];
    m[0][2] = skScalar(0);
    m[0][3] = v.m[0][2];
    m[1][0] = v.m[1][0];
    m[1][1] = v.m[1][1];
    m[1][2] = skScalar(0);
    m[1][3] = v.m[1][2];
    m[2][0] = skScalar(0);
    m[2][1] = skScalar(0);
    m[2][2] = skScalar(1);
    m[2][3] = skScalar(0);
    m[3][0] = skScalar(0);
    m[3][1] = skScalar(0);
    m[3][2] = skScalar(0);
    m[3][3] = skScalar(1);
}

skMatrix4::skMatrix4(const skScalar* v)
{
    m[0][0] = *v++;
    m[0][1] = *v++;
    m[0][2] = *v++;
    m[0][3] = *v++;
    m[1][0] = *v++;
    m[1][1] = *v++;
    m[1][2] = *v++;
    m[1][3] = *v++;
    m[2][0] = *v++;
    m[2][1] = *v++;
    m[2][2] = *v++;
    m[2][3] = *v++;
    m[3][0] = *v++;
    m[3][1] = *v++;
    m[3][2] = *v++;
    m[3][3] = *v;
}

skMatrix4& skMatrix4::operator=(const skMatrix4& v)
{
    m[0][0] = v.m[0][0];
    m[0][1] = v.m[0][1];
    m[0][2] = v.m[0][2];
    m[0][3] = v.m[0][3];
    m[1][0] = v.m[1][0];
    m[1][1] = v.m[1][1];
    m[1][2] = v.m[1][2];
    m[1][3] = v.m[1][3];
    m[2][0] = v.m[2][0];
    m[2][1] = v.m[2][1];
    m[2][2] = v.m[2][2];
    m[2][3] = v.m[2][3];
    m[3][0] = v.m[3][0];
    m[3][1] = v.m[3][1];
    m[3][2] = v.m[3][2];
    m[3][3] = v.m[3][3];
    return *this;
}

skMatrix4 skMatrix4::operator*(const skMatrix4& v) const
{
    return skMatrix4(
        m[0][0] * v.m[0][0] + m[0][1] * v.m[1][0] + m[0][2] * v.m[2][0] + m[0][3] * v.m[3][0],
        m[0][0] * v.m[0][1] + m[0][1] * v.m[1][1] + m[0][2] * v.m[2][1] + m[0][3] * v.m[3][1],
        m[0][0] * v.m[0][2] + m[0][1] * v.m[1][2] + m[0][2] * v.m[2][2] + m[0][3] * v.m[3][2],
        m[0][0] * v.m[0][3] + m[0][1] * v.m[1][3] + m[0][2] * v.m[2][3] + m[0][3] * v.m[3][3],

        m[1][0] * v.m[0][0] + m[1][1] * v.m[1][0] + m[1][2] * v.m[2][0] + m[1][3] * v.m[3][0],
        m[1][0] * v.m[0][1] + m[1][1] * v.m[1][1] + m[1][2] * v.m[2][1] + m[1][3] * v.m[3][1],
        m[1][0] * v.m[0][2] + m[1][1] * v.m[1][2] + m[1][2] * v.m[2][2] + m[1][3] * v.m[3][2],
        m[1][0] * v.m[0][3] + m[1][1] * v.m[1][3] + m[1][2] * v.m[2][3] + m[1][3] * v.m[3][3],

        m[2][0] * v.m[0][0] + m[2][1] * v.m[1][0] + m[2][2] * v.m[2][0] + m[2][3] * v.m[3][0],
        m[2][0] * v.m[0][1] + m[2][1] * v.m[1][1] + m[2][2] * v.m[2][1] + m[2][3] * v.m[3][1],
        m[2][0] * v.m[0][2] + m[2][1] * v.m[1][2] + m[2][2] * v.m[2][2] + m[2][3] * v.m[3][2],
        m[2][0] * v.m[0][3] + m[2][1] * v.m[1][3] + m[2][2] * v.m[2][3] + m[2][3] * v.m[3][3],

        m[3][0] * v.m[0][0] + m[3][1] * v.m[1][0] + m[3][2] * v.m[2][0] + m[3][3] * v.m[3][0],
        m[3][0] * v.m[0][1] + m[3][1] * v.m[1][1] + m[3][2] * v.m[2][1] + m[3][3] * v.m[3][1],
        m[3][0] * v.m[0][2] + m[3][1] * v.m[1][2] + m[3][2] * v.m[2][2] + m[3][3] * v.m[3][2],
        m[3][0] * v.m[0][3] + m[3][1] * v.m[1][3] + m[3][2] * v.m[2][3] + m[3][3] * v.m[3][3]);
}

void skMatrix4::multAssign(const skMatrix4& a, const skMatrix4 &b)
{
    m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
    m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
    m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
    m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

    m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
    m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
    m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
    m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

    m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
    m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
    m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
    m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

    m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
    m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
    m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
    m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];
}

void skMatrix4::merge(skMatrix4& d, const skMatrix4& a, const skMatrix4& b)
{
    d.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
    d.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
    d.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
    d.m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

    d.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
    d.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
    d.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
    d.m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

    d.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
    d.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
    d.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
    d.m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

    d.m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
    d.m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
    d.m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
    d.m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];
}

skMatrix4& skMatrix4::transpose()
{
    *this = transposed();
    return *this;
}

skMatrix4 skMatrix4::transposed() const
{
    return skMatrix4(m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2], m[0][3], m[1][3], m[2][3], m[3][3]);
}

void skMatrix4::setTrans(const skVector3& v)
{
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
    m[3][3] = 1;
}

void skMatrix4::setTrans(skScalar x, skScalar y, skScalar z)
{
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
    m[3][3] = 1;
}

void skMatrix4::setScale(const skVector3& v)
{
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
    m[3][3] = 1;
}

void skMatrix4::setScale(skScalar x, skScalar y, skScalar z)
{
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    m[3][3] = 1;
}

skVector3 skMatrix4::getTrans() const
{
    return skVector3(m[0][3], m[1][3], m[2][3]);
}

skVector3 skMatrix4::getScale() const
{
    return skVector3(m[0][0], m[1][1], m[2][2]);
}

void skMatrix4::makeIdentity()
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void skMatrix4::makeTransform(const skVector3& loc, const skVector3& scale, const skQuaternion& rot)
{
    skMatrix3 m3;

    m3.fromQuat(rot);
    makeTransform(loc, scale, m3);
}

void skMatrix4::makeInverseTransform(const skVector3& loc, const skVector3& scale, const skQuaternion& rot)
{
    skMatrix3 m3;
    m3.fromQuat(rot.inverse());
    makeInverseTransform(loc, scale, m3);
}

void skMatrix4::makeTransform(const skVector3& loc, const skVector3& scale, const skMatrix3& rot)
{
    m[0][0] = scale.x * rot.m[0][0];
    m[0][1] = scale.y * rot.m[0][1];
    m[0][2] = scale.z * rot.m[0][2];
    m[0][3] = loc.x;

    m[1][0] = scale.x * rot.m[1][0];
    m[1][1] = scale.y * rot.m[1][1];
    m[1][2] = scale.z * rot.m[1][2];
    m[1][3] = loc.y;

    m[2][0] = scale.x * rot.m[2][0];
    m[2][1] = scale.y * rot.m[2][1];
    m[2][2] = scale.z * rot.m[2][2];
    m[2][3] = loc.z;

    m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3]                     = 1;
}

void skMatrix4::makeInverseTransform(const skVector3& loc, const skVector3& scale, const skMatrix3& rot)
{
    const skVector3 is = 1.0 / scale;

    m[0][0] = is.x * rot.m[0][0];
    m[0][1] = is.y * rot.m[0][1];
    m[0][2] = is.z * rot.m[0][2];
    m[0][3] = -loc.x;

    m[1][0] = is.x * rot.m[1][0];
    m[1][1] = is.y * rot.m[1][1];
    m[1][2] = is.z * rot.m[1][2];
    m[1][3] = -loc.y;

    m[2][0] = is.x * rot.m[2][0];
    m[2][1] = is.y * rot.m[2][1];
    m[2][2] = is.z * rot.m[2][2];
    m[2][3] = -loc.z;

    m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3]                     = 1;
}

skScalar skMatrix4::det() const
{
    return m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] - m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
           m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] - m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
           m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
           m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] - m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
           m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] - m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
           m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] - m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
}

skMatrix4 skMatrix4::inverted() const
{
    skMatrix4 r;

    skScalar d = det();
    if (skIsZero(d))
        return Identity;

    d = skScalar(1.0) / d;

    r.m[0][0] = d * (m[1][2] * m[2][3] * m[3][1] - m[1][3] * m[2][2] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] + m[1][1] * m[2][2] * m[3][3]);
    r.m[1][0] = d * (m[0][3] * m[2][2] * m[3][1] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2] + m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] - m[0][1] * m[2][2] * m[3][3]);
    r.m[2][0] = d * (m[0][2] * m[1][3] * m[3][1] - m[0][3] * m[1][2] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] + m[0][1] * m[1][2] * m[3][3]);
    r.m[3][0] = d * (m[0][3] * m[1][2] * m[2][1] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2] + m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] - m[0][1] * m[1][2] * m[2][3]);
    r.m[0][1] = d * (m[1][3] * m[2][2] * m[3][0] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2] + m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] - m[1][0] * m[2][2] * m[3][3]);
    r.m[1][1] = d * (m[0][2] * m[2][3] * m[3][0] - m[0][3] * m[2][2] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] + m[0][0] * m[2][2] * m[3][3]);
    r.m[2][1] = d * (m[0][3] * m[1][2] * m[3][0] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2] + m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] - m[0][0] * m[1][2] * m[3][3]);
    r.m[3][1] = d * (m[0][2] * m[1][3] * m[2][0] - m[0][3] * m[1][2] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] + m[0][0] * m[1][2] * m[2][3]);
    r.m[0][2] = d * (m[1][1] * m[2][3] * m[3][0] - m[1][3] * m[2][1] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] + m[1][0] * m[2][1] * m[3][3]);
    r.m[1][2] = d * (m[0][3] * m[2][1] * m[3][0] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1] + m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] - m[0][0] * m[2][1] * m[3][3]);
    r.m[2][2] = d * (m[0][1] * m[1][3] * m[3][0] - m[0][3] * m[1][1] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] + m[0][0] * m[1][1] * m[3][3]);
    r.m[3][2] = d * (m[0][3] * m[1][1] * m[2][0] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1] + m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] - m[0][0] * m[1][1] * m[2][3]);
    r.m[0][3] = d * (m[1][2] * m[2][1] * m[3][0] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1] + m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] - m[1][0] * m[2][1] * m[3][2]);
    r.m[1][3] = d * (m[0][1] * m[2][2] * m[3][0] - m[0][2] * m[2][1] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] + m[0][0] * m[2][1] * m[3][2]);
    r.m[2][3] = d * (m[0][2] * m[1][1] * m[3][0] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1] + m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] - m[0][0] * m[1][1] * m[3][2]);
    r.m[3][3] = d * (m[0][1] * m[1][2] * m[2][0] - m[0][2] * m[1][1] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] + m[0][0] * m[1][1] * m[2][2]);

    return r;
}
