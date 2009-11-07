
#ifndef COMBINER_STRUCTS_H_
#define COMBINER_STRUCTS_H_

#include "platform.h"
#include <GL/gl.h>

// Internal combiner commands
#define LOAD		0
#define SUB			1
#define MUL			2
#define ADD			3
#define INTERPOLATE 4

// Internal generalized combiner inputs
#define COMBINED		0
#define TEXEL0			1
#define TEXEL1			2
#define PRIMITIVE		3
#define SHADE			4
#define ENVIRONMENT		5
#define CENTER			6
#define SCALE			7
#define COMBINED_ALPHA	8
#define TEXEL0_ALPHA	9
#define TEXEL1_ALPHA	10
#define PRIMITIVE_ALPHA	11
#define SHADE_ALPHA		12
#define ENV_ALPHA		13
#define LOD_FRACTION	14
#define PRIM_LOD_FRAC	15
#define NOISE			16
#define K4				17
#define K5				18
#define ONE				19
#define ZERO			20

//* Combiner data
//! Defines how mux-values are coded.
struct CombineData
{
	union
	{
		struct
		{
			// muxs1
			unsigned	aA1		: 3;
			unsigned	sbA1	: 3;
			unsigned	aRGB1	: 3;
			unsigned	aA0		: 3;
			unsigned	sbA0	: 3;
			unsigned	aRGB0	: 3;
			unsigned	mA1		: 3;
			unsigned	saA1	: 3;
			unsigned	sbRGB1	: 4;
			unsigned	sbRGB0	: 4;

			// muxs0
			unsigned	mRGB1	: 5;
			unsigned	saRGB1	: 4;
			unsigned	mA0		: 3;
			unsigned	saA0	: 3;
			unsigned	mRGB0	: 5;
			unsigned	saRGB0	: 4;
		};

		struct
		{
			unsigned long	muxs1, muxs0;
		};

		unsigned long long	mux;
	};
};

//! Combiner operation
struct CombinerOp
{
	int op;
	int param1;
	int param2;
	int param3;
};

//! Combiner Stage with combiner operations
struct CombinerStage
{
	int numOps;
	CombinerOp op[6];
};

//! Combiner with combiner stages
struct Combiner
{
	int numStages;
	CombinerStage stage[2];
};

//! Combiner cycle
struct CombineCycle
{
	int loadValue;  //!< Load
	int addValue;   //!< Addition
	int subValue;   //!< Subract
	int multValue;  //!< Multiplication
};

static int saRGBExpanded[] = 
{
	COMBINED,			TEXEL0,				TEXEL1,				PRIMITIVE,		
	SHADE,				ENVIRONMENT,		ONE,				NOISE,
	ZERO,				ZERO,				ZERO,				ZERO,
	ZERO,				ZERO,				ZERO,				ZERO
};

static int sbRGBExpanded[] = 
{
	COMBINED,			TEXEL0,				TEXEL1,				PRIMITIVE,		
	SHADE,				ENVIRONMENT,		CENTER,				K4,
	ZERO,				ZERO,				ZERO,				ZERO,
	ZERO,				ZERO,				ZERO,				ZERO
};

static int mRGBExpanded[] = 
{
	COMBINED,			TEXEL0,				TEXEL1,				PRIMITIVE,		
	SHADE,				ENVIRONMENT,		SCALE,				COMBINED_ALPHA,
	TEXEL0_ALPHA,		TEXEL1_ALPHA,		PRIMITIVE_ALPHA,	SHADE_ALPHA,
	ENV_ALPHA,			LOD_FRACTION,		PRIM_LOD_FRAC,		K5,
	ZERO,				ZERO,				ZERO,				ZERO,
	ZERO,				ZERO,				ZERO,				ZERO,
	ZERO,				ZERO,				ZERO,				ZERO,
	ZERO,				ZERO,				ZERO,				ZERO
};

static int aRGBExpanded[] = 
{
	COMBINED,			TEXEL0,				TEXEL1,				PRIMITIVE,		
	SHADE,				ENVIRONMENT,		ONE,				ZERO
};

static int saAExpanded[] = 
{
	COMBINED,			TEXEL0_ALPHA,		TEXEL1_ALPHA,		PRIMITIVE_ALPHA,		
	SHADE_ALPHA,		ENV_ALPHA,			ONE,				ZERO
};

static int sbAExpanded[] = 
{
	COMBINED,			TEXEL0_ALPHA,		TEXEL1_ALPHA,		PRIMITIVE_ALPHA,		
	SHADE_ALPHA,		ENV_ALPHA,			ONE,				ZERO
};

static int mAExpanded[] = 
{
	LOD_FRACTION,		TEXEL0_ALPHA,		TEXEL1_ALPHA,		PRIMITIVE_ALPHA,		
	SHADE_ALPHA,		ENV_ALPHA,			PRIM_LOD_FRAC,		ZERO,
};

static int aAExpanded[] = 
{
	COMBINED,			TEXEL0_ALPHA,		TEXEL1_ALPHA,		PRIMITIVE_ALPHA,		
	SHADE_ALPHA,		ENV_ALPHA,			ONE,				ZERO
};

//*****************************************************************************
//! Texture Environment Combiner Argument
//! Difines a color source and mathimatical operation to be performed
//*****************************************************************************
struct TexEnvCombinerArg
{
	GLenum source;
	GLenum operand;
};

//*****************************************************************************
//! Texture Environment Combiner Stage
//*****************************************************************************
struct TexEnvCombinerStage
{
	WORD constant;
	bool used;
	GLenum combine;
	TexEnvCombinerArg arg0, arg1, arg2;
	WORD outputTexture;
};

//*****************************************************************************
//* Texture Environment Combiner 
//! Stores information how graphics API should combine colors and set 
//! Texture environment.
//*****************************************************************************
struct TexEnvCombiner
{
	bool usesT0, usesT1, usesNoise;

    GLint mode;

	WORD usedUnits;
	
	struct
	{
		WORD color, secondaryColor, alpha;
	} vertex;

	TexEnvCombinerStage color[8];
	TexEnvCombinerStage alpha[8];
};

#endif