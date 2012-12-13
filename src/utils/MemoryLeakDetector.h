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

#ifndef MEMORY_LEAK_DETECTOR_H
#define MEMORY_LEAK_DETECTOR_H

#include "m64p.h"
#include <cstdio>
#if defined(WIN32) && !defined(__MINGW32__)
#include <crtdbg.h>
#endif

//*****************************************************************************
//* Memory Leak Detector Class
//! Class for checking if there are any memory leaks.
//*****************************************************************************
class CMemoryLeakDetector
{
public:
    
    //Constructor
    CMemoryLeakDetector() 
    { 
#if defined(WIN32) && !defined(__MINGW32__)
        ::OutputDebugString(">>> Memory leak detection enabled <<<\n"); 
#endif
    }

    //Destructor
    ~CMemoryLeakDetector()
    { 
#if defined(WIN32) && !defined(__MINGW32__)
        if ( !_CrtDumpMemoryLeaks() )
        {
            ::OutputDebugString(">>> No memory leak detected <<<\n"); 
        }
#endif
    }
};

CMemoryLeakDetector md;

#endif
