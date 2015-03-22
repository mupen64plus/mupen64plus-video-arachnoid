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

#include "GBI.h"
#include "Logger.h"
#include "MathLib.h"
#include "Memory.h"
#include "RSPLightManager.h"

#define RGBA_GETALPHA(rgb)      ((rgb) >> 24)
#define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RSPLightManager::RSPLightManager() 
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RSPLightManager::~RSPLightManager()
{
}

bool RSPLightManager::initialize(Memory* memory)
{
    m_memory = memory;
    m_numLights = 0;
    m_lightEnabled = false;

    return true;
}

//-----------------------------------------------------------------------------
// Set Light
//-----------------------------------------------------------------------------
void RSPLightManager::setLight( unsigned int lightIndex, unsigned int rdramAddress  )
{
    //Error control 
    if ((rdramAddress + sizeof(RDRAMLight)) > m_memory->getRDRAMSize() ) {
        return;
    }

    //Get Light from memory
    RDRAMLight* light = (RDRAMLight*)m_memory->getRDRAM(rdramAddress);

    if ( lightIndex < 8 ) //Only supports 8 lights
    {
        m_lights[lightIndex].r = light->r * 0.0039215689f;  //Convert from 0-255 to 0-1
        m_lights[lightIndex].g = light->g * 0.0039215689f;
        m_lights[lightIndex].b = light->b * 0.0039215689f;

        m_lights[lightIndex].x = light->x;
        m_lights[lightIndex].y = light->y;
        m_lights[lightIndex].z = light->z;    

        Vec3Normalize( &m_lights[lightIndex].x );
    }
}

//-----------------------------------------------------------------------------
// Set Num Lights
//-----------------------------------------------------------------------------
void RSPLightManager::setNumLights(int numLights)
{
    if (numLights <= 8) 
    {
        m_numLights = numLights;    
    }
}

//-----------------------------------------------------------------------------
// Set Light Color
//-----------------------------------------------------------------------------
void RSPLightManager::setLightColor( unsigned int lightIndex, unsigned int packedColor )
{
    if (lightIndex < 8)
    {
        m_lights[lightIndex].r = _SHIFTR( packedColor, 24, 8 ) * 0.0039215689f;
        m_lights[lightIndex].g = _SHIFTR( packedColor, 16, 8 ) * 0.0039215689f;
        m_lights[lightIndex].b = _SHIFTR( packedColor, 8, 8 ) * 0.0039215689f;
    }
}
