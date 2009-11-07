#ifndef UCODE_10_H_
#define UCODE_10_H_

//Includes
#include "UCodeDefs.h"

//Forward declaration
class GBI;
class RSP;
class RDP;
class Memory;
class DisplayListParser;

//*****************************************************************************
//! UCode10
//! Microcode used for Conker's Bad Fur Day
//*****************************************************************************
class UCode10
{
public:

	UCode10();
	~UCode10();

	static void initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp);
	static void initializeGBI();

    static void ConkerBFD_Vertex(MicrocodeArgument* ucode);
    static void ConkerBFD_Add4Triangles(MicrocodeArgument* ucode);
    static void ConkerBFD_MoveWord(MicrocodeArgument* ucode);
    static void ConkerBFD_MoveMem(MicrocodeArgument* ucode);

private:

	static GBI* m_gbi;        //!< Graphics Binary Interface
	static RSP* m_rsp;        //!< Pointer to Reality Signal Processor 
	static RDP* m_rdp;        //!< Pointer to Reality Drawing Processor  
	static Memory* m_memory;  //!< Pointer accessing memory like RDRAM and Texture Memory
    static DisplayListParser* m_displayListParser;


};

#endif