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

#include "AdvancedCombinerManager.h"

#include "AdvancedTexEnvCombiner.h"
#include "CombinerBase.h"
#include "CombinerStageCreator.h"
#include "CombinerStageMerger.h"
#include "CombinerStructs.h"
#include "DummyCombiner.h"
#include "ExtensionChecker.h"
#include "GBIDefs.h"
#include "MultiTexturingExt.h"
#include "OpenGL.h"
#include "RomDetector.h"
#include "SimpleTexEnvCombiner.h"
#include "m64p.h"

static int saRGBExpanded[] = 
{
    COMBINED,            TEXEL0,              TEXEL1,              PRIMITIVE,        
    SHADE,               ENVIRONMENT,         CB_ONE,                 NOISE,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO
};

static int sbRGBExpanded[] = 
{
    COMBINED,            TEXEL0,              TEXEL1,              PRIMITIVE,        
    SHADE,               ENVIRONMENT,         CENTER,              K4,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO
};

static int mRGBExpanded[] = 
{
    COMBINED,            TEXEL0,              TEXEL1,              PRIMITIVE,        
    SHADE,               ENVIRONMENT,         SCALE,               COMBINED_ALPHA,
    TEXEL0_ALPHA,        TEXEL1_ALPHA,        PRIMITIVE_ALPHA,     SHADE_ALPHA,
    ENV_ALPHA,           LOD_FRACTION,        PRIM_LOD_FRAC,       K5,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO,
    CB_ZERO,                CB_ZERO,                CB_ZERO,                CB_ZERO
};

static int aRGBExpanded[] = 
{
    COMBINED,            TEXEL0,              TEXEL1,              PRIMITIVE,        
    SHADE,               ENVIRONMENT,         CB_ONE,                 CB_ZERO
};

static int saAExpanded[] = 
{
    COMBINED,            TEXEL0_ALPHA,        TEXEL1_ALPHA,        PRIMITIVE_ALPHA,        
    SHADE_ALPHA,         ENV_ALPHA,           CB_ONE,                 CB_ZERO
};

static int sbAExpanded[] = 
{
    COMBINED,            TEXEL0_ALPHA,        TEXEL1_ALPHA,        PRIMITIVE_ALPHA,        
    SHADE_ALPHA,         ENV_ALPHA,           CB_ONE,                 CB_ZERO
};

static int mAExpanded[] = 
{
    LOD_FRACTION,        TEXEL0_ALPHA,        TEXEL1_ALPHA,        PRIMITIVE_ALPHA,        
    SHADE_ALPHA,         ENV_ALPHA,           PRIM_LOD_FRAC,       CB_ZERO,
};

static int aAExpanded[] = 
{
    COMBINED,            TEXEL0_ALPHA,        TEXEL1_ALPHA,        PRIMITIVE_ALPHA,        
    SHADE_ALPHA,         ENV_ALPHA,           CB_ONE,                 CB_ZERO
};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
AdvancedCombinerManager::AdvancedCombinerManager()
{
    currentTexEnv = 0;
    m_combiner = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
AdvancedCombinerManager::~AdvancedCombinerManager()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//! Selects sutible combiner and initializes it
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::initialize()
{
    currentTexEnv = 0;

    //Select Combiner
    switch ( ROMDetector::getSingleton().getCombinerType() )
    {
        case CT_DUMMY:
            m_combiner = new DummyCombiner();
            break;

        case CT_SIMPLE:
            m_combiner = new SimpleTexEnvCombiner();
            break;

        case CT_ADVANCED:
        default:
            m_combiner = new AdvancedTexEnvCombiner();
            break;
    }

    //init combiner
    m_combiner->initialize();
}

//-----------------------------------------------------------------------------
//! Dispose
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::dispose()
{
    if ( m_combiner ) { delete m_combiner; m_combiner = 0; }
    m_combinerCache.dispose();
}

//-----------------------------------------------------------------------------
//! Select Combine
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::selectCombine(unsigned int cycleType)
{
    //Hack for the Banjo-Tooie shadow 
    if ( cycleType == G_CYC_1CYCLE && m_combineData.mux == 0x00ffe7ffffcf9fcfLL )
    {
        m_combineData.mux = 71943244815007743LL;
        m_combiner->setBlendColor(0,0,0,0);
        m_combiner->setPrimColor(0,0,0,0);
        m_combiner->setEnvColor(0,0,0,0);
        m_combiner->setFillColor(0,0,0,0);
    }

    CachedCombiner* old = m_combinerCache.findCachedCombiner(m_combineData.mux);

    if ( old == 0 )
    {
        //Cound not find an old combiner
        this->update(cycleType);         //Create a new combiner
    }
    else
    {
        currentTexEnv = old->compiled;
    }

    //Set Texture Enviroment
    this->endTextureUpdate();
}

//-----------------------------------------------------------------------------
//! Update Combine Colors
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::updateCombineColors()
{
    m_combiner->setTextureEnviromentColors( currentTexEnv );
}

//-----------------------------------------------------------------------------
//! Update 
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::update(unsigned int cycleType)
{
    int numCycles;

    Combiner colorCombiner;
    Combiner alphaCombiner;

    //Set number of cycles
    if ( cycleType == G_CYC_2CYCLE )
    {
        numCycles = 2;
        colorCombiner.numStages = 2;
        alphaCombiner.numStages = 2;
    }
    else
    {
        numCycles = 1;
        colorCombiner.numStages = 1;
        alphaCombiner.numStages = 1;
    }

    CombineCycle colorCycle[2];
    CombineCycle alphaCycle[2];

    // Decode and expand the combine mode into a more general form
    colorCycle[0].loadValue  = saRGBExpanded[m_combineData.saRGB0];
    colorCycle[0].subValue   = sbRGBExpanded[m_combineData.sbRGB0];
    colorCycle[0].multValue  = mRGBExpanded[m_combineData.mRGB0];
    colorCycle[0].addValue   = aRGBExpanded[m_combineData.aRGB0];
    colorCycle[1].loadValue  = saRGBExpanded[m_combineData.saRGB1];
    colorCycle[1].subValue   = sbRGBExpanded[m_combineData.sbRGB1];
    colorCycle[1].multValue  = mRGBExpanded[m_combineData.mRGB1];
    colorCycle[1].addValue   = aRGBExpanded[m_combineData.aRGB1];

    alphaCycle[0].loadValue  = saAExpanded[m_combineData.saA0];
    alphaCycle[0].subValue   = sbAExpanded[m_combineData.sbA0];
    alphaCycle[0].multValue  = mAExpanded[m_combineData.mA0];
    alphaCycle[0].addValue   = aAExpanded[m_combineData.aA0];
    alphaCycle[1].loadValue  = saAExpanded[m_combineData.saA1];
    alphaCycle[1].subValue   = sbAExpanded[m_combineData.sbA1];
    alphaCycle[1].multValue  = mAExpanded[m_combineData.mA1];
    alphaCycle[1].addValue   = aAExpanded[m_combineData.aA1];

    //For each cycle    
    for (int i=0; i<numCycles; ++i)
    {
        //Set stages on color combiner
        setStage(&colorCycle[i], &colorCombiner.stage[i]);

        //Set stages on alpha combiner
        setStage(&alphaCycle[i], &alphaCombiner.stage[i]);
    }

    if (numCycles == 2)
    {
        // Attempt to merge the two stages into one
        mergeStages( &colorCombiner );
        mergeStages( &alphaCombiner );
    }

    //Create New Enviroment
    currentTexEnv = m_combiner->createNewTextureEnviroment(&colorCombiner, &alphaCombiner);

    if ( !ROMDetector::getSingleton().getUseMultiTexture() )
    {
        currentTexEnv->usesT1 = false;
    }

    //Store combiner for reuse
    m_combinerCache.newCompiledCombiner(m_combineData.mux, currentTexEnv);
}

//-----------------------------------------------------------------------------
//! Set Mux
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setMux(unsigned long long mux, unsigned int cycleType)
{
    m_combineData.mux = mux;    
}

//-----------------------------------------------------------------------------
//! Set Mux
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setMux(unsigned int muxs0, unsigned int muxs1, unsigned int cycleType)
{
    m_combineData.muxs0 = muxs0;
    m_combineData.muxs1 = muxs1;    
}

//-----------------------------------------------------------------------------
//! Begin Texture Update
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::beginTextureUpdate()
{
    m_combiner->beginTextureUpdate();
}

//-----------------------------------------------------------------------------
//! End Texture Update
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::endTextureUpdate()
{
    //Set Texture Enviroment
    m_combiner->setTextureEnviroment(currentTexEnv);
}

//-----------------------------------------------------------------------------
//* Get Combiner Color
//! Get Combiner Color which will be assigned to vertices
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::getCombinerColor(float out[4])
{
    m_combiner->getCombinerColor(out, currentTexEnv->vertex.color, currentTexEnv->vertex.alpha);
}

//-----------------------------------------------------------------------------
//* Get Secondary Combiner Color
//! Get Secondary Combiner Color which will be assigned to vertices
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::getSecondaryCombinerColor(float out[4])
{
    if ( !ROMDetector::getSingleton().getUseSecondaryColor() )
    {
        return;    
    }

    //Get color
    m_combiner->getCombinerColor(out, currentTexEnv->vertex.secondaryColor, CB_ONE);
}

//-----------------------------------------------------------------------------
//* Get Fill Color
//! @return Fill Color as <r,g,b,a> (0.0 - 1.0)
//-----------------------------------------------------------------------------
float* AdvancedCombinerManager::getFillColor()  
{ 
    return m_combiner->getFillColor();  
}

//-----------------------------------------------------------------------------
//* Get Blend Color
//! @return Blend Color as <r,g,b,a> (0.0 - 1.0)
//-----------------------------------------------------------------------------
float* AdvancedCombinerManager::getBlendColor() 
{ 
    return m_combiner->getBlendColor(); 
}

//-----------------------------------------------------------------------------
//* Get Prim Color
//! @return Prim Color as <r,g,b,a> (0.0 - 1.0)
//-----------------------------------------------------------------------------
float* AdvancedCombinerManager::getPrimColor()  
{ 
    return m_combiner->getPrimColor();  
}

//-----------------------------------------------------------------------------
//! Set Fill Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setFillColor (float r, float g, float b, float a)
{ 
    m_combiner->setFillColor(r,g,b,a); 
}

//-----------------------------------------------------------------------------
//! Set Blend Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setBlendColor(float r, float g, float b, float a) 
{ 
    m_combiner->setBlendColor(r,g,b,a); 
}

//-----------------------------------------------------------------------------
//! Set Prim Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setPrimColor (float r, float g, float b, float a) 
{ 
    m_combiner->setPrimColor(r,g,b,a); 
}

//-----------------------------------------------------------------------------
//! Set Environment Color
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setEnvColor  (float r, float g, float b, float a) 
{ 
    m_combiner->setEnvColor(r,g,b,a); 
}

//-----------------------------------------------------------------------------
//* Set Prim LOD Min
//! @param primLodMin
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setPrimLodMin(unsigned int primLodMin) 
{ 
    m_combiner->setPrimLodMin(primLodMin); 
};

//-----------------------------------------------------------------------------
//* Set Prim LOD Frac
//! @param primLodFrac
//-----------------------------------------------------------------------------
void AdvancedCombinerManager::setPrimLodFrac(float primLodFrac) 
{ 
    m_combiner->setPrimLodFrac(primLodFrac); 
};
