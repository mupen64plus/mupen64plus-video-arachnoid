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

#include "Memory.h"

//-----------------------------------------------------------------------------
//* Static variables
//-----------------------------------------------------------------------------
unsigned long long Memory::m_TMEM[512] = {0};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
Memory::Memory()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Memory::~Memory()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointers to memory areas, resets segment register, and detects
//! size of RDRAM.
//! @param RDRAM Pointer to "Rambus Dynamic Random Access Memory"
//! @param DMEM Pointer to "RSP Data Memory"
//-----------------------------------------------------------------------------
bool Memory::initialize(unsigned char* RDRAM, unsigned char* DMEM)
{
    //Save pointer to memory
    m_RDRAM = RDRAM;
    m_DMEM = DMEM;

    //Reset Segment
    for (int i=0; i<16; ++i) 
    {
        m_segments[i] = 0;   
    }

    //Reset Texture Memory
    //for (int i=0; i<512; ++i)
    //{
    //    m_TMEM[i] = 0;
    //}    

    m_RDRAMSize = 0x800000;
    return true;
}
