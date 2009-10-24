
#include "MultiTexturingExt.h"
#include "ExtensionChecker.h"

//Multi Texturing functions
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

bool g_MultiTextureARBSupport = false;

//-----------------------------------------------------------------------------
//! Function for initializeing multitextureing extensions
//-----------------------------------------------------------------------------
bool initializeMultiTexturingExtensions()
{
    //Initialize Extentions
	if ( g_MultiTextureARBSupport = isExtensionSupported("GL_ARB_multitexture") )
	{
		glActiveTextureARB			= (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress( "glActiveTextureARB" );
		glClientActiveTextureARB	= (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress( "glClientActiveTextureARB" );
		glMultiTexCoord2fARB		= (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress( "glMultiTexCoord2fARB" );
        return true;
	}
    return false;
}