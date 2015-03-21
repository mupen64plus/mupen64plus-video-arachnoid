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

#ifndef FOG_MANAGER_H_
#define FOG_MANAGER_H_

//*****************************************************************************
//* Fog Manager
//! Class for setting fog using OpenGL
//! @details This manager has support for vertex based using OpenGL Extensions
//*****************************************************************************
class FogManager
{
public:

    //Constructor / Destructor
    FogManager();
    ~FogManager();

    //Initialize / Dispose
    void initialize();
    void dispose();

    //Set Fog settings
    void setLinearFog(float start=0, float end=255);
    void setFogColor(float r, float g, float b, float a);

    //Extensions (for vertex based fog)
    void setFogCoordPointer(unsigned int type, int stride, const void* pointer);
    void enableFogCoordArray();
    void disableFogCoordArray();

    //Get/Set N64 Settings 
    void setFogSettings(float multiplier, float offset) { m_multiplier = multiplier; m_offset = offset; }
    float getMultiplier()                               { return m_multiplier;                          }
    float getOffset()                                   { return m_offset;                              }

public:

    //! Is fog extension supported?
    static bool fogExtensionsSupported() { return m_fogExtensionsSupported; }

private:

    static bool m_fogExtensionsSupported;  //!< Is fog extension supported
    float m_multiplier;                    //!< Fog multiplier
    float m_offset;                        //!< Fog offset

};

#endif
