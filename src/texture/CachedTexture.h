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

#ifndef CACHED_TEXTURE_H_
#define CACHED_TEXTURE_H_

//*****************************************************************************
//* Cached Texture
//! Struct used by Texture Cache to store used textures.
//*****************************************************************************
class CachedTexture
{
public:

    //Constructor / Destrucotr
    CachedTexture();
    ~CachedTexture();

    //Activate / Deactivate
    void activate();
    void deactivate();

    //Get texture size
    unsigned int getTextureSize() { return m_textureSize; }

    //Assignment operator
    CachedTexture& operator = (const CachedTexture& v);

    //Equal operator
    bool operator == (const CachedTexture& t) const;

public:

    unsigned int  m_id;                      //!< id used by OpenGL to identify texture
    unsigned int  m_textureSize;             //!< Size of texture in bytes

    unsigned int  address;
    unsigned int  crc;                       //!< A CRC "checksum" (Cyclic redundancy check)
//    float          fulS, fulT;
//    unsigned short ulS, ulT, lrS, lrT;
    float         offsetS, offsetT;
    unsigned int  maskS, maskT;
    unsigned int  clampS, clampT;
    unsigned int  mirrorS, mirrorT;
    unsigned int  line;
    unsigned int  size;
    unsigned int  format;                    //!< Texture format
    unsigned int  tMem;
    unsigned int  palette;                   //!< What Texture Look Up Table to use
    unsigned int  width, height;             //!< N64 width and height
    unsigned int  clampWidth, clampHeight;   //!< Size to clamp to
    unsigned int  realWidth, realHeight;     //!< Actual texture size
    float         scaleS, scaleT;            //!< Scale to map to 0.0-1.0
    float         shiftScaleS, shiftScaleT;  //!< Scale to shift
//    unsigned int lastDList;
//    unsigned int frameBufferTexture;

};

#endif
