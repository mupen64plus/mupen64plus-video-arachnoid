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

#ifndef UCODE_1_H_
#define UCODE_1_H_

//Includes
#include "UCodeDefs.h"

#define F3DEX_MTX_STACKSIZE        18

#define F3DEX_MTX_MODELVIEW        0x00
#define F3DEX_MTX_PROJECTION    0x01
#define F3DEX_MTX_MUL            0x00
#define F3DEX_MTX_LOAD            0x02
#define F3DEX_MTX_NOPUSH        0x00
#define F3DEX_MTX_PUSH            0x04

#define F3DEX_TEXTURE_ENABLE    0x00000002
#define F3DEX_SHADING_SMOOTH    0x00000200
#define F3DEX_CULL_FRONT        0x00001000
#define F3DEX_CULL_BACK            0x00002000
#define F3DEX_CULL_BOTH            0x00003000
#define F3DEX_CLIPPING            0x00800000

#define F3DEX_MV_VIEWPORT        0x80

#define F3DEX_MWO_aLIGHT_1        0x00
#define F3DEX_MWO_bLIGHT_1        0x04
#define F3DEX_MWO_aLIGHT_2        0x20
#define F3DEX_MWO_bLIGHT_2        0x24
#define F3DEX_MWO_aLIGHT_3        0x40
#define F3DEX_MWO_bLIGHT_3        0x44
#define F3DEX_MWO_aLIGHT_4        0x60
#define F3DEX_MWO_bLIGHT_4        0x64
#define F3DEX_MWO_aLIGHT_5        0x80
#define F3DEX_MWO_bLIGHT_5        0x84
#define F3DEX_MWO_aLIGHT_6        0xa0
#define F3DEX_MWO_bLIGHT_6        0xa4
#define F3DEX_MWO_aLIGHT_7        0xc0
#define F3DEX_MWO_bLIGHT_7        0xc4
#define F3DEX_MWO_aLIGHT_8        0xe0
#define F3DEX_MWO_bLIGHT_8        0xe4

// F3DEX commands
#define F3DEX_MODIFYVTX                0xB2
#define F3DEX_TRI2                    0xB1
#define F3DEX_BRANCH_Z                0xB0
#define F3DEX_LOAD_UCODE            0xAF // 0xCF

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;

//*****************************************************************************
//! UCode1 (aka F3DEX - Fast 3D EX)
//! Microcode used for most games
//*****************************************************************************
class UCode1
{
public:

    UCode1();
    ~UCode1();

    static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory);
    static void initializeGBI();

    static void F3DEX_Load_uCode(MicrocodeArgument* ucode);
    static void F3DEX_Vtx(MicrocodeArgument* ucode);
    static void F3DEX_ModifyVtx(MicrocodeArgument* ucode);
    static void F3DEX_Tri1(MicrocodeArgument* ucode);    
    static void F3DEX_Tri2(MicrocodeArgument* ucode);
    static void F3DEX_Quad(MicrocodeArgument* ucode);
    static void F3DEX_CullDL(MicrocodeArgument* ucode);
    static void F3DEX_Branch_Z(MicrocodeArgument* ucode);    

private:

    static GBI* m_gbi;        //!< Graphics Binary Interface
    static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;        //!< Pointer to Reality Drawing Processor  
    static Memory* m_memory;  //!< Pointer accessing memory like RDRAM and Texture Memory

};


#endif
