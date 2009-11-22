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

#ifndef RSP_LIGHT_MANAGER_H_
#define RSP_LIGHT_MANAGER_H_

//Forward declarations
class Memory;

//-----------------------------------------------------------------------------
//* Light
//! Struct defining how lights are stored in RDRAM 
//-----------------------------------------------------------------------------
struct RDRAMLight
{
    unsigned char pad0;       //!< Padding
    unsigned char b, g, r;    //!< Color <blue, green, red>
    unsigned char pad1;       //!< Padding
    unsigned char b2, g2, r2; //!< Color <blue, green, red>
    char pad2;                //!< Padding
    char z, y, x;             //!< Direction
};

//-----------------------------------------------------------------------------
//* RSP Light
//! Struct used to store information about lights
//-----------------------------------------------------------------------------
struct RSPLight
{
    float r, g, b;  //Color
    float x, y, z;  //Direction
};

//*****************************************************************************
//! RSP Light Manager
//*****************************************************************************
class RSPLightManager
{
public:

    //Constructor / Destructor
    RSPLightManager();
    ~RSPLightManager();

    bool initialize(Memory* memory);

    void setLight( unsigned int lightIndex, unsigned int rdramAddress  );
    void setNumLights(int numLights);
    void setLightColor( unsigned int lightIndex, unsigned int packedColor );    
    const float* getAmbientLight() { return &m_lights[m_numLights].r; }

    //Get
    int getNumLights()                              { return m_numLights; }
    void setLightEnabled(bool enabled)              { m_lightEnabled = enabled; }
    bool getLightEnabled()                          { return m_lightEnabled; }    
    const float* getLightColor(int lightIndex)      { return &m_lights[lightIndex].r; }
    const float* getLightDirection(int lightIndex)  { return &m_lights[lightIndex].x; }

private:

    Memory* m_memory;

    //Light
    bool m_lightEnabled;
    RSPLight m_lights[8];
    int m_numLights;

};


#endif
