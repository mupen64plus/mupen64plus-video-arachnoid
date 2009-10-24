#ifndef GRAPHIC_BINARY_INTERFACE_H_
#define GRAPHIC_BINARY_INTERFACE_H_


#include "UCodeDefs.h"
#include "UCode0.h"     //Super Mario 64
#include "UCode1.h"     //Games like Mario Kart 64, Starfox 64
#include "UCode2.h"     //Golden Eye
#include "UCode4.h"     //Wave Race 64
#include "UCode5.h"     //Newer Games
#include "UCode6.h"     //Diddy Kong Racing
#include "UCode7.h"     //Yoshi's Story
#include "UCode9.h"     //Perfect Dark
#include "UCode10.h"    //Conker Bad Fur Day
#include "RDPInstructions.h"

class RSP;
class RDP;
class Memory;
class UCodeSelector;

// Allows easier setting of GBI commands
#define GBI_SetGBI( command, value, target, function ) \
	command = value; \
	target[command] = function

// Useful macros for decoding GBI command's parameters
#define _SHIFTL( v, s, w )	(((unsigned long)v & ((0x01 << w) - 1)) << s)
#define _SHIFTR( v, s, w )	(((unsigned long)v >> s) & ((0x01 << w) - 1))


//-----------------------------------------------------------------------------
//* GBI Func
//! Definition for GBI function pointers
//-----------------------------------------------------------------------------
typedef void (*GBIFunc)( MicrocodeArgument* );

//-----------------------------------------------------------------------------
//* GBI
//! Defines the Graphical Binary Interface meaning how the graphic 
//! processors should operate.
//-----------------------------------------------------------------------------
class GBI
{
public:
	//Constructor / Deconstructor
	GBI();
	~GBI();

	bool initialize(RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp);  
    void dispose();
	void selectUCode( unsigned long ucStart, 
		              unsigned long ucDStart, 
					  unsigned long ucSize, 
					  unsigned long ucDSize);

	//Dummy instruction
	static void unknownInstruction(MicrocodeArgument* arg);

	static unsigned long G_MOVEMEM, G_MOVEWORD;
	static unsigned long G_RDPHALF_1, G_RDPHALF_2, G_RDPHALF_CONT;
	static unsigned long G_SPNOOP;
	static unsigned long G_SETOTHERMODE_H, G_SETOTHERMODE_L;
	static unsigned long G_DL, G_ENDDL, G_CULLDL, G_BRANCH_Z;
	static unsigned long G_LOAD_UCODE;

	static unsigned long G_MTX, G_POPMTX;
	static unsigned long G_GEOMETRYMODE, G_SETGEOMETRYMODE, G_CLEARGEOMETRYMODE;
	static unsigned long G_TEXTURE;
	static unsigned long G_DMA_IO, G_DMA_DL, G_DMA_TRI, G_DMA_MTX, G_DMA_VTX, G_DMA_OFFSETS;
	static unsigned long G_SPECIAL_1, G_SPECIAL_2, G_SPECIAL_3;
	static unsigned long G_VTX, G_MODIFYVTX, G_VTXCOLORBASE;
	static unsigned long G_TRI1, G_TRI2, G_TRI4;
	static unsigned long G_QUAD, G_LINE3D;
	static unsigned long G_RESERVED0, G_RESERVED1, G_RESERVED2, G_RESERVED3;
	static unsigned long G_SPRITE2D_BASE;
	static unsigned long G_BG_1CYC, G_BG_COPY;
	static unsigned long G_OBJ_RECTANGLE, G_OBJ_SPRITE, G_OBJ_MOVEMEM;
	static unsigned long G_SELECT_DL, G_OBJ_RENDERMODE, G_OBJ_RECTANGLE_R;
	static unsigned long G_OBJ_LOADTXTR, G_OBJ_LDTX_SPRITE, G_OBJ_LDTX_RECT, G_OBJ_LDTX_RECT_R;
	static unsigned long G_RDPHALF_0;

	static unsigned long G_MTX_STACKSIZE;
	static unsigned long G_MTX_MODELVIEW;
	static unsigned long G_MTX_PROJECTION;
	static unsigned long G_MTX_MUL;
	static unsigned long G_MTX_LOAD;
	static unsigned long G_MTX_NOPUSH;
	static unsigned long G_MTX_PUSH;

	static unsigned long G_TEXTURE_ENABLE;
	static unsigned long G_SHADING_SMOOTH;
	static unsigned long G_CULL_FRONT;
	static unsigned long G_CULL_BACK;
	static unsigned long G_CULL_BOTH;
	static unsigned long G_CLIPPING;

	static unsigned long G_MV_VIEWPORT;

	static unsigned long G_MWO_aLIGHT_1, G_MWO_bLIGHT_1;
	static unsigned long G_MWO_aLIGHT_2, G_MWO_bLIGHT_2;
	static unsigned long G_MWO_aLIGHT_3, G_MWO_bLIGHT_3;
	static unsigned long G_MWO_aLIGHT_4, G_MWO_bLIGHT_4;
	static unsigned long G_MWO_aLIGHT_5, G_MWO_bLIGHT_5;
	static unsigned long G_MWO_aLIGHT_6, G_MWO_bLIGHT_6;
	static unsigned long G_MWO_aLIGHT_7, G_MWO_bLIGHT_7;
	static unsigned long G_MWO_aLIGHT_8, G_MWO_bLIGHT_8;

public:

	//Function pointer list 
	GBIFunc m_cmds[256];  //! Function pointers to diffrent GBI instructions

	//Pointers
	RSP* m_rsp;           //!< Pointer to Reality Signal Processor 
	RDP* m_rdp;           //!< Pointer to Reality Drawing Processor 
	Memory* m_memory;     //!< Pointer to Memory manager (handles RDRAM, Texture Memory...)

	//RDP Instruction
	RDPInstructions m_rdpInsructions;

	//UCode selector
	UCodeSelector* m_ucodeSelector;    //!< Selects apropriete ucode depending on rom.

	//UCodes
	UCode0 m_ucode0;    //!< UCode for F3D    (Super Mario 64)
	UCode1 m_ucode1;    //!< UCode for F3DEX  (Mario Kart 64, Star Fox 64, Fighter's Destiny...)
	UCode2 m_ucode2;    //!< UCode for Golden Eye
    UCode4 m_ucode4;    //!< UCode for Wave Racer 64 
	UCode5 m_ucode5;    //!< UCode for F3DEX2
    UCode6 m_ucode6;    //!< UCode for Diddy Kong Racing
	UCode7 m_ucode7;    //!< UCode for Yoshi's Story
	UCode9 m_ucode9;    //!< UCode for Perfect Dark
    UCode10 m_ucode10;  //!< UCode for Conker Bad Fur Day

	//Previus ucode
	unsigned int m_previusUCodeStart;
};


#endif