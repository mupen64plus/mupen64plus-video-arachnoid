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
	static void RDP_SetCImg(MicrocodeArgument* ucode);
	static void RDP_SetZImg(MicrocodeArgument* ucode);
	static void RDP_SetTImg(MicrocodeArgument* ucode);
	static void RDP_SetTile(MicrocodeArgument* ucode);
	static void RDP_SetTileSize(MicrocodeArgument* ucode);
	static void RDP_LoadTile(MicrocodeArgument* ucode);
	static void RDP_LoadBlock(MicrocodeArgument* ucode);
	static void RDP_LoadTLUT(MicrocodeArgument* ucode);

	//Rendering
	static void RDP_TexRectFlip(MicrocodeArgument* ucode);
	static void RDP_TexRect(MicrocodeArgument* ucode);
	static void RDP_FillRect(MicrocodeArgument* ucode);

	//Colors
	static void RDP_SetEnvColor(MicrocodeArgument* ucode);
	static void RDP_SetPrimColor(MicrocodeArgument* ucode);
	static void RDP_SetBlendColor(MicrocodeArgument* ucode);
	static void RDP_SetFogColor(MicrocodeArgument* ucode);
	static void RDP_SetFillColor(MicrocodeArgument* ucode);

	//Combiner
	static void RDP_SetCombine(MicrocodeArgument* ucode);

	//Misc
	static void RDP_SetOtherMode(MicrocodeArgument* ucode);
	static void RDP_SetPrimDepth(MicrocodeArgument* ucode);
	static void RDP_SetScissor(MicrocodeArgument* ucode);

	//Sync
	static void RDP_FullSync(MicrocodeArgument* ucode);

    //Unimportant
	static void RDP_TileSync(MicrocodeArgument* ucode);
	static void RDP_PipeSync(MicrocodeArgument* ucode);
	static void RDP_LoadSync(MicrocodeArgument* ucode);
	static void RDP_SetConvert(MicrocodeArgument* ucode);
	static void RDP_SetKeyR(MicrocodeArgument* ucode);
	static void RDP_SetKeyGB(MicrocodeArgument* ucode);
	static void RDP_Unknown(MicrocodeArgument* ucode);
	static void RDP_NoOp(MicrocodeArgument* ucode);

private:

	static RDP* m_rdp;                                 //!< Pointer to Reality Drawing Processor
    static DisplayListParser* m_displayListParser;     //!< Pointer to displaylist parser

};

#endif