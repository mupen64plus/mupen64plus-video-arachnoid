#ifndef UCODE_2_H_
#define UCODE_2_H_

#include "UcodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//*****************************************************************************
//! UCode2
//! Microcode used for Golden Eye
//! Note: This ucode is very similar to F3D, The diffrence is that this 
//!       UCode has a special way to render sky. That sky rendering is
//!       also used in perfect dark.
//*****************************************************************************
class UCode2
{
public:

	//Constructor / Destructor
	UCode2();
	~UCode2();

	static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp);
	static void initializeGBI();

	static void renderSky(MicrocodeArgument* ucode);

private:

	static GBI* m_gbi;                               //!< Pointer to Graphics Binary Interface
	static RSP* m_rsp;                               //!< Pointer to Reality Signal Processor 
	static RDP* m_rdp;                               //!< Pointer to Reality Drawing Processor 
	static Memory* m_memory;                         //!< Pointer to Memory Manager
	static DisplayListParser* m_displayListParser;   //!< Pointer to Display-List Parser

};

#endif




