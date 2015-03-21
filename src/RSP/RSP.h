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

#ifndef REALITY_SIGNAL_PROCESSOR_H_
#define REALITY_SIGNAL_PROCESSOR_H_

#define M64P_PLUGIN_PROTOTYPES 1
#include "Matrix4.h"
#include "RSPMatrixManager.h"
#include "RSPVertexManager.h"
#include "UCodeDefs.h"
#include "m64p_plugin.h"

//Forward declarations
class VI;
struct RDPTile;
class RDP;
class Memory;
class DisplayListParser;
class RSPLightManager;
class FogManager;

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define RSP_1ST                 0xBF
#define RSP_TRI1                (RSP_1ST-0)
#define RSP_CULLDL              (RSP_1ST-1)
#define RSP_POPMTX              (RSP_1ST-2)
#define RSP_MOVEWORD            (RSP_1ST-3)
#define RSP_TEXTURE             (RSP_1ST-4)
#define RSP_SETOTHERMODE_H      (RSP_1ST-5)
#define RSP_SETOTHERMODE_L      (RSP_1ST-6)
#define RSP_ENDDL               (RSP_1ST-7)
#define RSP_SETGEOMETRYMODE     (RSP_1ST-8)
#define RSP_CLEARGEOMETRYMODE   (RSP_1ST-9)
#define RSP_LINE3D              (RSP_1ST-10)
#define RSP_RDPHALF_1           (RSP_1ST-11)
#define RSP_RDPHALF_2           (RSP_1ST-12)
#define RSP_RDPHALF_CONT        (RSP_1ST-13)

#define RSP_MODIFYVTX           (RSP_1ST-13)
#define RSP_TRI2                (RSP_1ST-14)
#define RSP_BRANCH_Z            (RSP_1ST-15)
#define RSP_LOAD_UCODE          (RSP_1ST-16)

#define RSP_SPRITE2D_SCALEFLIP  (RSP_1ST-1)
#define RSP_SPRITE2D_DRAW       (RSP_1ST-2)

// flags to inhibit pushing of the display list (on branch)
#define RSP_DLIST_PUSH          0x00
#define RSP_DLIST_NOPUSH        0x01

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
    unsigned int address;  //!< Where texture is stored (in RDRAM)
    unsigned int width;    //!< Width of texture
    unsigned int height;   //!< Height of texture
    unsigned int format;   //!< Format of texture
    unsigned int size;     //!< Size of texture
    unsigned int palette;  //!< What Texture Lookup Table to use
};

//-----------------------------------------------------------------------------
//* RSPTexture
//! Struct used to store information about current texture on rsp
//-----------------------------------------------------------------------------
struct RSPTexture
{
    float scaleS, scaleT;
    int level, on, tile;
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
    bool initialize(GFX_INFO* graphicsInfo, RDP* rdp, Memory* memory, VI* vi, DisplayListParser* displayListParser, FogManager* fogMgr);
    void dispose();
    void reset();

    void updateGeometryStates();

    //Trigger Interrupt
    void triggerInterrupt();

    void moveSegment(int segmentID, int value);

    void moveMemViewport(unsigned int segmentAddress);

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
    void RSP_PopMatrixN(unsigned int num);
    void RSP_InsertMatrix(unsigned int where, unsigned int num);
    void RSP_DMAMatrix( unsigned int matrix, unsigned char index, unsigned char multiply );
    void RSP_ForceMatrix( unsigned int segmentAddress );
    void RSP_LookAt( unsigned int l );
    void RSP_PerspNormalize( unsigned short scale );    

    //Display List
    void RSP_DisplayList(unsigned int segmentAddress);
    void RSP_DMADisplayList( unsigned int w0, unsigned int w1 );
    void RSP_CullDisplayList( unsigned int v0, unsigned int vn );
    void RSP_BranchList( unsigned int dl );
    void RSP_BranchLessZ( unsigned int branchdl, unsigned int vtx, float zval );    
    void RSP_EndDisplayList();

    //Light
    void RSP_Light( unsigned int lightIndex, unsigned int segmentAddress  );
    void RSP_NumLights( int n );
    void RSP_LightColor( unsigned int lightIndex, unsigned int packedColor );    

    //Vertices
    void RSP_Vertex(unsigned int segmentAddress, unsigned int numVertices, unsigned int firstVertexIndex);
    void RSP_CIVertex(unsigned int segmentAddress, unsigned int numVertices, unsigned int firstVertexIndex);
    void RSP_ModifyVertex( unsigned int vtx, unsigned int where, unsigned int val );    
    void RSP_SetVertexColor( unsigned int vtx, float r, float g, float b, float a);    
    void RSP_SetVertexTexCoord( unsigned int vtx, float s, float t);    

    void RSP_DMAVertex( unsigned int v, unsigned int n, unsigned int v0 );
    void RSP_SetDMAOffsets( unsigned int mtxoffset, unsigned int vtxoffset );
    void RSP_SetVertexColorBase(unsigned int segmentAddress);

    //Indices
    void RSP_1Triangle( int v00, int v01, int v02 );
    void RSP_2Triangles( int v00, int v01, int v02, int flag0, 
                         int v10, int v11, int v12, int flag1 );
    void RSP_4Triangles( int v00, int v01, int v02,
                         int v10, int v11, int v12,
                         int v20, int v21, int v22,
                         int v30, int v31, int v32 );
    void RSP_DMATriangles( unsigned int tris, unsigned int n );
    void RSP_1Quadrangle( int v0, int v1, int v2, int v4 );

    //Object
    void RSP_ObjRectangle( unsigned int sp );
    void RSP_ObjSprite( unsigned int sp );
    void RSP_ObjLoadTxtr( unsigned int tx );
    void RSP_ObjLoadTxSprite( unsigned int txsp );
    void RSP_ObjLoadTxRectR( unsigned int txsp );
    void RSP_ObjMatrix( unsigned int mtx );
    void RSP_ObjSubMatrix( unsigned int mtx );

    //Rendering
    void RSP_Line3D( int v0, int v1, int flag );
    void RSP_LineW3D( int v0, int v1, int wd, int flag );
    void RSP_BgRect1Cyc( unsigned int bg );
    void RSP_BgRectCopy( unsigned int bg );
    void RSP_Sprite2DBase( unsigned int base );

    //States
    void RSP_GeometryMode( unsigned int clear, unsigned int set );
    void RSP_SetGeometryMode( unsigned int mode );
    void RSP_ClearGeometryMode( unsigned int mode );

    //Clipping    
    void RSP_ClipRatio( unsigned int r );

    //Texturing
    void RSP_Texture( float sc, float tc, int level, int tile, int on );

    //Fog
    void RSP_FogFactor( short fm, short fo );    

    //UCode
    void RSP_LoadUcodeEx( unsigned int uc_start, unsigned int uc_dstart, unsigned short uc_dsize );

private:

    //Pointers to big objects and managers
    GFX_INFO*          m_graphicsInfo;       //!< Access to emulator data (like RDRAM ...)
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
    unsigned int m_geometryMode;  //!< Contains states (lighting, shading, culling...)

    //Textures
    RSPTexture m_texture;  
    RDPTile* m_textureTiles[2];
    bool m_texturesChanged;

    //Viewport
    Viewport m_viewport;

};

#endif
