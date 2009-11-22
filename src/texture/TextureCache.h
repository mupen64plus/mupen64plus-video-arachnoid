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

#ifndef TEXTURE_CACHE_H_
#define TEXTURE_CACHE_H_

#include "CachedTexture.h"
#include "CRCCalculator2.h"
#include "ImageFormatSelector.h"
#include <list>

//Forward declarations
class Memory;
class RSP;
class RDP;

//*****************************************************************************
//* Texture Cache
//! Class used to activate textures and store used textures for reuse.
//! @details Nintendo64 has a texture cache of 4 KiB
//*****************************************************************************
class TextureCache
{
public:

    //Constructor / Destructor
    TextureCache();
    ~TextureCache();

    //Functions
    bool initialize(RSP* rsp, RDP* rdp, Memory* memory, unsigned int textureBitDepth, unsigned int cacheSize=(32 * 1048576));
    void update(unsigned int tile);
    void dispose();

    //Add and Remove
    CachedTexture* addTop();
    void removeBottom();
    void remove( CachedTexture *texture );

    //Move Texture to top
    void moveToTop( CachedTexture *newtop );

    //Get Current Texture
    CachedTexture* getCurrentTexture(int index) { return m_currentTextures[index]; }
    
private:

    void _loadTexture(CachedTexture* texture);
    void _calculateTextureSize(unsigned int tile, CachedTexture* out, unsigned int& maskWidth, unsigned int& maskHeight);
    void _activateTexture( unsigned int t, CachedTexture *texture );
    unsigned int _calculateCRC(unsigned int t, unsigned int width, unsigned int height);

private:
//public:

    RSP*    m_rsp;                         //!< Pointer to Reality Signal Processor 
    RDP*    m_rdp;                         //!< Pointer to Reality Drawing Processor 
    Memory* m_memory;                      //!< Pointer to Memory manager (handles RDRAM, Texture Memory...)

    ImageFormatSelector m_formatSelector;  //!< Image Format Selector used when decoding textures
    CRCCalculator2      m_crcCalculator;   //!< Hash value calculator for textures

    unsigned int m_maxBytes;              //!< Maximum number of bytes this cache have
    unsigned int m_cachedBytes;           //!< Current number of bytes in cache
    unsigned int m_bitDepth;              //!<

    //Cached textures
    typedef std::list<CachedTexture*> TextureList;
    TextureList m_cachedTextures;          //!< List of cached textures

    //Pointers to current textures
    CachedTexture* m_currentTextures[2];   //!< Two textures for multi-texturing.
    
};

#endif
