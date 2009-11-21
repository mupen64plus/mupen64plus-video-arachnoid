#include "RSPVertexManager.h"
#include "OpenGLManager.h"
#include "Memory.h"
#include "RSPMatrixManager.h"
#include "RSPLightManager.h"
#include "OpenGLRenderer.h"
#include "GBIDefs.h"   //hmm
#include "MathLib.h"   //Transform, Vec3Normalize
#include <cmath> //sqrt
#include "Logger.h"

//Vertex
struct Vertex
{
	//Position (x,y,z)
	short y;
	short x;
	unsigned short flag;  //Special Flags
	short z;

	//TexCoords
	short t;
	short s;

	//Color or Normal
	union 
	{
		struct { unsigned char a; unsigned char b; unsigned char g; unsigned char r; } color;
		struct { signed char a;   signed char z;   signed char y;   signed char x; } normal;
	};
};

//Perfect Dark Vertex
struct PerfectDarkVertex
{
	short y;
	short x;
	unsigned short ci;  //Color Index
	short z;
	short t;
	short s;
};

struct DKRTriangle 
{
	unsigned char v2, v1, v0, flag;
	short		t0, s0;
	short		t1, s1;
	short		t2, s2;
};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RSPVertexManager::RSPVertexManager() 
{
	m_openGLMgr = 0;
	m_memory    = 0;
	m_matrixMgr = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RSPVertexManager::~RSPVertexManager()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool RSPVertexManager::initialize(OpenGLManager* openGLMgr, Memory* memory, RSPMatrixManager* matrixMgr, RSPLightManager* lightMgr)
{
	m_openGLMgr = openGLMgr;
	m_memory    = memory;
	m_matrixMgr = matrixMgr;	
	m_lightMgr  = lightMgr;
	m_texCoordGenType = TCGT_NONE;
    m_rdramOffset = 0;
    m_billboard = false;
	return true;
}

//-----------------------------------------------------------------------------
// Set Vertices
//-----------------------------------------------------------------------------
void RSPVertexManager::setVertices( unsigned int address, unsigned int numVertices, unsigned int firstVertexIndex)
{
	//Make sure address is valid
	if ((address + sizeof( Vertex ) * numVertices) > m_memory->getRDRAMSize()) {
		return;
	}

	//Get vertex from rdram
	Vertex *vertex = (Vertex*) m_memory->getRDRAM(address);

	//Avoid overflow. There can only be MAX_VERTICES in size.
	if ( numVertices+firstVertexIndex >= MAX_VERTICES)
	{
		return;
	}

	//For each vertex
	for (unsigned int i=firstVertexIndex; i <numVertices+firstVertexIndex; ++i)
	{
		m_vertices[i].x = vertex->x;
		m_vertices[i].y = vertex->y;
		m_vertices[i].z = vertex->z;
		m_vertices[i].flag = vertex->flag;
		m_vertices[i].s = _FIXED2FLOAT( vertex->s, 5 );
		m_vertices[i].t = _FIXED2FLOAT( vertex->t, 5 );

		if ( m_lightMgr->getLightEnabled() )
		{
			m_vertices[i].nx = vertex->normal.x;
			m_vertices[i].ny = vertex->normal.y;
			m_vertices[i].nz = vertex->normal.z;
			m_vertices[i].a = vertex->color.a * 0.0039215689f;
		}
		else
		{
			m_vertices[i].r = vertex->color.r * 0.0039215689f;
			m_vertices[i].g = vertex->color.g * 0.0039215689f;
			m_vertices[i].b = vertex->color.b * 0.0039215689f;
			m_vertices[i].a = vertex->color.a * 0.0039215689f;
		}

		_processVertex(i);
		vertex++;
	}
}

//-----------------------------------------------------------------------------
// Modify Vertex
//-----------------------------------------------------------------------------
void RSPVertexManager::modifyVertex( unsigned int vtx, unsigned int where, unsigned int val )
{
	switch (where)
	{
		case G_MWO_POINT_RGBA:
			m_vertices[vtx].r = _SHIFTR( val, 24, 8 ) * 0.0039215689f;
			m_vertices[vtx].g = _SHIFTR( val, 16, 8 ) * 0.0039215689f;
			m_vertices[vtx].b = _SHIFTR( val, 8, 8 ) * 0.0039215689f;
			m_vertices[vtx].a = _SHIFTR( val, 0, 8 ) * 0.0039215689f;
			break;
		case G_MWO_POINT_ST:
			m_vertices[vtx].s = _FIXED2FLOAT( (short)_SHIFTR( val, 16, 16 ), 5 );
			m_vertices[vtx].t = _FIXED2FLOAT( (short)_SHIFTR( val, 0, 16 ), 5 );
			break;
		case G_MWO_POINT_XYSCREEN:
			break;
		case G_MWO_POINT_ZSCREEN:
			break;
	}
}

void RSPVertexManager::setVertexColor(unsigned int vertexIndex, float r, float g, float b, float a)
{
	m_vertices[vertexIndex].r = r;
	m_vertices[vertexIndex].g = g;
	m_vertices[vertexIndex].b = b;
	m_vertices[vertexIndex].a = a;
}

void RSPVertexManager::setVertexTextureCoord(unsigned int vertexIndex, float s, float t)
{
	m_vertices[vertexIndex].s = s;
	m_vertices[vertexIndex].t = t;
}


//-----------------------------------------------------------------------------
// ?
//-----------------------------------------------------------------------------
void RSPVertexManager::ciVertex(unsigned int segmentAddress, unsigned int numVertices, unsigned int firstVertexIndex)
{
	unsigned int rdramAddress = m_memory->getRDRAMAddress(segmentAddress);

	if ((rdramAddress + sizeof(PerfectDarkVertex) * numVertices) > m_memory->getRDRAMSize())
	{
		return;
	}

	PerfectDarkVertex* vertex = (PerfectDarkVertex*)m_memory->getRDRAM(rdramAddress);

	//Avoid overflow. There can only be MAX_VERTICES in size.
	if ( numVertices+firstVertexIndex >= MAX_VERTICES)
	{
		return;	
	}

	//For each vertex
	for (unsigned int i=firstVertexIndex; i <numVertices+firstVertexIndex; ++i)
	{
		m_vertices[i].x    = vertex->x;
		m_vertices[i].y    = vertex->y;
		m_vertices[i].z    = vertex->z;
		m_vertices[i].flag = 0;
		m_vertices[i].s    = _FIXED2FLOAT( vertex->s, 5 );
		m_vertices[i].t    = _FIXED2FLOAT( vertex->t, 5 );

		//Get color
		unsigned char* color = m_memory->getRDRAM(m_colorBaseRDRAMAddress + (vertex->ci & 0xff));

		//Assign color
		if ( m_lightMgr->getLightEnabled() )
		{
			m_vertices[i].nx = (short)color[3];
			m_vertices[i].ny = (short)color[2];
			m_vertices[i].nz = (short)color[1];
			m_vertices[i].a = color[0] * 0.0039215689f;
		}
		else
		{
			m_vertices[i].r = color[3] * 0.0039215689f;
			m_vertices[i].g = color[2] * 0.0039215689f;
			m_vertices[i].b = color[1] * 0.0039215689f;
			m_vertices[i].a = color[0] * 0.0039215689f;
		}

		_processVertex( i );
		vertex++;
	}
}

//-----------------------------------------------------------------------------
// ?
//-----------------------------------------------------------------------------
void RSPVertexManager::DMAVertex( unsigned int v, unsigned int numVertices, unsigned int firstVertexIndex)
{
    unsigned int address = m_rdramOffset + m_memory->getRDRAMAddress( v );

    if ((address + 10 * numVertices) > m_memory->getRDRAMSize())
	{
		return;
	}

    unsigned char* RDRAM = m_memory->getRDRAM();

	if ((numVertices + firstVertexIndex) < (80))
	{
		for (int i = firstVertexIndex; i < numVertices + firstVertexIndex; i++)
		{
			m_vertices[i].x = *(short*)&RDRAM[address ^ 2];
			m_vertices[i].y = *(short*)&RDRAM[(address + 2) ^ 2];
			m_vertices[i].z = *(short*)&RDRAM[(address + 4) ^ 2];

			if ( m_lightMgr->getLightEnabled() )
			{
				m_vertices[i].nx = *(char*)&RDRAM[(address + 6) ^ 3];
				m_vertices[i].ny = *(char*)&RDRAM[(address + 7) ^ 3];
				m_vertices[i].nz = *(char*)&RDRAM[(address + 8) ^ 3];
				m_vertices[i].a = *(unsigned char*)&RDRAM[(address + 9) ^ 3] * 0.0039215689f;
			}
			else
			{
				m_vertices[i].r = *(unsigned char*)&RDRAM[(address + 6) ^ 3] * 0.0039215689f;
				m_vertices[i].g = *(unsigned char*)&RDRAM[(address + 7) ^ 3] * 0.0039215689f;
				m_vertices[i].b = *(unsigned char*)&RDRAM[(address + 8) ^ 3] * 0.0039215689f;
				m_vertices[i].a = *(unsigned char*)&RDRAM[(address + 9) ^ 3] * 0.0039215689f;
			}

			_processVertex( i );

			address += 10;
		}
	}
}

//-----------------------------------------------------------------------------
// ?
//-----------------------------------------------------------------------------
void RSPVertexManager::DMAOffsets( unsigned int mtxoffset, unsigned int vtxoffset )
{
	static bool warned = false;
	if ( !warned ) {
		Logger::getSingleton().printMsg("VertexManager - DMAOffsets - Unimplemented", M64MSG_WARNING);
		warned = true;
	}
}

//-----------------------------------------------------------------------------
//! @param v0 Index of first vertex in triangle
//! @param v1 Index of second vertex in triangle
//! @param v2 Index of third vertex in triangle
//-----------------------------------------------------------------------------
bool RSPVertexManager::add1Triangle(unsigned int v0, unsigned int v1, unsigned int v2 )
{
	bool triangleAdded = false;

	if (true)//IsTriangleVisible(dwV0, dwV1, dwV2))
	{
		if ( !((v0 < MAX_VERTICES) && (v1 < MAX_VERTICES) && (v2 < MAX_VERTICES)) )
		{
			return false;
		}

		// Don't bother with triangles completely outside clipping frustrum
		if (((m_vertices[v0].xClip <  0.0f) && (m_vertices[v1].xClip <  0.0f) && (m_vertices[v2].xClip <  0.0f)) ||
			((m_vertices[v0].xClip >  0.0f) && (m_vertices[v1].xClip >  0.0f) && (m_vertices[v2].xClip >  0.0f)) ||
			((m_vertices[v0].yClip <  0.0f) && (m_vertices[v1].yClip <  0.0f) && (m_vertices[v2].yClip <  0.0f)) ||
			((m_vertices[v0].yClip >  0.0f) && (m_vertices[v1].yClip >  0.0f) && (m_vertices[v2].yClip >  0.0f)) ||
			((m_vertices[v0].zClip >  0.1f) && (m_vertices[v1].zClip >  0.1f) && (m_vertices[v2].zClip >  0.1f)) ||
			((m_vertices[v0].zClip < -0.1f) && (m_vertices[v1].zClip < -0.1f) && (m_vertices[v2].zClip < -0.1f))    )
		{
			return false;
		}
		triangleAdded = true;

		//Add vertex to vertex buffer
		OpenGLRenderer::getSingleton().addTriangle( m_vertices, v0, v1, v2 );
	}

	return triangleAdded;
}

void RSPVertexManager::add2Triangles( int v00, int v01, int v02, int flag0,
					                  int v10, int v11, int v12, int flag1 )
{
	//implemented by calling add1Triangle multiple times

	static bool warned = false;
	if ( !warned ) {
		Logger::getSingleton().printMsg("VertexManager - add2Triangles - Unimplemented", M64MSG_WARNING);
		warned = true;
	}
}

void RSPVertexManager::add4Triangles( int v00, int v01, int v02,
					int v10, int v11, int v12,
					int v20, int v21, int v22,
					int v30, int v31, int v32 )
{
	//implemented by calling add1Triangle multiple times


	static bool warned = false;
	if ( !warned ) {
		Logger::getSingleton().printMsg("VertexManager - add4Triangles - Unimplemented", M64MSG_WARNING);
		warned = true;
	}
}

//! @todo Set culling
void RSPVertexManager::addDMATriangles( unsigned int tris, unsigned int n )
{
    unsigned int address = m_memory->getRDRAMAddress(tris);

	if (address + sizeof( DKRTriangle ) * n > m_memory->getRDRAMSize() )
	{
		return;
	}

    DKRTriangle *triangles = (DKRTriangle*)m_memory->getRDRAM(address);

	for (unsigned int i = 0; i < n; i++)
	{
        //TODO Set culling
		//gSP.geometryMode &= ~G_CULL_BOTH;
		//if (!(triangles->flag & 0x40))
		//{
		//	if (gSP.viewport.vscale[0] > 0)
		//		gSP.geometryMode |= G_CULL_BACK;
		//	else
		//		gSP.geometryMode |= G_CULL_FRONT;
		//}
		//gSP.changed |= CHANGED_GEOMETRYMODE;
        glDisable(GL_CULL_FACE);
		
		m_vertices[triangles->v0].s = _FIXED2FLOAT( triangles->s0, 5 );
		m_vertices[triangles->v0].t = _FIXED2FLOAT( triangles->t0, 5 );
		m_vertices[triangles->v1].s = _FIXED2FLOAT( triangles->s1, 5 );
		m_vertices[triangles->v1].t = _FIXED2FLOAT( triangles->t1, 5 );
		m_vertices[triangles->v2].s = _FIXED2FLOAT( triangles->s2, 5 );
		m_vertices[triangles->v2].t = _FIXED2FLOAT( triangles->t2, 5 );

        add1Triangle( triangles->v0, triangles->v1, triangles->v2 /*, 0 */ );

		triangles++;
	}
}


void RSPVertexManager::setConkerAddress(unsigned int segmentAddress)
{
    m_conkerRDRAMAddress = m_memory->getRDRAMAddress(segmentAddress);
}
    

void RSPVertexManager::add1Quadrangle( int v0, int v1, int v2, int v4 )
{
	//implemented by calling add1Triangle multiple times

	static bool warned = false;
	if ( !warned ) {
		Logger::getSingleton().printMsg("VertexManager - add1Quadrangle - Unimplemented", M64MSG_WARNING);
		warned = true;
	}
}


inline float DotProduct( float* v0, float* v1 )
{
	float	dot;
#ifdef WIN32
	__asm {
		mov		esi, dword ptr [v0]
		mov		edi, dword ptr [v1]
		lea		ebx, [dot]

		fld		dword ptr [esi]
		fmul	dword ptr [edi]
		fld		dword ptr [esi+04h]
		fmul	dword ptr [edi+04h]
		fld		dword ptr [esi+08h]
		fmul	dword ptr [edi+08h]
		fadd
		fadd
		fstp	dword ptr [ebx]
	}
#else
	dot = v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
#endif
	return dot;
}


void RSPVertexManager::_processVertex( unsigned int v )
{
//	float intensity;
//	float r, g, b;

	transformVertex( m_matrixMgr->getViewProjectionMatrix(), &m_vertices[v].x, &m_vertices[v].x);
	//TransformVertex( &m_vertices[v].x, m_worldProject );


	if ( m_billboard )
	{
		m_vertices[v].x += m_vertices[0].x;
		m_vertices[v].y += m_vertices[0].y;
		m_vertices[v].z += m_vertices[0].z;
		m_vertices[v].w += m_vertices[0].w;
	}


	if ( !OpenGLManager::getSingleton().getZBufferEnabled() )
	{
		m_vertices[v].z = -m_vertices[v].w;
	}

	//Temporary variables
	float intensity;
	float r, g, b;

	if ( m_lightMgr->getLightEnabled() )
	{
		//Transform normal
		transformVector( m_matrixMgr->getModelViewMatrix(), &m_vertices[v].nx, &m_vertices[v].nx );
		Vec3Normalize( &m_vertices[v].nx );

		//Get Ambient Color
		const float* ambientColor = m_lightMgr->getAmbientLight();
		r = ambientColor[0];
		g = ambientColor[1];
		b = ambientColor[2];

		for (int i=0; i<m_lightMgr->getNumLights(); ++i)
		{
			intensity = DotProduct( (float*)&m_vertices[v].nx, (float*)m_lightMgr->getLightDirection(i) );

			if (intensity < 0.0f) intensity = 0.0f;

			const float* lightColor = m_lightMgr->getLightColor(i);
			r += lightColor[0] * intensity;
			g += lightColor[1] * intensity;
			b += lightColor[2] * intensity;
		}

		//Set Color
		m_vertices[v].r = r;
		m_vertices[v].g = g;
		m_vertices[v].b = b;	
	}

	//Texture Generation
	if ( m_texCoordGenType != TCGT_NONE )
	{
		transformVector( m_matrixMgr->getProjectionMatrix(), &m_vertices[v].nx, &m_vertices[v].nx );

		Vec3Normalize( &m_vertices[v].nx );

		if ( m_texCoordGenType == TCGT_LINEAR )
		{   
			m_vertices[v].s = acosf(m_vertices[v].nx) * 325.94931f;
			m_vertices[v].t = acosf(m_vertices[v].ny) * 325.94931f;
		}
		else // TGT_GEN
		{
			m_vertices[v].s = (m_vertices[v].nx + 1.0f) * 512.0f;
			m_vertices[v].t = (m_vertices[v].ny + 1.0f) * 512.0f;
		}
	}

	//Clipping
	if (m_vertices[v].x < -m_vertices[v].w)  
		m_vertices[v].xClip = -1.0f;
	else if (m_vertices[v].x > m_vertices[v].w)
		m_vertices[v].xClip = 1.0f;
	else
		m_vertices[v].xClip = 0.0f;

	if (m_vertices[v].y < -m_vertices[v].w)
		m_vertices[v].yClip = -1.0f;
	else if (m_vertices[v].y > m_vertices[v].w)
		m_vertices[v].yClip = 1.0f;
	else
		m_vertices[v].yClip = 0.0f;

	if (m_vertices[v].w <= 0.0f)
		m_vertices[v].zClip = -1.0f;
	else if (m_vertices[v].z < -m_vertices[v].w)
		m_vertices[v].zClip = -0.1f;
	else if (m_vertices[v].z > m_vertices[v].w)
		m_vertices[v].zClip = 1.0f;
	else
		m_vertices[v].zClip = 0.0f;

}


void RSPVertexManager::addConkerVertices(unsigned int segmentAddress, unsigned int n, unsigned int v0 )
{
    unsigned int numVertices = n;
    unsigned int firstVertexIndex = v0;
    unsigned int address = m_memory->getRDRAMAddress( segmentAddress );

	//Make sure address is valid
	if ((address + sizeof( Vertex ) * numVertices) > m_memory->getRDRAMSize()) {
		return;
	}

	//Get vertex from rdram
	Vertex *vertex = (Vertex*) m_memory->getRDRAM(address);

    //Colors
    short *vertexColoraddr = (short*)(m_memory->getRDRAM(m_conkerRDRAMAddress));


	//For each vertex
	for (unsigned int i=firstVertexIndex; i <numVertices+firstVertexIndex; ++i)
	{
		m_vertices[i].x = vertex->x;
		m_vertices[i].y = vertex->y;
		m_vertices[i].z = vertex->z;
		m_vertices[i].flag = vertex->flag;
		m_vertices[i].s = _FIXED2FLOAT( vertex->s, 5 );
		m_vertices[i].t = _FIXED2FLOAT( vertex->t, 5 );

		if ( m_lightMgr->getLightEnabled() )
		{
			m_vertices[i].nx = vertex->normal.x;
			m_vertices[i].ny = vertex->normal.y;
			m_vertices[i].nz = vertex->normal.z;
			m_vertices[i].a = vertex->color.a * 0.0039215689f;
		}
		else
		{
			m_vertices[i].r = vertex->color.r * 0.0039215689f;
			m_vertices[i].g = vertex->color.g * 0.0039215689f;
			m_vertices[i].b = vertex->color.b * 0.0039215689f;
			m_vertices[i].a = vertex->color.a * 0.0039215689f;
		}

		_processVertex(i);
		vertex++;
	}
}