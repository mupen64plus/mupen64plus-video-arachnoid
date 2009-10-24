#include "UCode10.h"
#include "UCode5.h"

//Forward declaration
#include "GBI.h"
#include "RSP.h"
#include "RDP.h"
#include "Memory.h"
#include "DisplayListParser.h"
#include "UcodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"


#define CONKER_BFD_ADD_VERTICES 1   /*F3DEX2_VTX ?*/ 

#define RSP_MOVE_WORD_MATRIX		0x00	// NOTE: also used by movemem 
#define RSP_MOVE_WORD_NUMLIGHT	0x02
#define RSP_MOVE_WORD_CLIP		0x04
#define RSP_MOVE_WORD_SEGMENT	0x06
#define RSP_MOVE_WORD_FOG		0x08
#define RSP_MOVE_WORD_LIGHTCOL	0x0a
#define	RSP_MOVE_WORD_POINTS		0x0c
#define	RSP_MOVE_WORD_PERSPNORM	0x0e

# define RSP_GBI2_MV_MEM__VIEWPORT	8
# define RSP_GBI2_MV_MEM__LIGHT		10
# define RSP_GBI2_MV_MEM__POINT		12
# define RSP_GBI2_MV_MEM__MATRIX	14		/* NOTE: this is in moveword table */

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP*    UCode10::m_rsp    = 0;   //!< Pointer to Reality Signal Processor 
RDP*    UCode10::m_rdp    = 0;   //!< Pointer to Reality Drawing Processor 
Memory* UCode10::m_memory = 0;
GBI*    UCode10::m_gbi    = 0;
DisplayListParser* UCode10::m_displayListParser = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode10::UCode10()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode10::~UCode10()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode10::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp)
{
	m_gbi = gbi;
    m_rsp = rsp;
	m_rdp = rdp;
	m_memory = mem;
    m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode10::initializeGBI()
{
    //Load F3DEX
    UCode5::initializeGBI();

    GBI_SetGBI( GBI::G_VTX,	     CONKER_BFD_ADD_VERTICES,    m_gbi->m_cmds,	   ConkerBFD_Vertex );
    GBI_SetGBI( GBI::G_TRI4,	 0x10,	                     m_gbi->m_cmds,	   ConkerBFD_Vertex );
    GBI_SetGBI( GBI::G_TRI4,	 0x11,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x12,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x13,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x14,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x15,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x16,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x17,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x18,	                     m_gbi->m_cmds,    ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x19,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1a,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1b,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1c,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1d,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1e,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_TRI4,	 0x1f,	                     m_gbi->m_cmds,	   ConkerBFD_Add4Triangles );
    GBI_SetGBI( GBI::G_MOVEWORD, F3DEX2_MOVEWORD,	         m_gbi->m_cmds,	   ConkerBFD_MoveWord );
    GBI_SetGBI( GBI::G_MOVEMEM,	 F3DEX2_MOVEMEM,	         m_gbi->m_cmds,	   ConkerBFD_MoveMem );
}

//-----------------------------------------------------------------------------
//! Conker Bad Fur Day Vertex
//-----------------------------------------------------------------------------
void UCode10::ConkerBFD_Vertex(MicrocodeArgument* ucode)
{
	unsigned long vertexEnd   = (((ucode->w0)   )&0xFFF)/2;
	unsigned long numVertices = (((ucode->w0)>>12)&0xFFF);

    //Add Vertices
    m_rsp->getVertexMgr()->addConkerVertices( ucode->w1, numVertices, vertexEnd - numVertices );
   // m_rsp->RSP_Vertex( ucode->w1, numVertices, vertexEnd - numVertices); 
}

//-----------------------------------------------------------------------------
//! Conker Bad Fur Day Add Four Triangles
//-----------------------------------------------------------------------------
void UCode10::ConkerBFD_Add4Triangles(MicrocodeArgument* ucode)
{
	unsigned long w0 = ucode->w0;
	unsigned long w1 = ucode->w1;

	unsigned long idx[12];
	idx[0]  = (w1   )&0x1F;    //Tri1
	idx[1]  = (w1>> 5)&0x1F;
	idx[2]  = (w1>>10)&0x1F;
	idx[3]  = (w1>>15)&0x1F;   //Tri2
	idx[4]  = (w1>>20)&0x1F;
	idx[5]  = (w1>>25)&0x1F;
	idx[6]  = (w0    )&0x1F;   //Tri3
	idx[7]  = (w0>> 5)&0x1F;
	idx[8]  = (w0>>10)&0x1F;
	idx[9]  = (((w0>>15)&0x7)<<2)|(w1>>30);  //Tri4
	idx[10] = (w0>>18)&0x1F;
	idx[11] = (w0>>23)&0x1F;

    //Add Triagles
    m_rsp->RSP_1Triangle(idx[0], idx[1], idx[2]);
    m_rsp->RSP_1Triangle(idx[3], idx[4], idx[5]);
    m_rsp->RSP_1Triangle(idx[6], idx[7], idx[8]);
    m_rsp->RSP_1Triangle(idx[9], idx[10], idx[11]);

    unsigned long* RDRAMu32 = m_memory->getRDRAMint32();

	//Get next command		
    MicrocodeArgument* ucodeNext =  (MicrocodeArgument*)&RDRAMu32[(m_displayListParser->getPC()>>2)];

    //Conker Bad Fur Day Render Fix (because conker has multiple Tri4)
    ucode->cmd = GBI::G_TRI4;
    if ( ucodeNext->cmd >= 0x10 && ucodeNext->cmd <= 0x1f )
    {
        ucodeNext->cmd = GBI::G_TRI4; 
    }
}

//-----------------------------------------------------------------------------
//! Conker Bad Fur Day Move Word
//-----------------------------------------------------------------------------
void UCode10::ConkerBFD_MoveWord(MicrocodeArgument* ucode)
{
	unsigned long dwType   = ((ucode->w0) >> 16) & 0xFF;

	if( dwType == RSP_MOVE_WORD_NUMLIGHT )
	{
        m_rsp->RSP_NumLights( ((ucode->w1)/48) );
	}
	else
	{
        UCode5::F3DEX2_MoveWord(ucode);
	}
}

//-----------------------------------------------------------------------------
//! Conker Bad Fur Day Move Memory
//-----------------------------------------------------------------------------
void UCode10::ConkerBFD_MoveMem(MicrocodeArgument* ucode)
{
	unsigned long dwType = ((ucode->w0)     ) & 0xFE;
	unsigned long dwAddr =  ucode->w1;     //RSPSegmentAddr((gfx->words.w1));

    switch ( dwType )
    {
        case RSP_GBI2_MV_MEM__MATRIX:
            m_rsp->getVertexMgr()->setConkerAddress(dwAddr);
            break;

        case RSP_GBI2_MV_MEM__LIGHT:
	        {
		        unsigned long dwOffset2 = ((ucode->w0) >> 5) & 0x3FFF;
		        if( dwOffset2 >= 0x30 )
		        {
                    m_rsp->RSP_Light(((dwOffset2 - 0x30)/0x30), dwAddr);
		        }
		        else
		        {
                    //FIX ME
		        }
	        }
            break;

        default:
            UCode5::F3DEX2_MoveMem(ucode);
            break;
	}
}