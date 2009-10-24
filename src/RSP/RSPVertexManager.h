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
	float		x, y, z, w;
	float		nx, ny, nz;
	float		r, g, b, a;
	float		s, t;
	float		xClip, yClip, zClip;
	float		flag;
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
	void setVertices( unsigned long address, unsigned int numVertices, unsigned int firstVertexIndex);
	void modifyVertex( unsigned long vtx, unsigned long where, unsigned long val );	
	void setVertexColor(unsigned long vertexIndex, float r, float g, float b, float a);
	void setVertexTextureCoord(unsigned long vertexIndex, float s, float t);

	void ciVertex(unsigned long segmentAddress, unsigned long n, unsigned long v0 );

    void addConkerVertices(unsigned long segmentAddress, unsigned long n, unsigned long v0 );

	void DMAVertex( unsigned long v, unsigned long n, unsigned long v0 );
	void DMAOffsets( unsigned long mtxoffset, unsigned long vtxoffset );
	void setVertexColorBase( unsigned long rdramAddress ) { m_colorBaseRDRAMAddress = rdramAddress; }

	bool add1Triangle( unsigned long v0, unsigned long v1, unsigned long v2);
	void add2Triangles( long v00, long v01, long v02, long flag0, 
						long v10, long v11, long v12, long flag1 );
	void add4Triangles( long v00, long v01, long v02,
						long v10, long v11, long v12,
						long v20, long v21, long v22,
						long v30, long v31, long v32 );
	void addDMATriangles( unsigned long tris, unsigned long n );
	void add1Quadrangle( long v0, long v1, long v2, long v4 );
	void setTexCoordGenType(TexCoordGenType texCoordGenType) { m_texCoordGenType       = texCoordGenType; }
	
    void setRDRAMOffset(unsigned long offset) { m_rdramOffset = offset; }
    void setBillboard(unsigned long billboard) { m_billboard = billboard; }
    unsigned long getBillboard() { return m_billboard; }

    void setConkerAddress(unsigned long segmentAddress);   

public:

	SPVertex* getVertex(unsigned int index) { return &m_vertices[index]; }

private:

	void _processVertex( unsigned long v );

private:

	OpenGLManager* m_openGLMgr;
	Memory* m_memory;
	RSPMatrixManager* m_matrixMgr;
	RSPLightManager* m_lightMgr;
	
	//Vertex Buffer
	static const int MAX_VERTICES = 300;
	SPVertex m_vertices[MAX_VERTICES];

	unsigned long m_colorBaseRDRAMAddress;  //!< Address in RDRAM where colors for vertices are located (used by Perfect Dark)

     unsigned long m_rdramOffset;

     unsigned long m_billboard;
	TexCoordGenType m_texCoordGenType;  //!< Texture Coordinate Generation Technique

    unsigned long m_conkerRDRAMAddress;
};

#endif