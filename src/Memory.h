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

#ifndef MEMORY_H_
#define MEMORY_H_

//*****************************************************************************
//* Memory
//! Handle RDRAM, Texture Memory and Segments
//*****************************************************************************
class Memory
{
public:

    //Constructor / Destructor
    Memory();
    ~Memory();

    //Initialize
    bool initialize(unsigned char* RDRAM, unsigned char* DMEM);

    //Get RDRAM

    unsigned char*  getRDRAM(int address=0)                { return &m_RDRAM[address]; }
    unsigned int*   getRDRAMint32()                        { return (unsigned int*)m_RDRAM; }
    unsigned int    getRDRAMSize()                         { return m_RDRAMSize; }

    //Get DMEM
    unsigned char*  getDMEM(int address=0)                 { return &m_DMEM[address]; }

    //Get Texture memory
    static unsigned long long* getTextureMemory(int address=0) { return &m_TMEM[address]; }

    //Get Segment adress
    unsigned int getRDRAMAddress(unsigned int segmentAddress) 
    {         
        return (m_segments[(segmentAddress >> 24) & 0x0F] + (segmentAddress & 0x00FFFFFF)) & 0x00FFFFFF; 
    }

    void setSegment(unsigned int address, unsigned int value)
    {
        if ( address >= 16 ) {
            //ERROR
            return;
        }

        m_segments[address] = value;
    }

private:
    
    unsigned char*          m_RDRAM;          //!< Rambus Dynamic Random Access Memory
    unsigned char*          m_DMEM;           //!< RSP Data Memory
    static unsigned long long m_TMEM[512];    //!< Texture Memory        
    unsigned int           m_segments[16];    //!< Temporary memory for storing segment values
    unsigned int           m_RDRAMSize;       //!< Size of RDRAM

};

#endif
