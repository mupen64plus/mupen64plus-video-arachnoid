//*****************************************************************************
// 
// NOTE THAT THIS FILE IS BASED ON MATERIAL FROM glN64. 
// http://gln64.emulation64.com/
//
//*****************************************************************************

#include "CombinerStageCreator.h"

//-----------------------------------------------------------------------------
//* Set Stage
//! Function used to set combiner stage, tries to simplify and optimize as 
//! much as possible.
//! From glN64
//! @param[in] combineCycle Values to be added/subracted/multiplied and interpolated.
//! @param[out] stageOut Simplified combine cycle with combiner formula.
//-----------------------------------------------------------------------------
void setStage(CombineCycle* combineCycle, CombinerStage* stageOut)
{
	// Load the first operand
	stageOut->op[0].op = LOAD;
	stageOut->op[0].param1 = combineCycle->loadValue;
	stageOut->numOps = 1;

	// If we're just subtracting zero, skip it
	if (combineCycle->subValue != ZERO)
	{
		if (combineCycle->subValue == stageOut->op[0].param1)
			stageOut->op[0].param1 = ZERO;
		else
		{
			//Subract operation
			stageOut->op[1].op = SUB;
			stageOut->op[1].param1 = combineCycle->subValue;
			stageOut->numOps++;
		}
	}

	//Multiply operation
	if ((stageOut->numOps > 1) || (stageOut->op[0].param1 != ZERO))
	{
		if (combineCycle->multValue == ZERO)
		{
			stageOut->numOps = 1;
			stageOut->op[0].op = LOAD;
			stageOut->op[0].param1 = ZERO;
		}
		else
		{
			if ( stageOut->numOps == 1 && stageOut->op[0].param1 == ONE )
			{
				//LOAD
				stageOut->op[0].param1 = combineCycle->multValue;
			}
			else
			{
				//MULT
				stageOut->op[stageOut->numOps].op = MUL;
				stageOut->op[stageOut->numOps].param1 = combineCycle->multValue;
				stageOut->numOps++;
			}
		}
	}

	//Don't bother adding zero
	if (combineCycle->addValue != ZERO)
	{
			// If all we have so far is zero, then load this instead
		if ((stageOut->numOps == 1) && (stageOut->op[0].param1 == ZERO))
		{
			stageOut->op[0].param1 = combineCycle->addValue;
		}
		else
		{
			stageOut->op[stageOut->numOps].op = ADD;
			stageOut->op[stageOut->numOps].param1 = combineCycle->addValue;
			stageOut->numOps++;
		}
	}

	// Handle interpolation
	if ((stageOut->numOps == 4) && (stageOut->op[1].param1 == stageOut->op[3].param1))
	{
		stageOut->numOps = 1;
		stageOut->op[0].op = INTERPOLATE;
		stageOut->op[0].param2 = stageOut->op[1].param1;
		stageOut->op[0].param3 = stageOut->op[2].param1;
	}
}
