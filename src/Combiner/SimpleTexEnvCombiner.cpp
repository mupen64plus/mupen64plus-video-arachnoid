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

#include "SimpleTexEnvCombiner.h"
#include "CombinerStructs.h"
#include "MultiTexturingExt.h"
#include "ExtensionChecker.h"
#include "m64p.h"
#include "OpenGL.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
SimpleTexEnvCombiner::SimpleTexEnvCombiner()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
SimpleTexEnvCombiner::~SimpleTexEnvCombiner()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//! Checks if multitexturing is supported 
//-----------------------------------------------------------------------------
void SimpleTexEnvCombiner::initialize()
{
    ARB_multitexture = isExtensionSupported("GL_ARB_multitexture");
}

//-----------------------------------------------------------------------------
//* Begin Texture Update
//! Called before texture channels are updated on the RDP
//-----------------------------------------------------------------------------
void SimpleTexEnvCombiner::beginTextureUpdate()
{
    //Ignore?
}

//-----------------------------------------------------------------------------
//* End Texture Update
//! Called after texture channels are updated on the RDP
//! @param[in] texEnv Texture Environment
//-----------------------------------------------------------------------------
void SimpleTexEnvCombiner::endTextureUpdate(TexEnvCombiner* texEnv)
{  
    //Ignore
}

//-----------------------------------------------------------------------------
//! Set Texture Envirorment Colors
//! @param[in] texEnv Texture Environment
//-----------------------------------------------------------------------------
void SimpleTexEnvCombiner::setTextureEnviromentColors(TexEnvCombiner* texEnv)
{
    //Ignore
}

//-----------------------------------------------------------------------------
//* Set Texture Environment
//! Enables texturing and sets texture environment in OpenGL
//! @param[in] texEnv Texture Environment
//-----------------------------------------------------------------------------
void SimpleTexEnvCombiner::setTextureEnviroment(TexEnvCombiner* texEnv)
{
    if ( ARB_multitexture )
        glActiveTextureARB( GL_TEXTURE0_ARB );

    if (texEnv->usesT0 || texEnv->usesT1)
        glEnable( GL_TEXTURE_2D );
    else
        glDisable( GL_TEXTURE_2D );

    //Set Mode
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texEnv->mode);
}

//-----------------------------------------------------------------------------
//* Create New Texture Enviornment
//! Allocates a new texture enviroment
//! @param[in] colorCombiner How to combine and get a color
//! @param[in] alphaCombiner How to combine and get an alpha value
//! @return The texture enviroment that was created
//-----------------------------------------------------------------------------
TexEnvCombiner* SimpleTexEnvCombiner::createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner)
{
    TexEnvCombiner* texEnv = new TexEnvCombiner();

    bool m_usesTexture0 = false;
    bool m_usesTexture1 = false;
    int mode = GL_REPLACE;
    unsigned short m_color = COMBINED;
    unsigned short m_alpha = COMBINED;    

    //For each stage in alpha combiner
    for (int i = 0; i < alphaCombiner->numStages; i++)
    {
        //For each operation in stage
        for (int j = 0; j < alphaCombiner->stage[i].numOps; j++)
        {
            CombinerOp* op = &alphaCombiner->stage[i].op[j];

            //Apply operation
            switch ( alphaCombiner->stage[i].op[j].op )
            {
                case LOAD:
                    if ( op->param1 != TEXEL0_ALPHA &&  op->param1 != TEXEL1_ALPHA  )
                    {
                        m_alpha = op->param1;
                        m_usesTexture0 = false;
                        m_usesTexture1 = false;
                    }
                    else
                    {
                        mode = GL_REPLACE;
                        m_usesTexture0 = op->param1 == TEXEL0_ALPHA;
                        m_usesTexture1 = op->param1 == TEXEL1_ALPHA;
                    }
                    break;
                case MUL: {
                        CombinerOp* prevOp = &alphaCombiner->stage[i].op[j-1];

                        if (((op->param1 == TEXEL0_ALPHA) || (op->param1 == TEXEL1_ALPHA)) &&
                            ((prevOp->param1 != TEXEL0_ALPHA) || (prevOp->param1 != TEXEL1_ALPHA)))
                        {
                            mode = GL_MODULATE;
                        }
                        else if ( ( op->param1 != TEXEL0_ALPHA || op->param1 != TEXEL1_ALPHA) &&
                                (prevOp->param1 == TEXEL0_ALPHA || prevOp->param1 == TEXEL1_ALPHA) )
                        {
                            m_alpha = op->param1;
                            mode = GL_MODULATE;
                        }
                    }
                    break;
            }
        }
    }


    //For each stage in colorCombiner
    for (int i = 0; i < colorCombiner->numStages; i++)
    {
        for (int j = 0; j < colorCombiner->stage[i].numOps; j++)
        {
            CombinerOp* op = &colorCombiner->stage[i].op[j];

            switch ( colorCombiner->stage[i].op[j].op )
            {
                case LOAD:
                    if (op->param1 == TEXEL0 || op->param1 == TEXEL0_ALPHA)
                    {
                        if ( mode == GL_MODULATE )
                            m_color = ONE;

                        m_usesTexture0 = true;
                        m_usesTexture1 = false;
                    }
                    else if ( op->param1 == TEXEL1 || op->param1 == TEXEL1_ALPHA )
                    {
                        if ( mode == GL_MODULATE )
                            m_color = ONE;

                        m_usesTexture0 = false;
                        m_usesTexture1 = true;
                    }
                    else
                    {
                        m_color = op->param1;
                        m_usesTexture0 = m_usesTexture1 = false;
                    }
                    break;
                case MUL:
                    if ( op->param1 == TEXEL0 || op->param1 == TEXEL0_ALPHA )
                    {
                        if (!m_usesTexture0 && !m_usesTexture1)
                        {
                            mode = GL_MODULATE;
                            m_usesTexture0 = true;
                            m_usesTexture1 = false;
                        }
                    }
                    else if ( op->param1 == TEXEL1 || op->param1 == TEXEL1_ALPHA )
                    {
                        if (!m_usesTexture0 && !m_usesTexture1)
                        {
                            mode = GL_MODULATE;
                            m_usesTexture0 = false;
                            m_usesTexture1 = true;
                        }
                    }
                    else if ( m_usesTexture0 || m_usesTexture1 )
                    {
                        mode = GL_MODULATE;
                        m_color = op->param1;
                    }
                    break;
                case INTERPOLATE:
                    if ((op->param1 == TEXEL0) &&
                        ((op->param2 != TEXEL0) && (op->param2 != TEXEL0_ALPHA) &&
                         (op->param2 != TEXEL1) && (op->param2 != TEXEL1_ALPHA)) &&
                         (op->param3 == TEXEL0_ALPHA))
                    {
                        mode = GL_DECAL;
                        m_color = op->param2;
                        m_usesTexture0 = true;
                        m_usesTexture1 = false;
                    }
                    else if ((op->param1 == TEXEL0) &&
                        ((op->param2 != TEXEL0) && (op->param2 != TEXEL0_ALPHA) &&
                         (op->param2 != TEXEL1) && (op->param2 != TEXEL1_ALPHA)) &&
                         (op->param3 == TEXEL0_ALPHA))
                    {
                        mode = GL_DECAL;
                        m_color = op->param2;
                        m_usesTexture0 = false;
                        m_usesTexture1 = true;
                    }
                    break;
            }
        }
    }

    texEnv->usesT0         = m_usesTexture0;
    texEnv->usesT1         = m_usesTexture1;
    texEnv->mode           = mode;
    texEnv->vertex.color   = m_color;
    texEnv->vertex.alpha   = m_alpha;     
    return texEnv;
}
