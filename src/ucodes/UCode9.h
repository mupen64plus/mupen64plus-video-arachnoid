#ifndef UCODE_9_H_
#define UCODE_9_H_

//Includes
#include "UCodeDefs.h"

class GBI;
class RSP;

//*****************************************************************************
//! UCode9
//! Microcode used to play Perfect Dark
//*****************************************************************************
class UCode9
{
public:

	//Constructor / Destructor
	UCode9();
	~UCode9();

	static void initialize(RSP* rsp);
	static void initializeGBI(GBI* gbi);

	static void PerfectDark_Vertex(MicrocodeArgument* ucode);
	static void PerfectDark_VertexColorBase(MicrocodeArgument* ucode);

private:

	static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
};

#endif