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
#include "OpenGL.h"
#include "m64p.h"

//-----------------------------------------------------------------------------
//! Is Extension Supported
//-----------------------------------------------------------------------------
bool isExtensionSupported( const char *extension )
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    extensions = glGetString(GL_EXTENSIONS);

    start = extensions;
    for (;;)
    {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;

        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;

        start = terminator;
    }

    return false;
}
