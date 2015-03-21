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

#include "UCode9.h"
#include "UCode0.h"
#include "GBI.h"
#include "RSP.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"

#define F3DPD_VTXCOLORBASE        0x07
                             
//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP*    UCode9::m_rsp    = 0;   //!< Pointer to Reality Signal Processor 

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode9::UCode9()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode9::~UCode9()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode9::initialize(RSP* rsp)
{
    m_rsp = rsp;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode9::initializeGBI(GBI* gbi)
{
    // Set GeometryMode flags
    GBI_InitFlags( F3D );

    //          GBI Command              Command Value                           Command Function
    GBI_SetGBI( GBI::G_SPNOOP,           F3D_SPNOOP,             gbi->m_cmds,    UCode0::F3D_SPNoOp );
    GBI_SetGBI( GBI::G_MTX,              F3D_MTX,                gbi->m_cmds,    UCode0::F3D_Mtx );
    GBI_SetGBI( GBI::G_RESERVED0,        F3D_RESERVED0,          gbi->m_cmds,    UCode0::F3D_Reserved0 );
    GBI_SetGBI( GBI::G_MOVEMEM,          F3D_MOVEMEM,            gbi->m_cmds,    UCode0::F3D_MoveMem );
    GBI_SetGBI( GBI::G_VTX,              F3D_VTX,                gbi->m_cmds,    PerfectDark_Vertex );
    GBI_SetGBI( GBI::G_RESERVED1,        F3D_RESERVED1,          gbi->m_cmds,    UCode0::F3D_Reserved1 );
    GBI_SetGBI( GBI::G_DL,               F3D_DL,                 gbi->m_cmds,    UCode0::F3D_DList );
    GBI_SetGBI( GBI::G_VTXCOLORBASE,     F3DPD_VTXCOLORBASE,     gbi->m_cmds,    PerfectDark_VertexColorBase );
    GBI_SetGBI( GBI::G_RESERVED3,        F3D_RESERVED3,          gbi->m_cmds,    UCode0::F3D_Reserved3 );
    GBI_SetGBI( GBI::G_SPRITE2D_BASE,    F3D_SPRITE2D_BASE,      gbi->m_cmds,    UCode0::F3D_Sprite2D_Base );
    GBI_SetGBI( GBI::G_TRI1,             F3D_TRI1,               gbi->m_cmds,    UCode0::F3D_Tri1 );
    GBI_SetGBI( GBI::G_CULLDL,           F3D_CULLDL,             gbi->m_cmds,    UCode0::F3D_CullDL );
    GBI_SetGBI( GBI::G_POPMTX,           F3D_POPMTX,             gbi->m_cmds,    UCode0::F3D_PopMtx );
    GBI_SetGBI( GBI::G_MOVEWORD,         F3D_MOVEWORD,           gbi->m_cmds,    UCode0::F3D_MoveWord );
    GBI_SetGBI( GBI::G_TEXTURE,          F3D_TEXTURE,            gbi->m_cmds,    UCode0::F3D_Texture );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,   F3D_SETOTHERMODE_H,     gbi->m_cmds,    UCode0::F3D_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,   F3D_SETOTHERMODE_L,     gbi->m_cmds,    UCode0::F3D_SetOtherMode_L );
    GBI_SetGBI( GBI::G_ENDDL,            F3D_ENDDL,              gbi->m_cmds,    UCode0::F3D_EndDL );
    GBI_SetGBI( GBI::G_SETGEOMETRYMODE,  F3D_SETGEOMETRYMODE,    gbi->m_cmds,    UCode0::F3D_SetGeometryMode );
    GBI_SetGBI( GBI::G_CLEARGEOMETRYMODE,F3D_CLEARGEOMETRYMODE,  gbi->m_cmds,    UCode0::F3D_ClearGeometryMode );
    GBI_SetGBI( GBI::G_QUAD,             F3D_QUAD,               gbi->m_cmds,    UCode0::F3D_Quad );
    GBI_SetGBI( GBI::G_RDPHALF_1,        F3D_RDPHALF_1,          gbi->m_cmds,    UCode0::F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_RDPHALF_2,        F3D_RDPHALF_2,          gbi->m_cmds,    UCode0::F3D_RDPHalf_2 );
    GBI_SetGBI( GBI::G_RDPHALF_CONT,     F3D_RDPHALF_CONT,       gbi->m_cmds,    UCode0::F3D_RDPHalf_Cont );
    GBI_SetGBI( GBI::G_TRI4,             F3D_TRI4,               gbi->m_cmds,    UCode0::F3D_Tri4 );

    //Set DMA Offset
    m_rsp->RSP_SetDMAOffsets(0,0);
}

//-----------------------------------------------------------------------------
//! Perfect Dark Vertex
//-----------------------------------------------------------------------------
void UCode9::PerfectDark_Vertex(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("PerfectDark_Vertex", M64MSG_VERBOSE);
    RSPUCode9AddColorIndexVertices* temp = (RSPUCode9AddColorIndexVertices*)ucode;

    //Set Color Index Vertices
    m_rsp->RSP_CIVertex(temp->segmentAddress, temp->numVertices + 1, temp->firstVertexIndex);
}

//-----------------------------------------------------------------------------
//! Perfect Dark Color Base
//-----------------------------------------------------------------------------
void UCode9::PerfectDark_VertexColorBase(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("PerfectDark_VertexColorBase", M64MSG_VERBOSE);
    RSPUCodeSetVertexColorBase* temp = (RSPUCodeSetVertexColorBase*)ucode;

    //Set Vertex Color Base
    m_rsp->RSP_SetVertexColorBase(temp->rdramAddress);
}
