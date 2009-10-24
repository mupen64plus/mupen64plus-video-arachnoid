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
#include "WindowManager.h"
#include "RSPLightManager.h"
#include "FogManager.h"
#include "DisplayListParser.h"
#include "MathLib.h"
#include "MathLib.h"

#define MI_INTR_SP                 0x00000001  //!< RSP Interrupt signal

//Geometry Mode Definitions
#define G_ZBUFFER				0x00000001
#define G_SHADE					0x00000004
#define G_FOG					0x00010000
#define G_LIGHTING				0x00020000
#define G_TEXTURE_GEN			0x00040000
#define G_TEXTURE_GEN_LINEAR	0x00080000
#define G_LOD					0x00100000

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
bool RSP::initialize(GFX_INFO* graphicsInfo, RDP* rdp, Memory* memory, WindowManager* windowMgr, VI* vi, DisplayListParser* dlp, FogManager* fogMgr)
{
	//Save pointers
	m_graphicsInfo = graphicsInfo;
	m_rdp = rdp;
	m_windowMgr = windowMgr;
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
	bool cullFront		   = (m_geometryMode & GBI::G_CULL_FRONT    ) != 0;
	bool cullBack		   = (m_geometryMode & GBI::G_CULL_BACK     ) != 0;
	bool shade			   = (m_geometryMode & G_SHADE              ) != 0;
	bool shadeSmooth	   = (m_geometryMode & GBI::G_SHADING_SMOOTH) != 0;
	bool fog			   = (m_geometryMode & G_FOG                ) != 0;
	bool textureGen	       = (m_geometryMode & G_TEXTURE_GEN        ) != 0;
	bool linearTextureGen  = (m_geometryMode & G_TEXTURE_GEN_LINEAR ) != 0;
	bool lighting          = (m_geometryMode & G_LIGHTING           ) != 0;
	bool zBuffer		   = (m_geometryMode & G_ZBUFFER            ) != 0;
	bool clipping          = (m_geometryMode & GBI::G_CLIPPING      ) != 0;

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
void RSP::moveSegment(long segmentID, long value)
{
	m_memory->setSegment(segmentID, value);
}

//-----------------------------------------------------------------------------
// Set Viewport
// FIXME
//-----------------------------------------------------------------------------
void RSP::moveMemViewport(unsigned long segmentAddress)
{
	//Get Adress
	unsigned long rdramAddress = m_memory->getRDRAMAddress(segmentAddress);

	//Error controll
	if ( rdramAddress + 16 > m_memory->getRDRAMSize() )
	{
		Logger::getSingleton().printMsg("ERROR: MoveMem Viewport, accessed invalid memory");
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

	m_viewport.x	  = m_viewport.vtrans[0] - m_viewport.vscale[0];
	m_viewport.y      = m_viewport.vtrans[1] - m_viewport.vscale[1];
	m_viewport.width  = m_viewport.vscale[0] * 2;
	m_viewport.height = m_viewport.vscale[1] * 2;
	m_viewport.nearz  = m_viewport.vtrans[2] - m_viewport.vscale[2];
	m_viewport.farz	  = (m_viewport.vtrans[2] + m_viewport.vscale[2]) ;

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
void RSP::RSP_LoadUcodeEx( unsigned long uc_start, unsigned long uc_dstart, unsigned short uc_dsize )
{
	Logger::getSingleton().printMsg("RSP_LoadUcodeEx - Unimplemented", LML_CRITICAL);
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
void RSP::RSP_DMAMatrix( unsigned long matrix, unsigned char index, unsigned char multiply )
{
    m_matrixMgr->DMAMatrix(m_memory->getRDRAMAddress(matrix), index, multiply);
}

//-----------------------------------------------------------------------------
// RSP Force Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_ForceMatrix( unsigned long segmentAddress )
{ 
	// Logger::getSingleton().printMsg("RSP_ForceMatrix", LML_CRITICAL);
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
void RSP::RSP_PopMatrixN( unsigned long num )
{ 
	m_matrixMgr->popMatrixN(num);
}

//-----------------------------------------------------------------------------
// Insert Matrix
//-----------------------------------------------------------------------------
void RSP::RSP_InsertMatrix(unsigned long where, unsigned long num)
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
void RSP::RSP_Texture( float scaleS, float scaleT, long level, long tile, long on )
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
void RSP::RSP_SetDMAOffsets( unsigned long mtxoffset, unsigned long vtxoffset )
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
void RSP::RSP_Light( unsigned long lightIndex, unsigned long segmentAddress )
{ 
    m_lightMgr->setLight(lightIndex, m_memory->getRDRAMAddress(segmentAddress) );
}

//-----------------------------------------------------------------------------
// Num Lights
//-----------------------------------------------------------------------------
void RSP::RSP_NumLights( long numLights )
{ 
	m_lightMgr->setNumLights(numLights);
}

//-----------------------------------------------------------------------------
// Light Color
//-----------------------------------------------------------------------------
void RSP::RSP_LightColor( unsigned long lightIndex, unsigned long packedColor )
{
	m_lightMgr->setLightColor(lightIndex, packedColor);
}

//*****************************************************************************
// Vertex
//*****************************************************************************

//-----------------------------------------------------------------------------
// Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_Vertex( unsigned long segmentAddress, unsigned long numVertices, unsigned long firstVertexIndex  )
{ 
	m_vertexMgr->setVertices(m_memory->getRDRAMAddress(segmentAddress), numVertices, firstVertexIndex);
}

//-----------------------------------------------------------------------------
// Modify Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_ModifyVertex( unsigned long vtx, unsigned long where, unsigned long val )
{
	m_vertexMgr->modifyVertex(vtx, where, val);
}

void RSP::RSP_SetVertexColor( unsigned long vtx, float r, float g, float b, float a)
{
	m_vertexMgr->setVertexColor(vtx, r,g,b,a);
}

void RSP::RSP_SetVertexTexCoord( unsigned long vtx, float s, float t)
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
void RSP::RSP_CIVertex(unsigned long segmentAddress, unsigned long numVertices, unsigned long firstVertexIndex )
{	
	m_vertexMgr->ciVertex(segmentAddress, numVertices, firstVertexIndex);
}

//-----------------------------------------------------------------------------
// DMA Vertex
//-----------------------------------------------------------------------------
void RSP::RSP_DMAVertex( unsigned long v, unsigned long n, unsigned long v0 )
{
	m_vertexMgr->DMAVertex(v, n, v0);
}

//-----------------------------------------------------------------------------
// Set Vertex Color Base
//-----------------------------------------------------------------------------
void RSP::RSP_SetVertexColorBase(unsigned long segmentAddress)
{
	m_vertexMgr->setVertexColorBase( m_memory->getRDRAMAddress(segmentAddress) );
}

//*****************************************************************************
// Display List
//*****************************************************************************

//-----------------------------------------------------------------------------
// Display List
//-----------------------------------------------------------------------------
void RSP::RSP_DisplayList(unsigned long segmentAddress)
{
	m_displayListParser->displayList(segmentAddress);
}

//-----------------------------------------------------------------------------
// DMA Display List
//-----------------------------------------------------------------------------
void RSP::RSP_DMADisplayList( unsigned long w0, unsigned long w1 )
{ 
    m_displayListParser->DMADisplayList(w0, w1);
}

//-----------------------------------------------------------------------------
// Branch Display List
//-----------------------------------------------------------------------------
void RSP::RSP_BranchList( unsigned long dl )
{ 
	m_displayListParser->branchDisplayList(dl);
}

//-----------------------------------------------------------------------------
// Branch Display List Z
//-----------------------------------------------------------------------------
void RSP::RSP_BranchLessZ( unsigned long branchdl, unsigned long vtx, float zval )
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
void RSP::RSP_CullDisplayList( unsigned long v0, unsigned long vn )
{
	//Logger::getSingleton().printMsg("RSP_CullDisplayList - Unimplemented", LML_CRITICAL);
	//TODO
}

//*****************************************************************************
// Indices
//*****************************************************************************

//-----------------------------------------------------------------------------
// 1 Triangle
//-----------------------------------------------------------------------------
void RSP::RSP_1Triangle( long v0, long v1, long v2)
{ 
	m_vertexMgr->add1Triangle(v0, v1, v2);
}

//-----------------------------------------------------------------------------
// 2 Trangles
//-----------------------------------------------------------------------------
void RSP::RSP_2Triangles( long v00, long v01, long v02, long flag0, 
					long v10, long v11, long v12, long flag1 )
{			
	m_vertexMgr->add1Triangle(v00, v01, v02);		
	m_vertexMgr->add1Triangle(v10, v11, v12);
}

//-----------------------------------------------------------------------------
// 4 Triangles
//-----------------------------------------------------------------------------
void RSP::RSP_4Triangles( long v00, long v01, long v02,
					long v10, long v11, long v12,
					long v20, long v21, long v22,
					long v30, long v31, long v32 )
{
	m_vertexMgr->add1Triangle(v00, v01, v02);
	m_vertexMgr->add1Triangle(v10, v11, v12);
	m_vertexMgr->add1Triangle(v20, v21, v22);
	m_vertexMgr->add1Triangle(v30, v31, v32);
}

//-----------------------------------------------------------------------------
// DMA Triangel
//-----------------------------------------------------------------------------
void RSP::RSP_DMATriangles( unsigned long tris, unsigned long n )
{ 
    m_vertexMgr->addDMATriangles(tris, n);
}

//-----------------------------------------------------------------------------
// Quadrangle
//-----------------------------------------------------------------------------
void RSP::RSP_1Quadrangle( long v0, long v1, long v2, long v3 )
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
void RSP::RSP_GeometryMode(unsigned long clear, unsigned long set)
{ 
	RSP_ClearGeometryMode(clear);
	RSP_SetGeometryMode(set);	
}

//-----------------------------------------------------------------------------
// Set Geometry Mode
//-----------------------------------------------------------------------------
void RSP::RSP_SetGeometryMode( unsigned long mode )
{ 
	m_geometryMode |= mode;
	updateGeometryStates();
}

//-----------------------------------------------------------------------------
// Clear Geometry Mode
//-----------------------------------------------------------------------------
void RSP::RSP_ClearGeometryMode( unsigned long mode )
{
	m_geometryMode &= ~mode;
	updateGeometryStates();
}

//*****************************************************************************
// Other
//*****************************************************************************

void RSP::RSP_Line3D( long v0, long v1, long flag )
{
	Logger::getSingleton().printMsg("RSP_Line3D - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_LineW3D( long v0, long v1, long wd, long flag )
{
	Logger::getSingleton().printMsg("RSP_LineW3D - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjRectangle( unsigned long sp )
{
	Logger::getSingleton().printMsg("RSP_ObjRectangle - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjSprite( unsigned long sp )
{
	Logger::getSingleton().printMsg("RSP_ObjSprite - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjLoadTxtr( unsigned long tx )
{
	Logger::getSingleton().printMsg("RSP_ObjLoadTxtr - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjLoadTxSprite( unsigned long txsp )
{
	Logger::getSingleton().printMsg("RSP_ObjLoadTxSprite - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjLoadTxRectR( unsigned long txsp )
{
	Logger::getSingleton().printMsg("RSP_ObjLoadTxRectR - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_BgRect1Cyc( unsigned long bg )
{
	Logger::getSingleton().printMsg("RSP_BgRect1Cyc - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_BgRectCopy( unsigned long bg )
{
	Logger::getSingleton().printMsg("RSP_BgRectCopy - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjMatrix( unsigned long mtx )
{ 
	Logger::getSingleton().printMsg("RSP_ObjMatrix - Unimplemented", LML_CRITICAL);
}
void RSP::RSP_ObjSubMatrix( unsigned long mtx )
{
	Logger::getSingleton().printMsg("RSP_ObjSubMatrix - Unimplemented", LML_CRITICAL);
}

//*****************************************************************************
// Non important functions
//*****************************************************************************

void RSP::RSP_Sprite2DBase( unsigned long base ) {
	Logger::getSingleton().printMsg("RSP_Sprite2DBase - Unimplemented", LML_CRITICAL);
}

void RSP::RSP_LookAt( unsigned long l ) {
	Logger::getSingleton().printMsg("RSP_LookAt - Unimplemented", LML_CRITICAL);
}

void RSP::RSP_ClipRatio( unsigned long r ) {
	Logger::getSingleton().printMsg("RSP_ClipRatio - Unimplemented", LML_CRITICAL);
}

void RSP::RSP_PerspNormalize( unsigned short scale ) {
	Logger::getSingleton().printMsg("RSP_PerspNormalize - Unimplemented", LML_CRITICAL);
}