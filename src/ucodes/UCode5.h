#ifndef UCODE_5_H_
#define UCODE_5_H_

//Includes
#include "UcodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

#define	F3DEX2_MOVEMEM			0xDC
#define	F3DEX2_MOVEWORD			0xDB

//*****************************************************************************
//! UCode5 (aka F3DEX2)
//! Microcode used for Zelda and newer games
//*****************************************************************************
class UCode5
{
public: 

	// Constructor / Destructor
	UCode5();
	~UCode5();

	static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp);
	static void initializeGBI();

	//Matrices
	static void F3DEX2_Mtx( MicrocodeArgument* ucode );
	static void F3DEX2_PopMtx( MicrocodeArgument* ucode );

	//Textures
	static void F3DEX2_Texture( MicrocodeArgument* ucode );

	//Vertices
	static void F3DEX2_Vtx( MicrocodeArgument* ucode );
	static void F3DEX2_Tri1( MicrocodeArgument* ucode );
	static void F3DEX2_Quad( MicrocodeArgument* ucode );
	static void F3DEX2_Line3D( MicrocodeArgument* ucode );

	//Misc
	static void F3DEX2_MoveMem( MicrocodeArgument* ucode );
	static void F3DEX2_MoveWord( MicrocodeArgument* ucode );
	static void F3DEX2_GeometryMode( MicrocodeArgument* ucode );
	static void F3DEX2_SetOtherMode_H( MicrocodeArgument* ucode );
	static void F3DEX2_SetOtherMode_L( MicrocodeArgument* ucode );

	//Other
	static void F3DEX2_DMAIO( MicrocodeArgument* ucode );
	static void F3DEX2_Special_1( MicrocodeArgument* ucode );
	static void F3DEX2_Special_2( MicrocodeArgument* ucode );
	static void F3DEX2_Special_3( MicrocodeArgument* ucode );
	static void F3DEX2_Reserved1( MicrocodeArgument* ucode );

private:

    static GBI* m_gbi;        //!< Graphics Binary Interface
	static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
	static RDP* m_rdp;        //!< Pointer to Reality Drawing Processor  
	static Memory* m_memory;  //!< Pointer accessing memory like RDRAM and Texture Memory
	static DisplayListParser* m_displayListParser;

};

#endif