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

#include "CRCCalculator.h"

#define CRC32_POLYNOMIAL    0xedb88320L    //0x04C11DB7
typedef unsigned char byte;

//-----------------------------------------------------------------------------
// Static Variabels
//-----------------------------------------------------------------------------
unsigned int CRCCalculator::m_crcTable[256] = {0};   

static unsigned int crc_table[256];

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
CRCCalculator::CRCCalculator()
{
    static bool hashTableInitialized = false;

    //Build hash table
    //http://www.gamedev.net/reference/articles/article1941.asp
    if ( !hashTableInitialized )
    {
     //   unsigned int crc;

     //   //For each value 
     //   for (int i=0; i<256; i++)
        //{
     //       crc = _reflect( i, 8 ) << 24;

     //       for (int j = 0; j < 8; j++) 
     //       {
              //  crc = (crc << 1) ^ (crc & (1 << 31) ? CRC32_POLYNOMIAL : 0);
     //       }
     //       
     //       m_crcTable[i] = _reflect( crc, 32 );
     //   }

        unsigned int crc;
        unsigned int poly;  // polynomial exclusive-or pattern 
        // terms of polynomial defining this crc (except x^32): 
        static const unsigned char p[] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};

        /* make exclusive-or pattern from polynomial (0xedb88320L) */
        poly = 0L;
        for (int n = 0; n < sizeof(p)/sizeof(unsigned char); n++)
        {
            poly |= 1L << (31 - p[n]);
        }
         
        for (int n=0; n<256; ++n)
        {
            crc = (unsigned int)n;

            for (int k = 0; k < 8; k++)
            {
                crc = (crc & 1) ? (poly ^ (crc >> 1)) : crc >> 1;
            }
            crc_table[n] = crc;
        }

        hashTableInitialized = true;
    }
}

//-----------------------------------------------------------------------------
// CalculateCRC
//-----------------------------------------------------------------------------
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO1b(buf) crc = (crc >> 8) ^ crc_table[(crc & 0xFF) ^ *buf++]
unsigned int CRCCalculator::calcCRC(unsigned int crc, void *buffer, unsigned int count)
{
    byte* p = (byte*) buffer; 
    //unsigned int orig = crc;

 //   p = (byte*) buffer;
 //   while (count--) 
 //   {
    //    crc = (crc >> 8) ^ m_crcTable[(crc & 0xFF) ^ *p++];
 //   }

 //   return crc ^ orig;

    if (buffer == 0) return 0L;

    crc = crc ^ 0xffffffffL;
    while (count--)
    {
        crc = crc_table[((int)crc ^ (*p++)) & 0xff] ^ (crc >> 8);
    }
   // if (len) do {
   //   DO1(buf);
   // } while (--len);
    return crc ^ 0xffffffffL;
}

//-----------------------------------------------------------------------------
// CalculatePaletteCRC
//-----------------------------------------------------------------------------
unsigned int CRCCalculator::calcPaletteCRC(unsigned int crc, void *buffer, unsigned int count)
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
unsigned int CRCCalculator::_reflect(unsigned int ref, char ch)
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
