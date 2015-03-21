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

#include "CRCCalculator2.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "Memory.h"
#include "RDP.h"
#include "TextureCache.h"
#include "TextureLoader.h"
#include "assembler.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
TextureLoader::TextureLoader()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
TextureLoader::~TextureLoader()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointers to objects
//-----------------------------------------------------------------------------
bool TextureLoader::initialize(RDP* rdp, Memory* memory)
{
    m_rdp    = rdp;
    m_memory = memory;

    m_currentTile        = &m_tiles[7];
    return true;
}

//-----------------------------------------------------------------------------
//* Set Texture Image
//! Stores information about texture image.
//-----------------------------------------------------------------------------
void TextureLoader::setTextureImage(unsigned int format, unsigned int size, unsigned int width, unsigned int segmentAddress)
{
    m_textureImage.address = m_memory->getRDRAMAddress( segmentAddress );  
    m_textureImage.format  = format; 
    m_textureImage.size    = size;   
    m_textureImage.width   = width + 1;  //Note: add plus one, glN64 does it outside of function
    m_textureImage.bpl     = m_textureImage.width << m_textureImage.size >> 1;
}

//-----------------------------------------------------------------------------
//* Set Tile
//! Stores information about an rdp tile.
//-----------------------------------------------------------------------------
void TextureLoader::setTile( int format, int size, int line, int tmem, int tile, int palette, 
                             int clampS, int clampT, int mirrorS, int mirrorT, int maskS, 
                             int maskT, int shiftS, int shiftT )
{
    m_tiles[tile].format  = format;
    m_tiles[tile].size    = size;
    m_tiles[tile].line    = line;
    m_tiles[tile].tmem    = tmem;
    m_tiles[tile].palette = palette;
    m_tiles[tile].clamps  = clampS;
    m_tiles[tile].clampt  = clampT;
    m_tiles[tile].mirrors = mirrorS;
    m_tiles[tile].mirrort = mirrorT;
    m_tiles[tile].masks   = maskS;
    m_tiles[tile].maskt   = maskT;
    m_tiles[tile].shiftt  = shiftT;
    m_tiles[tile].shifts  = shiftS;
    
    if ( !m_tiles[tile].masks ) 
        m_tiles[tile].clamps = 1;
    if ( !m_tiles[tile].maskt ) 
        m_tiles[tile].clampt = 1;

    //
    if (((size == G_IM_SIZ_4b) || (size == G_IM_SIZ_8b)) && (format == G_IM_FMT_RGBA)) {
        m_tiles[tile].format = G_IM_FMT_CI;
    }
}

//-----------------------------------------------------------------------------
//* Set Tile Size
//! Stores size information for a rdp tile.
//-----------------------------------------------------------------------------
void TextureLoader::setTileSize(int tile, unsigned int s0, unsigned int t0, unsigned int s1, unsigned int t1)
{
    m_tiles[tile].uls = _SHIFTR( s0, 2, 10 );
    m_tiles[tile].ult = _SHIFTR( t0, 2, 10 );
    m_tiles[tile].lrs = _SHIFTR( s1, 2, 10 );
    m_tiles[tile].lrt = _SHIFTR( t1, 2, 10 );
    m_tiles[tile].fuls = _FIXED2FLOAT( s0, 2 );
    m_tiles[tile].fult = _FIXED2FLOAT( t0, 2 );
    m_tiles[tile].flrs = _FIXED2FLOAT( s1, 2 );
    m_tiles[tile].flrt = _FIXED2FLOAT( t1, 2 );
}

//-----------------------------------------------------------------------------
//* Load Tile (to texture memory)
//! Kopies texture data from RDRAM to Texture Memory
//-----------------------------------------------------------------------------
void TextureLoader::loadTile(int tile, int s0, int t0, int s1, int t1)
{
    void (*Interleave)( void *mem, unsigned int numDWords );


    //unsigned int tile   = _SHIFTR( ucode->w1, 24, 3 );

    unsigned int address, height, bpl, line, y;
    unsigned long long *dest;
    unsigned char *src;

    //Set new Tile Size
    this->setTileSize(tile, s0, t0, s1, t1);
    m_currentTile = &m_tiles[tile];

    if (m_currentTile->line == 0)
        return;

    address = m_textureImage.address + m_currentTile->ult * m_textureImage.bpl + (m_currentTile->uls << m_textureImage.size >> 1);
    dest = m_memory->getTextureMemory( m_currentTile->tmem );
    bpl = (m_currentTile->lrs - m_currentTile->uls + 1) << m_currentTile->size >> 1;
    height = m_currentTile->lrt - m_currentTile->ult + 1;
    src = m_memory->getRDRAM(address);

    if (((address + height * bpl) > m_memory->getRDRAMSize()) || (((m_currentTile->tmem << 3) + bpl * height) > 4096)) // Stay within TMEM
    {
        return;
    }

    // Line given for 32-bit is half what it seems it should since they split the
    // high and low words. I'm cheating by putting them together.
    if (m_currentTile->size == G_IM_SIZ_32b)
    {
        line = m_currentTile->line << 1;
        Interleave = QWordInterleave;
    }
    else
    {
        line = m_currentTile->line;
        Interleave = DWordInterleave;
    }

    for (y = 0; y < height; y++)
    {
        UnswapCopy( src, dest, bpl );
        if (y & 1) Interleave( dest, line );

        src += m_textureImage.bpl;
         dest += line;
    }
}

//-----------------------------------------------------------------------------
//* Load Block (to texture memory)
//! Kopies texture data from RDRAM to Texture Memory
//-----------------------------------------------------------------------------
void TextureLoader::loadBlock(int tile, int s0, int t0, int s1, int t1)
{
    unsigned int dxt = t1; 

    //Set new Tile Size
    this->setTileSize(tile, s0, t0, s1, t1);
    m_currentTile = &m_tiles[tile];


    unsigned int bytes = (s1 + 1) << m_currentTile->size >> 1;
    unsigned int address = m_textureImage.address + t0 * m_textureImage.bpl + (s0 << m_textureImage.size >> 1);

    if ((bytes == 0) || ((address + bytes) > m_memory->getRDRAMSize()) || (((m_currentTile->tmem << 3) + bytes) > 4096))
    {
        return;
    }

    unsigned long long* src = (unsigned long long*)m_memory->getRDRAM(address);
    unsigned long long* dest = m_memory->getTextureMemory(m_currentTile->tmem);

    unsigned int line = 0;

    if (dxt > 0)
    {
        void (*Interleave)( void *mem, unsigned int numDWords );

        line = (2047 + dxt) / dxt;
        unsigned int bpl = line << 3;
        unsigned int height = bytes / bpl;

        if (m_currentTile->size == G_IM_SIZ_32b)
            Interleave = QWordInterleave;
        else
            Interleave = DWordInterleave;

        for (unsigned int y = 0; y < height; y++)
        {
            UnswapCopy( src, dest, bpl );
            if (y & 1) Interleave( dest, line );

            src += line;
            dest += line;
        }
    }
    else
        UnswapCopy( src, dest, bytes );
}

//-----------------------------------------------------------------------------
//* Load Texture Look up table (to texture memory)
//! Kopies texture data from RDRAM to Texture Memory
//-----------------------------------------------------------------------------
void TextureLoader::loadTLUT(int tile, int s0, int t0, int s1, int t1)
{
    CRCCalculator2 crcCalculator;

    //Set new Tile Size
    this->setTileSize(tile, s0, t0, s1, t1);


    unsigned short count   = (m_tiles[tile].lrs - m_tiles[tile].uls + 1) * (m_tiles[tile].lrt - m_tiles[tile].ult + 1);
    unsigned int  address = m_textureImage.address + m_tiles[tile].ult * m_textureImage.bpl + (m_tiles[tile].uls << m_textureImage.size >> 1);

    //Copy from rdram to texture memory
    unsigned short *src = (unsigned short*)m_memory->getRDRAM(address);
    unsigned short *dest = (unsigned short*)m_memory->getTextureMemory(m_tiles[tile].tmem);     

    unsigned short pal = (m_tiles[tile].tmem - 256) >> 4;

    int i = 0;
    while (i < count)
    {
        for (unsigned short j = 0; (j < 16) && (i < count); j++, i++)
        {
            unsigned short color = swapword( src[i^1] );
            *dest = color;
            dest += 4;
        }
        

        m_rdp->m_paletteCRC16[pal] = crcCalculator.calcPaletteCRC(0xFFFFFFFF, m_memory->getTextureMemory(256 + (pal << 4)), 16);
        pal++;
    }

    m_rdp->m_paletteCRC256 = crcCalculator.calcCRC(0xFFFFFFFF, m_rdp->m_paletteCRC16, 64);
}
