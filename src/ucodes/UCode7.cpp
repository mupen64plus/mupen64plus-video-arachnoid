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

#include "UCode7.h"
#include "UCode0.h"
#include "UCode1.h"
#include "GBI.h"
#include "RSP.h"
#include "RDP.h"
#include "Memory.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"

#define    S2DEX_BG_1CYC            0x01
#define    S2DEX_BG_COPY            0x02
#define    S2DEX_OBJ_RECTANGLE      0x03
#define    S2DEX_OBJ_SPRITE         0x04
#define    S2DEX_OBJ_MOVEMEM        0x05
#define    S2DEX_LOAD_UCODE         0xAF
#define    S2DEX_SELECT_DL          0xB0
#define    S2DEX_OBJ_RENDERMODE     0xB1
#define    S2DEX_OBJ_RECTANGLE_R    0xB2
#define    S2DEX_OBJ_LOADTXTR       0xC1
#define    S2DEX_OBJ_LDTX_SPRITE    0xC2
#define    S2DEX_OBJ_LDTX_RECT      0xC3
#define    S2DEX_OBJ_LDTX_RECT_R    0xC4
#define    S2DEX_RDPHALF_0          0xE4

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP*    UCode7::m_rsp    = 0;   //!< Pointer to Reality Signal Processor 

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode7::UCode7()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode7::~UCode7()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode7::initialize(RSP* rsp)
{
    m_rsp = rsp;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode7::initializeGBI(GBI* gbi)
{
    // Set GeometryMode flags
    GBI_InitFlags( F3DEX );

    //          GBI Command             Command Value            Command Function
    GBI_SetGBI( GBI::G_SPNOOP,          F3D_SPNOOP,              gbi->m_cmds,    UCode0::F3D_SPNoOp );
    GBI_SetGBI( GBI::G_BG_1CYC,         S2DEX_BG_1CYC,           gbi->m_cmds,    S2DEX_BG_1Cyc );
    GBI_SetGBI( GBI::G_BG_COPY,         S2DEX_BG_COPY,           gbi->m_cmds,    S2DEX_BG_Copy );
    GBI_SetGBI( GBI::G_OBJ_RECTANGLE,   S2DEX_OBJ_RECTANGLE,     gbi->m_cmds,    S2DEX_Obj_Rectangle );
    GBI_SetGBI( GBI::G_OBJ_SPRITE,      S2DEX_OBJ_SPRITE,        gbi->m_cmds,    S2DEX_Obj_Sprite );
    GBI_SetGBI( GBI::G_OBJ_MOVEMEM,     S2DEX_OBJ_MOVEMEM,       gbi->m_cmds,    S2DEX_Obj_MoveMem );
    GBI_SetGBI( GBI::G_DL,              F3D_DL,                  gbi->m_cmds,    UCode0::F3D_DList );
    GBI_SetGBI( GBI::G_SELECT_DL,       S2DEX_SELECT_DL,         gbi->m_cmds,    S2DEX_Select_DL );
    GBI_SetGBI( GBI::G_OBJ_RENDERMODE,  S2DEX_OBJ_RENDERMODE,    gbi->m_cmds,    S2DEX_Obj_RenderMode );
    GBI_SetGBI( GBI::G_OBJ_RECTANGLE_R, S2DEX_OBJ_RECTANGLE_R,   gbi->m_cmds,    S2DEX_Obj_Rectangle_R );
    GBI_SetGBI( GBI::G_OBJ_LOADTXTR,    S2DEX_OBJ_LOADTXTR,      gbi->m_cmds,    S2DEX_Obj_LoadTxtr );
    GBI_SetGBI( GBI::G_OBJ_LDTX_SPRITE, S2DEX_OBJ_LDTX_SPRITE,   gbi->m_cmds,    S2DEX_Obj_LdTx_Sprite );
    GBI_SetGBI( GBI::G_OBJ_LDTX_RECT,   S2DEX_OBJ_LDTX_RECT,     gbi->m_cmds,    S2DEX_Obj_LdTx_Rect );
    GBI_SetGBI( GBI::G_OBJ_LDTX_RECT_R, S2DEX_OBJ_LDTX_RECT_R,   gbi->m_cmds,    S2DEX_Obj_LdTx_Rect_R );
    GBI_SetGBI( GBI::G_MOVEWORD,        F3D_MOVEWORD,            gbi->m_cmds,    UCode0::F3D_MoveWord );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,  F3D_SETOTHERMODE_H,      gbi->m_cmds,    UCode0::F3D_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,  F3D_SETOTHERMODE_L,      gbi->m_cmds,    UCode0::F3D_SetOtherMode_L );
    GBI_SetGBI( GBI::G_ENDDL,           F3D_ENDDL,               gbi->m_cmds,    UCode0::F3D_EndDL );
    GBI_SetGBI( GBI::G_RDPHALF_1,       F3D_RDPHALF_1,           gbi->m_cmds,    UCode0::F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_RDPHALF_2,       F3D_RDPHALF_2,           gbi->m_cmds,    UCode0::F3D_RDPHalf_2 );
    GBI_SetGBI(    GBI::G_LOAD_UCODE,   S2DEX_LOAD_UCODE,        gbi->m_cmds,    UCode1::F3DEX_Load_uCode );
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_BG_1Cyc(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_BG_1Cyc - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_BG_Copy(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_BG_Copy - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! Obj Ractangle
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_Rectangle(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_Rectangle - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_Sprite(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_Sprite - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_MoveMem(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_MoveMem - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_LoadTxtr(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_LoadTxtr - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_LdTx_Sprite(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_LdTx_Sprite - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_LdTx_Rect_R(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_LdTx_Rect_R - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! Select Display List
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Select_DL(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Select_DL - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_RenderMode(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_RenderMode - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_Rectangle_R(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_Rectangle_R - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void UCode7::S2DEX_Obj_LdTx_Rect(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("S2DEX_Obj_LdTx_Rect - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}
