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

#include "DummyCombiner.h"
#include "CombinerStructs.h"
#include "ExtensionChecker.h"
#include "MultiTexturingExt.h"

//-----------------------------------------------------------------------------
//* Initialize
//! Checks if multitexturing is supported
//-----------------------------------------------------------------------------
void DummyCombiner::initialize() 
{ 
    ARB_multitexture = isExtensionSupported("GL_ARB_multitexture"); 
}

//-----------------------------------------------------------------------------
//* Create New Texture Enviroment
//! Allocates a new texture enviroment
//! @param[in] colorCombiner How to combine and get a color
//! @param[in] alphaCombiner How to combine and get an alpha value
//! @return The texture enviroment that was created
//-----------------------------------------------------------------------------
TexEnvCombiner* DummyCombiner::createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner)
{
    TexEnvCombiner* texEnv = new TexEnvCombiner();    
    texEnv->usesT0 = false;
    texEnv->usesT1 = false;
    texEnv->mode = GL_REPLACE;
    texEnv->vertex.color = COMBINED;
    texEnv->vertex.alpha = COMBINED;    

    //For each stage in alpha combiner
    for (int i = 0; i < alphaCombiner->numStages; i++)
    {
        //For each operation in stage
        for (int j = 0; j < alphaCombiner->stage[i].numOps; j++)
        {
            CombinerOp* op = &colorCombiner->stage[i].op[j];

            if ( op->param1 == TEXEL0 )
            {
                texEnv->usesT0 = true;
            }
        }
    }
    return texEnv; 
}

//-----------------------------------------------------------------------------
//* Set Texture Enviroment
//! Sets OpenGL (enables texturing)
//-----------------------------------------------------------------------------
void DummyCombiner::setTextureEnviroment(TexEnvCombiner* texEnv)
{
    //Enable Texturing
    if ( ARB_multitexture )
        glActiveTextureARB( GL_TEXTURE0_ARB );

    if ( texEnv->usesT0 )
        glEnable( GL_TEXTURE_2D );
    else
        glDisable( GL_TEXTURE_2D );
}
