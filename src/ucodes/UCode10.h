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

#ifndef UCODE_10_H_
#define UCODE_10_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//*****************************************************************************
//! UCode10
//! Microcode used for Conker's Bad Fur Day
//*****************************************************************************
class UCode10
{
public:

    UCode10();
    ~UCode10();

    static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp);
    static void initializeGBI();

    static void ConkerBFD_Vertex(MicrocodeArgument* ucode);
    static void ConkerBFD_Add4Triangles(MicrocodeArgument* ucode);
    static void ConkerBFD_MoveWord(MicrocodeArgument* ucode);
    static void ConkerBFD_MoveMem(MicrocodeArgument* ucode);

private:

    static GBI* m_gbi;        //!< Graphics Binary Interface
    static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;        //!< Pointer to Reality Drawing Processor  
    static Memory* m_memory;  //!< Pointer accessing memory like RDRAM and Texture Memory
    static DisplayListParser* m_displayListParser;


};

#endif