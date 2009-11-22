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

#ifndef UCODE_6_H_
#define UCODE_6_H_

//Includes
#include "UCodeDefs.h"

#define F3DDKR_VTX_APPEND       0x00010000
#define F3DDKR_DMA_MTX          0x01
#define F3DDKR_DMA_VTX          0x04
#define F3DDKR_DMA_TRI          0x05
#define F3DDKR_DMA_DL           0x07
#define F3DDKR_DMA_OFFSETS      0xBF

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//*****************************************************************************
//! UCode6 
//! Microcode used for Diddy Kong Racing
//*****************************************************************************
class UCode6
{
public:

    UCode6();
    ~UCode6();

    static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp);
    static void initializeGBI();

    static void F3DDKR_MoveWord(MicrocodeArgument* ucode);
    static void F3DDKR_DMA_Offsets(MicrocodeArgument* ucode);
    static void F3DDKR_DMA_DList(MicrocodeArgument* ucode);
    static void F3DDKR_DMA_Tri(MicrocodeArgument* ucode);
    static void F3DDKR_DMA_Vtx(MicrocodeArgument* ucode);
    static void F3DDKR_DMA_Mtx(MicrocodeArgument* ucode);

private:

    static GBI* m_gbi;                               //!< Pointer to Graphics Binary Interface
    static RSP* m_rsp;                               //!< Pointer to Reality Signal Processor 
    static RDP* m_rdp;                               //!< Pointer to Reality Drawing Processor  
    static Memory* m_memory;                         //!< Pointer to Memory Manager
    static DisplayListParser* m_displayListParser;   //!< Pointer to Display list parser

    static unsigned int m_vertexIndex;

};

#endif
