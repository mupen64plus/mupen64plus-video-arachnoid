#include "Config.h"
#include <cstdio> 
#include <iostream> //std::endl
#include <fstream> 
    using std::ofstream;       //for reading files 
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
	ConfigSaveFile();
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
	m_cfg.startFullscreen		= ConfigGetParamBool(m_videoGeneralSection, "Fullscreen");
	m_cfg.textureCacheSize      = ConfigGetParamInt(m_videoArachnoidSection, "TextureCacheSize");
	m_cfg.wireframe             = ConfigGetParamBool(m_videoArachnoidSection, "Wireframe");
	m_cfg.fog                   = ConfigGetParamBool(m_videoArachnoidSection, "Fog");
}



