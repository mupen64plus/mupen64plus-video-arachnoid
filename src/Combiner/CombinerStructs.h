/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
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

#ifndef COMBINER_STRUCTS_H_
#define COMBINER_STRUCTS_H_

#include "m64p.h"
#include "OpenGL.h"

// Internal combiner commands
#define LOAD        0
#define SUB            1
#define MUL            2
#define ADD            3
#define INTERPOLATE 4

// Internal generalized combiner inputs

#define COMBINED        0
#define TEXEL0          1
#define TEXEL1          2
#define PRIMITIVE       3
#define SHADE           4
#define ENVIRONMENT     5
#define CENTER          6
#define SCALE           7
#define COMBINED_ALPHA  8
#define TEXEL0_ALPHA    9
#define TEXEL1_ALPHA    10
#define PRIMITIVE_ALPHA 11
#define SHADE_ALPHA     12
#define ENV_ALPHA       13
#define LOD_FRACTION    14
#define PRIM_LOD_FRAC   15
#define NOISE           16
#define K4              17
#define K5              18
#define CB_ONE          19
#define CB_ZERO         20

//* Combiner data
//! Defines how mux-values are coded.
//TODO: check this size against GCC when i'm awake
struct CombineData
{
    union
    {
        struct
        {
            // muxs1
            unsigned    aA1     : 3;
            unsigned    sbA1    : 3;
            unsigned    aRGB1   : 3;
            unsigned    aA0     : 3;
            unsigned    sbA0    : 3;
            unsigned    aRGB0   : 3;
            unsigned    mA1     : 3;
            unsigned    saA1    : 3;
            unsigned    sbRGB1  : 4;
            unsigned    sbRGB0  : 4;

            // muxs0
            unsigned    mRGB1   : 5;
            unsigned    saRGB1  : 4;
            unsigned    mA0     : 3;
            unsigned    saA0    : 3;
            unsigned    mRGB0   : 5;
            unsigned    saRGB0  : 4;
        };

        struct
        {
            unsigned int    muxs1, muxs0;
        };

        unsigned long long    mux;
    };
};

//! Combiner operation
struct CombinerOp
{
    int op;
    int param1;
    int param2;
    int param3;
};

//! Combiner Stage with combiner operations
struct CombinerStage
{
    int numOps;
    CombinerOp op[6];
};

//! Combiner with combiner stages
struct Combiner
{
    int numStages;
    CombinerStage stage[2];
};

//! Combiner cycle
struct CombineCycle
{
    int loadValue;  //!< Load
    int addValue;   //!< Addition
    int subValue;   //!< Subract
    int multValue;  //!< Multiplication
};


//*****************************************************************************
//! Texture Environment Combiner Argument
//! Difines a color source and mathimatical operation to be performed
//*****************************************************************************
struct TexEnvCombinerArg
{
    GLenum source;
    GLenum operand;
};

//*****************************************************************************
//! Texture Environment Combiner Stage
//*****************************************************************************
struct TexEnvCombinerStage
{
    unsigned short constant;
    bool used;
    GLenum combine;
    TexEnvCombinerArg arg0, arg1, arg2;
    unsigned short outputTexture;
};

//*****************************************************************************
//* Texture Environment Combiner 
//! Stores information how graphics API should combine colors and set 
//! Texture environment.
//*****************************************************************************
struct TexEnvCombiner
{
    bool usesT0, usesT1, usesNoise;

    GLint mode;

    unsigned short usedUnits;
    
    struct
    {
        unsigned short color, secondaryColor, alpha;
    } vertex;

    TexEnvCombinerStage color[8];
    TexEnvCombinerStage alpha[8];
};

#endif
