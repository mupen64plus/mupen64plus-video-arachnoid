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

#include "RSP.h"
#include "../UCodeDefs.h"
#include "../RDP/RDP.h"
#include <cmath>
#include "GBIDefs.h"
#include "VI.h"
#include "Memory.h"
#include "Logger.h"
#include "OpenGLRenderer.h"
#include "OpenGLManager.h"
#include "RSPLightManager.h"
#include "FogManager.h"
#include "DisplayListParser.h"
#include "MathLib.h"
#include "MathLib.h"

#define MI_INTR_SP                 0x00000001  //!< RSP Interrupt signal

//Geometry Mode Definitions
#define G_ZBUFFER                0x00000001
#define G_SHADE                    0x00000004
#define G_FOG                    0x00010000
#define G_LIGHTING                0x00020000
#define G_TEXTURE_GEN            0x00040000
#define G_TEXTURE_GEN_LINEAR    0x00080000
#define G_LOD                    0x00100000

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RSP::RSP()
{
    m_texturesChanged = false;
    m_matrixMgr = 0;
    m_vertexMgr = 0;
    m_lightMgr  = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RSP::~RSP()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool RSP::initialize(GFX_INFO* graphicsInfo, RDP* rdp, Memory* memory, VI* vi, DisplayListParser* dlp, FogManager* fogMgr)
{
    //Save pointers
    m_graphicsInfo = graphicsInfo;
    m_rdp = rdp;
    m_vi = vi;
    m_memory = memory;
    m_displayListParser = dlp;
    m_fogMgr = fogMgr;       

    //Initialize Matrix Manager
    m_matrixMgr = new RSPMatrixManager();
    if ( !m_matrixMgr->initialize(m_memory) ) {
        return false;
    }

    //Initialzie Light Manager
    m_lightMgr = new RSPLightManager();
    if ( !m_lightMgr->initialize(m_memory) ) {
        return false;
    }

    //Initialzie Vertex Manager
    m_vertexMgr = new RSPVertexManager();
    if ( !m_vertexMgr->initialize(&OpenGLManager::getSingleton(), m_memory, m_matrixMgr, m_lightMgr) ) {
        return false;
    }

    m_textureTiles[0] = m_rdp->getTile(0);
    m_textureTiles[1] = m_rdp->getTile(1);

    return true;
}

//-----------------------------------------------------------------------------
//* Dispose
//-----------------------------------------------------------------------------
void RSP::dispose()
{
    if ( m_matrixMgr ) { delete m_matrixMgr; m_matrixMgr = 0; }
    if ( m_vertexMgr ) { delete m_vertexMgr; m_vertexMgr = 0; }
    if ( m_lightMgr  ) { delete m_lightMgr ; m_lightMgr  = 0; }    
}

//-----------------------------------------------------------------------------
//* Update Geometry States
//-----------------------------------------------------------------------------
void RSP::updateGeometryStates()
{
    bool cullFront           = (m_geometryMode & GBI::G_CULL_FRONT    ) != 0;
    bool cullBack            = (m_geometryMode & GBI::G_CULL_BACK     ) != 0;
    bool shade               = (m_geometryMode & G_SHADE              ) != 0;
    bool shadeSmooth         = (m_geometryMode & GBI::G_SHADING_SMOOTH) != 0;
    bool fog                 = (m_geometryMode & G_FOG                ) != 0;
    bool textureGen          = (m_geometryMode & G_TEXTURE_GEN        ) != 0;
    bool linearTextureGen    = (m_geometryMode & G_TEXTURE_GEN_LINEAR ) != 0;
    bool lighting            = (m_geometryMode & G_LIGHTING           ) != 0;
    bool zBuffer             = (m_geometryMode & G_ZBUFFER            ) != 0;
    bool clipping            = (m_geometryMode & GBI::G_CLIPPING      ) != 0;

    //Update states
    m_lightMgr->setLightEnabled(lighting);
    m_vertexMgr->setTexCoordGenType( textureGen ? TCGT_LINEAR : TCGT_NONE);
    OpenGLManager::getSingleton().setZBufferEnabled(zBuffer);
    OpenGLManager::getSingleton().setCullMode(cullFront, cullBack);
    OpenGLManager::getSingleton().setFogEnabled(fog);
}

//-----------------------------------------------------------------------------
// Reset
//-----------------------------------------------------------------------------
void RSP::reset()
{
    m_matrixMgr->resetMatrices();
}

//-----------------------------------------------------------------------------
//* Trigger Interupt
//-----------------------------------------------------------------------------
void RSP::triggerInterrupt()
{
    *(m_graphicsInfo->MI_INTR_REG) |= MI_INTR_SP;
    m_graphicsInfo->CheckInterrupts();    
}

//-----------------------------------------------------------------------------
//* Move Segment
//-----------------------------------------------------------------------------
void RSP::moveSegment(int segmentID, int value)
{
    m_memory->setSegment(segmentID, value);
}

//-----------------------------------------------------------------------------
// Set Viewport
// FIXME
//-----------------------------------------------------------------------------
void RSP::moveMemViewport(unsigned int segmentAddress)
{
    //Get Adress
    unsigned int rdramAddress = m_memory->getRDRAMAddress(segmentAddress);

    //Error controll
    if ( rdramAddress + 16 > m_memory->getRDRAMSize() )
    {
        Logger::getSingleton().printMsg("MoveMem Viewport, accessed invalid memory", M64MSG_ERROR);
        return;
    }

    m_viewport.vscale[0] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress +  2), 2 );
    m_viewport.vscale[1] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress     ), 2 );
    m_viewport.vscale[2] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress +  6), 10 );// * 0.00097847357f;
    m_viewport.vscale[3] = *(short*)m_memory->getRDRAM(rdramAddress +  4);
    m_viewport.vtrans[0] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress + 10), 2 );
    m_viewport.vtrans[1] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress +  8), 2 );
    m_viewport.vtrans[2] = _FIXED2FLOAT( *(short*)m_memory->getRDRAM(rdramAddress + 14), 10 );// * 0.00097847357f;
    m_viewport.vtrans[3] = *(short*)m_memory->getRDRAM(rdramAddress + 12);

    m_viewport.x      = m_viewport.vtrans[0] - m_viewport.vscale[0];
    m_viewport.y      = m_viewport.vtrans[1] - m_viewport.vscale[1];
    m_viewport.width  = m_viewport.vscale[0] * 2;
    m_viewport.height = m_viewport.vscale[1] * 2;
    m_viewport.nearz  = m_viewport.vtrans[2] - m_viewport.vscale[2];
    m_viewport.farz   = (m_viewport.vtrans[2] + m_viewport.vscale[2]) ;

    /*
    //Set Viewport
    OpenGLManager::getSingleton().setViewport( 
            m_viewport.x, // * OGL.scaleX, 
            m_viewport.y, //(VI.height - (gSP.viewport.y + gSP.viewport.height)) * OGL.scaleY + OGL.heightOffset, 
            //(m_vi->getHeight() - (m_viewport.y + m_viewport.height)),
            m_viewport.width, // * OGL.scaleX, 
            m_viewport.height, // * OGL.scaleY,
            0.0f,   //m_viewport.nearz, 
            1.0f ); //m_viewport.farz );            
            */
}

//-----------------------------------------------------------------------------
//* Load UCode Ex
//! @todo Change ucode while running
//-----------------------------------------------------------------------------
void RSP::RSP_LoadUcodeEx( unsigned int uc_start, unsigned int uc_dstart, unsigned short uc_dsize )
{
    Logger::getSingleton().printMsg("RSP_LoadUcodeEx - Unimplemented", M64MSG_WARNING);
    //TODO or skip

    /*

    RSP.PCi = 0;
    gSP.matrix.modelViewi = 0;
    gSP.changed |= CHANGED_MATRIX;
    gSP.status[0] = gSP.status[1] = gSP.status[2] = gSP.status[3] = 0;

    if ((((uc_start & 0x1FFFFFFF) + 4096) > RDRAMSize) || (((uc_dstart & 0x1FFFFFFF) + uc_dsize) > RDRAMSize))
    {
            return;
    }

    MicrocodeInfo *ucode = GBI_DetectMicrocode( uc_start, uc_dstart, uc_dsize );

    if (ucode->type != NONE)
        GBI_MakeCurrent( ucode );
    else
        SetEvent( RSP.threadMsg[RSPMSG_CLOSE] );
    */
}

//*****************************************************************************
// Matrix Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
// RSP Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_Matrix( unsigned int segmentAddress, bool projectionMatrix, bool push, bool replace )
{ 
    Logger::getSingleton().printMsg("RSP_Matrix");
    m_matrixMgr->addMatrix( segmentAddress,             //Segment adress 
                            projectionMatrix,           //Projection or view matrix?
                            push,                       //Save Current Matrix?
                            replace );                  //Replace aka Load or Mult 
}

//-----------------------------------------------------------------------------
// RSP DMA Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_DMAMatrix( unsigned int matrix, unsigned char index, unsigned char multiply )
{
    m_matrixMgr->DMAMatrix(m_memory->getRDRAMAddress(matrix), index, multiply);
}

//-----------------------------------------------------------------------------
// RSP Force Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_ForceMatrix( unsigned int segmentAddress )
{ 
    // Logger::getSingleton().printMsg("RSP_ForceMatrix", M64MSG_WARNING);
    m_matrixMgr->ForceMatrix( m_memory->getRDRAMAddress(segmentAddress));
}

//-----------------------------------------------------------------------------
// Pop Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_PopMatrix( )
{
    m_matrixMgr->popMatrix();
}

//-----------------------------------------------------------------------------
//* Pop Matrix N
//! Pop Matrix from stack N number of times
//! @param num The number of matrices to pop from matrix-stack
//-----------------------------------------------------------------------------
void RSP::RSP_PopMatrixN( unsigned int num )
{ 
    m_matrixMgr->popMatrixN(num);
}

//-----------------------------------------------------------------------------
// Insert Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_InsertMatrix(unsigned int where, unsigned int num)
{
    m_matrixMgr->insertMatrix(where, num);
}

//*****************************************************************************
// Misc
//*****************************************************************************

//-----------------------------------------------------------------------------
// Fog Factor
//-----------------------------------------------------------------------------
void RSP::RSP_FogFactor(short fogMultiplier, short fogOffset)
{
    m_fogMgr->setFogSettings((float)fogMultiplier, (float)fogOffset);
}

//-----------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------
void RSP::RSP_Texture( float scaleS, float scaleT, int level, int tile, int on )
{ 
    //Set Texture
    m_texture.scaleS = (scaleS != 0.0f) ? scaleS : 1.0f;
    m_texture.scaleT = (scaleT != 0.0f) ? scaleT : 1.0f;
    m_texture.level  = level;
    m_texture.on     = on;
    m_texture.tile   = tile;

    //Set Tiles (note: There are max 8 tiles)    
    if ( tile < 7 )
    {
        m_textureTiles[0] = m_rdp->getTile(tile);
        m_textureTiles[1] = m_rdp->getTile(tile+1);
    }
    else
    {
        m_textureTiles[0] = m_rdp->getTile(tile);
        m_textureTiles[1] = m_rdp->getTile(tile);
    }

    m_texturesChanged = true;
}

//-----------------------------------------------------------------------------
// Set DMA Offsets
//-----------------------------------------------------------------------------
void RSP::RSP_SetDMAOffsets( unsigned int mtxoffset, unsigned int vtxoffset )
{
    m_matrixMgr->setRDRAMOffset(mtxoffset);
    m_vertexMgr->setRDRAMOffset(vtxoffset);
}

//*****************************************************************************
// Light
//*****************************************************************************

//-----------------------------------------------------------------------------
// RSP Light
//-----------------------------------------------------------------------------
void RSP::RSP_Light( unsigned int lightIndex, unsigned int segmentAddress )
{ 
    m_lightMgr->setLight(lightIndex, m_memory->getRDRAMAddress(segmentAddress) );
}

//-----------------------------------------------------------------------------
// Num Lights
//-----------------------------------------------------------------------------
void RSP::RSP_NumLights( int numLights )
{ 
    m_lightMgr->setNumLights(numLights);
}

//-----------------------------------------------------------------------------
// Light Color
//-----------------------------------------------------------------------------
void RSP::RSP_LightColor( unsigned int lightIndex, unsigned int packedColor )
{
    m_lightMgr->setLightColor(lightIndex, packedColor);
}

//*****************************************************************************
// Vertex
//*****************************************************************************

//-----------------------------------------------------------------------------
// Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_Vertex( unsigned int segmentAddress, unsigned int numVertices, unsigned int firstVertexIndex  )
{ 
    m_vertexMgr->setVertices(m_memory->getRDRAMAddress(segmentAddress), numVertices, firstVertexIndex);
}

//-----------------------------------------------------------------------------
// Modify Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_ModifyVertex( unsigned int vtx, unsigned int where, unsigned int val )
{
    m_vertexMgr->modifyVertex(vtx, where, val);
}

void RSP::RSP_SetVertexColor( unsigned int vtx, float r, float g, float b, float a)
{
    m_vertexMgr->setVertexColor(vtx, r,g,b,a);
}

void RSP::RSP_SetVertexTexCoord( unsigned int vtx, float s, float t)
{
    m_vertexMgr->setVertexTextureCoord(vtx, s,t);
}

//-----------------------------------------------------------------------------
//! Color Index Vertex
//! param segmentAddress Address to register where there is an RDRAM address 
//!                      used to retrieve vertices from RDRAM.
//! param numVertices Number of vertices to retrive from RDRAM.
//! param firstVertexIndex Index of first vertex
//-----------------------------------------------------------------------------
void RSP::RSP_CIVertex(unsigned int segmentAddress, unsigned int numVertices, unsigned int firstVertexIndex )
{    
    m_vertexMgr->ciVertex(segmentAddress, numVertices, firstVertexIndex);
}

//-----------------------------------------------------------------------------
// DMA Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_DMAVertex( unsigned int v, unsigned int n, unsigned int v0 )
{
    m_vertexMgr->DMAVertex(v, n, v0);
}

//-----------------------------------------------------------------------------
// Set Vertex Color Base
//-----------------------------------------------------------------------------
void RSP::RSP_SetVertexColorBase(unsigned int segmentAddress)
{
    m_vertexMgr->setVertexColorBase( m_memory->getRDRAMAddress(segmentAddress) );
}

//*****************************************************************************
// Display List
//*****************************************************************************

//-----------------------------------------------------------------------------
// Display List
//-----------------------------------------------------------------------------
void RSP::RSP_DisplayList(unsigned int segmentAddress)
{
    m_displayListParser->displayList(segmentAddress);
}

//-----------------------------------------------------------------------------
// DMA Display List
//-----------------------------------------------------------------------------
void RSP::RSP_DMADisplayList( unsigned int w0, unsigned int w1 )
{ 
    m_displayListParser->DMADisplayList(w0, w1);
}

//-----------------------------------------------------------------------------
// Branch Display List
//-----------------------------------------------------------------------------
void RSP::RSP_BranchList( unsigned int dl )
{ 
    m_displayListParser->branchDisplayList(dl);
}

//-----------------------------------------------------------------------------
// Branch Display List Z
//-----------------------------------------------------------------------------
void RSP::RSP_BranchLessZ( unsigned int branchdl, unsigned int vtx, float zval )
{ 
    if ( m_vertexMgr->getVertex(vtx)->z <= zval ) {
        m_displayListParser->branchDisplayList(branchdl);
    }
}

//-----------------------------------------------------------------------------
// End Display List
//-----------------------------------------------------------------------------
void RSP::RSP_EndDisplayList() 
{ 
    m_displayListParser->endDisplayList();
}

//-----------------------------------------------------------------------------
//* Cull Display List
//! @todo Cull Display List
//-----------------------------------------------------------------------------
void RSP::RSP_CullDisplayList( unsigned int v0, unsigned int vn )
{
    //Logger::getSingleton().printMsg("RSP_CullDisplayList - Unimplemented", M64MSG_WARNING);
    //TODO
}

//*****************************************************************************
// Indices
//*****************************************************************************

//-----------------------------------------------------------------------------
// 1 Triangle
//-----------------------------------------------------------------------------
void RSP::RSP_1Triangle( int v0, int v1, int v2)
{ 
    m_vertexMgr->add1Triangle(v0, v1, v2);
}

//-----------------------------------------------------------------------------
// 2 Trangles
//-----------------------------------------------------------------------------
void RSP::RSP_2Triangles( int v00, int v01, int v02, int flag0, 
                    int v10, int v11, int v12, int flag1 )
{            
    m_vertexMgr->add1Triangle(v00, v01, v02);        
    m_vertexMgr->add1Triangle(v10, v11, v12);
}

//-----------------------------------------------------------------------------
// 4 Triangles
//-----------------------------------------------------------------------------
void RSP::RSP_4Triangles( int v00, int v01, int v02,
                    int v10, int v11, int v12,
                    int v20, int v21, int v22,
                    int v30, int v31, int v32 )
{
    m_vertexMgr->add1Triangle(v00, v01, v02);
    m_vertexMgr->add1Triangle(v10, v11, v12);
    m_vertexMgr->add1Triangle(v20, v21, v22);
    m_vertexMgr->add1Triangle(v30, v31, v32);
}

//-----------------------------------------------------------------------------
// DMA Triangel
//-----------------------------------------------------------------------------
void RSP::RSP_DMATriangles( unsigned int tris, unsigned int n )
{ 
    m_vertexMgr->addDMATriangles(tris, n);
}

//-----------------------------------------------------------------------------
// Quadrangle
//-----------------------------------------------------------------------------
void RSP::RSP_1Quadrangle( int v0, int v1, int v2, int v3 )
{
    m_vertexMgr->add1Triangle(v0, v1, v2);
    m_vertexMgr->add1Triangle(v0, v2, v3);
}

//*****************************************************************************
// Geometry Mode
//*****************************************************************************

//-----------------------------------------------------------------------------
// Geometry Mode
//-----------------------------------------------------------------------------
void RSP::RSP_GeometryMode(unsigned int clear, unsigned int set)
{ 
    RSP_ClearGeometryMode(clear);
    RSP_SetGeometryMode(set);    
}

//-----------------------------------------------------------------------------
// Set Geometry Mode
//-----------------------------------------------------------------------------
void RSP::RSP_SetGeometryMode( unsigned int mode )
{ 
    m_geometryMode |= mode;
    updateGeometryStates();
}

//-----------------------------------------------------------------------------
// Clear Geometry Mode
//-----------------------------------------------------------------------------
void RSP::RSP_ClearGeometryMode( unsigned int mode )
{
    m_geometryMode &= ~mode;
    updateGeometryStates();
}

//*****************************************************************************
// Other
//*****************************************************************************

void RSP::RSP_Line3D( int v0, int v1, int flag )
{
    Logger::getSingleton().printMsg("RSP_Line3D - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_LineW3D( int v0, int v1, int wd, int flag )
{
    Logger::getSingleton().printMsg("RSP_LineW3D - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjRectangle( unsigned int sp )
{
    Logger::getSingleton().printMsg("RSP_ObjRectangle - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjSprite( unsigned int sp )
{
    Logger::getSingleton().printMsg("RSP_ObjSprite - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjLoadTxtr( unsigned int tx )
{
    Logger::getSingleton().printMsg("RSP_ObjLoadTxtr - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjLoadTxSprite( unsigned int txsp )
{
    Logger::getSingleton().printMsg("RSP_ObjLoadTxSprite - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjLoadTxRectR( unsigned int txsp )
{
    Logger::getSingleton().printMsg("RSP_ObjLoadTxRectR - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_BgRect1Cyc( unsigned int bg )
{
    Logger::getSingleton().printMsg("RSP_BgRect1Cyc - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_BgRectCopy( unsigned int bg )
{
    Logger::getSingleton().printMsg("RSP_BgRectCopy - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjMatrix( unsigned int mtx )
{ 
    Logger::getSingleton().printMsg("RSP_ObjMatrix - Unimplemented", M64MSG_WARNING);
}
void RSP::RSP_ObjSubMatrix( unsigned int mtx )
{
    Logger::getSingleton().printMsg("RSP_ObjSubMatrix - Unimplemented", M64MSG_WARNING);
}

//*****************************************************************************
// Non important functions
//*****************************************************************************

void RSP::RSP_Sprite2DBase( unsigned int base ) {
    Logger::getSingleton().printMsg("RSP_Sprite2DBase - Unimplemented", M64MSG_WARNING);
}

void RSP::RSP_LookAt( unsigned int l ) {
    Logger::getSingleton().printMsg("RSP_LookAt - Unimplemented", M64MSG_WARNING);
}

void RSP::RSP_ClipRatio( unsigned int r ) {
    Logger::getSingleton().printMsg("RSP_ClipRatio - Unimplemented", M64MSG_WARNING);
}

void RSP::RSP_PerspNormalize( unsigned short scale ) {
    Logger::getSingleton().printMsg("RSP_PerspNormalize - Unimplemented", M64MSG_WARNING);
}
