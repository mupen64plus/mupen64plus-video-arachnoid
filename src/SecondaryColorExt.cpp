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

#include "SecondaryColorExt.h"
#include "ExtensionChecker.h"

// EXT_secondary_color functions
#ifndef GL_GLEXT_VERSION
PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
#endif

bool initializeSecondaryColorExtension()
{
    if ( isExtensionSupported( "GL_EXT_secondary_color" ))
    {
#ifndef GL_GLEXT_VERSION
        glSecondaryColor3bEXT = (PFNGLSECONDARYCOLOR3BEXTPROC)wglGetProcAddress( "glSecondaryColor3bEXT" );
        glSecondaryColor3bvEXT = (PFNGLSECONDARYCOLOR3BVEXTPROC)wglGetProcAddress( "glSecondaryColor3bvEXT" );
        glSecondaryColor3dEXT = (PFNGLSECONDARYCOLOR3DEXTPROC)wglGetProcAddress( "glSecondaryColor3dEXT" );
        glSecondaryColor3dvEXT = (PFNGLSECONDARYCOLOR3DVEXTPROC)wglGetProcAddress( "glSecondaryColor3dvEXT" );
        glSecondaryColor3fEXT = (PFNGLSECONDARYCOLOR3FEXTPROC)wglGetProcAddress( "glSecondaryColor3fEXT" );
        glSecondaryColor3fvEXT = (PFNGLSECONDARYCOLOR3FVEXTPROC)wglGetProcAddress( "glSecondaryColor3fvEXT" );
        glSecondaryColor3iEXT = (PFNGLSECONDARYCOLOR3IEXTPROC)wglGetProcAddress( "glSecondaryColor3iEXT" );
        glSecondaryColor3ivEXT = (PFNGLSECONDARYCOLOR3IVEXTPROC)wglGetProcAddress( "glSecondaryColor3ivEXT" );
        glSecondaryColor3sEXT = (PFNGLSECONDARYCOLOR3SEXTPROC)wglGetProcAddress( "glSecondaryColor3sEXT" );
        glSecondaryColor3svEXT = (PFNGLSECONDARYCOLOR3SVEXTPROC)wglGetProcAddress( "glSecondaryColor3svEXT" );
        glSecondaryColor3ubEXT = (PFNGLSECONDARYCOLOR3UBEXTPROC)wglGetProcAddress( "glSecondaryColor3ubEXT" );
        glSecondaryColor3ubvEXT = (PFNGLSECONDARYCOLOR3UBVEXTPROC)wglGetProcAddress( "glSecondaryColor3ubvEXT" );
        glSecondaryColor3uiEXT = (PFNGLSECONDARYCOLOR3UIEXTPROC)wglGetProcAddress( "glSecondaryColor3uiEXT" );
        glSecondaryColor3uivEXT = (PFNGLSECONDARYCOLOR3UIVEXTPROC)wglGetProcAddress( "glSecondaryColor3uivEXT" );
        glSecondaryColor3usEXT = (PFNGLSECONDARYCOLOR3USEXTPROC)wglGetProcAddress( "glSecondaryColor3usEXT" );
        glSecondaryColor3usvEXT = (PFNGLSECONDARYCOLOR3USVEXTPROC)wglGetProcAddress( "glSecondaryColor3usvEXT" );
        glSecondaryColorPointerEXT = (PFNGLSECONDARYCOLORPOINTEREXTPROC)wglGetProcAddress( "glSecondaryColorPointerEXT" );
#endif
        return true;
    }
    return false;
}
