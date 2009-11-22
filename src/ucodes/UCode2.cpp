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

#include "UCode2.h"
#include "UCode0.h"
#include "GBI.h"
#include "RSP.h"
#include "RDP.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "Memory.h"
#include "DisplayListParser.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
GBI*               UCode2::m_gbi = 0;   //!< Pointer to Graphics Binary Interface
RSP*               UCode2::m_rsp = 0;   //!< Pointer to Reality Signal Processor 
RDP*               UCode2::m_rdp = 0;   //!< Pointer to Reality Drawing Processor 
Memory*            UCode2::m_memory            = 0;
DisplayListParser* UCode2::m_displayListParser = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode2::UCode2()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode2::~UCode2()
{
}

//-----------------------------------------------------------------------------
// Initialize 
//-----------------------------------------------------------------------------
void UCode2::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp)
{
    m_rsp = rsp;
    m_rdp = rdp;
    m_gbi = gbi;
    m_memory = mem;
    m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//* Initialize GBI
//! Assigns functions to the GBI
//-----------------------------------------------------------------------------
void UCode2::initializeGBI()
{
    UCode0::initializeGBI(m_gbi);
    GBI_SetGBI(GBI::G_RDPHALF_1, F3D_RDPHALF_1, m_gbi->m_cmds, renderSky);
}

//-----------------------------------------------------------------------------
//* RDP Half 1 With Sky Rendering
//! Render Sky
//! @todo Set Half 1 also?
//! @todo Use extracted color
//! @todo set x0 and x1
//-----------------------------------------------------------------------------
void UCode2::renderSky(MicrocodeArgument* ucode)
{    
    //Check for error
    if ( (ucode->w1)>>24 != 0xCE )
    {
        return;
    }

    unsigned int w2  = m_displayListParser->getNextWord();
    unsigned int w3  = m_displayListParser->getNextWord();
    unsigned int w4  = m_displayListParser->getNextWord();
    unsigned int w5  = m_displayListParser->getNextWord();
    unsigned int w6  = m_displayListParser->getNextWord();
    unsigned int w7  = m_displayListParser->getNextWord();
    unsigned int w8  = m_displayListParser->getNextWord();
    unsigned int w9  = m_displayListParser->getNextWord();
    unsigned int w10 = m_displayListParser->getNextWord();

    int width  = m_rsp->getTile(0)->getWidth();
    int height = m_rsp->getTile(0)->getHeight();
    
    //Extract Vertex Coordinats
    unsigned int x0 = 0;     //TODO Use VI pos or Viewport pos or Scissor pos ?
    unsigned int y0 = (unsigned int)int(w2&0xFFFF)/4;
    unsigned int x1 = 320;   //TODO Use VI Height or Viewport Height or Scissor Height ?
    unsigned int y1 = (unsigned int)int(w2>>16)/4;

    //Extract Texture Coordinats
    float uS = (short)(w4&0xFFFF)/32768.0f*width;
    float uT = (short)(w4>>16)/32768.0f*256;    
    float uDSDX = (width / (float)(x1-x0));
    float uDTDY = (height / (float)(y1-y0));

    //Extract Color
    float r = ((w9>>16)&0xFF) / 255.0f;
    float g = ((w9    )&0xFF) / 255.0f;
    float b = ((w10>>16)&0xFF) / 255.0f;
    float a = ((w10    )&0xFF) / 255.0f;

    m_rdp->RDP_TexRect(x0, y0, x1, y1, 0, 0, 0, 1024, 1024);  
}
