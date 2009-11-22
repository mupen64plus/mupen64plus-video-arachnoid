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

#include "GBI.h"
#include "GBIDefs.h"
#include "RSP.h"
#include "RDP.h"
#include "UCodeSelector.h"
#include "UCodeIdentificationData.h"
#include "OpenGLManager.h"
#include "Logger.h"
#include "m64p.h"

//-----------------------------------------------------------------------------
//! Static Variables
//-----------------------------------------------------------------------------
unsigned int GBI::G_MOVEMEM, GBI::G_MOVEWORD;
unsigned int GBI::G_RDPHALF_1, GBI::G_RDPHALF_2, GBI::G_RDPHALF_CONT;
unsigned int GBI::G_SPNOOP;
unsigned int GBI::G_SETOTHERMODE_H, GBI::G_SETOTHERMODE_L;
unsigned int GBI::G_DL, GBI::G_ENDDL, GBI::G_CULLDL, GBI::G_BRANCH_Z;
unsigned int GBI::G_LOAD_UCODE;

unsigned int GBI::G_MTX, GBI::G_POPMTX;
unsigned int GBI::G_GEOMETRYMODE, GBI::G_SETGEOMETRYMODE, GBI::G_CLEARGEOMETRYMODE;
unsigned int GBI::G_TEXTURE;
unsigned int GBI::G_DMA_IO, GBI::G_DMA_DL, GBI::G_DMA_TRI, GBI::G_DMA_MTX, GBI::G_DMA_VTX, GBI::G_DMA_OFFSETS;
unsigned int GBI::G_SPECIAL_1, GBI::G_SPECIAL_2, GBI::G_SPECIAL_3;
unsigned int GBI::G_VTX, GBI::G_MODIFYVTX, GBI::G_VTXCOLORBASE;
unsigned int GBI::G_TRI1, GBI::G_TRI2, GBI::G_TRI4;
unsigned int GBI::G_QUAD, GBI::G_LINE3D;
unsigned int GBI::G_RESERVED0, GBI::G_RESERVED1, GBI::G_RESERVED2, GBI::G_RESERVED3;
unsigned int GBI::G_SPRITE2D_BASE;
unsigned int GBI::G_BG_1CYC, GBI::G_BG_COPY;
unsigned int GBI::G_OBJ_RECTANGLE, GBI::G_OBJ_SPRITE, GBI::G_OBJ_MOVEMEM;
unsigned int GBI::G_SELECT_DL, GBI::G_OBJ_RENDERMODE, GBI::G_OBJ_RECTANGLE_R;
unsigned int GBI::G_OBJ_LOADTXTR, GBI::G_OBJ_LDTX_SPRITE, GBI::G_OBJ_LDTX_RECT, GBI::G_OBJ_LDTX_RECT_R;
unsigned int GBI::G_RDPHALF_0;

unsigned int GBI::G_MTX_STACKSIZE;
unsigned int GBI::G_MTX_MODELVIEW;
unsigned int GBI::G_MTX_PROJECTION;
unsigned int GBI::G_MTX_MUL;
unsigned int GBI::G_MTX_LOAD;
unsigned int GBI::G_MTX_NOPUSH;
unsigned int GBI::G_MTX_PUSH;

unsigned int GBI::G_TEXTURE_ENABLE;
unsigned int GBI::G_SHADING_SMOOTH;
unsigned int GBI::G_CULL_FRONT;
unsigned int GBI::G_CULL_BACK;
unsigned int GBI::G_CULL_BOTH;
unsigned int GBI::G_CLIPPING;

unsigned int GBI::G_MV_VIEWPORT;

unsigned int GBI::G_MWO_aLIGHT_1, GBI::G_MWO_bLIGHT_1;
unsigned int GBI::G_MWO_aLIGHT_2, GBI::G_MWO_bLIGHT_2;
unsigned int GBI::G_MWO_aLIGHT_3, GBI::G_MWO_bLIGHT_3;
unsigned int GBI::G_MWO_aLIGHT_4, GBI::G_MWO_bLIGHT_4;
unsigned int GBI::G_MWO_aLIGHT_5, GBI::G_MWO_bLIGHT_5;
unsigned int GBI::G_MWO_aLIGHT_6, GBI::G_MWO_bLIGHT_6;
unsigned int GBI::G_MWO_aLIGHT_7, GBI::G_MWO_bLIGHT_7;
unsigned int GBI::G_MWO_aLIGHT_8, GBI::G_MWO_bLIGHT_8;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
GBI::GBI()
{
    m_ucodeSelector = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
GBI::~GBI()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool GBI::initialize(RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp)
{
    m_rsp = rsp;
    m_rdp = rdp;
    m_memory = memory;

    //Reset Binary Interface
    for(int i=0; i<256; ++i)
    {
        m_cmds[i] = (GBIFunc)GBI::unknownInstruction;
    }

    //Add RDP Instructions
    m_rdpInsructions.initialize(m_rdp, dlp);

    m_cmds[G_SETCIMG]            = (GBIFunc)RDPInstructions::RDP_SetCImg;
    m_cmds[G_SETZIMG]            = (GBIFunc)RDPInstructions::RDP_SetZImg;
    m_cmds[G_SETTIMG]            = (GBIFunc)RDPInstructions::RDP_SetTImg;
    m_cmds[G_SETTILE]            = (GBIFunc)RDPInstructions::RDP_SetTile;
    m_cmds[G_LOADTILE]           = (GBIFunc)RDPInstructions::RDP_LoadTile;
    m_cmds[G_LOADBLOCK]          = (GBIFunc)RDPInstructions::RDP_LoadBlock;
    m_cmds[G_SETTILESIZE]        = (GBIFunc)RDPInstructions::RDP_SetTileSize;
    m_cmds[G_LOADTLUT]           = (GBIFunc)RDPInstructions::RDP_LoadTLUT;
    m_cmds[G_FILLRECT]           = (GBIFunc)RDPInstructions::RDP_FillRect;
    m_cmds[G_TEXRECTFLIP]        = (GBIFunc)RDPInstructions::RDP_TexRectFlip;
    m_cmds[G_TEXRECT]            = (GBIFunc)RDPInstructions::RDP_TexRect;
    m_cmds[G_SETENVCOLOR]        = (GBIFunc)RDPInstructions::RDP_SetEnvColor;
    m_cmds[G_SETPRIMCOLOR]       = (GBIFunc)RDPInstructions::RDP_SetPrimColor;
    m_cmds[G_SETBLENDCOLOR]      = (GBIFunc)RDPInstructions::RDP_SetBlendColor;
    m_cmds[G_SETFOGCOLOR]        = (GBIFunc)RDPInstructions::RDP_SetFogColor;
    m_cmds[G_SETFILLCOLOR]       = (GBIFunc)RDPInstructions::RDP_SetFillColor;
    m_cmds[G_SETCOMBINE]         = (GBIFunc)RDPInstructions::RDP_SetCombine;
    m_cmds[G_RDPSETOTHERMODE]    = (GBIFunc)RDPInstructions::RDP_SetOtherMode;
    m_cmds[G_SETPRIMDEPTH]       = (GBIFunc)RDPInstructions::RDP_SetPrimDepth;
    m_cmds[G_SETSCISSOR]         = (GBIFunc)RDPInstructions::RDP_SetScissor;
    m_cmds[G_SETCONVERT]         = (GBIFunc)RDPInstructions::RDP_SetConvert;
    m_cmds[G_SETKEYR]            = (GBIFunc)RDPInstructions::RDP_SetKeyR;
    m_cmds[G_SETKEYGB]           = (GBIFunc)RDPInstructions::RDP_SetKeyGB;
    m_cmds[G_NOOP]               = (GBIFunc)RDPInstructions::RDP_NoOp;
    m_cmds[G_RDPFULLSYNC]        = (GBIFunc)RDPInstructions::RDP_FullSync;
    m_cmds[G_RDPTILESYNC]        = (GBIFunc)RDPInstructions::RDP_TileSync;
    m_cmds[G_RDPPIPESYNC]        = (GBIFunc)RDPInstructions::RDP_PipeSync;
    m_cmds[G_RDPLOADSYNC]        = (GBIFunc)RDPInstructions::RDP_LoadSync;

    m_ucodeSelector = new UCodeSelector();
    if ( !m_ucodeSelector->initialize(memory) ) {
        return false;
    }

    //Initilize Ucode

    m_ucode0.initialize(m_rsp, m_rdp, memory, dlp);        //F3D
    m_ucode1.initialize(this, m_rsp, m_rdp, memory);       //F3DEX
    m_ucode2.initialize(this, m_rsp, m_rdp, memory, dlp);  //Golden Eye
    m_ucode4.initialize(this, m_rsp);                      //Wave Race 64
    m_ucode5.initialize(this, m_rsp, m_rdp, memory, dlp);  //F3DEX2
    m_ucode6.initialize(this, m_rsp, m_rdp, memory, dlp);  //Diddy Kong Racing
    m_ucode7.initialize(m_rsp);
    m_ucode9.initialize(m_rsp);                            //Perfect Dark
    m_ucode10.initialize(this, m_rsp, m_rdp, memory, dlp);

    m_previusUCodeStart = -1;

    return true;
}

//-----------------------------------------------------------------------------
// Dispose
//-----------------------------------------------------------------------------
void GBI::dispose()
{
    if ( m_ucodeSelector ) { delete m_ucodeSelector; m_ucodeSelector = 0; }

    m_previusUCodeStart = -1;
}

//-----------------------------------------------------------------------------
// Select UCode
//-----------------------------------------------------------------------------
void GBI::selectUCode( unsigned int ucStart, 
                    unsigned int ucDStart, 
                    unsigned int ucSize, 
                    unsigned int ucDSize)
{
    if ( m_previusUCodeStart == ucStart )
    {
        return; //Already have correct ucode no need to find one
    }
    m_previusUCodeStart = ucStart;

    //Identify ucode
    unsigned int ucode = m_ucodeSelector->checkUCode(ucStart, ucDStart, ucSize, ucDSize);

    //Unsupported ucodes
    if ( ucode >= 6 || ucode == 3 )
    {
        //MessageBox(0, 
        //    "This graphics plugin does not support this game, please try another.", 
        //    "Arachnoid Graphics Plugin", 
        //    MB_OK|MB_SETFOREGROUND );
    }

    //if ( ucode == 5 )
    //{
    //    OpenGLManager::getSingleton().setForceDisableCulling(true);  //Do not support Face Culling?
    //}

    //Set Ucode
    switch ( ucode )
    {
        case 0   : m_ucode0.initializeGBI(this); break;  //F3D    - Super Mario 64
        case 1   : m_ucode1.initializeGBI();     break;  //F3DEX  - Mario Kart, Star Fox 64, Bomberman 64, ...
        case 2   : m_ucode2.initializeGBI();     break;  //       - Golden Eye
        case 3   :                                       //S2DEX  - Chou Snobow Kids, V-Rally Edition 99, Zelda Majoras Mask
        case 4   : m_ucode4.initializeGBI();     break;  //       - Wave Racer 64
        case 5   : m_ucode5.initializeGBI();     break;  //F3DEX2 - Donkey Kong 64, Fighter's Destiny 2, Mario Golf 64, F-Zero X, Paper Mario, ...
        case 6   : m_ucode6.initializeGBI();     break;  //       - Diddy Kong Racing
        case 7   : m_ucode7.initializeGBI(this); break;  //       - Yoshi's Story
        case 8   :                                       //       - Puzzle Master 64
        case 9   : m_ucode9.initializeGBI(this); break;  //       - Perfect Dark
        case 10  : m_ucode10.initializeGBI();            //       - Conker The Bad Fur Day
        case 11  : m_ucode6.initializeGBI();     break;  //       - Jet Force Gemini
        case 12  : m_ucode1.initializeGBI();     break;  //       - 1080
        default  : 
            //m_ucode5.initializeGBI(this); break;  //F3DEX2
            //m_ucode1.initializeGBI(this); 
            m_ucode0.initializeGBI(this); 
            break;
    }

}

//-----------------------------------------------------------------------------
//! Unknown Instruction
//! This function will be called when the GBI can't find the correct function
//! to call.
//-----------------------------------------------------------------------------
void GBI::unknownInstruction(MicrocodeArgument* arg)
{
    static bool warned = false;
    if ( !warned ) {
        Logger::getSingleton().printMsg("GBI - Unknown Function", M64MSG_WARNING);
        warned = true;
    }
}
