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

#include "CachedTexture.h"
#include "OpenGL.h"
#include "m64p.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
CachedTexture::CachedTexture()
{
    m_id = 0;                //!< id used by OpenGL to identify texture
    m_textureSize = 0;       //!< Size of texture in bytes
    address = 0;
    crc  = 0;
    offsetS = offsetT  = 0;
    maskS = maskT  = 0;
    clampS = clampT  = 0;
    mirrorS = mirrorT  = 0;
    line = 0;
    size = 0;
    format = 0;                    //!< Texture format
    tMem = 0;
    palette = 0;                   //!< What Texture Look Up Table to use
    width  = height = 0;           // N64 width and height
    clampWidth = clampHeight = 0;  // Size to clamp to
    realWidth = realHeight = 0;    // Actual texture size
    scaleS = scaleT  = 0;          // Scale to map to 0.0-1.0
    shiftScaleS = shiftScaleT = 0; // Scale to shift
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
CachedTexture::~CachedTexture()
{

}

//-----------------------------------------------------------------------------
//! Activate texture
//-----------------------------------------------------------------------------
void CachedTexture::activate()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, m_id );
}

//-----------------------------------------------------------------------------
//! Deactivate texture
//-----------------------------------------------------------------------------
void CachedTexture::deactivate()
{
    glDisable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, 0 );    
}

//-----------------------------------------------------------------------------
//! Equal operator
//-----------------------------------------------------------------------------
bool CachedTexture::operator == (const CachedTexture& t) const
{
    return( crc         == t.crc &&
            width       == t.width &&
            height      == t.height &&
            clampWidth  == t.clampWidth &&
            clampHeight == t.clampHeight &&
            maskS       == t.maskS &&
            maskT       == t.maskT &&
            mirrorS     == t.mirrorS &&
            mirrorT     == t.mirrorT &&
            clampS      == t.clampS &&
            clampT      == t.clampT &&
            format      == t.format /* &&
            size        == t.size */ );
}

//-----------------------------------------------------------------------------
//! Assign operator
//-----------------------------------------------------------------------------
CachedTexture& CachedTexture::operator = (const CachedTexture& v)
{     
    address = v.address;
    crc     = v.crc;
    format  = v.format;
    size    = v.size;
    width   = v.width;
    height  = v.height;
    clampWidth  = v.clampWidth;
    clampHeight = v.clampHeight;
    palette = v.palette;
/*    cache.current[tile]->fulS = gSP.textureTile[tile]->fulS;
    cache.current[tile]->fulT = gSP.textureTile[tile]->fulT;
    cache.current[tile]->ulS = gSP.textureTile[tile]->ulS;
    cache.current[tile]->ulT = gSP.textureTile[tile]->ulT;
    cache.current[tile]->lrS = gSP.textureTile[tile]->lrS;
    cache.current[tile]->lrT = gSP.textureTile[tile]->lrT;*/
    maskS = v.maskS;
    maskT = v.maskT;
    mirrorS = v.mirrorS;
    mirrorT = v.mirrorT;
    clampS = v.clampS;
    clampT = v.clampT;
    line = v.line;
    tMem = v.tMem;
    //lastDList = RSP.DList;
    //frameBufferTexture = FALSE;
    return *this;     
}
