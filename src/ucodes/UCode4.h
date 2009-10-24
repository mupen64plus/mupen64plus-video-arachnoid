#ifndef UCODE_4_H_
#define UCODE_4_H_

//Includes
#include "UcodeDefs.h"

//Forward declaration
class GBI;
class RSP;

//*****************************************************************************
//! UCode4 
//! Microcode used for Wave Race 64
//*****************************************************************************
class UCode4
{
public:

	UCode4();
	~UCode4();

	static void initialize(GBI* gbi, RSP* rsp);
	static void initializeGBI();

	static void WaveRace64_Vertex(MicrocodeArgument* ucode);
	static void WaveRace64_Tri1(MicrocodeArgument* ucode);
	static void WaveRace64_Tri2(MicrocodeArgument* ucode);
	static void WaveRace64_Quad(MicrocodeArgument* ucode);

private:

    static GBI* m_gbi;
	static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 

};


#endif