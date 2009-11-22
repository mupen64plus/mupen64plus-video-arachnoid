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

#ifndef UCODE_9_H_
#define UCODE_9_H_

//Includes
#include "UCodeDefs.h"

class GBI;
class RSP;

//*****************************************************************************
//! UCode9
//! Microcode used to play Perfect Dark
//*****************************************************************************
class UCode9
{
public:

    //Constructor / Destructor
    UCode9();
    ~UCode9();

    static void initialize(RSP* rsp);
    static void initializeGBI(GBI* gbi);

    static void PerfectDark_Vertex(MicrocodeArgument* ucode);
    static void PerfectDark_VertexColorBase(MicrocodeArgument* ucode);

private:

    static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
};

#endif
