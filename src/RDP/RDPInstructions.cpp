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

#include "RDPInstructions.h"
#include "RDPUCodeStructs.h"
#include "RDP.h"
#include "DisplayListParser.h"
#include "Logger.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------

RDP*               RDPInstructions::m_rdp               = 0;
DisplayListParser* RDPInstructions::m_displayListParser = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RDPInstructions::RDPInstructions()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RDPInstructions::~RDPInstructions()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool RDPInstructions::initialize(RDP* rdp, DisplayListParser* displayListParser)
{
    m_rdp = rdp;
    m_displayListParser = displayListParser;
    return true;
}

//-----------------------------------------------------------------------------
//! Set Color Image
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetCImg(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetCImg");
    RDPUCodeSetImage* temp = (RDPUCodeSetImage*)ucode;

    //Set Color Image
    m_rdp->RDP_SetCImg(temp->format, temp->size, temp->width, temp->segmentAddress);
}

//-----------------------------------------------------------------------------
//! Set Z Image
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetZImg(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetZImg");
    RDPUCodeSetImage* temp = (RDPUCodeSetImage*)ucode;

    //Set Depth Image
    m_rdp->RDP_SetZImg(temp->format, temp->size, temp->width, temp->segmentAddress);
}

//-----------------------------------------------------------------------------
//! Set Texture Image
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetTImg(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetTImg");
    RDPUCodeSetImage* temp = (RDPUCodeSetImage*)ucode;

    //Set Texture Image
    m_rdp->RDP_SetTImg(temp->format, temp->size, temp->width, temp->segmentAddress);
}

//-----------------------------------------------------------------------------
//! Set Tile
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetTile(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetTile");
    RDPUCodeSetTile* temp = (RDPUCodeSetTile*)ucode;

    //Set Tile
    m_rdp->RDP_SetTile( temp->format, temp->size, temp->line, temp->tmem, temp->tile, 
        temp->palette, temp->clampS, temp->clampT, temp->mirrorS, temp->mirrorT, 
        temp->maskS, temp->maskT, temp->shiftS, temp->shiftT
    );
}

//-----------------------------------------------------------------------------
//! Load Tile
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_LoadTile(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_LoadTile");
    RDPUCodeTileSize* temp = (RDPUCodeTileSize*)ucode;

    //Load Tile
    m_rdp->RDP_LoadTile(temp->tile, temp->s0, temp->t0, temp->s1, temp->t1);
}

//-----------------------------------------------------------------------------
//! Load Block
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_LoadBlock(MicrocodeArgument* ucode)
{  
    Logger::getSingleton().printMsg("RDP_LoadBlock");
    RDPUCodeTileSize* temp = (RDPUCodeTileSize*)ucode;

    //Load Block
    m_rdp->RDP_LoadBlock(temp->tile, temp->s0, temp->t0, temp->s1, temp->t1);
}

//-----------------------------------------------------------------------------
//! Sets the size of tile
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetTileSize(MicrocodeArgument* ucode)
{  
    Logger::getSingleton().printMsg("RDP_SetTileSize");
    RDPUCodeTileSize* temp = (RDPUCodeTileSize*)ucode;

    //Set Tile Size
    m_rdp->RDP_SetTileSize(temp->tile, temp->s0, temp->t0, temp->s1, temp->t1);
}

//-----------------------------------------------------------------------------
//! Set Texture Look Up Table
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_LoadTLUT(MicrocodeArgument* ucode)
{   
    Logger::getSingleton().printMsg("RDP_LoadTLUT");
    RDPUCodeTileSize* temp = (RDPUCodeTileSize*)ucode;

    //Load Texture Look Up Table
    m_rdp->RDP_LoadTLUT(temp->tile, temp->s0, temp->t0, temp->s1, temp->t1);
}

//-----------------------------------------------------------------------------
//* Fill Rect
//! Renders a rectangle
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_FillRect(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_FillRect");
    RDPUCodeRectangle* temp = (RDPUCodeRectangle*)ucode;

    //Render a Rectangle
    m_rdp->RDP_FillRect(temp->x0, temp->y0, temp->x1, temp->y1);
}

//-----------------------------------------------------------------------------
//* Texture Rectangle Flipped
//! Renders a textured rectangle
//! @todo Better extraction of data
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_TexRectFlip(MicrocodeArgument* ucode)
{   
    Logger::getSingleton().printMsg("RDP_TexRectFlip");
    RDPUCodeTextureRectangle* temp = (RDPUCodeTextureRectangle*)ucode;

    //Get Extra Words
    unsigned int w2 = m_displayListParser->getNextWord();
    unsigned int w3 = m_displayListParser->getNextWord();

    //Extract Data
    unsigned int dwS        = (  w2>>16)&0xFFFF;
    unsigned int dwT        = (  w2    )&0xFFFF;
    int nDSDX                   = (int)(short)((  w3>>16)&0xFFFF);
    int nDTDY                  = (int)(short)((  w3    )&0xFFFF);

    //Render Texture Rectangle Flipped
    m_rdp->RDP_TexRectFlip(temp->x1 / 4, temp->y1 / 4, 
                           temp->x0 / 4, temp->y0 / 4,
                           temp->tile,                            
                           dwS, dwT, 
                           nDSDX, nDTDY);
}

//-----------------------------------------------------------------------------
//* Texture Rect
//! Not this command use 128bits and not 64 bits wich could cause some
//! problems with the program counter.
//! @todo Better extraction of data
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_TexRect(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_TexRect");    
    RDPUCodeTextureRectangle* temp = (RDPUCodeTextureRectangle*)ucode;

    unsigned int w2 = m_displayListParser->getNextWord();
    unsigned int w3 = m_displayListParser->getNextWord();

    //Extract Data
    unsigned short uS     = (unsigned short)(  w2>>16)&0xFFFF;
    unsigned short uT     = (unsigned short)(  w2    )&0xFFFF;
    unsigned short uDSDX = (unsigned short)((  w3>>16)&0xFFFF);
    unsigned short uDTDY = (unsigned short)((  w3    )&0xFFFF);

    //Render Texture Rectangle
    m_rdp->RDP_TexRect( temp->x0 / 4, temp->y0 / 4, 
                        temp->x1 / 4, temp->y1 / 4, 
                        temp->tile, 
                        uS, uT, 
                        uDSDX, uDTDY);
}

//-----------------------------------------------------------------------------
//! Set Enviroment Color
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetEnvColor(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetEnvColor");
    RDPUCodeSetColor* temp = (RDPUCodeSetColor*)ucode;

    //Set enviorment color
    m_rdp->RDP_SetEnvColor(temp->r / 255.0f, temp->g / 255.0f, temp->b / 255.0f, temp->a / 255.0f);
}

//-----------------------------------------------------------------------------
//! Set Blend Color
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetBlendColor(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetBlendColor");
    RDPUCodeSetColor* temp = (RDPUCodeSetColor*)ucode;

    //Set blend color
    m_rdp->RDP_SetBlendColor(temp->r / 255.0f, temp->g / 255.0f, temp->b / 255.0f, temp->a / 255.0f);
}

//-----------------------------------------------------------------------------
//! Set Prim Color
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetPrimColor(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetPrimColor");
    RDPUCodeSetColor* temp = (RDPUCodeSetColor*)ucode;

    //Set primitive color
    m_rdp->RDP_SetPrimColor( temp->r / 255.0f,    //red
                             temp->g / 255.0f,    //green 
                             temp->b / 255.0f,    //blue
                             temp->a / 255.0f,    //alpha
                             temp->prim_min_level, 
                             temp->prim_level );
}

//-----------------------------------------------------------------------------
//! Set Fog Color
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetFogColor(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDPInstructions_SetFogColor");
    RDPUCodeSetColor* temp = (RDPUCodeSetColor*)ucode;

    //Set fog color
    m_rdp->RDP_SetFogColor(temp->r / 255.0f, temp->g / 255.0f, temp->b / 255.0f, temp->a / 255.0f);
}

//-----------------------------------------------------------------------------
//! Set Fill Color
//! Note: Fill color is stored diffrently from the other types of colors
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetFillColor(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetFillColor");
    RDPUCodeSetFillColor* temp = (RDPUCodeSetFillColor*)ucode;

    //Set fill color (Note: alpha is 0.0 or 1.0) FIXME: 32 or 31? 31 seems to work better with Super Mario 64
    m_rdp->RDP_SetFillColor(temp->r / 31.0f, temp->g / 31.0f, temp->b / 31.0f, (float)temp->a);  
}

//-----------------------------------------------------------------------------
//! Set Combine
//! @todo Extract data
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetCombine(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_SetCombine");
    m_rdp->RDP_SetCombine(ucode);
}

//-----------------------------------------------------------------------------
//! Set Other Mode
//! @todo Extract data
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetOtherMode(MicrocodeArgument* ucode)
{  
    Logger::getSingleton().printMsg("RDP_SetOtherMode");
    m_rdp->RDP_SetOtherMode(ucode);
}

//-----------------------------------------------------------------------------
//! Set Prim Depth
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetPrimDepth(MicrocodeArgument* ucode)
{  
    Logger::getSingleton().printMsg("RDP_SetPrimDepth");
    RDPUCodeSetPrimDepth* temp = (RDPUCodeSetPrimDepth*)ucode;

    //Set Prim Depth
    m_rdp->RDP_SetPrimDepth(temp->z, temp->dz);
}

//-----------------------------------------------------------------------------
//! Set Scissor
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetScissor(MicrocodeArgument* ucode)
{  
    Logger::getSingleton().printMsg("RDP_SetScissor");
    RDPUCodeScissor* temp = (RDPUCodeScissor*)ucode;

    //Set Scissor
    m_rdp->RDP_SetScissor(temp->x0 / 4, temp->y0 / 4, temp->x1 / 4, temp->y1 / 4, temp->mode);
}

//-----------------------------------------------------------------------------
//* Full Sync
//! Function that signals end of a frame.
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_FullSync(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("RDP_FullSync");
    m_rdp->RDP_FullSync();
}

//-----------------------------------------------------------------------------
//* Tile Sync
//! Ignored (Function that signals synchronize of texture tile change)
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_TileSync(MicrocodeArgument* ucode)
{   
    //Ignore

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_TileSync - Ignored", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//* Pipe Sync 
//! Ignored (Function that signals synchronize of RDP attribute change)
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_PipeSync(MicrocodeArgument* ucode)
{   
    //Ignore

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_PipeSync - Ignored", M64MSG_WARNING);
        warned = true;
    }    
}

//-----------------------------------------------------------------------------
//* Load Sync
//! Ignored (Function that signals synchronize of textureloading Ignored)
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_LoadSync(MicrocodeArgument* ucode)
{   
    //Ignored

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_LoadSync - Ignored", M64MSG_WARNING);
        warned = true;
    }   
}

//-----------------------------------------------------------------------------
//* Set Convert
//! Unimplemented
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetConvert(MicrocodeArgument* ucode)
{   
    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_SetConvert - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//* Set Key Red
//! Unimplemented
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetKeyR(MicrocodeArgument* ucode)
{   
    Logger::getSingleton().printMsg("RDP_SetKeyR");

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_SetKeyR - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//* Set Key Green Blue
//! Unimplemented
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_SetKeyGB(MicrocodeArgument* ucode)
{
    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_SetKeyGB - Unimplemented", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//* Unknown
//! This function gets called when the GBI recives an unknown instruction.
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_Unknown(MicrocodeArgument* ucode)
{ 
    //Ignore

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_Unknown - Ignored", M64MSG_WARNING);
        warned = true;
    }
}

//-----------------------------------------------------------------------------
//* No Operation
//! Do nothing
//-----------------------------------------------------------------------------
void RDPInstructions::RDP_NoOp(MicrocodeArgument* ucode)
{ 
    //Ignore

    static bool warned = false;
    if ( warned ) {
        Logger::getSingleton().printMsg("RDP_NoOp - Ignored", M64MSG_WARNING);
        warned = true;
    }
}
