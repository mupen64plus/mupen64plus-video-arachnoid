#ifndef REALITY_DRAWING_PROCESSOR_H_
#define REALITY_DRAWING_PROCESSOR_H_

//Includes
#include "m64p_plugin.h"
#include "UCodeDefs.h"
#include "GBI.h"
#include "GBIDefs.h"
#include "TextureLoader.h"

//Forward declaration
class RSP;
class VI;
class Memory;
class DisplayListParser;
class WindowManager;
class TextureCache;
class AdvancedCombinerManager;
class FogManager;
class TextureLoader;
class OpenGL2DRenderer;

//*****************************************************************************
//! RDPSetImgInfo
//! Used by RDP to set color- and depth- image info
//*****************************************************************************
struct RDPSetImgInfo
{
	unsigned long format;
	unsigned long size;
	unsigned long width;
	unsigned long rdramAddress;
	unsigned long bpl;
};

//*****************************************************************************
//* OtherMode
//! Struct used to get input to combiner
//*****************************************************************************
union OtherMode
{
	struct
	{
		//Low 32:
		unsigned int alphaCompare : 2;
		unsigned int depthSource : 1;

		//RenderMode
		unsigned int AAEnable : 1;
		unsigned int depthCompare : 1;
		unsigned int depthUpdate : 1;
		unsigned int imageRead : 1;
		unsigned int clearOnCvg : 1;
		unsigned int cvgDest : 2;
		unsigned int depthMode : 2;
		unsigned int cvgXAlpha : 1;
		unsigned int alphaCvgSel : 1;
		unsigned int forceBlender : 1;
		unsigned int textureEdge : 1;

		//Blender
		unsigned int c2_m2b : 2;
		unsigned int c1_m2b : 2;
		unsigned int c2_m2a : 2;
		unsigned int c1_m2a : 2;
		unsigned int c2_m1b : 2;
		unsigned int c1_m1b : 2;
		unsigned int c2_m1a : 2;
		unsigned int c1_m1a : 2;

		// High 32:
		unsigned int blendMask : 4;
		unsigned int alphaDither : 2;
		unsigned int colorDither : 2;	
		unsigned int combineKey : 1;
		unsigned int textureConvert : 3;
		unsigned int textureFilter : 2;
		unsigned int textureLUT : 2;
		unsigned int textureLOD : 1;
		unsigned int textureDetail : 2;
		unsigned int texturePersp : 1;
		unsigned int cycleType : 2;
		unsigned int unusedColorDither : 1; // unsupported
		unsigned int pipelineMode : 1;
		unsigned int pad : 8;
	};

	struct
	{
		unsigned long l, h;   //!< Low and high values
	};
};

//*****************************************************************************
//* RDP
//! Class for emulating the Reality Drawing Processor
//*****************************************************************************
class RDP
{
public:

    //Constructor / Destructor
	RDP();
	~RDP();

	//initialize
	bool initialize(GFX_INFO* graphicsInfo, RSP* rsp, Memory* memory, GBI* gbi, TextureCache* textureCache, WindowManager* windowMgr, VI* vi, DisplayListParser* displayListParser, FogManager* fogMgr);
	void updateStates();
    void dispose();
    void reset();
	void triggerInterrupt();

public:

	//Get Combiner Manager
	AdvancedCombinerManager* getCombinerMgr() { return m_combinerMgr; }

	//Set/Get Cycle Type
	void setCycleType(unsigned long cycleType) { m_otherMode.cycleType = cycleType; m_updateCombiner = true; }
	unsigned long getCycleType() { return m_otherMode.cycleType; }

	//Set Texture LUT
	void setTextureLUT(unsigned long lut) { m_textureLUT = lut; }
	unsigned int getTextureLUT() { return m_textureLUT; }

	//Set/Get Texture Filtering
	void setTextureFiltering(unsigned int filterMode) { m_otherMode.textureFilter = filterMode; }
	unsigned int getTextureFiltering() { return m_otherMode.textureFilter; }

	// Set / Get Alpha Compare
	void setAlphaCompareMode(unsigned long mode);
	unsigned int getAlphaCompareMode() { return m_otherMode.alphaCompare; }

	// Set Rendermode
	void setRenderMode(unsigned long w1);		

	//Get Textures, Tiles	
	TextureImage* getTextureImage()          { return m_textureLoader->getTextureImage(); }
	RDPTile*      getCurrentTile()           { return m_textureLoader->getCurrentTile();  }
	RDPTile*      getTile(unsigned int tile) { return m_textureLoader->getTile(tile);     }

	//Get texture modes
	TextureMode getTextureMode() { return m_textureMode;  }
	LoadType getLoadType()       { return m_loadType;     }
    bool getChangedTiles()       { return m_changedTiles; }
    bool getChangedTMEM()        { return m_tmemChanged;  }

	//Texture rectangle Size
	unsigned long getTexRectWidth() { return m_texRectWidth; }
	unsigned long getTexRectHeight() { return m_texRectHeight; }

	//Half
	void setHalf1(unsigned long half1) { m_half1 = half1; }
	void setHalf2(unsigned long half2) { m_half2 = half2; }
	unsigned long getHalf1() { return m_half1; }
	unsigned long getHalf2() { return m_half2; }

	//Get Primitive Depth
	float getPrimitiveZ() { return m_primitiveZ; }

	//Depth Source (get z value from prim depth?)
	void setDepthSource(unsigned long source) { m_otherMode.depthSource = source; }
	unsigned long getDepthSource()            { return m_otherMode.depthSource;   }

	void setUpdateCombiner(bool update) { m_updateCombiner = update; }

public:

	//Texture Rectangle
	void _textureRectangle(float ulx, float uly, float lrx, float lry, long tile, float s, float t, float dsdx, float dtdy,bool flip);
	void _textureRectangleFlip(long nX0, long nY0, long nX1, long nY1, float fS0, float fT0, float fS1, float fT1, long tile);

public:

	//Texturing
	void RDP_SetCImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress);
	void RDP_SetZImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress);
	void RDP_SetTImg(unsigned long format, unsigned long size, unsigned long width, unsigned long segmentAddress);
	void RDP_SetTile(int format, int size, int line, int tmem, int tile, int palette, 
		                  int clampS, int clampT, int mirrorS, int mirrorT, int maskS, 
						  int maskT, int shiftS, int shiftT);
	void RDP_SetTileSize(int tile, unsigned long s0, unsigned long t0, unsigned long s1, unsigned long t1);
	void RDP_LoadTile(int tile, int s0, int t0, int s1, int t1);
	void RDP_LoadBlock(int tile, int s0, int t0, int s1, int t1);
	void RDP_LoadTLUT(int tile, int s0, int t0, int s1, int t1);

	//Colors
	void RDP_SetEnvColor(float r, float g, float b, float a);
	void RDP_SetFogColor(float r, float g, float b, float a);
    void RDP_SetBlendColor(float r, float g, float b, float a);	
	void RDP_SetPrimColor(float r, float g, float b, float a, unsigned long primLodMin, unsigned long primLevel);	
	void RDP_SetFillColor(float r, float g, float b, float a);	

    //Combiner
	void RDP_SetCombine(MicrocodeArgument* ucode);

    //Misc
	void RDP_SetOtherMode(MicrocodeArgument* ucode);
	void RDP_SetPrimDepth(unsigned long dwZ, unsigned long dwDZ);
	void RDP_SetScissor(int x0, int y0, int x1, int y1, int mode);	

	//Rendering
	void RDP_FillRect(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1);
	void RDP_TexRectFlip(unsigned long dwXH, unsigned long dwYH, unsigned long dwXL, unsigned long dwYL, 
                              unsigned long tileno, unsigned long dwS, unsigned long dwT, long nDSDX, long nDTDY);
	void RDP_TexRect(unsigned long dwXH, unsigned long dwYH, unsigned long dwXL, unsigned long dwYL, 
                          unsigned long tileno, unsigned short dwS, unsigned short dwT, unsigned short nDSDX, unsigned short nDTDY);

	//Other
	void RDP_FullSync();

public:

	static Memory* m_memory;                       //!< Pointer to memory manager

public:

	//Other Mode
	OtherMode m_otherMode;             //!< Lots of states for graphics and combiner

	//Hash values for paletts
	unsigned long m_paletteCRC16[16];  //!< Hash values used to select correct texture
	unsigned long m_paletteCRC256;     //!< Hash values used to select correct texture 

protected:

	//Pointers to other objects and managers
	GFX_INFO*                m_graphicsInfo;       //!< Access to emulator data (like RDRAM ...)
	RSP*	                 m_rsp;                //!< Pointer to Reality Signal Processor    
	WindowManager*           m_windowMgr;          //!< Pointer to window manager
	VI*                      m_vi;                 //!< Pointer to video interface
	DisplayListParser*       m_displayListParser;  //!< Pointer to display list parser
	TextureCache*            m_textureCache;       //!< Pointer to texture cache
	FogManager*              m_fogMgr;             //!< Pointer to fog manager
	AdvancedCombinerManager* m_combinerMgr;        //!< Pointer to combiner manager
	TextureLoader*           m_textureLoader;      //!< Pointer to texture loader
    OpenGL2DRenderer*        m_openGL2DRenderer;   //!< Pointer to OpenGL 2D Renderer

	//Prim Depth
	float m_primitiveZ;                //!< Z value assigned to vertices z value if depth source says so
	float m_primitiveDeltaZ;           //!< ??? Unused by this plugin

	//Update?
	bool m_updateCombiner;             //!< Update combiner?
	bool m_updateCombineColors;        //!< Update colors combiner uses?
	bool m_tmemChanged;                //!< True when texture memory has been altered
	bool m_changedTiles;               //!< True if tiles have been changed  

	//Textures		
	LoadType m_loadType;               //!< What kind of texture was previusly loaded
	TextureMode m_textureMode;         //!< Texture mode (NORMAL, TEXRECT, BACKGROUND, FRAMEBUFFER)
	unsigned int m_textureLUT;         //!< Texture Look Up Table
    unsigned long m_texRectWidth;      //!< Width of the texture rectangle to be rendered
    unsigned long m_texRectHeight;     //!< Height of the texture rectangle to be rendered

	//Images
	RDPSetImgInfo m_colorImageInfo;    //!< Information about color image
	RDPSetImgInfo m_depthImageInfo;    //!< Information about depth image

	//Half
	unsigned long m_half1;             //!< First half value
	unsigned long m_half2;             //!< Second half value

};
 
#endif