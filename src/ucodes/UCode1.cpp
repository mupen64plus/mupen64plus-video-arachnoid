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

#include "GBI.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "Memory.h"
#include "RDP.h"
#include "RSP.h"
#include "UCode0.h"
#include "UCode1.h"
#include "UCodeDefs.h"
#include "m64p_types.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP*    UCode1::m_rsp    = 0;   //!< Pointer to Reality Signal Processor 
RDP*    UCode1::m_rdp    = 0;   //!< Pointer to Reality Drawing Processor 
Memory* UCode1::m_memory = 0;
GBI*    UCode1::m_gbi    = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode1::UCode1()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode1::~UCode1()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode1::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory)
{
    m_gbi = gbi;
    m_rsp = rsp;
    m_rdp = rdp;
    m_memory = memory;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode1::initializeGBI()
{
    // Set GeometryMode flags
    GBI_InitFlags( F3DEX );

    //          GBI Command              Command Value            //Target          Command Function
    GBI_SetGBI( GBI::G_SPNOOP,           F3D_SPNOOP,              m_gbi->m_cmds,    UCode0::F3D_SPNoOp );
    GBI_SetGBI( GBI::G_MTX,              F3D_MTX,                 m_gbi->m_cmds,    UCode0::F3D_Mtx );
    GBI_SetGBI( GBI::G_RESERVED0,        F3D_RESERVED0,           m_gbi->m_cmds,    UCode0::F3D_Reserved0 );
    GBI_SetGBI( GBI::G_MOVEMEM,          F3D_MOVEMEM,             m_gbi->m_cmds,    UCode0::F3D_MoveMem );
    GBI_SetGBI( GBI::G_VTX,              F3D_VTX,                 m_gbi->m_cmds,    F3DEX_Vtx );
    GBI_SetGBI( GBI::G_RESERVED1,        F3D_RESERVED1,           m_gbi->m_cmds,    UCode0::F3D_Reserved1 );
    GBI_SetGBI( GBI::G_DL,               F3D_DL,                  m_gbi->m_cmds,    UCode0::F3D_DList );
    GBI_SetGBI( GBI::G_RESERVED2,        F3D_RESERVED2,           m_gbi->m_cmds,    UCode0::F3D_Reserved2 );
    GBI_SetGBI( GBI::G_RESERVED3,        F3D_RESERVED3,           m_gbi->m_cmds,    UCode0::F3D_Reserved3 );
    GBI_SetGBI( GBI::G_SPRITE2D_BASE,    F3D_SPRITE2D_BASE,       m_gbi->m_cmds,    UCode0::F3D_Sprite2D_Base );
    GBI_SetGBI( GBI::G_TRI1,             F3D_TRI1,                m_gbi->m_cmds,    F3DEX_Tri1 );
    GBI_SetGBI( GBI::G_CULLDL,           F3D_CULLDL,              m_gbi->m_cmds,    F3DEX_CullDL );
    GBI_SetGBI( GBI::G_POPMTX,           F3D_POPMTX,              m_gbi->m_cmds,    UCode0::F3D_PopMtx );
    GBI_SetGBI( GBI::G_MOVEWORD,         F3D_MOVEWORD,            m_gbi->m_cmds,    UCode0::F3D_MoveWord );
    GBI_SetGBI( GBI::G_TEXTURE,          F3D_TEXTURE,             m_gbi->m_cmds,    UCode0::F3D_Texture );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,   F3D_SETOTHERMODE_H,      m_gbi->m_cmds,    UCode0::F3D_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,   F3D_SETOTHERMODE_L,      m_gbi->m_cmds,    UCode0::F3D_SetOtherMode_L );
    GBI_SetGBI( GBI::G_ENDDL,            F3D_ENDDL,               m_gbi->m_cmds,    UCode0::F3D_EndDL );
    GBI_SetGBI( GBI::G_SETGEOMETRYMODE,  F3D_SETGEOMETRYMODE,     m_gbi->m_cmds,    UCode0::F3D_SetGeometryMode );
    GBI_SetGBI( GBI::G_CLEARGEOMETRYMODE,F3D_CLEARGEOMETRYMODE,   m_gbi->m_cmds,    UCode0::F3D_ClearGeometryMode );
    GBI_SetGBI( GBI::G_QUAD,             F3D_QUAD,                m_gbi->m_cmds,    F3DEX_Quad );
    GBI_SetGBI( GBI::G_RDPHALF_1,        F3D_RDPHALF_1,           m_gbi->m_cmds,    UCode0::F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_RDPHALF_2,        F3D_RDPHALF_2,           m_gbi->m_cmds,    UCode0::F3D_RDPHalf_2 );
    GBI_SetGBI( GBI::G_MODIFYVTX,        F3DEX_MODIFYVTX,         m_gbi->m_cmds,    F3DEX_ModifyVtx );
    GBI_SetGBI( GBI::G_TRI2,             F3DEX_TRI2,              m_gbi->m_cmds,    F3DEX_Tri2 );
    GBI_SetGBI( GBI::G_BRANCH_Z,         F3DEX_BRANCH_Z,          m_gbi->m_cmds,    F3DEX_Branch_Z );
    GBI_SetGBI( GBI::G_LOAD_UCODE,       F3DEX_LOAD_UCODE,        m_gbi->m_cmds,    F3DEX_Load_uCode );
}

//-----------------------------------------------------------------------------
//! Load UCode
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Load_uCode(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_Load_uCode - experimental", M64MSG_WARNING);
    RSPUCodeLoadUCode* temp = (RSPUCodeLoadUCode*)ucode;

    //unsigned int ucodeDataStart1 = m_memory->getRDRAMAddress( (*(unsigned int*)( m_memory->getRDRAM(pc-12))  );
    unsigned int ucodeDataStart2 = m_rdp->getHalf1();

    //if ( ucodeDataStart1 != ucodeDataStart2 )
    //{
    //    Logger::getSingleton().printMsg("Warning - UCode Data Start differs", M64MSG_INFO);
    //}

    //Select UCode
    m_gbi->selectUCode( temp->ucodeStart,        //UCodeStart 
                        ucodeDataStart2,         //UCodeDataStart
                        temp->ucodeSize+1,       //UCodeSize
                        8);                      //UCodeDataSize  //Always 8 ???                  
}

//-----------------------------------------------------------------------------
//! Add Vertices
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Vtx(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_Vtx", M64MSG_VERBOSE);
    RSPUCodeAddVertices1* temp = (RSPUCodeAddVertices1*)ucode;

    //Add Vertices
    m_rsp->RSP_Vertex(temp->segmentAddress, temp->numVertices, temp->firstVertex);
}

//-----------------------------------------------------------------------------
//! Modify Vertex
//-----------------------------------------------------------------------------
void UCode1::F3DEX_ModifyVtx(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_ModifyVtx", M64MSG_VERBOSE);
    RSPUCodeModifyVertex* temp = (RSPUCodeModifyVertex*)ucode;

    switch ( temp->modifyType )
    {
        case G_MWO_POINT_RGBA:
            m_rsp->RSP_SetVertexColor( temp->vertexIndex, 
                                       temp->r/255.0f, 
                                       temp->g/255.0f, 
                                       temp->b/255.0f, 
                                       temp->a/255.0f );
            break;
        case G_MWO_POINT_ST:
            m_rsp->RSP_SetVertexTexCoord(temp->vertexIndex, 
                                         temp->t / 32.0f, 
                                         temp->s / 32.0f );
            break;
        case G_MWO_POINT_XYSCREEN:
            break;
        case G_MWO_POINT_ZSCREEN:
            break;
    };
}

//-----------------------------------------------------------------------------
//! Add 1 Triangle
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Tri1(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_Tri1", M64MSG_VERBOSE);
    RSPUCodeAddOneTriangleF3DEX* temp = (RSPUCodeAddOneTriangleF3DEX*)ucode;

    //Add one triangle 
    m_rsp->RSP_1Triangle(temp->index0, temp->index1, temp->index2);
}

//-----------------------------------------------------------------------------
//! Add 2 Triangles
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Tri2(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_Tri2", M64MSG_VERBOSE);
    RSPUCodeAddTwoTrianglesF3DEX* temp = (RSPUCodeAddTwoTrianglesF3DEX*)ucode;

    //Add two triangles
    m_rsp->RSP_2Triangles( temp->v0, temp->v1, temp->v2, 0,
                           temp->v3, temp->v4, temp->v5, 0);
}

//-----------------------------------------------------------------------------
//! Add 1 Quadrangle
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Quad( MicrocodeArgument* ucode )
{
    Logger::getSingleton().printMsg("F3DEX_Quad", M64MSG_VERBOSE);
    RSPUCodeAddOneQuadF3DEX* temp = (RSPUCodeAddOneQuadF3DEX*)ucode;

    //Add one Quad
    m_rsp->RSP_1Quadrangle(temp->index0, temp->index1, temp->index2, temp->index3);
}

//-----------------------------------------------------------------------------
//! Cull Display List
//-----------------------------------------------------------------------------
void UCode1::F3DEX_CullDL(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( !warned )
    {
        Logger::getSingleton().printMsg("F3DEX_CullDL - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
    RSPUCodeCullDisplayList* temp = (RSPUCodeCullDisplayList*)ucode;

    //Cull display list?
    m_rsp->RSP_CullDisplayList( temp->vertexIndex, temp->numVerticies  );
}

//-----------------------------------------------------------------------------
//! Branch Z
//-----------------------------------------------------------------------------
void UCode1::F3DEX_Branch_Z(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3DEX_Branch_Z", M64MSG_VERBOSE);
    RSPUCodeBranchZF3DEX* temp = (RSPUCodeBranchZF3DEX*)ucode;

    //Branch Display List?
    m_rsp->RSP_BranchLessZ(m_rdp->getHalf1(), temp->vertex, (float)(int)temp->zvalue );
}
