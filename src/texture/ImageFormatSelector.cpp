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

//*****************************************************************************
// 
// NOTE THAT THIS FILE IS BASED ON MATERIAL FROM glN64.
// http://gln64.emulation64.com/
//
//*****************************************************************************

#include "ImageFormatSelector.h"
#include "CachedTexture.h"
#include "assembler.h"
#include "GBIDefs.h"
#include "m64p.h"
#include "OpenGL.h"
#include "Memory.h"

#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels                 1
    #define GL_UNSIGNED_BYTE_3_3_2_EXT           0x8032
    #define GL_UNSIGNED_SHORT_4_4_4_4_EXT        0x8033
    #define GL_UNSIGNED_SHORT_5_5_5_1_EXT        0x8034
    #define GL_UNSIGNED_INT_8_8_8_8_EXT          0x8035
    #define GL_UNSIGNED_INT_10_10_10_2_EXT       0x8036
#endif /* GL_EXT_packed_pixels */

unsigned long long* TMEM = Memory::getTextureMemory();


inline unsigned int GetNone( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return 0x00000000;
}

inline unsigned int GetCI4IA_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    if (x & 1)
        return IA88_RGBA4444( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B & 0x0F)] );
    else
        return IA88_RGBA4444( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B >> 4)] );
}

inline unsigned int GetCI4IA_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    if (x & 1)
        return IA88_RGBA8888( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B & 0x0F)] );
    else
        return IA88_RGBA8888( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B >> 4)] );
}

inline unsigned int GetCI4RGBA_RGBA5551( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    if (x & 1)
        return RGBA5551_RGBA5551( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B & 0x0F)] );
    else
        return RGBA5551_RGBA5551( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B >> 4)] );
}

inline unsigned int GetCI4RGBA_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    if (x & 1)
        return RGBA5551_RGBA8888( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B & 0x0F)] );
    else
        return RGBA5551_RGBA8888( *(unsigned short*)&TMEM[256 + (palette << 4) + (color4B >> 4)] );
}

inline unsigned int GetIA31_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    return IA31_RGBA8888( (x & 1) ? (color4B & 0x0F) : (color4B >> 4) );
}

inline unsigned int GetIA31_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    return IA31_RGBA4444( (x & 1) ? (color4B & 0x0F) : (color4B >> 4) );
}

inline unsigned int GetI4_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    return I4_RGBA8888( (x & 1) ? (color4B & 0x0F) : (color4B >> 4) );
}

inline unsigned int GetI4_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    unsigned char color4B;

    color4B = ((unsigned char*)src)[(x>>1)^(i<<1)];

    return I4_RGBA4444( (x & 1) ? (color4B & 0x0F) : (color4B >> 4) );
}

inline unsigned int GetCI8IA_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA88_RGBA4444( *(unsigned short*)&TMEM[256 + ((unsigned char*)src)[x^(i<<1)]] );
}

inline unsigned int GetCI8IA_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA88_RGBA8888( *(unsigned short*)&TMEM[256 + ((unsigned char*)src)[x^(i<<1)]] );
}

inline unsigned int GetCI8RGBA_RGBA5551( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return RGBA5551_RGBA5551( *(unsigned short*)&TMEM[256 + ((unsigned char*)src)[x^(i<<1)]] );
}

inline unsigned int GetCI8RGBA_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return RGBA5551_RGBA8888( *(unsigned short*)&TMEM[256 + ((unsigned char*)src)[x^(i<<1)]] );
}

inline unsigned int GetIA44_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA44_RGBA8888(((unsigned char*)src)[x^(i<<1)]);
}

inline unsigned int GetIA44_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA44_RGBA4444(((unsigned char*)src)[x^(i<<1)]);
}

inline unsigned int GetI8_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return I8_RGBA8888(((unsigned char*)src)[x^(i<<1)]);
}

inline unsigned int GetI8_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return I8_RGBA4444(((unsigned char*)src)[x^(i<<1)]);
}

inline unsigned int GetRGBA5551_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return RGBA5551_RGBA8888( ((unsigned short*)src)[x^i] );
}

inline unsigned int GetRGBA5551_RGBA5551( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return RGBA5551_RGBA5551( ((unsigned short*)src)[x^i] );
}

inline unsigned int GetIA88_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA88_RGBA8888(((unsigned short*)src)[x^i]);
}

inline unsigned int GetIA88_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return IA88_RGBA4444(((unsigned short*)src)[x^i]);
}

inline unsigned int GetRGBA8888_RGBA8888( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return ((unsigned int*)src)[x^i];
}

inline unsigned int GetRGBA8888_RGBA4444( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette )
{
    return RGBA8888_RGBA4444(((unsigned int*)src)[x^i]);
}

/*
const struct
{
    GetTexelFunc    Get16;
    unsigned int    glType16;
    int              glInternalFormat16;
    GetTexelFunc    Get32;
    unsigned int     glType32;
    int                glInternalFormat32;
    unsigned int   autoFormat, lineShift, maxTexels;
} 
*/
ImageFormat ImageFormatSelector::imageFormats[4][5]  =
{ //        Get16                    glType16                        glInternalFormat16    Get32                   glType32             glInternalFormat32   autoFormat
    { // 4-bit
        {    GetCI4RGBA_RGBA5551,    GL_UNSIGNED_SHORT_5_5_5_1_EXT,  GL_RGB5_A1,           GetCI4RGBA_RGBA8888,    GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGB5_A1, 4, 4096 }, // CI (Banjo-Kazooie uses this, doesn't make sense, but it works...)
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 4, 8192 }, // YUV
        {    GetCI4RGBA_RGBA5551,    GL_UNSIGNED_SHORT_5_5_5_1_EXT,  GL_RGB5_A1,           GetCI4RGBA_RGBA8888,    GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGB5_A1, 4, 4096 }, // CI
        {    GetIA31_RGBA4444,       GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetIA31_RGBA8888,       GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 4, 8192 }, // IA
        {    GetI4_RGBA4444,         GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetI4_RGBA8888,         GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 4, 8192 }, // I
    },
    { // 8-bit
        {    GetCI8RGBA_RGBA5551,    GL_UNSIGNED_SHORT_5_5_5_1_EXT,  GL_RGB5_A1,           GetCI8RGBA_RGBA8888,    GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGB5_A1, 3, 2048 }, // RGBA
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 4096 }, // YUV
        {    GetCI8RGBA_RGBA5551,    GL_UNSIGNED_SHORT_5_5_5_1_EXT,  GL_RGB5_A1,           GetCI8RGBA_RGBA8888,    GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGB5_A1, 3, 2048 }, // CI
        {    GetIA44_RGBA4444,       GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetIA44_RGBA8888,       GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 3, 4096 }, // IA
        {    GetI8_RGBA4444,         GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetI8_RGBA8888,         GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA8, 3, 4096 }, // I
    },
    { // 16-bit
        {    GetRGBA5551_RGBA5551,   GL_UNSIGNED_SHORT_5_5_5_1_EXT,  GL_RGB5_A1,           GetRGBA5551_RGBA8888,   GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGB5_A1, 2, 2048 }, // RGBA
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 2, 2048 }, // YUV
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 2048 }, // CI
        {    GetIA88_RGBA4444,       GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetIA88_RGBA8888,       GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA8, 2, 2048 }, // IA
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 2048 }, // I
    },
    { // 32-bit
        {    GetRGBA8888_RGBA4444,   GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetRGBA8888_RGBA8888,   GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA8, 2, 1024 }, // RGBA
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 1024 }, // YUV
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 1024 }, // CI
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 1024 }, // IA
        {    GetNone,                GL_UNSIGNED_SHORT_4_4_4_4_EXT,  GL_RGBA4,             GetNone,                GL_UNSIGNED_BYTE,    GL_RGBA8,            GL_RGBA4, 0, 1024 }, // I
    }
};


//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
ImageFormatSelector::ImageFormatSelector()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
ImageFormatSelector::~ImageFormatSelector()
{

}

//-----------------------------------------------------------------------------
// Detect Image Format
//-----------------------------------------------------------------------------
void ImageFormatSelector::detectImageFormat(CachedTexture* texture, unsigned int textureBitDepth, GetTexelFunc& getTexelFunc, unsigned int& internalFormat, int& imageType, unsigned int textureLUT)
{
    if (((imageFormats[texture->size][texture->format].autoFormat == GL_RGBA8) || 
        ((texture->format == G_IM_FMT_CI) && (textureLUT == G_TT_IA16)) || 
        (textureBitDepth == 2)) && (textureBitDepth != 0))
    {
         texture->m_textureSize = (texture->realWidth * texture->realHeight) << 2;
        if ((texture->format == G_IM_FMT_CI) && (textureLUT == G_TT_IA16))
        {
            if (texture->size == G_IM_SIZ_4b)
                getTexelFunc = GetCI4IA_RGBA8888;
            else
                getTexelFunc = GetCI8IA_RGBA8888;

            internalFormat = GL_RGBA8;
            imageType = GL_UNSIGNED_BYTE;
        }
        else
        {
            getTexelFunc = imageFormats[texture->size][texture->format].Get32;
            internalFormat = imageFormats[texture->size][texture->format].glInternalFormat32;
            imageType = imageFormats[texture->size][texture->format].glType32;
        }
    }
    else
    {
        texture->m_textureSize = (texture->realWidth * texture->realHeight) << 1;
        if ((texture->format == G_IM_FMT_CI) && (textureLUT == G_TT_IA16))
        {
            if (texture->size == G_IM_SIZ_4b)
                getTexelFunc = GetCI4IA_RGBA4444;
            else
                getTexelFunc = GetCI8IA_RGBA4444;

            internalFormat = GL_RGBA4;
            imageType = GL_UNSIGNED_SHORT_4_4_4_4_EXT;
        }
        else
        {
            getTexelFunc = imageFormats[texture->size][texture->format].Get16;
            internalFormat = imageFormats[texture->size][texture->format].glInternalFormat16;
            imageType = imageFormats[texture->size][texture->format].glType16;
        }
    }
}
