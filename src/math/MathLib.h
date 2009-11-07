

#ifndef MATH_LIBRARY_H_
#define MATH_LIBRARY_H_

#include <cmath>     //sqrtf
#include "platform.h"  

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

inline unsigned long pow2( unsigned long dim )
{
	unsigned long i = 1;

	while (i < dim) i <<= 1;

	return i;
}

inline unsigned long powof( unsigned long dim )
{
	unsigned long num = 1;
	unsigned long i = 0;

	while (num < dim)
	{
		num <<= 1;
		i++;
	}

	return i;
}

#endif