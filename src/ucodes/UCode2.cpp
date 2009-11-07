#include "UCode2.h"
#include "UCode0.h"
#include "GBI.h"
#include "RSP.h"
#include "RDP.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "Memory.h"
#include "DisplayListParser.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
GBI*               UCode2::m_gbi = 0;   //!< Pointer to Graphics Binary Interface
RSP*               UCode2::m_rsp = 0;   //!< Pointer to Reality Signal Processor 
RDP*               UCode2::m_rdp = 0;   //!< Pointer to Reality Drawing Processor 
Memory*            UCode2::m_memory            = 0;
DisplayListParser* UCode2::m_displayListParser = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode2::UCode2()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode2::~UCode2()
{
}

//-----------------------------------------------------------------------------
// Initialize 
//-----------------------------------------------------------------------------
void UCode2::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* mem, DisplayListParser* dlp)
{
	m_rsp = rsp;
	m_rdp = rdp;
	m_gbi = gbi;
	m_memory = mem;
	m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//* Initialize GBI
//! Assigns functions to the GBI
//-----------------------------------------------------------------------------
void UCode2::initializeGBI()
{
	UCode0::initializeGBI(m_gbi);
	GBI_SetGBI(GBI::G_RDPHALF_1, F3D_RDPHALF_1, m_gbi->m_cmds, renderSky);
}

//-----------------------------------------------------------------------------
//* RDP Half 1 With Sky Rendering
//! Render Sky
//! @todo Set Half 1 also?
//! @todo Use extracted color
//! @todo set x0 and x1
//-----------------------------------------------------------------------------
void UCode2::renderSky(MicrocodeArgument* ucode)
{	
	//Check for error
	if ( (ucode->w1)>>24 != 0xCE )
	{
		return;
	}

	unsigned long w2  = m_displayListParser->getNextWord();
	unsigned long w3  = m_displayListParser->getNextWord();
	unsigned long w4  = m_displayListParser->getNextWord();
	unsigned long w5  = m_displayListParser->getNextWord();
	unsigned long w6  = m_displayListParser->getNextWord();
	unsigned long w7  = m_displayListParser->getNextWord();
	unsigned long w8  = m_displayListParser->getNextWord();
	unsigned long w9  = m_displayListParser->getNextWord();
	unsigned long w10 = m_displayListParser->getNextWord();

	int width  = m_rsp->getTile(0)->getWidth();
	int height = m_rsp->getTile(0)->getHeight();
	
	//Extract Vertex Coordinats
	unsigned long x0 = 0;     //TODO Use VI pos or Viewport pos or Scissor pos ?
	unsigned long y0 = (unsigned long)int(w2&0xFFFF)/4;
	unsigned long x1 = 320;   //TODO Use VI Height or Viewport Height or Scissor Height ?
	unsigned long y1 = (unsigned long)int(w2>>16)/4;

    //Extract Texture Coordinats
	float uS = (short)(w4&0xFFFF)/32768.0f*width;
	float uT = (short)(w4>>16)/32768.0f*256;	
	float uDSDX  = (width / (float)(x1-x0));
	float uDTDY  = (height / (float)(y1-y0));

	//Extract Color
	float r = ((w9>>16)&0xFF) / 255.0f;
	float g = ((w9    )&0xFF) / 255.0f;
	float b = ((w10>>16)&0xFF) / 255.0f;
	float a = ((w10    )&0xFF) / 255.0f;

	m_rdp->RDP_TexRect(x0, y0, x1, y1, 0, 0, 0, 1024, 1024);  
}



