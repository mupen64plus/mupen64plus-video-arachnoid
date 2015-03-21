/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
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

#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdlib>
#include "m64p_types.h"

//*****************************************************************************
//*****************************************************************************
//* Log
//! Class for sending log messages to M64P
//*****************************************************************************
class Logger
{
public:
    //Destructor
    ~Logger();

    //Singleton Instance
    static Logger& getSingleton()
    {
        static Logger pInstance;
        return pInstance;
    }

    //Initialize / Dispose
    bool initialize(void (*debugCallback)(void*, int, const char*), void *context);  
    void dispose();

    //Message
    void printMsg(const char* msg, m64p_msg_level lml=M64MSG_VERBOSE);

protected:
    //! Constructor
    Logger() { m_debugCallback = NULL; m_debugCallContext = NULL; }

protected:
    void (*m_debugCallback)(void *, int, const char *);
    void *m_debugCallContext;
};

#endif
