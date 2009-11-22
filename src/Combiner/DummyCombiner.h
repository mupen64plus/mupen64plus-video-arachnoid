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

#ifndef DUMMY_COMBINER_H_
#define DUMMY_COMBINER_H_

//Include
#include "CombinerBase.h"   //baseclass for combiners

//Forward declarations
struct TexEnvCombiner;
struct Combiner;

//*****************************************************************************
//* Dummy Combiner
//! Very simple combiner that does as little as possible 
//*****************************************************************************
class DummyCombiner : public CombinerBase
{
public:

    //Constructor / Destructor
    DummyCombiner() {}
    ~DummyCombiner() {}

    //Initialize
    void initialize();

    //Begin / End Texture Update
    void beginTextureUpdate() {}
    void endTextureUpdate(TexEnvCombiner* texEnv) {}   

    //Sets texture enviorment colors
    void setTextureEnviromentColors(TexEnvCombiner* texEnv) {}

    //Create New Texture Environment
    TexEnvCombiner* createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner);

    //Sets texture enviorment
    void setTextureEnviroment(TexEnvCombiner* texEnv);

private:
    
    bool ARB_multitexture;   //!< Extension supported?

};

#endif
