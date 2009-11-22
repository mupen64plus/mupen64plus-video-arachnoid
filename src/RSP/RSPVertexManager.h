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

#ifndef RSP_VERTEX_MANAGER_H_
#define RSP_VERTEX_MANAGER_H_

//Forward declarations
class Memory;
class RSPMatrixManager;
class RSPLightManager;
class OpenGLManager;

enum TexCoordGenType
{
    TCGT_NONE = 0,
    TCGT_LINEAR = 1,
    TCGT_GEN = 2,
};

//-----------------------------------------------------------------------------
//! Signal Processor Vertex
//-----------------------------------------------------------------------------
struct SPVertex
{
    float        x, y, z, w;
    float        nx, ny, nz;
    float        r, g, b, a;
    float        s, t;
    float        xClip, yClip, zClip;
    float        flag;
};

//*****************************************************************************
//! RSP Vertex Manager
//*****************************************************************************
class RSPVertexManager
{
public:

    //Constructor / Destructor
    RSPVertexManager();
    ~RSPVertexManager();

    bool initialize(OpenGLManager* openGLMgr, Memory* memory, RSPMatrixManager* matrixMgr, RSPLightManager* lightMgr);

    //Vertices
    void setVertices( unsigned int address, unsigned int numVertices, unsigned int firstVertexIndex);
    void modifyVertex( unsigned int vtx, unsigned int where, unsigned int val );    
    void setVertexColor(unsigned int vertexIndex, float r, float g, float b, float a);
    void setVertexTextureCoord(unsigned int vertexIndex, float s, float t);

    void ciVertex(unsigned int segmentAddress, unsigned int n, unsigned int v0 );

    void addConkerVertices(unsigned int segmentAddress, unsigned int n, unsigned int v0 );

    void DMAVertex( unsigned int v, unsigned int n, unsigned int v0 );
    void DMAOffsets( unsigned int mtxoffset, unsigned int vtxoffset );
    void setVertexColorBase( unsigned int rdramAddress ) { m_colorBaseRDRAMAddress = rdramAddress; }

    bool add1Triangle( unsigned int v0, unsigned int v1, unsigned int v2);
    void add2Triangles( int v00, int v01, int v02, int flag0, 
                        int v10, int v11, int v12, int flag1 );
    void add4Triangles( int v00, int v01, int v02,
                        int v10, int v11, int v12,
                        int v20, int v21, int v22,
                        int v30, int v31, int v32 );
    void addDMATriangles( unsigned int tris, unsigned int n );
    void add1Quadrangle( int v0, int v1, int v2, int v4 );
    void setTexCoordGenType(TexCoordGenType texCoordGenType) { m_texCoordGenType       = texCoordGenType; }
    
    void setRDRAMOffset(unsigned int offset) { m_rdramOffset = offset; }
    void setBillboard(unsigned int billboard) { m_billboard = billboard; }
    unsigned int getBillboard() { return m_billboard; }

    void setConkerAddress(unsigned int segmentAddress);   

public:

    SPVertex* getVertex(unsigned int index) { return &m_vertices[index]; }

private:

    void _processVertex( unsigned int v );

private:

    OpenGLManager* m_openGLMgr;
    Memory* m_memory;
    RSPMatrixManager* m_matrixMgr;
    RSPLightManager* m_lightMgr;
    
    //Vertex Buffer
    static const int MAX_VERTICES = 300;
    SPVertex m_vertices[MAX_VERTICES];

    unsigned int m_colorBaseRDRAMAddress;  //!< Address in RDRAM where colors for vertices are located (used by Perfect Dark)

    unsigned int m_rdramOffset;

    unsigned int m_billboard;
    TexCoordGenType m_texCoordGenType;  //!< Texture Coordinate Generation Technique

    unsigned int m_conkerRDRAMAddress;
};

#endif
