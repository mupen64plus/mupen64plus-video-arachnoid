
#include "MultiTexturingExt.h"
#include "ExtensionChecker.h"

//Multi Texturing functions
#ifdef WIN32
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
#endif
bool g_MultiTextureARBSupport = false;

//-----------------------------------------------------------------------------
//! Function for initializeing multitextureing extensions
//-----------------------------------------------------------------------------
bool initializeMultiTexturingExtensions()
{
    //Initialize Extentions
	if ( g_MultiTextureARBSupport = isExtensionSupported("GL_ARB_multitexture") )
	{
#ifdef WIN32
		glActiveTextureARB			= (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress( "glActiveTextureARB" );
		glClientActiveTextureARB	= (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress( "glClientActiveTextureARB" );
		glMultiTexCoord2fARB		= (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress( "glMultiTexCoord2fARB" );
#endif
        return true;
	}
    return false;
}