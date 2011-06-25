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

#ifndef ADVANCED_TEX_ENV_COMBINER_H_
#define ADVANCED_TEX_ENV_COMBINER_H_

#include "CombinerBase.h"
#include "CombinerStructs.h"

struct TexEnvCombiner;

#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine  1

    #define GL_COMBINE_ARB        0x8570
    #define GL_COMBINE_RGB_ARB    0x8571
    #define GL_COMBINE_ALPHA_ARB  0x8572
    #define GL_SOURCE0_RGB_ARB    0x8580
    #define GL_SOURCE1_RGB_ARB    0x8581
    #define GL_SOURCE2_RGB_ARB    0x8582
    #define GL_SOURCE0_ALPHA_ARB  0x8588
    #define GL_SOURCE1_ALPHA_ARB  0x8589
    #define GL_SOURCE2_ALPHA_ARB  0x858A
    #define GL_OPERAND0_RGB_ARB   0x8590
    #define GL_OPERAND1_RGB_ARB   0x8591
    #define GL_OPERAND2_RGB_ARB   0x8592
    #define GL_OPERAND0_ALPHA_ARB 0x8598
    #define GL_OPERAND1_ALPHA_ARB 0x8599
    #define GL_OPERAND2_ALPHA_ARB 0x859A
    #define GL_RGB_SCALE_ARB      0x8573
    #define GL_ADD_SIGNED_ARB     0x8574
    #define GL_INTERPOLATE_ARB    0x8575
    #define GL_CONSTANT_ARB       0x8576
    #define GL_PRIMARY_COLOR_ARB  0x8577
    #define GL_PREVIOUS_ARB       0x8578
    #define GL_SUBTRACT_ARB       0x84E7
#endif

#define SetColorCombinerValues( n, a, s, o ) \
    envCombiner->color[n].a.source = s; \
    envCombiner->color[n].a.operand = o

#define SetAlphaCombinerValues( n, a, s, o ) \
    envCombiner->alpha[n].a.source = s; \
    envCombiner->alpha[n].a.operand = o

#define SetAlphaCombinerArg( n, a, i ) \
    if (TexEnvArgs[i].source == GL_CONSTANT_ARB) \
    { \
        if ((envCombiner->alpha[n].constant == COMBINED) || (envCombiner->alpha[n].constant == i)) \
        { \
            envCombiner->alpha[n].constant = i; \
            envCombiner->alpha[n].a.source = GL_CONSTANT_ARB; \
            envCombiner->alpha[n].a.operand = GL_SRC_ALPHA; \
        } \
        else if ((envCombiner->vertex.alpha == COMBINED) || (envCombiner->vertex.alpha == i)) \
        { \
            envCombiner->vertex.alpha = i; \
            envCombiner->alpha[n].a.source = GL_PRIMARY_COLOR_ARB; \
            envCombiner->alpha[n].a.operand = GL_SRC_ALPHA; \
        } \
    } \
    else \
    { \
        envCombiner->alpha[n].a.source = TexEnvArgs[i].source; \
        envCombiner->alpha[n].a.operand = GL_SRC_ALPHA; \
    }

#define SetColorCombinerArg( n, a, i ) \
 if (TexEnvArgs[i].source == GL_CONSTANT_ARB) \
    { \
        if ((i > 5) && ((envCombiner->alpha[n].constant == COMBINED) || (envCombiner->alpha[n].constant == i))) \
        { \
            envCombiner->alpha[n].constant = i; \
            envCombiner->color[n].a.source = GL_CONSTANT_ARB; \
            envCombiner->color[n].a.operand = GL_SRC_ALPHA; \
        } \
        else if ((i > 5) && ((envCombiner->vertex.alpha == COMBINED) || (envCombiner->vertex.alpha == i))) \
        { \
            envCombiner->vertex.alpha = i; \
            envCombiner->color[n].a.source = GL_PRIMARY_COLOR_ARB; \
            envCombiner->color[n].a.operand = GL_SRC_ALPHA; \
        } \
        else if ((envCombiner->color[n].constant == COMBINED) || (envCombiner->color[n].constant == i)) \
        { \
            envCombiner->color[n].constant = i; \
            envCombiner->color[n].a.source = GL_CONSTANT_ARB; \
            envCombiner->color[n].a.operand = GL_SRC_COLOR; \
        } \
        else if ( ATIX_texture_env_route && ((envCombiner->vertex.secondaryColor == COMBINED) || (envCombiner->vertex.secondaryColor == i))) \
        { \
            envCombiner->vertex.secondaryColor = i; \
            envCombiner->color[n].a.source = GL_SECONDARY_COLOR_ATIX; \
            envCombiner->color[n].a.operand = GL_SRC_COLOR; \
        } \
        else if ((envCombiner->vertex.color == COMBINED) || (envCombiner->vertex.color == i))\
        { \
            envCombiner->vertex.color = i; \
            envCombiner->color[n].a.source = GL_PRIMARY_COLOR_ARB; \
            envCombiner->color[n].a.operand = GL_SRC_COLOR; \
        } \
    } \
    else \
    { \
        envCombiner->color[n].a.source = TexEnvArgs[i].source; \
        envCombiner->color[n].a.operand = TexEnvArgs[i].operand; \
    }

//*****************************************************************************
//! Advanced Texture Environment Combiner
//! Class used to combine colors by setting texture enviroment using OpenGL
//*****************************************************************************
class AdvancedTexEnvCombiner : public CombinerBase
{
public:

    //Constructor / Destructor
    AdvancedTexEnvCombiner();
    ~AdvancedTexEnvCombiner();

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

    //Extensions
    bool ARB_texture_env_combine;
    bool ARB_texture_env_crossbar;
    bool ATI_texture_env_combine3;
    bool ATIX_texture_env_route;
    bool NV_texture_env_combine4;

};

#endif
