#include "FogManager.h"
#include "ExtensionChecker.h"
#include <windows.h>
#include <GL/gl.h>

//-----------------------------------------------------------------------------
// EXT_fog_coord functions
//-----------------------------------------------------------------------------
#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord 1
	#ifdef GL_GLEXT_PROTOTYPES
		extern void APIENTRY glFogCoordfEXT (GLfloat);
		extern void APIENTRY glFogCoordfvEXT (const GLfloat *);
		extern void APIENTRY glFogCoorddEXT (GLdouble);
		extern void APIENTRY glFogCoorddvEXT (const GLdouble *);
		extern void APIENTRY glFogCoordPointerEXT (GLenum, GLsizei, const GLvoid *);
	#endif 
	typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
	typedef void (APIENTRY * PFNGLFOGCOORDFVEXTPROC) (const GLfloat *coord);
	typedef void (APIENTRY * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
	typedef void (APIENTRY * PFNGLFOGCOORDDVEXTPROC) (const GLdouble *coord);
	typedef void (APIENTRY * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);

	#define GL_FOG_COORDINATE_SOURCE_EXT      0x8450
	#define GL_FOG_COORDINATE_EXT             0x8451
	#define GL_FOG_COORDINATE_ARRAY_EXT       0x8457
#endif
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT;

//-----------------------------------------------------------------------------
//! Static Variables
//-----------------------------------------------------------------------------
bool FogManager::m_fogExtensionsSupported = false;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
FogManager::FogManager()
{
    m_multiplier = 0;
	m_offset     = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
FogManager::~FogManager()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//! Initializes fog extensions 
//-----------------------------------------------------------------------------
void FogManager::initialize()
{
    m_multiplier = 0;
	m_offset     = 0;

	//Initialize extensions
	static bool fogExtensionInitialized = false;
	if ( !fogExtensionInitialized )
	{
		if ( m_fogExtensionsSupported = isExtensionSupported("GL_EXT_fog_coord") )
		{
			glFogCoordfEXT       = (PFNGLFOGCOORDFEXTPROC)wglGetProcAddress( "glFogCoordfEXT" );
			glFogCoordfvEXT      = (PFNGLFOGCOORDFVEXTPROC)wglGetProcAddress( "glFogCoordfvEXT" );
			glFogCoorddEXT       = (PFNGLFOGCOORDDEXTPROC)wglGetProcAddress( "glFogCoorddEXT" );
			glFogCoorddvEXT      = (PFNGLFOGCOORDDVEXTPROC)wglGetProcAddress( "glFogCoorddvEXT" );
			glFogCoordPointerEXT = (PFNGLFOGCOORDPOINTEREXTPROC)wglGetProcAddress( "glFogCoordPointerEXT" );
			fogExtensionInitialized  = true;
		}		
	}

	glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
}

//-----------------------------------------------------------------------------
//! Dispose
//-----------------------------------------------------------------------------
void FogManager::dispose()
{
}

//-----------------------------------------------------------------------------
//* SetFogCoordPointer
//! Function used to set vertex based fog
//! @param[in] type Specifies the datatype of each fog coordinate in the array.
//! @param[in] stride Specifies the byte offset between consecutive fog coordinates 
//! @param[in] pointer	Specifies a pointer to the first component of the first fog coordinate in the array
//! http://publib.boulder.ibm.com/infocenter/pseries/v5r3/index.jsp?topic=/com.ibm.aix.opengl/doc/openglrf/glFogCoordPointerEXT.htm
//-----------------------------------------------------------------------------
void FogManager::setFogCoordPointer(unsigned int type, int stride, const void* pointer)
{
	if ( m_fogExtensionsSupported )
	{
		glFogCoordPointerEXT(type, stride, pointer);
	}
}

//-----------------------------------------------------------------------------
//* Enable Fog Coord Array
//! Function used to enable vertex based fog
//! @see disableFogCoordArray()
//-----------------------------------------------------------------------------
void FogManager::enableFogCoordArray()
{
	if ( m_fogExtensionsSupported )
	{
		glEnableClientState(GL_FOG_COORDINATE_ARRAY_EXT);
	}
}

//-----------------------------------------------------------------------------
//* Disable Fog Coord Array
//! Function used to disable vertex based fog
//! @see enableFogCoordArray()
//-----------------------------------------------------------------------------
void FogManager::disableFogCoordArray()
{
	if ( m_fogExtensionsSupported )
	{
		glDisableClientState(GL_FOG_COORDINATE_ARRAY_EXT);
	}
}

//-----------------------------------------------------------------------------
//* Set Linear Fog
//! Function used to set linear fog using a linear fog equation
//! Equation for linear fog is:
//!     <KBD> fog = (end - z) / (end - start) </KBD>
//! where  z is the distance in  eye coordinates from the origin to the fragment being fogged
//! @param start Specifies start (near) distance used in the linear  fog equation.  
//!            The initial near distance is 0.
//! @param end Specifies end (the far) distance used in the linear fog equation.  
//!            The initial far  distance  is 1.
//! http://www.hmug.org/man/3/glFogi.php
//-----------------------------------------------------------------------------
void FogManager::setLinearFog(float start, float end)
{
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
}

//-----------------------------------------------------------------------------
//* Set Fog Color
//! @param r The red component of the fog color
//! @param g The green component of the fog color
//! @param b The blue component of the fog color
//! @param a The alpha component of the fog color
//-----------------------------------------------------------------------------
void FogManager::setFogColor(float r, float g, float b, float a)
{
	float fogColor[4] = { r,g,b,a };
	glFogfv(GL_FOG_COLOR, fogColor );
}