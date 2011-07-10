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

#include "CombinerBase.h"
#include "CombinerStructs.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
CombinerBase::CombinerBase()
{
    m_fillColor[0] = m_blendColor[0] = m_primColor[0] = m_envColor[0] = 0;
    m_fillColor[1] = m_blendColor[1] = m_primColor[1] = m_envColor[1] = 0;
    m_fillColor[2] = m_blendColor[2] = m_primColor[2] = m_envColor[2] = 0;
    m_fillColor[3] = m_blendColor[3] = m_primColor[3] = m_envColor[3] = 1;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
CombinerBase::~CombinerBase()
{

}

//-----------------------------------------------------------------------------
//! Set Fill Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void CombinerBase::setFillColor (float r, float g, float b, float a)
{
    m_fillColor[0] = r;
    m_fillColor[1] = g;
    m_fillColor[2] = b;
    m_fillColor[3] = a;
}

//-----------------------------------------------------------------------------
//! Set Blend Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void CombinerBase::setBlendColor(float r, float g, float b, float a)
{
    m_blendColor[0] = r;
    m_blendColor[1] = g;
    m_blendColor[2] = b;
    m_blendColor[3] = a;
}

//-----------------------------------------------------------------------------
//! Set Prim Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void CombinerBase::setPrimColor (float r, float g, float b, float a)
{
    m_primColor[0] = r;
    m_primColor[1] = g;
    m_primColor[2] = b;
    m_primColor[3] = a;
}

//-----------------------------------------------------------------------------
//! Set Enviroment Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void CombinerBase::setEnvColor(float r, float g, float b, float a)
{
    m_envColor[0] = r;
    m_envColor[1] = g;
    m_envColor[2] = b;
    m_envColor[3] = a;
}

//-----------------------------------------------------------------------------
//! Get Combiner Color
//! @param[in] colorSource From which colorsource to retrive color values
//!    \arg \c PRIMITIVE Get color from primative color
//!    \arg \c ENVIRONMENT Get color from environment color
//!    \arg \c PRIMITIVE_ALPHA Get color from primatives alpha value
//!    \arg \c ENV_ALPHA Get color from environment colors alpha value
//!    \arg \c PRIM_LOD_FRAC Get color from primative-LOD-frac value
//!    \arg \c ONE Get white color
//!    \arg \c ZERO Get black color
//! @param[in] alphaSource From which alphasource to retrive alpha value
//!    \arg \c PRIMITIVE_ALPHA Get alpha value from primitive colors alpha value
//!    \arg \c ENV_ALPHA Get alpha value from environment colors alpha value
//!    \arg \c PRIM_LOD_FRAC Get alpha value from primative-LOD-frac value
//!    \arg \c ONE Set alpha value to 1.0
//!    \arg \c ZERO Set alpha value to 0.0
//! @param[out] out The combiner color with color and alpha value
//-----------------------------------------------------------------------------
void CombinerBase::getCombinerColor(float out[4], short colorSrc, short alphaSrc)
{
    //Set color values
    switch ( colorSrc ) 
    { 
        case PRIMITIVE: 
            out[0] = m_primColor[0]; 
            out[1] = m_primColor[1]; 
            out[2] = m_primColor[2]; 
            break; 
        case ENVIRONMENT: 
            out[0] = m_envColor[0]; 
            out[1] = m_envColor[1]; 
            out[2] = m_envColor[2]; 
            break; 
        case PRIMITIVE_ALPHA: 
            out[0] = m_primColor[3]; 
            out[1] = m_primColor[3]; 
            out[2] = m_primColor[3]; 
            break; 
        case ENV_ALPHA: 
            out[0] = m_envColor[3]; 
            out[1] = m_envColor[3]; 
            out[2] = m_envColor[3]; 
            break; 
        case PRIM_LOD_FRAC: 
            out[0] = m_primLodFrac; 
            out[1] = m_primLodFrac; 
            out[2] = m_primLodFrac; 
            break; 
        case ONE: 
            out[0] = 1.0f; 
            out[1] = 1.0f; 
            out[2] = 1.0f; 
            break; 
        case ZERO: 
            out[0] = 0.0f; 
            out[1] = 0.0f; 
            out[2] = 0.0f; 
            break; 
    } 

    //Set alpha value
    switch ( alphaSrc ) 
    { 
        case PRIMITIVE_ALPHA: 
            out[3] = m_primColor[3]; 
            break; 
        case ENV_ALPHA: 
            out[3] = m_envColor[3]; 
            break; 
        case PRIM_LOD_FRAC: 
            out[3] = m_primLodFrac; 
            break; 
        case ONE: 
            out[3] = 1.0f; 
            break; 
        case ZERO: 
            out[3] = 0.0f; 
            break; 
    } 
}
