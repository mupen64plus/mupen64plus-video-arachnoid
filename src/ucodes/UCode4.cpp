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

#include "UCode4.h"
#include "UCode0.h"
#include "GBI.h"
#include "RSP.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"

#define F3DWRUS_TRI2        0xB1

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
GBI* UCode4::m_gbi = 0;
RSP* UCode4::m_rsp = 0;   //!< Pointer to Reality Signal Processor 

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode4::UCode4()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode4::~UCode4()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode4::initialize(GBI* gbi, RSP* rsp)
{
    m_gbi = gbi;
    m_rsp = rsp;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode4::initializeGBI()
{
    UCode0::initializeGBI(m_gbi);

    //Init special Wave Race functions
    GBI_SetGBI( GBI::G_VTX,                 F3D_VTX,                 m_gbi->m_cmds,    WaveRace64_Vertex );
    GBI_SetGBI( GBI::G_TRI1,                F3D_TRI1,                m_gbi->m_cmds,    WaveRace64_Tri1 );
    GBI_SetGBI( GBI::G_QUAD,                F3D_QUAD,                m_gbi->m_cmds,    WaveRace64_Quad );
    GBI_SetGBI( GBI::G_TRI2,                F3DWRUS_TRI2,            m_gbi->m_cmds,    WaveRace64_Tri2 );
}

//-----------------------------------------------------------------------------
//! Add Vertices
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode4::WaveRace64_Vertex(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("WaveRace64_Vertex", M64MSG_VERBOSE);
    RSPUCodeAddVerticesWaveRace64* temp = (RSPUCodeAddVerticesWaveRace64*)ucode;

    //Add Vertices
    m_rsp->RSP_Vertex(temp->segmentAddress, temp->numVertices, temp->firstVertexIndex/5);
}

//-----------------------------------------------------------------------------
//! Add one Triangle
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode4::WaveRace64_Tri1(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("WaveRace64_Tri1", M64MSG_VERBOSE);
    RSPUCodeAddOneTriangleF3D* temp = (RSPUCodeAddOneTriangleF3D*)ucode;

    //Add triangle
    m_rsp->RSP_1Triangle(temp->index0 / 5, temp->index1 / 5, temp->index2 / 5 /*,temp->flag,*/ );
}

//-----------------------------------------------------------------------------
//! Add 2 Triangles
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode4::WaveRace64_Tri2(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("WaveRace64_Tri2", M64MSG_VERBOSE);
    RSPUCodeAddTwoTrianglesWaveRace64* temp = (RSPUCodeAddTwoTrianglesWaveRace64*)ucode;

    //Add Two triangles
    m_rsp->RSP_2Triangles( temp->v0 / 5, temp->v1 / 5, temp->v2 / 5, 0,
                           temp->v3 / 5, temp->v4 / 5, temp->v5 / 5, 0);
}

//-----------------------------------------------------------------------------
//! Add Quad
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode4::WaveRace64_Quad(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("WaveRace64_Quad", M64MSG_VERBOSE);
    RSPUCodeAddOneQuadF3D* temp = (RSPUCodeAddOneQuadF3D*)ucode;

    //Add Quad
    m_rsp->RSP_1Quadrangle(temp->index0/5, temp->index1/5, temp->index2/5, temp->index3/5);
}
