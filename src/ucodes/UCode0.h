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

#ifndef UCODE_0_H_
#define UCODE_0_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//Definitions
#define F3D_MTX_STACKSIZE       10

#define F3D_MTX_MODELVIEW       0x00
#define F3D_MTX_PROJECTION      0x01
#define F3D_MTX_MUL             0x00
#define F3D_MTX_LOAD            0x02
#define F3D_MTX_NOPUSH          0x00
#define F3D_MTX_PUSH            0x04

#define F3D_TEXTURE_ENABLE      0x00000002
#define F3D_SHADING_SMOOTH      0x00000200
#define F3D_CULL_FRONT          0x00001000
#define F3D_CULL_BACK           0x00002000
#define F3D_CULL_BOTH           0x00003000
#define F3D_CLIPPING            0x00000000

#define F3D_MV_VIEWPORT         0x80

#define F3D_MWO_aLIGHT_1        0x00
#define F3D_MWO_bLIGHT_1        0x04
#define F3D_MWO_aLIGHT_2        0x20
#define F3D_MWO_bLIGHT_2        0x24
#define F3D_MWO_aLIGHT_3        0x40
#define F3D_MWO_bLIGHT_3        0x44
#define F3D_MWO_aLIGHT_4        0x60
#define F3D_MWO_bLIGHT_4        0x64
#define F3D_MWO_aLIGHT_5        0x80
#define F3D_MWO_bLIGHT_5        0x84
#define F3D_MWO_aLIGHT_6        0xa0
#define F3D_MWO_bLIGHT_6        0xa4
#define F3D_MWO_aLIGHT_7        0xc0
#define F3D_MWO_bLIGHT_7        0xc4
#define F3D_MWO_aLIGHT_8        0xe0
#define F3D_MWO_bLIGHT_8        0xe4

// FAST3D commands
#define F3D_SPNOOP              0x00
#define F3D_MTX                 0x01
#define F3D_RESERVED0           0x02
#define F3D_MOVEMEM             0x03
#define F3D_VTX                 0x04
#define F3D_RESERVED1           0x05
#define F3D_DL                  0x06
#define F3D_RESERVED2           0x07
#define F3D_RESERVED3           0x08
#define F3D_SPRITE2D_BASE       0x09

#define F3D_TRI1                0xBF
#define F3D_CULLDL              0xBE
#define F3D_POPMTX              0xBD
#define F3D_MOVEWORD            0xBC
#define F3D_TEXTURE             0xBB
#define F3D_SETOTHERMODE_H      0xBA
#define F3D_SETOTHERMODE_L      0xB9
#define F3D_ENDDL               0xB8
#define F3D_SETGEOMETRYMODE     0xB7
#define F3D_CLEARGEOMETRYMODE   0xB6
//#define F3D_LINE3D              0xB5 // Only used in Line3D
#define F3D_QUAD                0xB5
#define F3D_RDPHALF_1           0xB4
#define F3D_RDPHALF_2           0xB3
#define F3D_RDPHALF_CONT        0xB2
#define F3D_TRI4                0xB1

//*****************************************************************************
//! UCode0 (aka F3D - Fast 3D)
//! Microcode used for Super Mario 64!!!
//*****************************************************************************
class UCode0
{
public:

    UCode0();
    ~UCode0();

    static void initialize(RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp);
    static void initializeGBI(GBI* gbi);

public:

    // Matrix Functions
    static void F3D_Mtx(MicrocodeArgument* ucode);
    static void F3D_PopMtx(MicrocodeArgument* ucode);

    //Vertex and indices
    static void F3D_Vtx(MicrocodeArgument* ucode);
    static void F3D_Tri1(MicrocodeArgument* ucode);
    static void F3D_Tri4(MicrocodeArgument* ucode);
    static void F3D_Quad(MicrocodeArgument* ucode);

    //Display list
    static void F3D_DList(MicrocodeArgument* ucode);
    static void F3D_EndDL(MicrocodeArgument* ucode);
    static void F3D_CullDL(MicrocodeArgument* ucode);  //Unimplemented

    // Texture
    static void F3D_Texture(MicrocodeArgument* ucode);

    // Half
    static void F3D_RDPHalf_1(MicrocodeArgument* ucode);
    static void F3D_RDPHalf_2(MicrocodeArgument* ucode);
    static void F3D_RDPHalf_Cont(MicrocodeArgument* ucode);  //Unimplmeneted

    // Geometry Mode
    static void F3D_SetGeometryMode(MicrocodeArgument* ucode);
    static void F3D_ClearGeometryMode(MicrocodeArgument* ucode);

    // Set Other Modes
    static void F3D_SetOtherMode_H(MicrocodeArgument* ucode);
    static void F3D_SetOtherMode_L(MicrocodeArgument* ucode);
    
    // Other
    static void F3D_MoveMem(MicrocodeArgument* ucode);
    static void F3D_MoveWord(MicrocodeArgument* ucode);
    static void F3D_Sprite2D_Base(MicrocodeArgument* ucode);  //Unimplemented

    //Unimportant
    static void F3D_SPNoOp(MicrocodeArgument* ucode);    
    static void F3D_Reserved0(MicrocodeArgument* ucode);    
    static void F3D_Reserved1(MicrocodeArgument* ucode);
    static void F3D_Reserved2(MicrocodeArgument* ucode);
    static void F3D_Reserved3(MicrocodeArgument* ucode);

private:

    static RSP* m_rsp;                              //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;                              //!< Pointer to Reality Drawing Processor  
    static Memory* m_memory;                        //!< Pointer to Memory Manager
    static DisplayListParser* m_displayListParser;  //!< Pointer to Display List Parser

};

#endif
