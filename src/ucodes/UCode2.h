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

#ifndef UCODE_2_H_
#define UCODE_2_H_

#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//*****************************************************************************
//! UCode2
//! Microcode used for Golden Eye
//! Note: This ucode is very similar to F3D, The diffrence is that this 
//!       UCode has a special way to render sky. That sky rendering is
//!       also used in perfect dark.
//*****************************************************************************
class UCode2
{
public:

    //Constructor / Destructor
    UCode2();
    ~UCode2();

    static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp);
    static void initializeGBI();

    static void renderSky(MicrocodeArgument* ucode);

private:

    static GBI* m_gbi;                               //!< Pointer to Graphics Binary Interface
    static RSP* m_rsp;                               //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;                               //!< Pointer to Reality Drawing Processor 
    static Memory* m_memory;                         //!< Pointer to Memory Manager
    static DisplayListParser* m_displayListParser;   //!< Pointer to Display-List Parser

};

#endif
