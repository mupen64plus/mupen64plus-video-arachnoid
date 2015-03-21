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

#ifndef SIMPLE_TEX_ENV_COMBINER_H_
#define SIMPLE_TEX_ENV_COMBINER_H_

#include "CombinerBase.h"
#include "CombinerStructs.h"
#include "GBIDefs.h"

//Forward declarations
struct CombineCycle;
struct CombinerStage;
struct Combiner;

//*****************************************************************************
//! Simple Texture Environment Combiner
//! Uses glTexEnvi to set texture environment i OpenGL.
//*****************************************************************************
class SimpleTexEnvCombiner : public CombinerBase
{
public:
    
    //Constructor / Destructor
    SimpleTexEnvCombiner();
    ~SimpleTexEnvCombiner();

    //Initialize
    void initialize();

    //Begin / End Texture Update
    void beginTextureUpdate();
    void endTextureUpdate(TexEnvCombiner* texEnv);   

    //Sets texture enviorment colors
    void setTextureEnviromentColors(TexEnvCombiner* texEnv);

    //Create New Texture Environment
    TexEnvCombiner* createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner);

    //Sets texture enviorment
    void setTextureEnviroment(TexEnvCombiner* texEnv);

private:

    bool ARB_multitexture;   //!< Multitexture Extension supported?

};

#endif
