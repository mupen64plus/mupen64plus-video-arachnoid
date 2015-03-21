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

#include "ExtensionChecker.h"
#include "MultiTexturingExt.h"


//Multi Texturing functions
#ifndef GL_GLEXT_VERSION
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
    g_MultiTextureARBSupport = isExtensionSupported("GL_ARB_multitexture");
    if ( g_MultiTextureARBSupport )
    {
#ifndef GL_GLEXT_VERSION
        glActiveTextureARB          = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress( "glActiveTextureARB" );
        glClientActiveTextureARB    = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress( "glClientActiveTextureARB" );
        glMultiTexCoord2fARB        = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress( "glMultiTexCoord2fARB" );
#endif
        return true;
    }
    return false;
}
