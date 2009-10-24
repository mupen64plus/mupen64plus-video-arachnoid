#ifndef COMBINER_BASE_H_
#define COMBINER_BASE_H_

#include "CombinerStructs.h"

//Forward declarations
struct TexEnvCombiner;

//*****************************************************************************
//* CombinerBase
//! Base class and interface for combiners
//! @see AdvancedTexEnvCombiner
//! @see SimpleTexEnvCombiner
//! @see DummyCombiner
//*****************************************************************************
class CombinerBase
{
public:

	//Constructor / Destructor
    CombinerBase();
    ~CombinerBase();

	//Set colors
	void setFillColor(float r, float g, float b, float a);
	void setBlendColor(float r, float g, float b, float a);
	void setPrimColor(float r, float g, float b, float a);
    void setEnvColor(float r, float g, float b, float a);

	//Set prim LOD
	void setPrimLodMin(unsigned long primLodMin) { m_primLodMin = primLodMin; };
	void setPrimLodFrac(float primLodFrac) { m_primLodFrac = primLodFrac; };

public:

	//Get Colors
	//----------

	//! Get Blend Color
	//! @retval float* Returns blend color as <r,g,b,a> channels (0.0-1.0)
    float* getBlendColor() { return m_blendColor; };

	//! Get Fill Color
	//! @retval float* Returns fill color as <r,g,b,a> channels (0.0-1.0)
    float* getFillColor()  { return m_fillColor;  };

	//! Get Prim Color
	//! @retval float* Returns prim color as <r,g,b,a> channels (0.0-1.0)
	float* getPrimColor()  { return m_primColor;  };

	//! Get Environment Color
	//! @retval float* Returns environment color as <r,g,b,a> channels (0.0-1.0)
    float* getEnvColor()   { return m_envColor;   };

	//Get Combiner color	
    void getCombinerColor(float out[4], short colorSource, short alphaSource);

public:

    //Interface
	//---------

	//* Initialize
	//! Used to initialize combiner
    virtual void            initialize() = 0;

	//* Begin Texture Environment
	//! Called before texture channels are updated in the RDP
    virtual void            beginTextureUpdate() = 0;

	//* End Texture Environment
	//! Called before texture channels are updated in the RDP
	//! @param[in] texEnv Texture environment with textures channels to be enabled
    virtual void            endTextureUpdate(TexEnvCombiner* texEnv) = 0;   

	//* Set Texture Environment Colors
	//! Used to send combiner color to graphics API
    virtual void            setTextureEnviromentColors(TexEnvCombiner* texEnv) = 0;

	//* Set Texture Environment Environment
	//! Used to enable textureing and set texture enviromnent for graphics API
	//! @param[in] texEnv Texture environment with input data to graphics API 
    virtual void            setTextureEnviroment(TexEnvCombiner* texEnv) = 0;

	//* Create New Texture Enviornment
	//! Allocates a new texture enviroment
	//! @param[in] colorCombiner How to combine and get a color
	//! @param[in] alphaCombiner How to combine and get an alpha value
	//! @return The texture enviroment that was created
	virtual TexEnvCombiner* createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner) = 0;

protected:

	CombineData m_combineData;    

	//Colors
	float m_fillColor[4] ; //!< <r,g,b,a> 
	float m_blendColor[4]; //!< <r,g,b,a>
	float m_primColor[4];  //!< <r,g,b,a>
	float m_envColor[4];   //!< <r,g,b,a>

    //Prim
	unsigned long m_primLodMin;
	float m_primLodFrac;

};

#endif