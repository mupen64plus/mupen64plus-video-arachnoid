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

#include "RomDetector.h"
#include "assembler.h"   //swapRomHeaderBytes
#include <string.h>      //memcpy
#include "StringFunctions.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
ROMDetector::ROMDetector()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
ROMDetector::~ROMDetector()
{

}

//-----------------------------------------------------------------------------
//* Initialzie
//! Saves rom header, detects which rom it is, and sets states after
//! which rom it is.
//! @param romHeader Header with information about rom.
//-----------------------------------------------------------------------------
void ROMDetector::initialize(unsigned char* romHeader)
{

    //Copy Header
    memcpy(&m_romHeader, romHeader, sizeof(ROMHeader));

    //Header are stored really strange, swap bytes around to make sense of it
    swapRomHeaderBytes((void*)&m_romHeader, sizeof(ROMHeader));

    //Trim rom name (remove unnecessary whitespaces)
    StringFunctions::trim(m_romHeader.romName);

    //What game is it?
    m_currentRomID = this->_getRomID(m_romHeader.romName);
}

//-----------------------------------------------------------------------------
//* Get ROM ID
//! Detects and returns which rom it is, and sets states after
//! which rom it is.
//! @param romName Name of rom from rom-header.
//! @return ID of the rom that was identified by rom name.
//-----------------------------------------------------------------------------
N64_ROM_ID ROMDetector::_getRomID(char romName[20])
{  
    m_combinerType            = CT_ADVANCED;  //Use advanced combiner
    m_clearType               = CT_NEVER;     //Never Clear Screen
    m_ignoreFillRects         = false;
    m_forceDisableFaceCulling = false;
    m_useMultiTexture         = true;
    m_useSecondaryColor       = true;

    //Return ROM-ID and set ROM options
    if ( !strncmp(romName, "Banjo-Kazooie", 13) )
    {
        m_combinerType = CT_SIMPLE;
        return BANJO_KAZOOIE;
    }
    else if ( !strncmp(romName, "BANJO TOOIE", 11) )
    {
        m_combinerType = CT_SIMPLE;
        return BANJO_TOOIE;
    }
    else if ( !strncmp(romName, "F-ZERO X", 8) )
    {
        m_clearType = CT_AFTER_ONE_DISPLAY_LIST;
        return F_ZERO_X;
    }
    else if ( !strncmp(romName, "STARFOX64", 9) )
    {
        m_clearType = CT_AFTER_ONE_DISPLAY_LIST;
        return STAR_FOX_64;
    }
    else if ( !strncmp(romName, "SMASH BROTHERS", 14) )
    {
        m_clearType = CT_AFTER_ONE_DISPLAY_LIST;
        return SUPER_SMASH_BROS;
    }
    else if ( !strncmp(romName, "SUPER MARIO 64", 14) )
    {
        return SUPER_MARIO_64;
    }
    else if ( !strncmp(romName, "BOMBERMAN64E", 11) )
    {
        m_clearType = CT_AFTER_ONE_DISPLAY_LIST;
        m_ignoreFillRects         = true;
        return BOMBERMAN_64;
    }
    else if ( !strncmp(romName, "DONKEY KONG 64", 14) )
    {
        return DONKEY_KONG_64;
    }
    else if ( !strncmp(romName, "WAVE RACE 64", 12) )
    {        
        m_clearType = CT_AFTER_ONE_DISPLAY_LIST;
        m_ignoreFillRects = true;
        return WAVE_RACE_64;
    }        
    else if ( !strncmp(romName, "GOLDENEYE", 9) )
    {
        return GOLDEN_EYE;
    }
    //else if ( !strncmp(romName, "PAPER MARIO", 11) )
    //{
    //    m_clearType = CT_NEVER;
    //    return PAPER_MARIO;
    //}
    else
    {
        return UNKNOWN_ROM;
    }
}
