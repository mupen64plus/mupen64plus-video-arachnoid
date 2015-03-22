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

#ifndef GBI_DEFINITIONS_H_
#define GBI_DEFINITIONS_H_

#include "GBI.h"


//TODO Write comments for each group

#define G_NOOP                    0x00

// These GBI commands are common to all ucodes

#define    G_SETCIMG                0xFF    /*  -1 */
#define    G_SETZIMG                0xFE    /*  -2 */
#define    G_SETTIMG                0xFD    /*  -3 */
#define    G_SETCOMBINE             0xFC    /*  -4 */
#define    G_SETENVCOLOR            0xFB    /*  -5 */
#define    G_SETPRIMCOLOR           0xFA    /*  -6 */
#define    G_SETBLENDCOLOR          0xF9    /*  -7 */
#define    G_SETFOGCOLOR            0xF8    /*  -8 */
#define    G_SETFILLCOLOR           0xF7    /*  -9 */
#define    G_FILLRECT               0xF6    /* -10 */
#define    G_SETTILE                0xF5    /* -11 */
#define    G_LOADTILE               0xF4    /* -12 */
#define    G_LOADBLOCK              0xF3    /* -13 */
#define    G_SETTILESIZE            0xF2    /* -14 */
#define    G_LOADTLUT               0xF0    /* -16 */
#define    G_RDPSETOTHERMODE        0xEF    /* -17 */
#define    G_SETPRIMDEPTH           0xEE    /* -18 */
#define    G_SETSCISSOR             0xED    /* -19 */
#define    G_SETCONVERT             0xEC    /* -20 */
#define    G_SETKEYR                0xEB    /* -21 */
#define    G_SETKEYGB               0xEA    /* -22 */
#define    G_RDPFULLSYNC            0xE9    /* -23 */
#define    G_RDPTILESYNC            0xE8    /* -24 */
#define    G_RDPPIPESYNC            0xE7    /* -25 */
#define    G_RDPLOADSYNC            0xE6    /* -26 */
#define G_TEXRECTFLIP               0xE5    /* -27 */
#define G_TEXRECT                   0xE4    /* -28 */

#define G_TRI_FILL                  0xC8    /* fill triangle:            11001000 */
#define G_TRI_SHADE                 0xCC    /* shade triangle:           11001100 */
#define G_TRI_TXTR                  0xCA    /* texture triangle:         11001010 */
#define G_TRI_SHADE_TXTR            0xCE    /* shade, texture triangle:  11001110 */
#define G_TRI_FILL_ZBUFF            0xC9    /* fill, zbuff triangle:     11001001 */
#define G_TRI_SHADE_ZBUFF           0xCD    /* shade, zbuff triangle:    11001101 */
#define G_TRI_TXTR_ZBUFF            0xCB    /* texture, zbuff triangle:  11001011 */
#define G_TRI_SHADE_TXTR_ZBUFF      0xCF    /* shade, txtr, zbuff trngl: 11001111 */

#define G_MV_MMTX           2    
#define G_MV_PMTX           6
#define G_MV_LIGHT          10
#define G_MV_POINT          12
#define G_MV_MATRIX         14

#define G_MW_MATRIX         0x00
#define G_MW_NUMLIGHT       0x02
#define G_MW_CLIP           0x04
#define G_MW_SEGMENT        0x06
#define G_MW_FOG            0x08
#define G_MW_LIGHTCOL       0x0A
#define G_MW_FORCEMTX       0x0C
#define G_MW_POINTS         0x0C
#define    G_MW_PERSPNORM   0x0E

#define G_MWO_POINT_RGBA        0x10
#define G_MWO_POINT_ST          0x14
#define G_MWO_POINT_XYSCREEN    0x18
#define G_MWO_POINT_ZSCREEN     0x1C

//Move Memory Defines
#define G_MV_LOOKATY       0x82
#define G_MV_LOOKATX       0x84
#define G_MV_L0            0x86
#define G_MV_L1            0x88
#define G_MV_L2            0x8a
#define G_MV_L3            0x8c
#define G_MV_L4            0x8e
#define G_MV_L5            0x90
#define G_MV_L6            0x92
#define G_MV_L7            0x94
#define G_MV_TXTATT        0x96
#define G_MV_MATRIX_1      0x9E
#define G_MV_MATRIX_2      0x98
#define G_MV_MATRIX_3      0x9A
#define G_MV_MATRIX_4      0x9C



// Image formats
#define G_IM_FMT_RGBA   0
#define G_IM_FMT_YUV    1
#define G_IM_FMT_CI     2
#define G_IM_FMT_IA     3
#define G_IM_FMT_I      4

// Image sizes
#define G_IM_SIZ_4b     0
#define G_IM_SIZ_8b     1
#define G_IM_SIZ_16b    2
#define G_IM_SIZ_32b    3
#define G_IM_SIZ_DD     5

#define G_TX_MIRROR     0x1
#define G_TX_CLAMP      0x2



//
#define G_DL_PUSH      0x00
#define G_DL_NOPUSH    0x01

enum TextureMode
{
    TM_NORMAL      = 0,
    TM_TEXRECT     = 1,
    TM_BGIMAGE     = 2,
    TM_FRAMEBUFFER = 3,
};


enum LoadType
{
    LOADTYPE_BLOCK = 0,
    LOADTYPE_TILE  = 1,
};

/*
 * G_SETOTHERMODE_L sft: shift count
 */
#define    G_MDSFT_ALPHACOMPARE     0
#define    G_MDSFT_ZSRCSEL          2
#define    G_MDSFT_RENDERMODE       3
#define    G_MDSFT_BLENDER          16

/*
 * G_SETOTHERMODE_H sft: shift count
 */
#define    G_MDSFT_BLENDMASK        0    /* unsupported */
#define    G_MDSFT_ALPHADITHER      4
#define    G_MDSFT_RGBDITHER        6

#define    G_MDSFT_COMBKEY          8
#define    G_MDSFT_TEXTCONV         9
#define    G_MDSFT_TEXTFILT        12
#define    G_MDSFT_TEXTLUT         14
#define    G_MDSFT_TEXTLOD         16
#define    G_MDSFT_TEXTDETAIL      17
#define    G_MDSFT_TEXTPERSP       19
#define    G_MDSFT_CYCLETYPE       20
#define    G_MDSFT_COLORDITHER     22    /* unsupported in HW 2.0 */
#define    G_MDSFT_PIPELINE        23

/* G_SETOTHERMODE_H gPipelineMode */
#define    G_PM_1PRIMITIVE            1
#define    G_PM_NPRIMITIVE            0

//G_SETOTHERMODE_H gSetCycleType 
enum G_CYCLE_TYPE {
    G_CYC_1CYCLE = 0,
    G_CYC_2CYCLE = 1,
    G_CYC_COPY   = 2,
    G_CYC_FILL   = 3,  
};

/* G_SETOTHERMODE_H gSetTexturePersp */
#define G_TP_NONE                0
#define G_TP_PERSP               1

/* G_SETOTHERMODE_H gSetTextureDetail */
#define G_TD_CLAMP               0
#define G_TD_SHARPEN             1
#define G_TD_DETAIL              2

/* G_SETOTHERMODE_H gSetTextureLOD */
#define G_TL_TILE                0
#define G_TL_LOD                 1

/* G_SETOTHERMODE_H gSetTextureLUT */
#define G_TT_NONE                0
#define G_TT_RGBA16              2
#define G_TT_IA16                3

/* G_SETOTHERMODE_H gSetTextureFilter */
#define G_TF_POINT               0
#define G_TF_AVERAGE             3
#define G_TF_BILERP              2

/* G_SETOTHERMODE_H gSetTextureConvert */
#define G_TC_CONV                0
#define G_TC_FILTCONV            5
#define G_TC_FILT                6

/* G_SETOTHERMODE_H gSetCombineKey */
#define G_CK_NONE                0
#define G_CK_KEY                 1

/* G_SETOTHERMODE_H gSetColorDither */
#define    G_CD_MAGICSQ          0
#define    G_CD_BAYER            1
#define    G_CD_NOISE            2

#define    G_CD_DISABLE          3
#define    G_CD_ENABLE           G_CD_NOISE    /* HW 1.0 compatibility mode */

/* G_SETOTHERMODE_H gSetAlphaDither */
#define    G_AD_PATTERN          0
#define    G_AD_NOTPATTERN       1
#define    G_AD_NOISE            2
#define    G_AD_DISABLE          3

/* G_SETOTHERMODE_L gSetAlphaCompare */
#define    G_AC_NONE             0
#define    G_AC_THRESHOLD        1
#define    G_AC_DITHER           3

/* G_SETOTHERMODE_L gSetDepthSource */
#define    G_ZS_PIXEL            0
#define    G_ZS_PRIM             1

/* G_SETOTHERMODE_L gSetRenderMode */
#define    AA_EN                 1
#define    Z_CMP                 1
#define    Z_UPD                 1
#define    IM_RD                 1
#define    CLR_ON_CVG            1
#define    CVG_DST_CLAMP         0
#define    CVG_DST_WRAP          1
#define    CVG_DST_FULL          2
#define    CVG_DST_SAVE          3
#define    ZMODE_OPA             0
#define    ZMODE_INTER           1
#define    ZMODE_XLU             2
#define    ZMODE_DEC             3
#define    CVG_X_ALPHA           1
#define    ALPHA_CVG_SEL         1
#define    FORCE_BL              1
#define    TEX_EDGE              0 // not used

#define    G_SC_NON_INTERLACE    0
#define    G_SC_EVEN_INTERLACE   2
#define    G_SC_ODD_INTERLACE    3


#define GBI_InitFlags( ucode ) \
    GBI::G_MTX_STACKSIZE        = ucode##_MTX_STACKSIZE; \
    GBI::G_MTX_MODELVIEW        = ucode##_MTX_MODELVIEW; \
    GBI::G_MTX_PROJECTION       = ucode##_MTX_PROJECTION; \
    GBI::G_MTX_MUL              = ucode##_MTX_MUL; \
    GBI::G_MTX_LOAD             = ucode##_MTX_LOAD; \
    GBI::G_MTX_NOPUSH           = ucode##_MTX_NOPUSH; \
    GBI::G_MTX_PUSH             = ucode##_MTX_PUSH; \
\
    GBI::G_TEXTURE_ENABLE       = ucode##_TEXTURE_ENABLE; \
    GBI::G_SHADING_SMOOTH       = ucode##_SHADING_SMOOTH; \
    GBI::G_CULL_FRONT           = ucode##_CULL_FRONT; \
    GBI::G_CULL_BACK            = ucode##_CULL_BACK; \
    GBI::G_CULL_BOTH            = ucode##_CULL_BOTH; \
    GBI::G_CLIPPING             = ucode##_CLIPPING; \
\
    GBI::G_MV_VIEWPORT          = ucode##_MV_VIEWPORT; \
    /* \
    GBI::G_MWO_aLIGHT_1        = ucode##_MWO_aLIGHT_1; \
    GBI::G_MWO_bLIGHT_1        = ucode##_MWO_bLIGHT_1; \
    GBI::G_MWO_aLIGHT_2        = ucode##_MWO_aLIGHT_2; \
    GBI::G_MWO_bLIGHT_2        = ucode##_MWO_bLIGHT_2; \
    GBI::G_MWO_aLIGHT_3        = ucode##_MWO_aLIGHT_3; \
    GBI::G_MWO_bLIGHT_3        = ucode##_MWO_bLIGHT_3; \
    GBI::G_MWO_aLIGHT_4        = ucode##_MWO_aLIGHT_4; \
    GBI::G_MWO_bLIGHT_4        = ucode##_MWO_bLIGHT_4; \
    GBI::G_MWO_aLIGHT_5        = ucode##_MWO_aLIGHT_5; \
    GBI::G_MWO_bLIGHT_5        = ucode##_MWO_bLIGHT_5; \
    GBI::G_MWO_aLIGHT_6        = ucode##_MWO_aLIGHT_6; \
    GBI::G_MWO_bLIGHT_6        = ucode##_MWO_bLIGHT_6; \
    GBI::G_MWO_aLIGHT_7        = ucode##_MWO_aLIGHT_7; \
    GBI::G_MWO_bLIGHT_7        = ucode##_MWO_bLIGHT_7; \
    GBI::G_MWO_aLIGHT_8        = ucode##_MWO_aLIGHT_8; \
    GBI::G_MWO_bLIGHT_8        = ucode##_MWO_bLIGHT_8; \
    */


// Fixed point conversion factors
#define FIXED2FLOATRECIP1    0.5f
#define FIXED2FLOATRECIP2    0.25f
#define FIXED2FLOATRECIP3    0.125f
#define FIXED2FLOATRECIP4    0.0625f
#define FIXED2FLOATRECIP5    0.03125f
#define FIXED2FLOATRECIP6    0.015625f
#define FIXED2FLOATRECIP7    0.0078125f
#define FIXED2FLOATRECIP8    0.00390625f
#define FIXED2FLOATRECIP9    0.001953125f
#define FIXED2FLOATRECIP10   0.0009765625f
#define FIXED2FLOATRECIP11   0.00048828125f
#define FIXED2FLOATRECIP12   0.00024414063f
#define FIXED2FLOATRECIP13   0.00012207031f
#define FIXED2FLOATRECIP14   6.1035156e-05f
#define FIXED2FLOATRECIP15   3.0517578e-05f
#define FIXED2FLOATRECIP16   1.5258789e-05f

#define _FIXED2FLOAT( v, b ) ((float)v * FIXED2FLOATRECIP##b)

#endif
