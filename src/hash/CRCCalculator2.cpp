
#include "CRCCalculator2.h"

#define CRC32_POLYNOMIAL    0xedb88320L    //0x04C11DB7
typedef unsigned char byte;

//-----------------------------------------------------------------------------
// Static Variabels
//-----------------------------------------------------------------------------
unsigned long CRCCalculator2::m_crcTable[256] = {0};   

static unsigned long crc_table[256];

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
        unsigned long crc;

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
unsigned long CRCCalculator2::calcCRC(unsigned long crc, void *buffer, unsigned long count)
{
    byte* p = (byte*) buffer; 
	unsigned long orig = crc;

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
unsigned long CRCCalculator2::calcPaletteCRC(unsigned long crc, void *buffer, unsigned long count)
{
    byte *p;
	unsigned long orig = crc;

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
unsigned long CRCCalculator2::_reflect(unsigned long ref, char ch)
{
    unsigned long value = 0;
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
