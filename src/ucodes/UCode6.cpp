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

#include "DisplayListParser.h"
#include "GBI.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "Memory.h"
#include "RDP.h"
#include "RSP.h"
#include "RSPMatrixManager.h"
#include "UCode0.h"
#include "UCode6.h"
#include "UCodeDefs.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
GBI*               UCode6::m_gbi = 0;                 // Pointer to Graphics Binary Interface
RSP*               UCode6::m_rsp = 0;                 // Pointer to Reality Signal Processor 
RDP*               UCode6::m_rdp = 0;                 // Pointer to Reality Drawing Processor 
DisplayListParser* UCode6::m_displayListParser = 0;
Memory*            UCode6::m_memory = 0;
unsigned int       UCode6::m_vertexIndex = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode6::UCode6()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode6::~UCode6()
{
}

//-----------------------------------------------------------------------------
// Initialize 
//-----------------------------------------------------------------------------
void UCode6::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp)
{
    m_gbi = gbi;
    m_rsp = rsp;
    m_rdp = rdp;
    m_memory = memory;
    m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//* Initialize GBI
//! Assigns functions to the GBI
//-----------------------------------------------------------------------------
void UCode6::initializeGBI()
{
    GBI_InitFlags( F3D );

    //          GBI Command              Command Value            Command Function
    GBI_SetGBI( GBI::G_SPNOOP,           F3D_SPNOOP,              m_gbi->m_cmds,    UCode0::F3D_SPNoOp );
    GBI_SetGBI( GBI::G_DMA_MTX,          F3DDKR_DMA_MTX,          m_gbi->m_cmds,    F3DDKR_DMA_Mtx );
    GBI_SetGBI( GBI::G_MOVEMEM,          F3D_MOVEMEM,             m_gbi->m_cmds,    UCode0::F3D_MoveMem );
    GBI_SetGBI( GBI::G_DMA_VTX,          F3DDKR_DMA_VTX,          m_gbi->m_cmds,    F3DDKR_DMA_Vtx );
    GBI_SetGBI( GBI::G_DL,               F3D_DL,                  m_gbi->m_cmds,    UCode0::F3D_DList );
    GBI_SetGBI( GBI::G_DMA_DL,           F3DDKR_DMA_DL,           m_gbi->m_cmds,    F3DDKR_DMA_DList );
    GBI_SetGBI( GBI::G_DMA_TRI,          F3DDKR_DMA_TRI,          m_gbi->m_cmds,    F3DDKR_DMA_Tri );
    GBI_SetGBI( GBI::G_DMA_OFFSETS,      F3DDKR_DMA_OFFSETS,      m_gbi->m_cmds,    F3DDKR_DMA_Offsets );
    GBI_SetGBI( GBI::G_CULLDL,           F3D_CULLDL,              m_gbi->m_cmds,    UCode0::F3D_CullDL );
    GBI_SetGBI( GBI::G_MOVEWORD,         F3D_MOVEWORD,            m_gbi->m_cmds,    F3DDKR_MoveWord );
    GBI_SetGBI( GBI::G_TEXTURE,          F3D_TEXTURE,             m_gbi->m_cmds,    UCode0::F3D_Texture );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,   F3D_SETOTHERMODE_H,      m_gbi->m_cmds,    UCode0::F3D_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,   F3D_SETOTHERMODE_L,      m_gbi->m_cmds,    UCode0::F3D_SetOtherMode_L );
    GBI_SetGBI( GBI::G_ENDDL,            F3D_ENDDL,               m_gbi->m_cmds,    UCode0::F3D_EndDL );
    GBI_SetGBI( GBI::G_SETGEOMETRYMODE,  F3D_SETGEOMETRYMODE,     m_gbi->m_cmds,    UCode0::F3D_SetGeometryMode );
    GBI_SetGBI( GBI::G_CLEARGEOMETRYMODE,F3D_CLEARGEOMETRYMODE,   m_gbi->m_cmds,    UCode0::F3D_ClearGeometryMode );
    GBI_SetGBI( GBI::G_QUAD,             F3D_QUAD,                m_gbi->m_cmds,    UCode0::F3D_Quad );
    GBI_SetGBI( GBI::G_RDPHALF_1,        F3D_RDPHALF_1,           m_gbi->m_cmds,    UCode0::F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_RDPHALF_2,        F3D_RDPHALF_2,           m_gbi->m_cmds,    UCode0::F3D_RDPHalf_2 );
    GBI_SetGBI( GBI::G_RDPHALF_CONT,     F3D_RDPHALF_CONT,        m_gbi->m_cmds,    UCode0::F3D_RDPHalf_Cont );
    GBI_SetGBI( GBI::G_TRI4,             F3D_TRI4,                m_gbi->m_cmds,    UCode0::F3D_Tri4 );
}

//-----------------------------------------------------------------------------
// DMA Matrix
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_DMA_Mtx(MicrocodeArgument* ucode)
{
    if (_SHIFTR( ucode->w0, 0, 16 ) != 64)
    {
        //GBI_DetectUCode(); // Something's wrong
        return;
    }

    unsigned int index = _SHIFTR( ucode->w0, 16, 4 );
    unsigned int multiply;

    if (index == 0) // DKR
    {
        index = _SHIFTR( ucode->w0, 22, 2 );
        multiply = 0;
    }
    else // Gemini
    {
        multiply = _SHIFTR( ucode->w0, 23, 1 );
    }

    m_rsp->RSP_DMAMatrix( ucode->w1, index, multiply );
}

//-----------------------------------------------------------------------------
// DMA Vertex 
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_DMA_Vtx(MicrocodeArgument* ucode)
{
    if ((ucode->w0 & F3DDKR_VTX_APPEND))
    {
        if ( m_rsp->getVertexMgr()->getBillboard() ) {
            m_vertexIndex = 1;
        }
    }
    else {
        m_vertexIndex = 0;
    }

    unsigned int n = _SHIFTR( ucode->w0, 19, 5 ) + 1;

    m_rsp->RSP_DMAVertex( ucode->w1, n, m_vertexIndex + _SHIFTR( ucode->w0, 9, 5 ) );

    m_vertexIndex += n;
}

//-----------------------------------------------------------------------------
// DMA Triangle
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_DMA_Tri(MicrocodeArgument* ucode)
{
    m_rsp->RSP_DMATriangles( ucode->w1, _SHIFTR( ucode->w0, 4, 12 ) );
    m_vertexIndex = 0;
}

//-----------------------------------------------------------------------------
// DMA Display List
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_DMA_DList(MicrocodeArgument* ucode)
{
    m_rsp->RSP_DMADisplayList( ucode->w0, ucode->w1   /*_SHIFTR( ucode->w0, 16, 8 )*/ );
}

//-----------------------------------------------------------------------------
// DMA Offsets
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_DMA_Offsets(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("PerfectDark_Vertex", M64MSG_VERBOSE);
    RSPUCodeSetDMAOffsets* temp = (RSPUCodeSetDMAOffsets*)ucode;

    //Set DMA Offsets
    m_rsp->RSP_SetDMAOffsets(temp->addressOffsetMatrix, temp->addressOffsetVertex);
}

//-----------------------------------------------------------------------------
// MoveWord
//-----------------------------------------------------------------------------
void UCode6::F3DDKR_MoveWord(MicrocodeArgument* ucode)
{
    switch (_SHIFTR( ucode->w0, 0, 8 ))
    {
        case 0x02:
            m_rsp->getVertexMgr()->setBillboard( ucode->w1 & 1 );
            break;
        case 0x0A:
            m_rsp->getMatrixMgr()->selectViewMatrix(_SHIFTR( ucode->w1, 6, 2 ));
            break;
        default:
            UCode0::F3D_MoveWord( ucode );
            break;
    }
}
