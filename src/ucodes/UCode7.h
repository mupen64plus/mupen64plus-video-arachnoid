#ifndef UCODE_7_H_
#define UCODE_7_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;

//*****************************************************************************
//! UCode4 (aka S2DEX)
//! Microcode used for Yoshi's Story
//*****************************************************************************
class UCode7
{
public:

	UCode7();
	~UCode7();

	static void initialize(RSP* rsp);
	static void initializeGBI(GBI* gbi);

	static void S2DEX_Select_DL(MicrocodeArgument* ucode);
	static void S2DEX_BG_1Cyc(MicrocodeArgument* ucode);
	static void S2DEX_BG_Copy(MicrocodeArgument* ucode);
	static void S2DEX_Obj_Rectangle(MicrocodeArgument* ucode);
	static void S2DEX_Obj_Sprite(MicrocodeArgument* ucode);
	static void S2DEX_Obj_MoveMem(MicrocodeArgument* ucode);
	static void S2DEX_Obj_RenderMode(MicrocodeArgument* ucode);
	static void S2DEX_Obj_Rectangle_R(MicrocodeArgument* ucode);
	static void S2DEX_Obj_LoadTxtr(MicrocodeArgument* ucode);
	static void S2DEX_Obj_LdTx_Sprite(MicrocodeArgument* ucode);
	static void S2DEX_Obj_LdTx_Rect(MicrocodeArgument* ucode);
	static void S2DEX_Obj_LdTx_Rect_R(MicrocodeArgument* ucode);

private:

	static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 

};

#endif