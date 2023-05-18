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
#ifndef _skMatrix3_h_
#define _skMatrix3_h_

#include "skEuler.h"
#include "skMath.h"
#include "skMatrix4.h"
#include "skQuaternion.h"
#include "skVector3.h"

class skMatrix3
{
public:
    union
    {
        skScalar m[3][3]{};
        skScalar p[9];
    };

public:
    skMatrix3() = default;

    explicit skMatrix3(const skMatrix4& m4)
    {
        this->fromMat4(m4);
    }

    skMatrix3(skScalar m00,
              skScalar m01,
              skScalar m02,
              skScalar m10,
              skScalar m11,
              skScalar m12,
              skScalar m20,
              skScalar m21,
              skScalar m22)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
    }

    explicit skMatrix3(const skScalar* v)
    {
        m[0][0] = *v++;
        m[0][1] = *v++;
        m[0][2] = *v++;
        m[1][0] = *v++;
        m[1][1] = *v++;
        m[1][2] = *v++;
        m[2][0] = *v++;
        m[2][1] = *v++;
        m[2][2] = *v;
    }

    skMatrix3(const skMatrix3& v)
    {
        m[0][0] = v.m[0][0];
        m[0][1] = v.m[0][1];
        m[0][2] = v.m[0][2];
        m[1][0] = v.m[1][0];
        m[1][1] = v.m[1][1];
        m[1][2] = v.m[1][2];
        m[2][0] = v.m[2][0];
        m[2][1] = v.m[2][1];
        m[2][2] = v.m[2][2];
    }

    skMatrix3 operator*(const skMatrix3& B) const
    {
        return skMatrix3(
            m[0][0] * B.m[0][0] + m[0][1] * B.m[1][0] + m[0][2] * B.m[2][0],
            m[0][0] * B.m[0][1] + m[0][1] * B.m[1][1] + m[0][2] * B.m[2][1],
            m[0][0] * B.m[0][2] + m[0][1] * B.m[1][2] + m[0][2] * B.m[2][2],

            m[1][0] * B.m[0][0] + m[1][1] * B.m[1][0] + m[1][2] * B.m[2][0],
            m[1][0] * B.m[0][1] + m[1][1] * B.m[1][1] + m[1][2] * B.m[2][1],
            m[1][0] * B.m[0][2] + m[1][1] * B.m[1][2] + m[1][2] * B.m[2][2],

            m[2][0] * B.m[0][0] + m[2][1] * B.m[1][0] + m[2][2] * B.m[2][0],
            m[2][0] * B.m[0][1] + m[2][1] * B.m[1][1] + m[2][2] * B.m[2][1],
            m[2][0] * B.m[0][2] + m[2][1] * B.m[1][2] + m[2][2] * B.m[2][2]);
    }

    skVector3 operator*(const skVector3& v) const
    {
        return skVector3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                         m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                         m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    void transpose()
    {
        *this = transposed();
    }

    skMatrix3 transposed() const
    {
        return skMatrix3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]);
    }

    skVector3 row(const int idx) const
    {
        if (idx < 3 && idx >= 0)
            return skVector3(m[0][idx], m[1][idx], m[2][idx]);
        return skVector3::Zero;
    }

    skVector3 col(const int idx) const
    {
        if (idx < 3 && idx >= 0)
            return skVector3(m[idx][0], m[idx][1], m[idx][2]);
        return skVector3::Zero;
    }

    void makeIdentity()
    {
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
    }

    void fromAngles(const skScalar pitch, const skScalar yaw, const skScalar roll)
    {
        skScalar s0, c0, s1, c1, s2, c2;
        skMath::sinCos(pitch, s0, c0);
        skMath::sinCos(yaw, s1, c1);
        skMath::sinCos(roll, s2, c2);

        const skScalar s2c0 = s2 * c0;
        const skScalar s2s0 = s2 * s0;

        m[0][0] = c1 * c2;
        m[0][1] = -c1 * s2c0 + s1 * s0;
        m[0][2] = c1 * s2s0 + s1 * c0;
        m[1][0] = s2;
        m[1][1] = c2 * c0;
        m[1][2] = -c2 * s0;
        m[2][0] = -s1 * c2;
        m[2][1] = s1 * s2c0 + c1 * s0;
        m[2][2] = -s1 * s2s0 + c1 * c0;
    }

    void fromEuler(const skEuler& e)
    {
        fromAngles(e.pitch, e.yaw, e.roll);
    }

    void fromQuat(const skQuaternion& q)
    {
        const skScalar qx2 = q.x * q.x;
        const skScalar qy2 = q.y * q.y;
        const skScalar qz2 = q.z * q.z;

        const skScalar qxy = q.x * q.y;
        const skScalar qxz = q.x * q.z;
        const skScalar qyz = q.y * q.z;

        const skScalar qwx = q.w * q.x;
        const skScalar qwy = q.w * q.y;
        const skScalar qwz = q.w * q.z;

        m[0][0] = skScalar(1.0) - skScalar(2.0) * (qy2 + qz2);
        m[0][1] = skScalar(2.0) * (qxy - qwz);
        m[0][2] = skScalar(2.0) * (qxz + qwy);

        m[1][0] = skScalar(2.0) * (qxy + qwz);
        m[1][1] = skScalar(1.0) - skScalar(2.0) * (qx2 + qz2);
        m[1][2] = skScalar(2.0) * (qyz - qwx);

        m[2][0] = skScalar(2.0) * (qxz - qwy);
        m[2][1] = skScalar(2.0) * (qyz + qwx);
        m[2][2] = skScalar(1.0) - skScalar(2.0) * (qx2 + qy2);
    }

    void fromMat4(const skMatrix4& m4x4)
    {
        m[0][0] = m4x4.m[0][0];
        m[0][1] = m4x4.m[0][1];
        m[0][2] = m4x4.m[0][2];

        m[1][0] = m4x4.m[1][0];
        m[1][1] = m4x4.m[1][1];
        m[1][2] = m4x4.m[1][2];

        m[2][0] = m4x4.m[2][0];
        m[2][1] = m4x4.m[2][1];
        m[2][2] = m4x4.m[2][2];
    }

    void print() const;

public:
    static const skMatrix3 Identity;
    static const skMatrix3 Zero;
};

#endif  //_skMatrix3_h_
