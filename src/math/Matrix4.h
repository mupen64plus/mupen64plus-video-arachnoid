/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#ifndef MATRIX_4_H_
#define MATRIX_4_H_

#include <iostream>
#include <ostream>

//*****************************************************************************
//* Matrix4
//! Math class defining a 4x4 matrix or an array of 16 float.
//! @details Used for viewprojection matrix and vertex transformation.
//*****************************************************************************
class Matrix4
{
public:
    
    //! The matrix entries, indexed by [row][col].
    union {
        float m[4][4];
        float _m[16];
    };

public:

    //! Default constructor.
    inline Matrix4() { operator=(IDENTITY);  }
    inline Matrix4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33 )
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

    //! Matrix multiplication
    inline Matrix4 operator * ( const Matrix4 &m2 ) const
    {
        Matrix4 r;
        r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
        r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
        r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
        r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

        r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
        r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
        r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
        r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

        r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
        r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
        r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
        r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

        r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
        r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
        r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
        r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

        return r;
    }

    //Access operators
    inline float* operator [] ( size_t iRow ) { return m[iRow]; }

    inline const float* const operator [] ( size_t iRow ) const { return m[iRow]; }

    //! Matrix addition.
    inline Matrix4 operator + ( const Matrix4 &m2 ) const
    {
        Matrix4 r;

        r.m[0][0] = m[0][0] + m2.m[0][0];
        r.m[0][1] = m[0][1] + m2.m[0][1];
        r.m[0][2] = m[0][2] + m2.m[0][2];
        r.m[0][3] = m[0][3] + m2.m[0][3];

        r.m[1][0] = m[1][0] + m2.m[1][0];
        r.m[1][1] = m[1][1] + m2.m[1][1];
        r.m[1][2] = m[1][2] + m2.m[1][2];
        r.m[1][3] = m[1][3] + m2.m[1][3];

        r.m[2][0] = m[2][0] + m2.m[2][0];
        r.m[2][1] = m[2][1] + m2.m[2][1];
        r.m[2][2] = m[2][2] + m2.m[2][2];
        r.m[2][3] = m[2][3] + m2.m[2][3];

        r.m[3][0] = m[3][0] + m2.m[3][0];
        r.m[3][1] = m[3][1] + m2.m[3][1];
        r.m[3][2] = m[3][2] + m2.m[3][2];
        r.m[3][3] = m[3][3] + m2.m[3][3];

        return r;
    }

    //! Matrix subtraction.
    inline Matrix4 operator - ( const Matrix4 &m2 ) const
    {
        Matrix4 r;
        r.m[0][0] = m[0][0] - m2.m[0][0];
        r.m[0][1] = m[0][1] - m2.m[0][1];
        r.m[0][2] = m[0][2] - m2.m[0][2];
        r.m[0][3] = m[0][3] - m2.m[0][3];

        r.m[1][0] = m[1][0] - m2.m[1][0];
        r.m[1][1] = m[1][1] - m2.m[1][1];
        r.m[1][2] = m[1][2] - m2.m[1][2];
        r.m[1][3] = m[1][3] - m2.m[1][3];

        r.m[2][0] = m[2][0] - m2.m[2][0];
        r.m[2][1] = m[2][1] - m2.m[2][1];
        r.m[2][2] = m[2][2] - m2.m[2][2];
        r.m[2][3] = m[2][3] - m2.m[2][3];

        r.m[3][0] = m[3][0] - m2.m[3][0];
        r.m[3][1] = m[3][1] - m2.m[3][1];
        r.m[3][2] = m[3][2] - m2.m[3][2];
        r.m[3][3] = m[3][3] - m2.m[3][3];

        return r;
    }

    //! Tests 2 matrices for equality.
    inline bool operator == ( const Matrix4& m2 ) const
    {
        if( 
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
            return false;
        return true;
    }

    //! Tests 2 matrices for inequality.
    inline bool operator != ( const Matrix4& m2 ) const
    {
        if( 
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
            return true;
        return false;
    }

    //!Transpose Matrix (Switch columns with rows)
    inline Matrix4 transpose() const
    {
        return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                       m[0][1], m[1][1], m[2][1], m[3][1],
                       m[0][2], m[1][2], m[2][2], m[3][2],
                       m[0][3], m[1][3], m[2][3], m[3][3]);
    }

    //! Set Translation Part of the matrix
    inline void setTranslationPart(const float v[3] )
    {
        m[0][3] = v[0];
        m[1][3] = v[1];
        m[2][3] = v[2];
    }      

    //! Builds a translation matrix
    inline void setTranslation(float tx, float ty, float tz)
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

    //! Set scale part of matrix
    inline void setScalePart( const float v[3] )
    {
        m[0][0] = v[0];
        m[1][1] = v[1];
        m[2][2] = v[2];
    }

    static const Matrix4 ZERO;
    static const Matrix4 IDENTITY;        
    static const Matrix4 CLIPSPACE2DTOIMAGESPACE;  //! Useful little matrix which takes 2D clipspace {-1, 1} to {0,1} and inverts the Y. 


    inline Matrix4 operator*(float scalar) const
    {
        return Matrix4(
            scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
            scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
            scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
            scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
    }

    //! Function for writing to a stream.
    inline friend std::ostream& operator << ( std::ostream& o, const Matrix4& m )
    {
        o << "Matrix4(";
        for (size_t i = 0; i < 4; ++i)
        {
            o << " row" << (unsigned)i << "{";
            for(size_t j = 0; j < 4; ++j)
            {
                o << m[i][j] << " ";
            }
            o << "}";
        }
        o << ")";
        return o;
    }
    
    float determinant() const;
    Matrix4 inverse() const;

};

#endif
