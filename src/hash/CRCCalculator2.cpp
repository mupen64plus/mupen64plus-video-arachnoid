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

#include "CRCCalculator2.h"

#define CRC32_POLYNOMIAL    0xedb88320    //0x04C11DB7
typedef unsigned char byte;

//-----------------------------------------------------------------------------
// Static Variabels
//-----------------------------------------------------------------------------
unsigned int CRCCalculator2::m_crcTable[256] = {0};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
CRCCalculator2::CRCCalculator2()
{
    static bool hashTableInitialized = false;

    //Build hash table
    //http://www.gamedev.net/reference/articles/article1941.asp
    if ( !hashTableInitialized )
    {
        unsigned int crc;

        //For each value 
        for (int i=0; i<256; i++)
        {
            crc = _reflect( i, 8 ) << 24;

            for (int j = 0; j < 8; j++) 
            {
                crc = (crc << 1) ^ (crc & (1 << 31) ? CRC32_POLYNOMIAL : 0);
            }
            
            m_crcTable[i] = _reflect( crc, 32 );
        }

        hashTableInitialized = true;
    }
}

//-----------------------------------------------------------------------------
// CalculateCRC
//-----------------------------------------------------------------------------
unsigned int CRCCalculator2::calcCRC(unsigned int crc, void *buffer, unsigned int count)
{
    byte* p = (byte*) buffer; 
    unsigned int orig = crc;

    p = (byte*) buffer;
    while (count--) 
    {
        crc = (crc >> 8) ^ m_crcTable[(crc & 0xFF) ^ *p++];
    }

    return crc ^ orig;
}

//-----------------------------------------------------------------------------
// CalculatePaletteCRC
//-----------------------------------------------------------------------------
unsigned int CRCCalculator2::calcPaletteCRC(unsigned int crc, void *buffer, unsigned int count)
{
    byte *p;
    unsigned int orig = crc;

    p = (byte*) buffer;
    while (count--)
    {
        crc = (crc >> 8) ^ m_crcTable[(crc & 0xFF) ^ *p++];
        crc = (crc >> 8) ^ m_crcTable[(crc & 0xFF) ^ *p++];

        p += 6;
    }

    return crc ^ orig;
}

//*****************************************************************************
// Private Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Reflect
//! Help function when creating the CRC Table
//! Swaps bit 0 for bit 7
//!       bit 1 for bit 6
//!       bit 2 for bit 5 ...
//-----------------------------------------------------------------------------
unsigned int CRCCalculator2::_reflect(unsigned int ref, char ch)
{
    unsigned int value = 0;
    for (int i=1; i<(ch + 1); ++i)
    {
        if(ref & 1) 
        {
            value |= 1 << (ch - i);
        }
        ref >>= 1;
    }
    return value;
}
