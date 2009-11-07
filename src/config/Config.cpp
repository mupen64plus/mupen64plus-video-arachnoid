#include "Config.h"
#include "resource.h"
#include <cstdio> 
#include <iostream> //std::endl
#include <fstream> 
    using std::ofstream;       //for reading files 
#include "GraphicsPlugin.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
Config::Config(GraphicsPlugin* graphicsPlugin)
{
	m_graphicsPlugin   = graphicsPlugin;

    //Open config file and load defualt settings
    m_configFile = new ConfigFile();
    m_cfg.reset();
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Config::~Config()
{
    if ( m_configFile ) { delete m_configFile; m_configFile = 0; }
}

//-----------------------------------------------------------------------------
//* Load From File
//! Loads configuration from configuration-file
//! @param filename Name of configuration-file where settings are stored
//! @see saveConfig()
//-----------------------------------------------------------------------------
void Config::loadFromFile(const char* filename)
{
    //Get Configurations
    if ( !m_configFile->initialize(filename) ) {

        //Create a split new config file
        saveConfig();

        //Try to load it again
        if ( !m_configFile->initialize(filename) ) {
            return;
        }         
    }
  
    m_cfg.reset();
    m_cfg.fullscreenWidth       = (int)m_configFile->get("FullscreenWidth", 0, 800);
    m_cfg.fullscreenHeight      = (int)m_configFile->get("FullscreenHeight", 0, 600);    
    m_cfg.fullscreenBitDepth    = (int)m_configFile->get("FullscreenBitDepth", 0, 32);
    m_cfg.fullscreenRefreshRate = (int)m_configFile->get("FullscreenRefreshRate", 0, 60);
    m_cfg.windowWidth           = (int)m_configFile->get("WindowWidth", 0, 800);
    m_cfg.windowHeight          = (int)m_configFile->get("WindowHeight", 0, 600);	
	m_cfg.textureCacheSize      = (int)m_configFile->get("TextureCacheSize", 0, 15*1024*1024); 
    m_cfg.wireframe             = (int)m_configFile->get("Wireframe", 0, false) != 0; 
    m_cfg.fog                   = (int)m_configFile->get("Fog", 0, true) != 0; 
}

//-----------------------------------------------------------------------------
//* Open Config Dialog
//! Opens config dialog and shows it to user
//! @param[in] instance Handle to the module whose executable file contains the dialog box template.
//! @param[in] a_configDlgProc Pointer to the dialog box procedure that handles events.
//! @param[in] parent Windows handle to parent window of dialog box.
//! @see closeConfigDialog()
//-----------------------------------------------------------------------------
bool Config::openConfigDialog(HINSTANCE instance, DLGPROC a_configDlgProc, HWND parent)
{
#ifdef WIN32
	if ( !m_configDlg ) 
	{
		//Config dialog was not open so open it
		DialogBox(instance, MAKEINTRESOURCE(IDD_CONFIG), parent, (DLGPROC)a_configDlgProc);
	}
#endif
	return true;
}

//-----------------------------------------------------------------------------
//* Close Config Dialog
//! Closes config dialog
//! @param[in] wParam Specifies the value to be returned to the application from 
//!                   the function that created the dialog box.
//! @see openConfigDialog()
//-----------------------------------------------------------------------------
void Config::closeConfigDialog(WPARAM wParam)
{
#ifdef WIN32
    EndDialog(m_configDlg, wParam);
#endif
	m_configDlg = 0;
}

//-----------------------------------------------------------------------------
//* Update GUI
//! Should be called when gui will be opened.
//! Sets all values in Config Dialog.
//-----------------------------------------------------------------------------
void Config::updateGUI()
{
	/*
	m_configGUI.initialize(m_configDlg);

	//Set GUI Values
    m_configGUI.setFullscreen(m_cfg.fullscreenWidth, m_cfg.fullscreenHeight, m_cfg.fullscreenBitDepth, m_cfg.fullscreenRefreshRate);
	m_configGUI.setWindowMode(m_cfg.windowWidth, m_cfg.windowHeight);	
	m_configGUI.setTextureCacheSize(m_cfg.textureCacheSize);
    m_configGUI.setWireframe(m_cfg.wireframe);
    m_configGUI.setFog(m_cfg.fog);
	*/
}

//-----------------------------------------------------------------------------
//* Save Config
//! Saves configuration to a config-file on disk
//! @see loadFromFile()
//-----------------------------------------------------------------------------
void Config::saveConfig()
{
    //Set and save settings
    m_configFile->set("FullscreenWidth", m_cfg.fullscreenWidth);
    m_configFile->set("FullscreenHeight", m_cfg.fullscreenHeight);
    m_configFile->set("FullscreenBitDepth", m_cfg.fullscreenBitDepth);
    m_configFile->set("FullscreenRefreshRate", m_cfg.fullscreenRefreshRate);
    m_configFile->set("WindowWidth", m_cfg.windowWidth);
    m_configFile->set("WindowHeight", m_cfg.windowHeight);
    m_configFile->set("TextureCacheSize", m_cfg.textureCacheSize);
    m_configFile->set("Wireframe", m_cfg.wireframe);
    m_configFile->set("Fog", m_cfg.fog);
    m_configFile->save();
}

//-----------------------------------------------------------------------------
//* Update Config
//! Should be called when gui was closed (using ok, not cancel)
//! Retrives all settings from config dialog and saves them to file
//-----------------------------------------------------------------------------
void Config::updateConfig()
{
	/*
	//Get Configuration from Config Dialog
    m_cfg.fullscreenWidth       = m_configGUI.getFullscreenWidth();
    m_cfg.fullscreenHeight      = m_configGUI.getFullscreenHeight();
    m_cfg.fullscreenBitDepth    = m_configGUI.getFullscreenBitDepth();
    m_cfg.fullscreenRefreshRate = m_configGUI.getFullscreenRefreshRate();
    m_cfg.windowWidth           = m_configGUI.getWindowWidth();
    m_cfg.windowHeight          = m_configGUI.getWindowHeight();
    m_cfg.textureCacheSize      = m_configGUI.getTextureCacheSize();
    m_cfg.wireframe             = m_configGUI.getWireframe();
    m_cfg.fog                   = m_configGUI.getFog();
	*/
    //Set and save settings
    saveConfig();
}

//-----------------------------------------------------------------------------
//* Update Graphics
//! Tell graphics plugin that configuration has changed
//-----------------------------------------------------------------------------
void Config::updateGraphics()
{
    m_graphicsPlugin->updateConfig();
}

//-----------------------------------------------------------------------------
//* Handle Window Message
//! Event handeling for config dialog
//! @param hWndDlg Windows handle to config dialog
//! @param[in] message What event occured
//!     \arg \c WM_INITDIALOG Called when user wants to open(initialize) dialog.
//!     \arg \c WM_COMMAND    Called when user pressed OK or Cancel in config dialog
//! @param[in] wParam Specifies additional message-specific information.
//! @param[in] lParam Specifies additional message-specific information.
//-----------------------------------------------------------------------------
bool Config::handleWindowMsg(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef WIN32
	switch (message) 
    { 
		//On Initialize
		case WM_INITDIALOG:
			m_configDlg = hWndDlg;   //Save handle to configuration dialog
			this->updateGUI();       //Initialize Configuration dialog
			return true;

		//On OK or CANCEL
        case WM_COMMAND: 
            switch ( LOWORD(wParam) ) 
            { 
                case IDOK: 
					updateConfig();                   //Retrive settings from configuration dialog                    
					updateGraphics();                 //Tell Graphics that configuraton has changed                    
					this->closeConfigDialog(wParam);  //Close dialog
					return true;
 
                case IDCANCEL:                     
					this->closeConfigDialog(wParam);  //Close dialog
                    return true;
			} 
    } 
#endif
	return true;
}



