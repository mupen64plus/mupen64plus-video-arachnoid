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

#include "Logger.h"

//-----------------------------------------------------------------------------
//* Initialize
//! Initializes log with debug callback
//-----------------------------------------------------------------------------
bool Logger::initialize(void (*debugCallback)(void*, int, const char*), void *context)
{
    m_debugCallback = debugCallback;
    m_debugCallContext = context;

    return true;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Logger::~Logger()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Dispose
//! Closes log file
//-----------------------------------------------------------------------------
void Logger::dispose()
{
    m_debugCallback = NULL;
    m_debugCallContext = NULL;
}

//-----------------------------------------------------------------------------
//* Print Message
//! Writes a message to log-file and/or console.
//! @param msg The text message to write to log-file.
//! @param lml How important the message is. Error message are more important then warnings for example.
//-----------------------------------------------------------------------------
void Logger::printMsg(const char* msg, m64p_msg_level lml)
{
    if (m_debugCallback != NULL)
    {
        (*m_debugCallback)(m_debugCallContext, lml, msg);
    }    
}
