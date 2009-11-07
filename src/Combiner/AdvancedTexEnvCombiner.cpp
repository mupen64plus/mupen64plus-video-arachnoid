#include <algorithm>
using std::max;
#include "AdvancedTexEnvCombiner.h"
#include "CombinerStructs.h"
#include "MultiTexturingExt.h"    //glActiveTextureARB
#include "ExtensionChecker.h"

#ifndef GL_ATI_texture_env_combine3
#define GL_ATI_texture_env_combine3
    #define GL_ATI_texture_env_combine3
    #define GL_MODULATE_ADD_ATI                             0x8744
    #define GL_MODULATE_SIGNED_ADD_ATI                      0x8745
    #define GL_MODULATE_SUBTRACT_ATI                        0x8746
#endif //GL_ATI_texture_env_combine3 

#ifndef GL_ATIX_texture_env_route
#define GL_ATIX_texture_env_route 1
	#define GL_SECONDARY_COLOR_ATIX                 0x8747
	#define GL_TEXTURE_OUTPUT_RGB_ATIX              0x8748
	#define GL_TEXTURE_OUTPUT_ALPHA_ATIX            0x8749
#endif // GL_ATIX_texture_env_route 

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
AdvancedTexEnvCombiner::AdvancedTexEnvCombiner()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
AdvancedTexEnvCombiner::~AdvancedTexEnvCombiner()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//! Checks if extensions are supported
//-----------------------------------------------------------------------------
void AdvancedTexEnvCombiner::initialize()
{
	ARB_texture_env_combine  = isExtensionSupported( "GL_ARB_texture_env_combine" );
	ARB_texture_env_crossbar = isExtensionSupported( "GL_ARB_texture_env_crossbar" );
	ATI_texture_env_combine3 = isExtensionSupported( "GL_ATI_texture_env_combine3" );
	ATIX_texture_env_route   = isExtensionSupported( "GL_ATIX_texture_env_route" );
	NV_texture_env_combine4  = isExtensionSupported( "GL_NV_texture_env_combine4" );;

	if ( ARB_texture_env_crossbar || NV_texture_env_combine4 || ATIX_texture_env_route )
	{
		TexEnvArgs[TEXEL0].source       = GL_TEXTURE0_ARB;
		TexEnvArgs[TEXEL0_ALPHA].source = GL_TEXTURE0_ARB;
		TexEnvArgs[TEXEL1].source       = GL_TEXTURE1_ARB;
		TexEnvArgs[TEXEL1_ALPHA].source = GL_TEXTURE1_ARB;
	}

	if ( ATI_texture_env_combine3 )
	{
		TexEnvArgs[ONE].source  = GL_ONE;
		TexEnvArgs[ZERO].source = GL_ZERO;
	}
}

//-----------------------------------------------------------------------------
//* Begin Texture Update
//! Disables all textures
//-----------------------------------------------------------------------------
void AdvancedTexEnvCombiner::beginTextureUpdate()
{
	const int openGLMaxTextureUnits = 8;

    //Disable all texture channels
	for (int i = 0; i <openGLMaxTextureUnits; i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glDisable(GL_TEXTURE_2D );
	}
}


//-----------------------------------------------------------------------------
//* End Texture Update
//! Enables selected textures in a texture environment
//! @param texEnv Texture environment with textures to be enabled
//-----------------------------------------------------------------------------
void AdvancedTexEnvCombiner::endTextureUpdate(TexEnvCombiner* texEnv)
{  
    //Enable texturing
	for (int i = 0; i <texEnv->usedUnits; i++)
	{
		glActiveTextureARB( GL_TEXTURE0_ARB + i );
		glEnable( GL_TEXTURE_2D );
	}
}

//-----------------------------------------------------------------------------
//* Set Texture Enviroment Colors
//! Sets texture enviorment colors for all active textures
//! @param texEnv Texture environment with textures that will be assigned colors
//-----------------------------------------------------------------------------
void AdvancedTexEnvCombiner::setTextureEnviromentColors(TexEnvCombiner* texEnv)
{
	float color[4];

	int openGLMaxTextureUnits = 8;

	for (int i = 0; i < openGLMaxTextureUnits; i++)
	{
		this->getCombinerColor( color, texEnv->color[i].constant, texEnv->alpha[i].constant );

		glActiveTextureARB(GL_TEXTURE0_ARB + i);
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, &color[0]);
	}
}

//-----------------------------------------------------------------------------
//* Set Texture Enviroment
//! Sets texture environment for all active textures
//! @param texEnv Texture environment with input data which will be 
//!               sent to OpenGL using function "glTexEnvi" using ARB extensions
//-----------------------------------------------------------------------------
void AdvancedTexEnvCombiner::setTextureEnviroment(TexEnvCombiner* texEnv)
{
	const int openGLMaxTextureUnits = 8;

	for (int i=0; i<openGLMaxTextureUnits; ++i)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + i);

		if ( (i < texEnv->usedUnits ) || (i < 2 && texEnv->usesT1) )
		{
			glEnable( GL_TEXTURE_2D );
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
			glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, texEnv->color[i].combine );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,  texEnv->color[i].arg0.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, texEnv->color[i].arg0.operand );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB,  texEnv->color[i].arg1.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, texEnv->color[i].arg1.operand );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB,  texEnv->color[i].arg2.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, texEnv->color[i].arg2.operand );
			glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, texEnv->alpha[i].combine );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,  texEnv->alpha[i].arg0.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, texEnv->alpha[i].arg0.operand );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB,  texEnv->alpha[i].arg1.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, texEnv->alpha[i].arg1.operand );
			glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB,  texEnv->alpha[i].arg2.source );
			glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, texEnv->alpha[i].arg2.operand );
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}			
	}
}

//-----------------------------------------------------------------------------
//* Create New Texture Enviornment
//! Allocates a new texture enviroment
//! @param[in] colorCombiner How to combine and get a color
//! @param[in] alphaCombiner How to combine and get an alpha value
//! @return The texture enviroment that was created
//-----------------------------------------------------------------------------
TexEnvCombiner* AdvancedTexEnvCombiner::createNewTextureEnviroment(Combiner* colorCombiner, Combiner *alphaCombiner)
{
    TexEnvCombiner* envCombiner = new TexEnvCombiner();

	int curUnit;
	int combinedUnit;

	const int openGLMaxTextureUnits = 8;

	//For each texture unit
	for (int i = 0; i < openGLMaxTextureUnits; i++)
	{
		envCombiner->color[i].combine = GL_REPLACE;
		envCombiner->alpha[i].combine = GL_REPLACE;

		SetColorCombinerValues( i, arg0, GL_PREVIOUS_ARB, GL_SRC_COLOR );
		SetColorCombinerValues( i, arg1, GL_PREVIOUS_ARB, GL_SRC_COLOR );
		SetColorCombinerValues( i, arg2, GL_PREVIOUS_ARB, GL_SRC_COLOR );
		envCombiner->color[i].constant = COMBINED;
		envCombiner->color[i].outputTexture = GL_TEXTURE0_ARB + i;

		SetAlphaCombinerValues( i, arg0, GL_PREVIOUS_ARB, GL_SRC_ALPHA );
		SetAlphaCombinerValues( i, arg1, GL_PREVIOUS_ARB, GL_SRC_ALPHA );
		SetAlphaCombinerValues( i, arg2, GL_PREVIOUS_ARB, GL_SRC_ALPHA );
		envCombiner->alpha[i].constant = COMBINED;
		envCombiner->alpha[i].outputTexture = GL_TEXTURE0_ARB + i;
	}

	envCombiner->usesT0 = false;
	envCombiner->usesT1 = false;

	envCombiner->vertex.color = COMBINED;
	envCombiner->vertex.secondaryColor = COMBINED;
	envCombiner->vertex.alpha = COMBINED;

	curUnit = 0;

	for (int i=0; i<alphaCombiner->numStages; i++)
	{
		for (int j = 0; j < alphaCombiner->stage[i].numOps; j++)
		{
			float sb = 0.0f;

			if (alphaCombiner->stage[i].op[j].param1 == PRIMITIVE_ALPHA)
				sb = m_primColor[3];
			else if (alphaCombiner->stage[i].op[j].param1 == ENV_ALPHA)
				sb = m_envColor[3];
			else if (alphaCombiner->stage[i].op[j].param1 == ONE)
				sb = 1.0f;

			if (((alphaCombiner->stage[i].numOps - j) >= 3) &&
				(alphaCombiner->stage[i].op[j].op == SUB) &&
				(alphaCombiner->stage[i].op[j+1].op == MUL) &&
				(alphaCombiner->stage[i].op[j+2].op == ADD) &&
				(sb > 0.5f) && 
				(ARB_texture_env_combine))
			{
				envCombiner->usesT0 |= alphaCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA;
				envCombiner->usesT1 |= alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA;

				if (alphaCombiner->stage[i].op[j].param1 == ONE)
				{
					SetAlphaCombinerValues( curUnit, arg0, envCombiner->alpha[curUnit].arg0.source, GL_ONE_MINUS_SRC_ALPHA );
				}
				else
				{
					envCombiner->alpha[curUnit].combine = GL_SUBTRACT_ARB;
					SetAlphaCombinerValues( curUnit, arg1, envCombiner->alpha[curUnit].arg0.source, GL_SRC_ALPHA );
					SetAlphaCombinerArg( curUnit, arg0, alphaCombiner->stage[i].op[j].param1 );

					curUnit++;
				}

				j++;

				envCombiner->usesT0 |= alphaCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA;
				envCombiner->usesT1 |= alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA;

				envCombiner->alpha[curUnit].combine = GL_MODULATE;
				SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );

				curUnit++;
				j++;

				envCombiner->usesT0 |= alphaCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA;
				envCombiner->usesT1 |= alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA;

				envCombiner->alpha[curUnit].combine = GL_SUBTRACT_ARB;
				SetAlphaCombinerArg( curUnit, arg0, alphaCombiner->stage[i].op[j].param1 );

				curUnit++;
			}
			else
			{
				envCombiner->usesT0 |= alphaCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA;
				envCombiner->usesT1 |= alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA;

				switch (alphaCombiner->stage[i].op[j].op)
				{
					case LOAD:
						if (!(ARB_texture_env_crossbar || NV_texture_env_combine4) &&
							(alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA) && (curUnit == 0))
							curUnit++;

						envCombiner->alpha[curUnit].combine = GL_REPLACE;

						SetAlphaCombinerArg( curUnit, arg0, alphaCombiner->stage[i].op[j].param1 );
						break;
					case SUB:
						if (!ARB_texture_env_combine)
							break;

						if (!(ARB_texture_env_crossbar || NV_texture_env_combine4) &&
							(alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA) && (curUnit == 0))
							curUnit++;

						if ((j > 0) && (alphaCombiner->stage[i].op[j-1].op == LOAD) && (alphaCombiner->stage[i].op[j-1].param1 == ONE))
						{
							SetAlphaCombinerArg( curUnit, arg0, alphaCombiner->stage[i].op[j].param1 );
							envCombiner->alpha[curUnit].arg0.operand = GL_ONE_MINUS_SRC_ALPHA;
						}
						else if ((ATI_texture_env_combine3) && (curUnit > 0) && (envCombiner->alpha[curUnit - 1].combine == GL_MODULATE))
						{
							curUnit--;
							SetAlphaCombinerValues( curUnit, arg2, envCombiner->alpha[curUnit].arg1.source, envCombiner->alpha[curUnit].arg1.operand );
							envCombiner->alpha[curUnit].combine = GL_MODULATE_SUBTRACT_ATI;
							SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );
							curUnit++;
						}
						else
						{
							envCombiner->alpha[curUnit].combine = GL_SUBTRACT_ARB;
							SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );
							curUnit++;
						}
						break;
					case MUL:
						if (!( ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							(alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA) && (curUnit == 0))
							curUnit++;

						envCombiner->alpha[curUnit].combine = GL_MODULATE;

						SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );
						curUnit++;
						break;
					case ADD:
						if (!(ARB_texture_env_crossbar || NV_texture_env_combine4) &&
							(alphaCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA) && (curUnit == 0))
							curUnit++;

						if ((ATI_texture_env_combine3) && (curUnit > 0) && (envCombiner->alpha[curUnit - 1].combine == GL_MODULATE))
						{
							curUnit--;
							SetAlphaCombinerValues( curUnit, arg2, envCombiner->alpha[curUnit].arg1.source, envCombiner->alpha[curUnit].arg1.operand );
							envCombiner->alpha[curUnit].combine = GL_MODULATE_ADD_ATI;
							SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );
						}
						else
						{
							envCombiner->alpha[curUnit].combine = GL_ADD;
							SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param1 );
						}
						curUnit++;
						break;
					case INTERPOLATE:
						envCombiner->usesT0 |= (alphaCombiner->stage[i].op[j].param2 == TEXEL0_ALPHA) || (alphaCombiner->stage[i].op[j].param3 == TEXEL0_ALPHA);
						envCombiner->usesT1 |= (alphaCombiner->stage[i].op[j].param2 == TEXEL1_ALPHA) || (alphaCombiner->stage[i].op[j].param3 == TEXEL1_ALPHA);

						envCombiner->alpha[curUnit].combine = GL_INTERPOLATE_ARB;

						SetAlphaCombinerArg( curUnit, arg0, alphaCombiner->stage[i].op[j].param1 );
						SetAlphaCombinerArg( curUnit, arg1, alphaCombiner->stage[i].op[j].param2 );
						SetAlphaCombinerArg( curUnit, arg2, alphaCombiner->stage[i].op[j].param3 );

						curUnit++;
						break;
				}
			}
		}
		combinedUnit = max( curUnit - 1, 0 );
	}

	envCombiner->usedUnits = max( curUnit, 1 );

	curUnit = 0;
	for (int i = 0; i < colorCombiner->numStages; i++)
	{
		for (int j = 0; j < colorCombiner->stage[i].numOps; j++)
		{
			float sb = 0.0f;

			if (colorCombiner->stage[i].op[j].param1 == PRIMITIVE)
				sb = (m_primColor[0] + m_primColor[2] + m_primColor[1]) / 3.0f;
			else if (colorCombiner->stage[i].op[j].param1 == ENVIRONMENT)
				sb = (m_envColor[0] + m_envColor[2] + m_envColor[1]) / 3.0f;

			// This helps with problems caused by not using signed values between texture units
			if (((colorCombiner->stage[i].numOps - j) >= 3) &&
				(colorCombiner->stage[i].op[j].op == SUB) &&
				(colorCombiner->stage[i].op[j+1].op == MUL) &&
				(colorCombiner->stage[i].op[j+2].op == ADD) &&
				(sb > 0.5f) && 
				(ARB_texture_env_combine))
			{
				envCombiner->usesT0 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL0) || (colorCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA));
				envCombiner->usesT1 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA));

				envCombiner->color[curUnit].combine = GL_SUBTRACT_ARB;
				SetColorCombinerValues( curUnit, arg1, envCombiner->color[curUnit].arg0.source, envCombiner->color[curUnit].arg0.operand );
				SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );

				curUnit++;
				j++;

				envCombiner->usesT0 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL0) || (colorCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA));
				envCombiner->usesT1 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA));

				envCombiner->color[curUnit].combine = GL_MODULATE;
				SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );

				curUnit++;
				j++;

				envCombiner->usesT0 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL0) || (colorCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA));
				envCombiner->usesT1 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA));

				envCombiner->color[curUnit].combine = GL_SUBTRACT_ARB;
				SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );

				curUnit++;
			}
			else
			{
				envCombiner->usesT0 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL0) || (colorCombiner->stage[i].op[j].param1 == TEXEL0_ALPHA));
				envCombiner->usesT1 |= ((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA));

				switch (colorCombiner->stage[i].op[j].op)
				{
					case LOAD:
						if (!( ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA)) && (curUnit == 0))
							curUnit++;

						envCombiner->color[curUnit].combine = GL_REPLACE;

						SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );
						break;
					case SUB:
						if (!ARB_texture_env_combine)
							break;

						if (!(ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA)) && (curUnit == 0))
							curUnit++;

						if ((j > 0) && (colorCombiner->stage[i].op[j-1].op == LOAD) && (colorCombiner->stage[i].op[j-1].param1 == ONE))
						{
							SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );
							envCombiner->color[curUnit].arg0.operand = GL_ONE_MINUS_SRC_COLOR;
						}
						else if (( ATI_texture_env_combine3) && (curUnit > 0) && (envCombiner->color[curUnit - 1].combine == GL_MODULATE))
						{
							curUnit--;
							SetColorCombinerValues( curUnit, arg2, envCombiner->color[curUnit].arg1.source, envCombiner->color[curUnit].arg1.operand );
							envCombiner->color[curUnit].combine = GL_MODULATE_SUBTRACT_ATI;
							SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );
							curUnit++;
						}
						else
						{
							envCombiner->color[curUnit].combine = GL_SUBTRACT_ARB;
							SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );
							curUnit++;
						}
						break;
					case MUL:
						if (!( ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA)) && (curUnit == 0))
							curUnit++;

						envCombiner->color[curUnit].combine = GL_MODULATE;

						SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );
						curUnit++;
						break;
					case ADD:
						if (!( ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param1 == TEXEL1_ALPHA)) && (curUnit == 0))
							curUnit++;

						// ATI_texture_env_combine3 adds GL_MODULATE_ADD_ATI; saves texture units
						if (( ATI_texture_env_combine3) && (curUnit > 0) && (envCombiner->color[curUnit - 1].combine == GL_MODULATE))
						{
							curUnit--;
							SetColorCombinerValues( curUnit, arg2, envCombiner->color[curUnit].arg1.source, envCombiner->color[curUnit].arg1.operand );
							envCombiner->color[curUnit].combine = GL_MODULATE_ADD_ATI;
							SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );
						}
						else
						{
							envCombiner->color[curUnit].combine = GL_ADD;
							SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param1 );
						}
						curUnit++;
						break;
					case INTERPOLATE:
						envCombiner->usesT0 |= (colorCombiner->stage[i].op[j].param2 == TEXEL0) || (colorCombiner->stage[i].op[j].param3 == TEXEL0) || (colorCombiner->stage[i].op[j].param3 == TEXEL0_ALPHA);
						envCombiner->usesT1 |= (colorCombiner->stage[i].op[j].param2 == TEXEL1) || (colorCombiner->stage[i].op[j].param3 == TEXEL1) || (colorCombiner->stage[i].op[j].param3 == TEXEL1_ALPHA);

						if (!( ARB_texture_env_crossbar ||  NV_texture_env_combine4) &&
							((colorCombiner->stage[i].op[j].param1 == TEXEL1) || (colorCombiner->stage[i].op[j].param2 == TEXEL1) || (colorCombiner->stage[i].op[j].param3 == TEXEL1) || (colorCombiner->stage[i].op[j].param3 == TEXEL1_ALPHA)) && (curUnit == 0))
						{
							if (colorCombiner->stage[i].op[j].param1 == TEXEL0)
							{
								envCombiner->color[curUnit].combine = GL_REPLACE;
								SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );
								colorCombiner->stage[i].op[j].param1 = COMBINED;
							}
							if (colorCombiner->stage[i].op[j].param2 == TEXEL0)
							{
								envCombiner->color[curUnit].combine = GL_REPLACE;
								SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param2 )

								colorCombiner->stage[i].op[j].param2 = COMBINED;
							}
							if (colorCombiner->stage[i].op[j].param3 == TEXEL0)
							{
								envCombiner->color[curUnit].combine = GL_REPLACE;
								SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param3 );
								colorCombiner->stage[i].op[j].param3 = COMBINED;
							}
							if (colorCombiner->stage[i].op[j].param3 == TEXEL0_ALPHA)
							{
								envCombiner->color[curUnit].combine = GL_REPLACE;
								SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param3 );
								colorCombiner->stage[i].op[j].param3 = COMBINED_ALPHA;
							}

							curUnit++;
						}

						envCombiner->color[curUnit].combine = GL_INTERPOLATE_ARB;

						SetColorCombinerArg( curUnit, arg0, colorCombiner->stage[i].op[j].param1 );
						SetColorCombinerArg( curUnit, arg1, colorCombiner->stage[i].op[j].param2 );
						SetColorCombinerArg( curUnit, arg2, colorCombiner->stage[i].op[j].param3 );

						curUnit++;
						break;
				}
			}
		}
		combinedUnit = max( curUnit - 1, 0 );
	}

	envCombiner->usedUnits = max( (short)curUnit, envCombiner->usedUnits );
	return envCombiner;
}