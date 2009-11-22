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

#ifndef VIDEO_INTERFACE_H_
#define VIDEO_INTERFACE_H_

//Forward declarations
#include "UCodeDefs.h"
#include "m64p_plugin.h"

//*****************************************************************************
//! Video Interface
//*****************************************************************************
class VI
{
public:

    //Constructor
    VI();
    ~VI();

    //Calculate height and width
    void calcSize(GFX_INFO*  graphicsInfo);

    //Get Height and Width
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

private:

    int m_width;   //!< Width of video interface viewport
    int m_height;  //!< Height of video interface viewport

};

#endif
