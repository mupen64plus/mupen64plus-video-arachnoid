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
	void setMux(unsigned __int64 muxs, unsigned long cycleType);
	void setMux(unsigned long muxs0, unsigned long muxs1, unsigned long cycleType);

    //Select Combiner
    void selectCombine(unsigned long cycleType);

    //Update
	void update(unsigned long cycleType);       
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
    void setEnvColor   (float r, float g, float b, float a);
    float* getFillColor();
	float* getBlendColor();
    float* getPrimColor();

    //Set Prim LOD
	void setPrimLodMin(unsigned long primLodMin);
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