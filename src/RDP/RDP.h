/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#ifndef REALITY_DRAWING_PROCESSOR_H_
#define REALITY_DRAWING_PROCESSOR_H_

//Includes
#define M64P_PLUGIN_PROTOTYPES 1
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
    unsigned int format;
    unsigned int size;
    unsigned int width;
    unsigned int rdramAddress;
    unsigned int bpl;
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
        unsigned int l, h;   //!< Low and high values
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
    bool initialize(GFX_INFO* graphicsInfo, RSP* rsp, Memory* memory, GBI* gbi, TextureCache* textureCache, VI* vi, DisplayListParser* displayListParser, FogManager* fogMgr);
    void updateStates();
    void dispose();
    void reset();
    void triggerInterrupt();

public:
    void signalUpdate() { m_screenUpdatePending = true; }

    //Get Combiner Manager
    AdvancedCombinerManager* getCombinerMgr() { return m_combinerMgr; }

    //Set/Get Cycle Type
    void setCycleType(unsigned int cycleType) { m_otherMode.cycleType = cycleType; m_updateCombiner = true; }
    unsigned int getCycleType() { return m_otherMode.cycleType; }

    //Set Texture LUT
    void setTextureLUT(unsigned int lut) { m_textureLUT = lut; }
    unsigned int getTextureLUT() { return m_textureLUT; }

    //Set/Get Texture Filtering
    void setTextureFiltering(unsigned int filterMode) { m_otherMode.textureFilter = filterMode; }
    unsigned int getTextureFiltering() { return m_otherMode.textureFilter; }

    // Set / Get Alpha Compare
    void setAlphaCompareMode(unsigned int mode);
    unsigned int getAlphaCompareMode() { return m_otherMode.alphaCompare; }

    // Set Rendermode
    void setRenderMode(unsigned int w1);        

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
    unsigned int getTexRectWidth() { return m_texRectWidth; }
    unsigned int getTexRectHeight() { return m_texRectHeight; }

    //Half
    void setHalf1(unsigned int half1) { m_half1 = half1; }
    void setHalf2(unsigned int half2) { m_half2 = half2; }
    unsigned int getHalf1() { return m_half1; }
    unsigned int getHalf2() { return m_half2; }

    //Get Primitive Depth
    float getPrimitiveZ() { return m_primitiveZ; }

    //Depth Source (get z value from prim depth?)
    void setDepthSource(unsigned int source) { m_otherMode.depthSource = source; }
    unsigned int getDepthSource()            { return m_otherMode.depthSource;   }

    void setUpdateCombiner(bool update) { m_updateCombiner = update; }

public:

    //Texture Rectangle
    void _textureRectangle(float ulx, float uly, float lrx, float lry, int tile, float s, float t, float dsdx, float dtdy,bool flip);
    void _textureRectangleFlip(int nX0, int nY0, int nX1, int nY1, float fS0, float fT0, float fS1, float fT1, int tile);

public:

    //Texturing
    void RDP_SetCImg(unsigned int format, unsigned int size, unsigned int width, unsigned int segmentAddress);
    void RDP_SetZImg(unsigned int format, unsigned int size, unsigned int width, unsigned int segmentAddress);
    void RDP_SetTImg(unsigned int format, unsigned int size, unsigned int width, unsigned int segmentAddress);
    void RDP_SetTile(int format, int size, int line, int tmem, int tile, int palette, 
                          int clampS, int clampT, int mirrorS, int mirrorT, int maskS, 
                          int maskT, int shiftS, int shiftT);
    void RDP_SetTileSize(int tile, unsigned int s0, unsigned int t0, unsigned int s1, unsigned int t1);
    void RDP_LoadTile(int tile, int s0, int t0, int s1, int t1);
    void RDP_LoadBlock(int tile, int s0, int t0, int s1, int t1);
    void RDP_LoadTLUT(int tile, int s0, int t0, int s1, int t1);

    //Colors
    void RDP_SetEnvColor(float r, float g, float b, float a);
    void RDP_SetFogColor(float r, float g, float b, float a);
    void RDP_SetBlendColor(float r, float g, float b, float a);    
    void RDP_SetPrimColor(float r, float g, float b, float a, unsigned int primLodMin, unsigned int primLevel);    
    void RDP_SetFillColor(float r, float g, float b, float a);    

    //Combiner
    void RDP_SetCombine(MicrocodeArgument* ucode);

    //Misc
    void RDP_SetOtherMode(MicrocodeArgument* ucode);
    void RDP_SetPrimDepth(unsigned int dwZ, unsigned int dwDZ);
    void RDP_SetScissor(int x0, int y0, int x1, int y1, int mode);    

    //Rendering
    void RDP_FillRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
    void RDP_TexRectFlip(unsigned int dwXH, unsigned int dwYH, unsigned int dwXL, unsigned int dwYL, 
                              unsigned int tileno, unsigned int dwS, unsigned int dwT, int nDSDX, int nDTDY);
    void RDP_TexRect(unsigned int dwXH, unsigned int dwYH, unsigned int dwXL, unsigned int dwYL, 
                          unsigned int tileno, unsigned short dwS, unsigned short dwT, unsigned short nDSDX, unsigned short nDTDY);

    //Other
    void RDP_FullSync();

public:

    static Memory* m_memory;                       //!< Pointer to memory manager

public:

    //Other Mode
    OtherMode m_otherMode;             //!< Lots of states for graphics and combiner

    //Hash values for paletts
    unsigned int m_paletteCRC16[16];  //!< Hash values used to select correct texture
    unsigned int m_paletteCRC256;     //!< Hash values used to select correct texture 

protected:

    //Pointers to other objects and managers
    GFX_INFO*                m_graphicsInfo;       //!< Access to emulator data (like RDRAM ...)
    RSP*                     m_rsp;                //!< Pointer to Reality Signal Processor    
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
    unsigned int m_texRectWidth;      //!< Width of the texture rectangle to be rendered
    unsigned int m_texRectHeight;     //!< Height of the texture rectangle to be rendered

    //Images
    RDPSetImgInfo m_colorImageInfo;    //!< Information about color image
    RDPSetImgInfo m_depthImageInfo;    //!< Information about depth image

    //Half
    unsigned int m_half1;             //!< First half value
    unsigned int m_half2;             //!< Second half value

    //Update on first CI
    bool m_screenUpdatePending;

};
 
#endif
