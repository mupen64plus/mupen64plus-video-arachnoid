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

#ifndef ADVANCED_COMBINER_MANAGER_H_
#define ADVANCED_COMBINER_MANAGER_H_

#include "GBIDefs.h"
#include "CombinerStructs.h"
#include "CombinerCache.h"

//Forward declarations
struct TexEnvCombiner;
struct CombineCycle;
struct CombinerStage;
struct Combiner;
class AdvancedTexEnvCombiner;
class CombinerCache;
class CombinerBase;

//*****************************************************************************
//! Advanced Texture Environment Combiner
//! Class used to select sutible combiners and sending input to selected
//! Combiners.
//*****************************************************************************
class AdvancedCombinerManager
{
public:
    
    //Constructor / Destructor
    AdvancedCombinerManager();
    ~AdvancedCombinerManager();

    //Initialize / Dispose
    void initialize();    
    void dispose();

    //Set Mux
    void setMux(unsigned long long muxs, unsigned int cycleType);
    void setMux(unsigned int muxs0, unsigned int muxs1, unsigned int cycleType);

    //Select Combiner
    void selectCombine(unsigned int cycleType);

    //Update
    void update(unsigned int cycleType);       
    void updateCombineColors();                 

    //Begin / End Texture update
    void beginTextureUpdate();
    void endTextureUpdate();

    //Get Combiner Colors which will be assigned to vertices
    void getCombinerColor(float out[4]);
    void getSecondaryCombinerColor(float out[4]);

    //Set / Get Colors
    void setFillColor (float r, float g, float b, float a);
    void setBlendColor(float r, float g, float b, float a);
    void setPrimColor (float r, float g, float b, float a);
    void setEnvColor  (float r, float g, float b, float a);
    float* getFillColor();
    float* getBlendColor();
    float* getPrimColor();

    //Set Prim LOD
    void setPrimLodMin(unsigned int primLodMin);
    void setPrimLodFrac(float primLodFrac);

    //! @return True if combiner wish to use texture channel 0
    bool getUsesTexture0() { return currentTexEnv->usesT0; }

    //! @return True if combiner wish to use texture channel 1
    bool getUsesTexture1() { return currentTexEnv->usesT1; }

private:

    //Private Members
    CombineData     m_combineData;    //!< Combiner Input (How to combine colors andset up enviroment)
    TexEnvCombiner* currentTexEnv;    //!< Texture Enviroment
    CombinerBase*   m_combiner;       //!< Combiner
    CombinerCache   m_combinerCache;  //!< Cache used to store old combiners for reuse

};

#endif
