#ifndef GRAPHICS_PLUGIN_H_
#define GRAPHICS_PLUGIN_H_

#include "TextureCache.h"
#include "OpenGLManager.h"         //Initializes OpenGL and handles OpenGL states
#include "ScreenshotCreator.h"     //For taking and saving screenshots to disk
#include "RSP.h"
#include "RDP.h"

//Forward declarations
struct GFX_INFO;
class VI;
class Memory;
class DisplayListParser;
class WindowManager;
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

	//Toggle Fullscreen
	void toggleFullscreen();

    //Take Screenshot
    void takeScreenshot(char* directory);

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
	RSP                   m_rsp;				 //!< Reality Signal Processor, does transform, clipping, lighting, triangle setup
	RDP                   m_rdp;				 //!< Reality Drawing Processor
	GBI                   m_gbi;                 //!< Graphics Binary Interface, handles mapping of GBI-commands
	VI*                   m_vi;                  //!< Video Interface
	Memory*               m_memory;              //!< Handle RDRAM, Texture Memory and Segments
	TextureCache          m_textureCache;        //!< Save used texture for reuse
    ROMDetector*          m_romDetector;         //!< 
	OpenGLManager&        m_openGLMgr;           //!< Handles initialization of OpenGL and OpenGL states.
	DisplayListParser*	  m_displayListParser;   //!< Parses and performs instructions from emulator
	WindowManager*        m_windowMgr;           //!< Handles render window and statusbars and toolbars.
    ConfigMap*            m_config;              //!< Settings from config dialog/file
	FogManager*           m_fogManager;          //!< Handles fog extension
    ScreenshotCreator     m_screenshotCreator;   //!< Takes screenshots and saves to disk
	bool                  m_updateConfig;        //!< Does configuration need to be updated?
	bool                  m_initialized;         //!< Have graphics plugin been initialized?
    char*                 m_screenshotDirectory; //!< Where do we want to save the next screenshot? 
	int                   m_numDListProcessed; 

};

#endif