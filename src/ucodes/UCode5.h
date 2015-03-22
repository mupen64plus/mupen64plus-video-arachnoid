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

#ifndef UCODE_5_H_
#define UCODE_5_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

#define    F3DEX2_MOVEMEM            0xDC
#define    F3DEX2_MOVEWORD           0xDB

//*****************************************************************************
//! UCode5 (aka F3DEX2)
//! Microcode used for Zelda and newer games
//*****************************************************************************
class UCode5
{
public: 

    // Constructor / Destructor
    UCode5();
    ~UCode5();

    static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp);
    static void initializeGBI();

    //Matrices
    static void F3DEX2_Mtx( MicrocodeArgument* ucode );
    static void F3DEX2_PopMtx( MicrocodeArgument* ucode );

    //Textures
    static void F3DEX2_Texture( MicrocodeArgument* ucode );

    //Vertices
    static void F3DEX2_Vtx( MicrocodeArgument* ucode );
    static void F3DEX2_Tri1( MicrocodeArgument* ucode );
    static void F3DEX2_Quad( MicrocodeArgument* ucode );
    static void F3DEX2_Line3D( MicrocodeArgument* ucode );

    //Misc
    static void F3DEX2_MoveMem( MicrocodeArgument* ucode );
    static void F3DEX2_MoveWord( MicrocodeArgument* ucode );
    static void F3DEX2_GeometryMode( MicrocodeArgument* ucode );
    static void F3DEX2_SetOtherMode_H( MicrocodeArgument* ucode );
    static void F3DEX2_SetOtherMode_L( MicrocodeArgument* ucode );

    //Other
    static void F3DEX2_DMAIO( MicrocodeArgument* ucode );
    static void F3DEX2_Special_1( MicrocodeArgument* ucode );
    static void F3DEX2_Special_2( MicrocodeArgument* ucode );
    static void F3DEX2_Special_3( MicrocodeArgument* ucode );
    static void F3DEX2_Reserved1( MicrocodeArgument* ucode );

private:

    static GBI* m_gbi;        //!< Graphics Binary Interface
    static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;        //!< Pointer to Reality Drawing Processor  
    static Memory* m_memory;  //!< Pointer accessing memory like RDRAM and Texture Memory
    static DisplayListParser* m_displayListParser;

};

#endif
