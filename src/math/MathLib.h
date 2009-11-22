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

#ifndef MATH_LIBRARY_H_
#define MATH_LIBRARY_H_

#include <cmath>     //sqrtf
#include "m64p.h"  

// Formula: a.b = a0*b0 + a1*b1 + a2*b2
#define Vec3Dot(a,b)         ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])

#define Vec3Normalize(v) { \
    float lenght = (v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2]; \
    if ( lenght > 0.00001f ) { \
        lenght = 1.0f / sqrt(lenght); \
        (v)[0] *= lenght; \
        (v)[1] *= lenght; \
        (v)[2] *= lenght; \
    } \
}


//-----------------------------------------------------------------------------
// Transform Vector
//-----------------------------------------------------------------------------

inline void transformVertex( float* m, float* v, float* out )
{
    float x = v[0];
    float y = v[1];
    float z = v[2];
    float w = v[3];
    out[0] = m[0] * x + m[4] * y + m[8]  * z + m[12]; 
    out[1] = m[1] * x + m[5] * y + m[9]  * z + m[13];
    out[2] = m[2] * x + m[6] * y + m[10] * z + m[14];
    out[3] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

inline void transformVector( float* m, float* v, float* out )
{
    float x = v[0];
    float y = v[1];
    float z = v[2];
    out[0] = m[0] * x + m[4] * y + m[8]  * z; 
    out[1] = m[1] * x + m[5] * y + m[9]  * z;
    out[2] = m[2] * x + m[6] * y + m[10] * z;
}



//-----------------------------------------------------------------------------
// Random Float
//-----------------------------------------------------------------------------
inline float randomFloat(float min, float max)
{
    return (float)( min + double(max-min)*rand()/RAND_MAX );
}

inline unsigned int pow2( unsigned int dim )
{
    unsigned int i = 1;

    while (i < dim) i <<= 1;

    return i;
}

inline unsigned int powof( unsigned int dim )
{
    unsigned int num = 1;
    unsigned int i = 0;

    while (num < dim)
    {
        num <<= 1;
        i++;
    }

    return i;
}

#endif
