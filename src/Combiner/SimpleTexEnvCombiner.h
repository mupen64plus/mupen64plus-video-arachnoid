#ifndef SIMPLE_TEX_ENV_COMBINER_H_
#define SIMPLE_TEX_ENV_COMBINER_H_

#include "GBIDefs.h"
#include "CombinerStructs.h"
#include "CombinerBase.h"

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