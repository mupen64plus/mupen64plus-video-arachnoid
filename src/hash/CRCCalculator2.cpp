
#include "CRCCalculator2.h"

#define CRC32_POLYNOMIAL    0xedb88320    //0x04C11DB7
typedef unsigned char byte;

//-----------------------------------------------------------------------------
// Static Variabels
//-----------------------------------------------------------------------------
unsigned int CRCCalculator2::m_crcTable[256] = {0};   

static unsigned int crc_table[256];

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
