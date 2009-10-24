#include "RDP.h"
#include "GBIDefs.h"
#include "GBI.h"
#include "RSP.h"
#include "DisplayListParser.h"
#include "assembler.h"
#include "OpenGLRenderer.h"
#include "TextureCache.h"
#include "VI.h"
#include "Memory.h"
#include "OpenGLManager.h"
#include "OpenGL2DRenderer.h"
#include "WindowManager.h"
#include "AdvancedCombinerManager.h"
#include "FogManager.h"
#include "Logger.h"
#include "MathLib.h"
#include "ROMDetector.h"
#include <windows.h>
#include <GL/gl.h>

//-----------------------------------------------------------------------------
//! Defines
//-----------------------------------------------------------------------------
#define MI_INTR_DP          0x00000020  //!< RDP Interrupt signal

//-----------------------------------------------------------------------------
//! Static Variables
//-----------------------------------------------------------------------------
Memory* RDP::m_memory  = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RDP::RDP()
{
	m_graphicsInfo       = 0;    
	m_changedTiles       = false;
	m_textureMode        = TM_NORMAL;
	m_loadType           = LOADTYPE_BLOCK;
	m_tmemChanged        = false;  
	m_textureLUT         = 0;   
	m_texRectWidth       = 1; 
	m_texRectHeight      = 1;
	m_displayListParser  = 0;
	m_primitiveZ         = 0;
    m_combinerMgr        = 0;
    m_textureLoader      = 0;
    m_openGL2DRenderer   = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RDP::~RDP()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//! Sets pointers to managers and initializes objects
//-----------------------------------------------------------------------------
bool RDP::initialize(GFX_INFO* graphicsInfo, RSP* rsp, Memory* memory, GBI* gbi, TextureCache* textureCache, WindowManager* windowMgr, VI* vi, DisplayListParser* displayListParser, FogManager* fogMgr)
{
	//Set pointers
	m_graphicsInfo      = graphicsInfo;
	m_rsp               = rsp;
	m_windowMgr         = windowMgr;
	m_vi                = vi;
	m_memory            = memory;
	m_displayListParser = displayListParser;
	m_textureCache      = textureCache;
	m_fogMgr            = fogMgr;

	//Create combiner manager
	m_combinerMgr = new AdvancedCombinerManager();
	m_combinerMgr->initialize();

	//Create texture loader
	m_textureLoader = new TextureLoader();
	m_textureLoader->initialize(this, m_memory);

	//Create OpenGL 2D Renderer
    m_openGL2DRenderer = new OpenGL2DRenderer();
    m_openGL2DRenderer->initialize(vi);

	return true;
}

//-----------------------------------------------------------------------------
//* Dispose
//! Delets all allocated memory
//-----------------------------------------------------------------------------
void RDP::dispose()
{
    if ( m_combinerMgr      ) { delete m_combinerMgr;      m_combinerMgr      = 0; }
    if ( m_textureLoader    ) { delete m_textureLoader;    m_textureLoader    = 0; }
    if ( m_openGL2DRenderer ) { delete m_openGL2DRenderer; m_openGL2DRenderer = 0; }
}

//-----------------------------------------------------------------------------
//* Update States
//! Sets OpenGL states, updates combiner, activates textures, sets blender
//-----------------------------------------------------------------------------
void RDP::updateStates()
{
	//Depth Compare
	if (m_otherMode.depthCompare)
		glDepthFunc( GL_LEQUAL );
	else
		glDepthFunc( GL_ALWAYS );

	//Depth Update
	if (m_otherMode.depthUpdate)
		glDepthMask( TRUE );
	else
		glDepthMask( FALSE );

	// Depth Mode
	if (m_otherMode.depthMode == ZMODE_DEC)
	{
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( -3.0f, -3.0f );
	}
	else
	{
		glDisable( GL_POLYGON_OFFSET_FILL );
	}

	// Alpha Compare
	if ((m_otherMode.alphaCompare == G_AC_THRESHOLD) && !(m_otherMode.alphaCvgSel))
	{
		glEnable( GL_ALPHA_TEST );
		glAlphaFunc( (m_combinerMgr->getBlendColor()[3] > 0.0f) ? GL_GEQUAL : GL_GREATER, m_combinerMgr->getBlendColor()[3] );
	}
	// Used in TEX_EDGE and similar render modes
	else if (m_otherMode.cvgXAlpha)
	{
		glEnable( GL_ALPHA_TEST );
		glAlphaFunc( GL_GEQUAL, 0.5f );  // Arbitrary number -- gives nice results though
	}
	else
		glDisable( GL_ALPHA_TEST );

	//Combiner
	if ( m_updateCombiner )
	{
		if ( m_otherMode.cycleType == G_CYC_COPY)
		{
			m_combinerMgr->setMux(72057594037727865, m_otherMode.cycleType); //Only normal texturing			                      							      
            m_combinerMgr->selectCombine(m_otherMode.cycleType);
		}
		else if  ( m_otherMode.cycleType == G_CYC_FILL )
		{
			m_combinerMgr->setMux( 72057594037828926, m_otherMode.cycleType );
			m_combinerMgr->selectCombine(m_otherMode.cycleType);
		}
		else
		{
			m_combinerMgr->selectCombine(m_otherMode.cycleType);
		}
		m_updateCombiner = false;
		m_updateCombineColors = true;
	}

	if ( m_updateCombineColors )
	{
		m_combinerMgr->updateCombineColors();
		m_updateCombineColors = false;
	}

	//Texturing
	if ( m_changedTiles || m_tmemChanged || m_rsp->getTexturesChanged() ) 
	{
		m_combinerMgr->beginTextureUpdate();

		//Update Texture channel 0
		if ( m_combinerMgr->getUsesTexture0() )
		{
			//Enable texture 0
			m_textureCache->update(0);
			m_rsp->setTexturesChanged(false);
			m_changedTiles = false;
			m_tmemChanged = false;
		}
		else
		{
			//Disable texture 0
			glActiveTextureARB( GL_TEXTURE0_ARB + 0 );
			glDisable(GL_TEXTURE_2D);  
		}

		//Update Texture channel 1
		if (  m_combinerMgr->getUsesTexture1() )
		{
			//Enable texture 1
			m_textureCache->update(1);
			m_rsp->setTexturesChanged(false);
			m_changedTiles = false;
			m_tmemChanged = false;
		}
		else
		{
			//Disable textureing 1
			glActiveTextureARB( GL_TEXTURE0_ARB + 1 );
			glDisable(GL_TEXTURE_2D); 
		}

		m_combinerMgr->endTextureUpdate();
	}

	// Blending
	if (	(m_otherMode.forceBlender) &&	
		    (m_otherMode.cycleType != G_CYC_COPY) &&
			(m_otherMode.cycleType != G_CYC_FILL) &&
			!(m_otherMode.alphaCvgSel))
	{
 		glEnable( GL_BLEND );
		switch (m_otherMode.l >> 16)
		{
			case 0x0448: // Add
			case 0x055A:
				glBlendFunc( GL_ONE, GL_ONE );
				break;
			case 0x0C08: // 1080 Sky
			case 0x0F0A: // Used LOTS of places
				glBlendFunc( GL_ONE, GL_ZERO );
				break;
			case 0xC810: // Blends fog
			case 0xC811: // Blends fog
			case 0x0C18: // Standard interpolated blend
			case 0x0C19: // Used for antialiasing
			case 0x0050: // Standard interpolated blend
			case 0x0055: // Used for antialiasing
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				break;
			case 0x0FA5: // Seems to be doing just blend color - maybe combiner can be used for this?
			case 0x5055: // Used in Paper Mario intro, I'm not sure if this is right...
				glBlendFunc( GL_ZERO, GL_ONE );
				break;
			default:
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				break;
		}
	}
	else
		glDisable( GL_BLEND );

	if (m_otherMode.cycleType == G_CYC_FILL)
	{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
	}
}

//-----------------------------------------------------------------------------
//* Reset
//! Resets all states on RDP
//-----------------------------------------------------------------------------
void RDP::reset()
{
	setTextureFiltering( G_TF_POINT );
	setRenderMode(0);
}

//-----------------------------------------------------------------------------
//* Trigger Interrupt
//! Tell emulator that the RDP is idle
//-----------------------------------------------------------------------------
void RDP::triggerInterrupt()
{
	*(m_graphicsInfo->MI_INTR_REG) |= MI_INTR_DP;
	m_graphicsInfo->CheckInterrupts();
}

//-----------------------------------------------------------------------------
//* Set Alpha Compare Mode
//-----------------------------------------------------------------------------
void RDP::setAlphaCompareMode(unsigned long mode)
{
	m_otherMode.alphaCompare = mode;
	OpenGLManager::getSingleton().setAlphaTest( m_otherMode.alphaCompare != 0 );
}

//-----------------------------------------------------------------------------
//* Set Render Mode
//-----------------------------------------------------------------------------
void RDP::setRenderMode(unsigned long w1)
{
	unsigned long mode1 = (w1 & 0xCCCCFFFF);
	unsigned long mode2 = (w1 & 0x3333FFFF);

	m_otherMode.l &= 0x00000007;
	m_otherMode.l |= mode1 | mode2;
}

//#############################################################################
//  UCODE FUNCTIONS:
//#############################################################################

//*****************************************************************************
// Combiner Mode
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Set Combine
//! Sets all combiner variables on combiner that tells it how to combine
//! the diffrent colors and textures.
//-----------------------------------------------------------------------------
void RDP::RDP_SetCombine(MicrocodeArgument* ucode)
{ 
	long mux0 = _SHIFTR( ucode->w0, 0, 24 );
	long mux1 = ucode->w1;
	m_combinerMgr->setMux(mux0, mux1, (G_CYCLE_TYPE)m_otherMode.cycleType);

	m_updateCombiner = true;	
}

//*****************************************************************************
// Colors
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Set Environment Color
//! Sets Environment Color on combiner
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void RDP::RDP_SetEnvColor(float r, float g, float b, float a)
{ 
	m_combinerMgr->setEnvColor(r, g, b, a);
	m_updateCombineColors = true;
}

//-----------------------------------------------------------------------------
//* Set Prim Color
//! Sets Prim Color on combiner
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//! @param primLodMin
//! @param primLevel
//-----------------------------------------------------------------------------
void RDP::RDP_SetPrimColor(float r, float g, float b, float a, unsigned long primLodMin, unsigned long primLevel)
{ 
	long primLodFrac = max(primLevel, primLodMin);
	m_combinerMgr->setPrimLodMin(primLodMin);
	m_combinerMgr->setPrimLodFrac(primLodFrac / 255.0f);
	m_combinerMgr->setPrimColor(r, g, b, a);
	m_updateCombineColors = true;
}

//-----------------------------------------------------------------------------
//* Set Blend Color
//! Sets Blend Color on combiner
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void RDP::RDP_SetBlendColor(float r, float g, float b, float a)
{
	m_combinerMgr->setBlendColor(r, g, b, a);
	m_updateCombineColors = true;
}

//-----------------------------------------------------------------------------
//* Set Fill Color
//! Sets Fill Color on combiner
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void RDP::RDP_SetFillColor(float r, float g, float b, float a)
{ 
	m_combinerMgr->setFillColor(r, g, b, a);
	m_updateCombineColors = true;
}

//-----------------------------------------------------------------------------
//* Set Fog Color
//! Sets fog color used when rendering fog
//! @param r Red component of color (0.0 - 1.0)
//! @param g Green component of color (0.0 - 1.0)
//! @param b Blue component of color (0.0 - 1.0)
//! @param a Alpha component of color (0.0 - 1.0)
//-----------------------------------------------------------------------------
void RDP::RDP_SetFogColor(float r, float g, float b, float a)
{ 
	m_fogMgr->setFogColor(r, g, b, a);
}

//*****************************************************************************
// Misc
//*****************************************************************************

//-----------------------------------------------------------------------------
//! Set Other Mode
//-----------------------------------------------------------------------------
void RDP::RDP_SetOtherMode(MicrocodeArgument* ucode)
{  		
	unsigned long mode0 = _SHIFTR( ucode->w0, 0, 24 );	
	unsigned long mode1 = ucode->w1;					

	m_otherMode.h = mode0;
	m_otherMode.l = mode1;
}

//-----------------------------------------------------------------------------
//! Set Prim Depth
//-----------------------------------------------------------------------------
void RDP::RDP_SetPrimDepth(unsigned long dwZ, unsigned long dwDZ)
{  
	unsigned int primitiveDepth = dwZ & 0x7FFF;

	//Convert to float
	m_primitiveZ = (float)(primitiveDepth)/(float)0x8000;
 }

//-----------------------------------------------------------------------------
//! Set Scissor
//-----------------------------------------------------------------------------
void RDP::RDP_SetScissor(int x0, int y0, int x1, int y1, int mode)
{  
    //Get Scale
    float vsx = OpenGLManager::getSingleton().getViewScaleX();
    float vsy = OpenGLManager::getSingleton().getViewScaleY();
    
    //Get Offset
    int offset = m_windowMgr->getHeightOffset();

    //Set Scissor
    OpenGLManager::getSingleton().setScissor( 
               x0 * vsx, 
               (m_vi->getHeight() - y1) * vsy + offset,
			   (x1 - x0) * vsx, 
               (y1 - y0) * vsy 
    );
}

//-----------------------------------------------------------------------------
//* Set Full Sync
//! Called when RDP is finished 
//-----------------------------------------------------------------------------
void RDP::RDP_FullSync()
{ 
	this->triggerInterrupt();
}

//*****************************************************************************
// Rendering
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Fill Rect
//! Renders a rectangle
//-----------------------------------------------------------------------------
void RDP::RDP_FillRect(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1)
{ 
	//Logger::getSingleton() << "RDP_FillRect: " << (int)x0 << " " << (int)y0 << " " << (int)x1 << " " << (int)y1 << "\n";

    //Increase rectangle size?
    if( m_otherMode.cycleType >= G_CYC_COPY )
	{
		x1++;
		y1++;
	}

	//Clear Depth Buffer?
	if ( m_depthImageInfo.rdramAddress == m_colorImageInfo.rdramAddress )
	{
		//Clear the Z Buffer
		updateStates();
		glDepthMask( TRUE );
		glClear(GL_DEPTH_BUFFER_BIT);

		// Depth update
		if (m_otherMode.depthUpdate)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		return;
	}

	//Clear Color Buffer?
	if ( m_otherMode.cycleType == G_CYC_FILL)
	{
		if ( x0 == 0 && y0 == 0 && x1 == m_vi->getWidth() && y1 == m_vi->getHeight() )
		{
			const float* fillColor = m_combinerMgr->getFillColor();
			glClearColor(fillColor[0], fillColor[1], fillColor[2], fillColor[3]);
			bool scissor = OpenGLManager::getSingleton().getScissorEnabled();
			OpenGLManager::getSingleton().setScissorEnabled(false);
			glClear(GL_COLOR_BUFFER_BIT);
			OpenGLManager::getSingleton().setScissorEnabled(scissor);
			return;
		}
	}

	//Update States
	this->updateStates();   

	//Ignore fill rects?
	if ( ROMDetector::getSingleton().getIgnoreFillRects() ) 
	{
		return;
	}

    //Disable Scissor
    glDisable( GL_SCISSOR_TEST );

    //Set Viewport
    int oldViewport[4];
    glGetIntegerv(GL_VIEWPORT, oldViewport);
    glViewport(0,m_windowMgr->getHeightOffset(), OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight() ); 
	glDepthRange(0.0f, 1.0f);

    //Get depth and color
	float depth = m_otherMode.depthSource == 1 ? m_primitiveZ : 0;  //TODO: Use RSP viewport nearz?
    float* color = m_otherMode.cycleType == G_CYC_FILL ? m_combinerMgr->getFillColor() : m_combinerMgr->getPrimColor();

    //Render rectangle
    m_openGL2DRenderer->renderQuad(color, x0, y0, x1, y1, depth);

    //Reset viewport
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);     

    //Reset Scissor
    glEnable( GL_SCISSOR_TEST );
}

//-----------------------------------------------------------------------------
// Texture Rectangle Flip
//-----------------------------------------------------------------------------
void RDP::RDP_TexRectFlip(unsigned long dwXH, unsigned long dwYH, unsigned long dwXL, unsigned long dwYL, 
                          unsigned long tileno, unsigned long dwS, unsigned long dwT, long nDSDX, long nDTDY)
{   
	Logger::getSingleton().printMsg("RDP_TexRect");

	float fS0 = (float)dwS / 32.0f;
	float fT0 = (float)dwT / 32.0f;

	float fDSDX = (float)nDSDX / 1024.0f;
	float fDTDY = (float)nDTDY / 1024.0f;

	if (m_otherMode.cycleType == G_CYC_COPY)
	{
		fDSDX /= 4.0f;	// In copy mode 4 pixels are copied at once.
		dwXH++;
		dwYH++;
	}
	else if (m_otherMode.cycleType == G_CYC_FILL )
	{
		dwXH++;
		dwYH++;
	}

	float fS1 = fS0 + (fDSDX * (dwYH - dwYL));
	float fT1 = fT0 + (fDTDY * (dwXH - dwXL));

    //Set Current Texture tiles
	m_rsp->setTile( m_textureLoader->getTile(tileno), 0);
	if ( tileno < 7 ) 
	{
		m_rsp->setTile( m_textureLoader->getTile(tileno + 1), 1);
	}
	else {
		m_rsp->setTile( m_textureLoader->getTile(tileno), 1);
	}

	m_texRectWidth = (unsigned long)fS1;
	m_texRectHeight = (unsigned long)fT1;

	//Update States
	this->updateStates();

	float t0u0 = 0, t0v0 = 0, t0u1 =0, t0v1 = 0;
	if ( m_textureCache->getCurrentTexture(0) )
	{
		t0u0 = (fS0) * m_textureCache->getCurrentTexture(0)->shiftScaleS - m_textureLoader->getTile(tileno)->uls;
		t0v0 = (fT0) * m_textureCache->getCurrentTexture(0)->shiftScaleT - m_textureLoader->getTile(tileno)->ult;
		t0u1 = t0u0 + (fDSDX * (dwYH - dwYL))*m_textureCache->getCurrentTexture(0)->shiftScaleS;
		t0v1 = t0v0 + (fDTDY * (dwXH - dwXL))*m_textureCache->getCurrentTexture(0)->shiftScaleT;
	}

	_textureRectangleFlip(dwXL, dwYL, dwXH, dwYH, t0u0, t0v0, t0u1, t0v1, tileno);
		
    //Restore RSP Tile
    int rspTile = m_rsp->getTexture().tile;
    m_rsp->setTile( m_textureLoader->getTile(tileno), 0);
	m_rsp->setTile( m_textureLoader->getTile(rspTile < 7 ? rspTile + 1 : rspTile), 1);
}

//-----------------------------------------------------------------------------
//* Texture Rect
//! Not this command use 128bits and not 64 bits wich could cause some
//! problems with the program counter.
//-----------------------------------------------------------------------------
void RDP::RDP_TexRect(unsigned long dwXH, unsigned long dwYH, unsigned long dwXL, unsigned long dwYL, 
                      unsigned long tileno, unsigned short dwS, unsigned short dwT, unsigned short nDSDX, unsigned short nDTDY)
{ 
	Logger::getSingleton().printMsg("RDP_TexRect");	

	glEnable(GL_TEXTURE_2D);

    //Convert to signed
	short s16S = *(short*)(&dwS);
	short s16T = *(short*)(&dwT);
	short s16DSDX  = *(short*)(&nDSDX);
	short s16DTDY	= *(short*)(&nDTDY);

    //Convert to float
	float s = s16S / 32.0f;
	float t = s16T / 32.0f;
	float dsdx = s16DSDX / 1024.0f;
	float dtdy = s16DTDY / 1024.0f;


    float ulx = (float)dwXH;
    float uly = (float)dwYH;
    float lrx = (float)dwXL;
    float lry = (float)dwYL;

    long tile = tileno;

	_textureRectangle(ulx, uly, lrx, lry, tile, s, t, dsdx, dtdy, false);
}

//*****************************************************************************
// Texturing
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Set Color Image
//! Sets information about color image
//-----------------------------------------------------------------------------
void RDP::RDP_SetCImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress)
{ 
	m_colorImageInfo.rdramAddress = m_memory->getRDRAMAddress( segmentAddress );
	m_colorImageInfo.format       = format;
	m_colorImageInfo.size         = size;
	m_colorImageInfo.width        = width + 1; //Note: add plus one
	m_colorImageInfo.bpl          = m_colorImageInfo.width << m_colorImageInfo.size >> 1;
}

//-----------------------------------------------------------------------------
//* Set Z Image
//! Sets information about depth image
//-----------------------------------------------------------------------------
void RDP::RDP_SetZImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress)
{ 
	m_depthImageInfo.rdramAddress = m_memory->getRDRAMAddress( segmentAddress );
	m_depthImageInfo.format       = format;
	m_depthImageInfo.size         = size;
	m_depthImageInfo.width        = width + 1; //Note: add plus one
	m_depthImageInfo.bpl          = m_colorImageInfo.width << m_colorImageInfo.size >> 1;
}

//-----------------------------------------------------------------------------
//* Set Texture Image
//! Sets information about texture image
//-----------------------------------------------------------------------------
void RDP::RDP_SetTImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress)
{ 
	m_textureLoader->setTextureImage(format, size, width, segmentAddress);
}

//-----------------------------------------------------------------------------
// Set Tile
//-----------------------------------------------------------------------------
void RDP::RDP_SetTile(int format, int size, int line, int tmem, int tile, 
					  int palette, int clampS, int clampT, int mirrorS, 
					  int mirrorT, int maskS, int maskT, int shiftS, int shiftT )
{ 
	//Set Tile
	m_textureLoader->setTile( format, size, line, tmem, tile, palette, 
		                      clampS, clampT, mirrorS, mirrorT, maskS, maskT,
							  shiftS, shiftT );

	//m_changedTiles = true;	??? Not needed?
}

//-----------------------------------------------------------------------------
//* Load Tile
//-----------------------------------------------------------------------------
void RDP::RDP_LoadTile(int tile, int s0, int t0, int s1, int t1)
{ 
	//Load Tile
	m_textureLoader->loadTile(tile, s0, t0, s1, t1);

	m_textureMode = TM_NORMAL;
	m_loadType    = LOADTYPE_TILE;
	m_tmemChanged = true;	
}	

//-----------------------------------------------------------------------------
// Load Block
//-----------------------------------------------------------------------------
void RDP::RDP_LoadBlock(int tile, int s0, int t0, int s1, int t1)
{  
	//Load Block
	m_textureLoader->loadBlock(tile, s0, t0, s1, t1);

	m_textureMode = TM_NORMAL;
	m_loadType    = LOADTYPE_BLOCK;
	m_tmemChanged = true;
}

//-----------------------------------------------------------------------------
//! Sets the size of tile
//! @Param tile Index of the tile to set size on
//! @param s0 Texture Coordinats for first vertex coordinate
//! @param t0 Texture Coordinats for first vertex coordinate
//! @param s1 Texture Coordinats for second vertex coordinate
//! @param t1 Texture Coordinats for second vertex coordinate
//-----------------------------------------------------------------------------
void RDP::RDP_SetTileSize(int tile, unsigned long s0, unsigned long t0, unsigned long s1, unsigned long t1)
{  
	m_textureLoader->setTileSize( tile, s0, t0, s1, t1);

	m_changedTiles = true;	
}

//-----------------------------------------------------------------------------
// Load Texture Look Up Table
//-----------------------------------------------------------------------------
void RDP::RDP_LoadTLUT(int tile, int s0, int t0, int s1, int t1)
{   
	m_textureLoader->loadTLUT(tile, s0, t0, s1, t1);

	m_tmemChanged = true;
}

//*****************************************************************************
// Private Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
// Texture Rectangle
//-----------------------------------------------------------------------------
void RDP::_textureRectangle(float ulx, float uly, float lrx, float lry, long tile, float s, float t, float dsdx, float dtdy,bool flip)
{
	bool zEnabled = OpenGLManager::getSingleton().getZBufferEnabled();
	OpenGLManager::getSingleton().setZBufferEnabled(false);

    //Copy Mode
    if (  m_otherMode.cycleType == G_CYC_COPY )
	{
        dsdx = 1.0;
        lrx += 1.0f;
        lry += 1.0f;
	}
	else if (m_otherMode.cycleType == G_CYC_FILL )
	{
		lrx++;
		lry++;
	}

    //Set Current Texture tiles
    m_rsp->setTile( m_textureLoader->getTile(tile), 0);
	m_rsp->setTile( m_textureLoader->getTile(tile < 7 ? tile + 1 : tile), 1);


	float lrs = s + (lrx - ulx - 1) * dsdx;
	float lrt = t + (lry - uly - 1) * dtdy;

    //Change mode to texture rectangle
    if ( m_textureMode == TM_NORMAL )
        m_textureMode = TM_TEXRECT;

	m_texRectWidth  = (unsigned long)(max( lrs, s ) + dsdx);
	m_texRectHeight = (unsigned long)(max( lrt, t ) + dtdy);

	//Update States
	this->updateStates();

	glViewport( 0, m_windowMgr->getHeightOffset(), OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight() );

    glDisable(GL_SCISSOR_TEST);

	if (lrs > s)
	{
		if (lrt > t)
            OpenGLRenderer::getSingleton().renderTexRect( ulx, uly, lrx, lry, s, t, lrs, lrt, flip );
		else
			OpenGLRenderer::getSingleton().renderTexRect( ulx, lry, lrx, uly, s, lrt, lrs, t, flip );
	}
	else
	{
		if (lrt > t)
			OpenGLRenderer::getSingleton().renderTexRect( lrx, uly, ulx, lry, lrs, t, s, lrt, flip );
		else
			OpenGLRenderer::getSingleton().renderTexRect( lrx, lry, ulx, uly, lrs, lrt, s, t, flip );
	}

    //Restore RSP Tile
    int rspTile = m_rsp->getTexture().tile;
    m_rsp->setTile( m_textureLoader->getTile( rspTile ), 0);
	m_rsp->setTile( m_textureLoader->getTile(rspTile < 7 ? rspTile + 1 : rspTile), 1);

	//glViewport( 0, m_windowMgr->getHeightOffset(), OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight() );

    glEnable(GL_SCISSOR_TEST);
	OpenGLManager::getSingleton().setZBufferEnabled(zEnabled);
}

//-----------------------------------------------------------------------------
//* Texture Rectangle Flip
//! @todo: Clamp Tile
//-----------------------------------------------------------------------------
void RDP::_textureRectangleFlip(long nX0, long nY0, long nX1, long nY1, float fS0, float fT0, float fS1, float fT1, long tile)
{
	//Disable z buffer
	bool zEnabled = OpenGLManager::getSingleton().getZBufferEnabled();
	OpenGLManager::getSingleton().setZBufferEnabled(false);

	float widthDiv = (float)m_textureLoader->getTile( m_rsp->getTexture().tile )->getWidth();
	float heightDiv = (float)m_textureLoader->getTile( m_rsp->getTexture().tile )->getHeight();

	float t0u0 = fS0 / widthDiv;
	float t0v0 = fT0 / heightDiv;
	float t0u1 = (fS1 - fS0)/ widthDiv + t0u0;
	float t0v1 = (fT1 - fT0)/ heightDiv + t0v0;

	float depth = m_otherMode.depthSource == 1 ? m_primitiveZ : 0;  //TODO: Use RSP viewport nearz?

	if( t0u0 >= 0 && t0u1 <= 1 && t0u1 >= t0u0 ) 
	{
		//TODO: Clamp Tile
		Logger::getSingleton().printMsg("_textureRectangleFlip - unimplemented");
	}
	if( t0v0 >= 0 && t0v1 <= 1 && t0v1 >= t0v0 ) 
	{
        //TODO: Clamp tile
		Logger::getSingleton().printMsg("_textureRectangleFlip - unimplemented");
	}

	//HACK 
	if ( ROMDetector::getSingleton().getRomID() == SUPER_MARIO_64 )
	{
		t0u0 *= 0.5f;
		t0v0 *= 0.5f;	
		t0u1 *= 0.5f;	
		t0v1 *= 0.5f;
	}

	//glViewport( 0, m_windowMgr->getHeightOffset(), OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight() );

    //Get Color
    float color[4] = { 1,1,1,0 }; 
    this->getCombinerMgr()->getCombinerColor(	&color[0] );
    float secondaryColor[4] = { 1,1,1,1 };

    if (  m_otherMode.cycleType == G_CYC_COPY )
	{
		glActiveTextureARB( GL_TEXTURE0_ARB );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}

    //Disable Scissor
    glDisable( GL_SCISSOR_TEST );

    //Render Quad
    m_openGL2DRenderer->renderFlippedTexturedQuad( color, secondaryColor,
                                                   (float)nX0, (float)nY0,
                                                   (float)nX1, (float)nY1,
                                                   depth,
                                                   t0u0, t0v0,
                                                   t0u1, t0v1, 
                                                   t0u0, t0v0,
                                                   t0u1, t0v1 );

    //Restore states
    glEnable(GL_SCISSOR_TEST);
	OpenGLManager::getSingleton().setZBufferEnabled(zEnabled);
}
