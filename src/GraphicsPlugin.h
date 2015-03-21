/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
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

#ifndef GRAPHICS_PLUGIN_H_
#define GRAPHICS_PLUGIN_H_

#include "OpenGLManager.h"         //Initializes OpenGL and handles OpenGL states
#include "RDP.h"
#include "RSP.h"
#include "TextureCache.h"

//Forward declarations
//struct GFX_INFO;
class VI;
class Memory;
class DisplayListParser;
class FogManager;
class ROMDetector;
struct ConfigMap;

//*****************************************************************************
//* Graphics Plugin
//! Main class for application
//*****************************************************************************
class GraphicsPlugin
{
public:

    //Constructor / Destructor
    GraphicsPlugin();
    ~GraphicsPlugin();

    //Set Configuration
    void setConfig(ConfigMap* config) { m_config = config; } 
    void updateConfig();

    //Function called when rom will be opened
    bool initialize(GFX_INFO* graphicsInfo);
    
    //Render
    void processDisplayList();
    void drawScreen();
    void setDrawScreenSignal();
    
    //Toggle Fullscreen
    void toggleFullscreen();

    //Take Screenshot
    void takeScreenshot(void *dest, int *width, int *height, int front);

    //Called when the video interface has been changed
    void viStatusChanged();

    //Function called when rom will be closed
    void dispose();

private:

    //Config Options
    void _setWindowMode(int width, int height);
    void _setTextureCacheSize(int sizeInBytes);

    void _motionBlur();

private:

    GFX_INFO*             m_graphicsInfo;        //!< Information about window, memory...
    RSP                   m_rsp;                 //!< Reality Signal Processor, does transform, clipping, lighting, triangle setup
    RDP                   m_rdp;                 //!< Reality Drawing Processor
    GBI                   m_gbi;                 //!< Graphics Binary Interface, handles mapping of GBI-commands
    VI*                   m_vi;                  //!< Video Interface
    Memory*               m_memory;              //!< Handle RDRAM, Texture Memory and Segments
    TextureCache          m_textureCache;        //!< Save used texture for reuse
    ROMDetector*          m_romDetector;         //!< 
    OpenGLManager&        m_openGLMgr;           //!< Handles initialization of OpenGL and OpenGL states.
    DisplayListParser*    m_displayListParser;   //!< Parses and performs instructions from emulator
    ConfigMap*            m_config;              //!< Settings from config dialog/file
    FogManager*           m_fogManager;          //!< Handles fog extension
    bool                  m_updateConfig;        //!< Does configuration need to be updated?
    bool                  m_initialized;         //!< Have graphics plugin been initialized?
    int                   m_numDListProcessed; 
};

#endif
