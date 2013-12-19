/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
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
#ifndef OPENGL_H
#define OPENGL_H

#ifndef WIN32
    #define GL_GLEXT_PROTOTYPES
#endif
#if defined(__MACOSX__)
#include <OpenGL/gl.h>
#elif defined(__MACOS__)
#include <gl.h>
#else
#include <GL/gl.h>
#endif
#ifndef WIN32
    #include <GL/glext.h>
#endif

#endif
