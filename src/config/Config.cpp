/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
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

#include "Config.h"
#include <cstdio> 
#include "GraphicsPlugin.h"
#include "Logger.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
Config::Config(GraphicsPlugin* graphicsPlugin)
{
    m_graphicsPlugin   = graphicsPlugin;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Config::~Config()
{
}

bool Config::initialize()
{
    if (ConfigOpenSection("Video-General", &m_videoGeneralSection) != M64ERR_SUCCESS ||
        ConfigOpenSection("Video-Arachnoid", &m_videoArachnoidSection) != M64ERR_SUCCESS)
    {
        Logger::getSingleton().printMsg("Could not open configuration", M64MSG_ERROR);
        return false;
    }
    
    ConfigSetDefaultBool(m_videoGeneralSection, "Fullscreen", false, "Use fullscreen mode if True, or windowed mode if False");
    ConfigSetDefaultInt(m_videoGeneralSection, "ScreenWidth", 640, "Width of output window or fullscreen width");
    ConfigSetDefaultInt(m_videoGeneralSection, "ScreenHeight", 480, "Height of output window or fullscreen height");
    ConfigSetDefaultInt(m_videoArachnoidSection, "ColorDepth", 32, "Color bit-depth in fullscreen mode");
    ConfigSetDefaultInt(m_videoArachnoidSection, "RefreshRate", 60, "Screen refresh-rate in fullscreen mode");
    ConfigSetDefaultInt(m_videoArachnoidSection, "TextureCacheSize", 15 * (1024 * 1024), "Size of texture cache used to store textures");
    ConfigSetDefaultBool(m_videoArachnoidSection, "Wireframe", false, "Render in wireframe?");
    ConfigSetDefaultBool(m_videoArachnoidSection, "Fog", false, "Render fog?");
#ifdef WIN32
    ConfigSetDefaultInt(m_videoArachnoidSection, "ScreenUpdateSetting", SCREEN_UPDATE_CI, "When to update the screen: 1 - on VI, 2 - on first CI");
#else
    ConfigSetDefaultInt(m_videoArachnoidSection, "ScreenUpdateSetting", SCREEN_UPDATE_VI, "When to update the screen: 1 - on VI, 2 - on first CI");
#endif
    return true;
}

void Config::load()
{
    m_cfg.fullscreenWidth       = ConfigGetParamInt(m_videoGeneralSection, "ScreenWidth");
    m_cfg.fullscreenHeight      = ConfigGetParamInt(m_videoGeneralSection, "ScreenHeight");
    m_cfg.fullscreenBitDepth    = ConfigGetParamInt(m_videoArachnoidSection, "ColorDepth");
    m_cfg.fullscreenRefreshRate = ConfigGetParamInt(m_videoArachnoidSection, "RefreshRate");
    m_cfg.windowWidth           = ConfigGetParamInt(m_videoGeneralSection, "ScreenWidth");
    m_cfg.windowHeight          = ConfigGetParamInt(m_videoGeneralSection, "ScreenHeight");
    m_cfg.startFullscreen       = ConfigGetParamBool(m_videoGeneralSection, "Fullscreen");
    m_cfg.textureCacheSize      = ConfigGetParamInt(m_videoArachnoidSection, "TextureCacheSize");
    m_cfg.wireframe             = ConfigGetParamBool(m_videoArachnoidSection, "Wireframe");
    m_cfg.fog                   = ConfigGetParamBool(m_videoArachnoidSection, "Fog");
    m_cfg.screenUpdateSetting   = ConfigGetParamInt(m_videoArachnoidSection, "ScreenUpdateSetting");
}
