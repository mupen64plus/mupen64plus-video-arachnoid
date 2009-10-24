#ifndef REALITY_SIGNAL_PROCESSOR_H_
#define REALITY_SIGNAL_PROCESSOR_H_

#include "ZilmarsPluginSpec.h"
#include "UCodeDefs.h"
#include "RSPMatrixManager.h"
#include "RSPVertexManager.h"
#include "Matrix4.h"

//Forward declarations
class VI;
struct RDPTile;
class RDP;
class Memory;
class DisplayListParser;
class RSPLightManager;
class WindowManager;
class FogManager;

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define	RSP_1ST					0xBF
#define	RSP_TRI1				(RSP_1ST-0)
#define RSP_CULLDL				(RSP_1ST-1)
#define	RSP_POPMTX				(RSP_1ST-2)
#define	RSP_MOVEWORD			(RSP_1ST-3)
#define	RSP_TEXTURE				(RSP_1ST-4)
#define	RSP_SETOTHERMODE_H		(RSP_1ST-5)
#define	RSP_SETOTHERMODE_L		(RSP_1ST-6)
#define RSP_ENDDL				(RSP_1ST-7)
#define RSP_SETGEOMETRYMODE		(RSP_1ST-8)
#define RSP_CLEARGEOMETRYMODE	(RSP_1ST-9)
#define RSP_LINE3D				(RSP_1ST-10)
#define RSP_RDPHALF_1			(RSP_1ST-11)
#define RSP_RDPHALF_2			(RSP_1ST-12)
#define RSP_RDPHALF_CONT		(RSP_1ST-13)

#define RSP_MODIFYVTX			(RSP_1ST-13)
#define RSP_TRI2				(RSP_1ST-14)
#define RSP_BRANCH_Z			(RSP_1ST-15)
#define RSP_LOAD_UCODE			(RSP_1ST-16)

#define RSP_SPRITE2D_SCALEFLIP    (RSP_1ST-1)
#define RSP_SPRITE2D_DRAW         (RSP_1ST-2)

// flags to inhibit pushing of the display list (on branch)
#define RSP_DLIST_PUSH		0x00
#define RSP_DLIST_NOPUSH		0x01

//-----------------------------------------------------------------------------
//! Viewport
//-----------------------------------------------------------------------------
struct Viewport
{
	float vscale[4];
	float vtrans[4];
	float x, y, width, height;
	float nearz, farz;
};


//-----------------------------------------------------------------------------
//! Struct used to store Information about Background Image
//-----------------------------------------------------------------------------
struct BGImageInfo
{
	unsigned long address;  //!< Where texture is stored (in RDRAM)
	unsigned long width;    //!< Width of texture
	unsigned long height;   //!< Height of texture
	unsigned long format;   //!< Format of texture
	unsigned long size;     //!< Size of texture
	unsigned long palette;  //!< What Texture Lookup Table to use
};

//-----------------------------------------------------------------------------
//* RSPTexture
//! Struct used to store information about current texture on rsp
//-----------------------------------------------------------------------------
struct RSPTexture
{
	float scaleS, scaleT;
	long level, on, tile;
};

//-----------------------------------------------------------------------------
//! Singnal Processor Triangle
//-----------------------------------------------------------------------------
typedef SPVertex SPTriangle[3];

//*****************************************************************************
//* RSP
//! Class for emulating the Reality Signal Processor
//*****************************************************************************
class RSP
{
public:
	//Constructor
	RSP();
	//Destructor	
	~RSP();

	//Initialize
	bool initialize(GFX_INFO* graphicsInfo, RDP* rdp, Memory* memory, WindowManager* windowMgr, VI* vi, DisplayListParser* displayListParser, FogManager* fogMgr);
    void dispose();
	void reset();

	void updateGeometryStates();

	//Trigger Interrupt
	void triggerInterrupt();

	void moveSegment(long segmentID, long value);

	void moveMemViewport(unsigned long segmentAddress);

	RDPTile* getTile(int tile) { return m_textureTiles[tile]; }
    void setTile(RDPTile* tile, int index) { m_textureTiles[index] = tile; }
    RSPTexture& getTexture() { return m_texture; }
	bool getTexturesChanged() { return m_texturesChanged; }
	void setTexturesChanged(bool changed) { m_texturesChanged = changed; }

    RSPMatrixManager* getMatrixMgr() { return m_matrixMgr; }
    RSPVertexManager* getVertexMgr() { return m_vertexMgr; }

public:

	//Matrix
	void RSP_Matrix( unsigned int segmentAddress, bool projectionMatrix, bool push, bool replace );
	void RSP_PopMatrix();
	void RSP_PopMatrixN(unsigned long num);
	void RSP_InsertMatrix(unsigned long where, unsigned long num);
	void RSP_DMAMatrix( unsigned long matrix, unsigned char index, unsigned char multiply );
	void RSP_ForceMatrix( unsigned long segmentAddress );
	void RSP_LookAt( unsigned long l );
	void RSP_PerspNormalize( unsigned short scale );	

	//Display List
	void RSP_DisplayList(unsigned long segmentAddress);
	void RSP_DMADisplayList( unsigned long w0, unsigned long w1 );
	void RSP_CullDisplayList( unsigned long v0, unsigned long vn );
	void RSP_BranchList( unsigned long dl );
	void RSP_BranchLessZ( unsigned long branchdl, unsigned long vtx, float zval );	
	void RSP_EndDisplayList();

	//Light
	void RSP_Light( unsigned long lightIndex, unsigned long segmentAddress  );
	void RSP_NumLights( long n );
	void RSP_LightColor( unsigned long lightIndex, unsigned long packedColor );	

	//Vertices
	void RSP_Vertex(unsigned long segmentAddress, unsigned long numVertices, unsigned long firstVertexIndex);
	void RSP_CIVertex(unsigned long segmentAddress, unsigned long numVertices, unsigned long firstVertexIndex);
	void RSP_ModifyVertex( unsigned long vtx, unsigned long where, unsigned long val );	
	void RSP_SetVertexColor( unsigned long vtx, float r, float g, float b, float a);	
	void RSP_SetVertexTexCoord( unsigned long vtx, float s, float t);	

	void RSP_DMAVertex( unsigned long v, unsigned long n, unsigned long v0 );
	void RSP_SetDMAOffsets( unsigned long mtxoffset, unsigned long vtxoffset );
	void RSP_SetVertexColorBase(unsigned long segmentAddress);

	//Indices
	void RSP_1Triangle( long v00, long v01, long v02 );
	void RSP_2Triangles( long v00, long v01, long v02, long flag0, 
						long v10, long v11, long v12, long flag1 );
	void RSP_4Triangles( long v00, long v01, long v02,
						long v10, long v11, long v12,
						long v20, long v21, long v22,
						long v30, long v31, long v32 );
	void RSP_DMATriangles( unsigned long tris, unsigned long n );
	void RSP_1Quadrangle( long v0, long v1, long v2, long v4 );

	//Object
	void RSP_ObjRectangle( unsigned long sp );
	void RSP_ObjSprite( unsigned long sp );
	void RSP_ObjLoadTxtr( unsigned long tx );
	void RSP_ObjLoadTxSprite( unsigned long txsp );
	void RSP_ObjLoadTxRectR( unsigned long txsp );
	void RSP_ObjMatrix( unsigned long mtx );
	void RSP_ObjSubMatrix( unsigned long mtx );

	//Rendering
	void RSP_Line3D( long v0, long v1, long flag );
	void RSP_LineW3D( long v0, long v1, long wd, long flag );
	void RSP_BgRect1Cyc( unsigned long bg );
	void RSP_BgRectCopy( unsigned long bg );
	void RSP_Sprite2DBase( unsigned long base );

	//States
	void RSP_GeometryMode( unsigned long clear, unsigned long set );
	void RSP_SetGeometryMode( unsigned long mode );
	void RSP_ClearGeometryMode( unsigned long mode );

	//Clipping	
	void RSP_ClipRatio( unsigned long r );

	//Texturing
	void RSP_Texture( float sc, float tc, long level, long tile, long on );

	//Fog
	void RSP_FogFactor( short fm, short fo );	

	//UCode
    void RSP_LoadUcodeEx( unsigned long uc_start, unsigned long uc_dstart, unsigned short uc_dsize );

private:

	//Pointers to big objects and managers
	GFX_INFO*          m_graphicsInfo;       //!< Access to emulator data (like RDRAM ...)
	WindowManager*     m_windowMgr;          //!< Handles window with menus and statusbar
	VI*                m_vi;                 //!< Videointerface
	Memory*            m_memory;             //!< Memory managers (handles RDRAM, Texture Memory...)
	DisplayListParser* m_displayListParser;  //!< Display list parser
	FogManager*        m_fogMgr;             //!< Manager that handles fog
	RDP*               m_rdp;                //!< Pointer to Reality Drawing Processor 

	//Helper managers
	RSPMatrixManager* m_matrixMgr;           //!< Handles matrix stack
	RSPVertexManager* m_vertexMgr;           //!< Vertex Manager, processes and modifies vertices 
	RSPLightManager*  m_lightMgr;            //!< Light Manager, handles lights

	//Geometry Mode
	unsigned long m_geometryMode;  //!< Contains states (lighting, shading, culling...)

	//Textures
	RSPTexture m_texture;  
	RDPTile* m_textureTiles[2];
	bool m_texturesChanged;

	//Viewport
	Viewport m_viewport;

};

#endif