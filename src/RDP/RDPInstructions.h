#ifndef RDP_INSTRUCTIONS_H_
#define RDP_INSTRUCTIONS_H_

#include "UCodeDefs.h"

//Forward declarations
class RDP;
class DisplayListParser;

//*****************************************************************************
//* RDP Instructions
//! Class for reciveing RDP instructions, and forward them to the RDP
//*****************************************************************************
class RDPInstructions
{
public:

	//Constructor / Destructor
	RDPInstructions();
	~RDPInstructions();

    //Initialize
	bool initialize(RDP* rdp, DisplayListParser* displayListParser);

	//Texturing
	static void RDPInstructions::RDP_SetCImg(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetZImg(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetTImg(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetTile(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetTileSize(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_LoadTile(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_LoadBlock(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_LoadTLUT(MicrocodeArgument* ucode);

	//Rendering
	static void RDPInstructions::RDP_TexRectFlip(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_TexRect(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_FillRect(MicrocodeArgument* ucode);

	//Colors
	static void RDPInstructions::RDP_SetEnvColor(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetPrimColor(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetBlendColor(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetFogColor(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetFillColor(MicrocodeArgument* ucode);

	//Combiner
	static void RDPInstructions::RDP_SetCombine(MicrocodeArgument* ucode);

	//Misc
	static void RDPInstructions::RDP_SetOtherMode(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetPrimDepth(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetScissor(MicrocodeArgument* ucode);

	//Sync
	static void RDPInstructions::RDP_FullSync(MicrocodeArgument* ucode);

    //Unimportant
	static void RDPInstructions::RDP_TileSync(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_PipeSync(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_LoadSync(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetConvert(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetKeyR(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_SetKeyGB(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_Unknown(MicrocodeArgument* ucode);
	static void RDPInstructions::RDP_NoOp(MicrocodeArgument* ucode);

private:

	static RDP* m_rdp;                                 //!< Pointer to Reality Drawing Processor
    static DisplayListParser* m_displayListParser;     //!< Pointer to displaylist parser

};

#endif