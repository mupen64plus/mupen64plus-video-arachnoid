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