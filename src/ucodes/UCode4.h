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

#ifndef UCODE_4_H_
#define UCODE_4_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;

//*****************************************************************************
//! UCode4 
//! Microcode used for Wave Race 64
//*****************************************************************************
class UCode4
{
public:

    UCode4();
    ~UCode4();

    static void initialize(GBI* gbi, RSP* rsp);
    static void initializeGBI();

    static void WaveRace64_Vertex(MicrocodeArgument* ucode);
    static void WaveRace64_Tri1(MicrocodeArgument* ucode);
    static void WaveRace64_Tri2(MicrocodeArgument* ucode);
    static void WaveRace64_Quad(MicrocodeArgument* ucode);

private:

    static GBI* m_gbi;
    static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 

};


#endif