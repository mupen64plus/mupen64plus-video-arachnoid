#ifndef CONFIG_H_
#define CONFIG_H_

#include "platform.h"
//#include "ConfigGUI.h"
#include "ConfigFile.h"
#include "ConfigMap.h"

//Forward declaration
class GraphicsPlugin;

//*****************************************************************************
//* Config
//! Manages configuration of graphics plugin
//!
//! Responability:
//!  * Loads config data from file
//!  * Sets Config Dialog with values
//!  * Retrieves Config settings with values
//!  * Saves settings to config file
//!
//! @see ConfigGUI
//! @see ConfigFile
//! @see ConfigMap
//*****************************************************************************
class Config
{
public:

	//Constructor / Destructor
	Config(GraphicsPlugin* graphicsPlugin);
	~Config();

    //Event handeling
	bool handleWindowMsg(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam);

    //Opens / Close Config Dialog
	bool openConfigDialog(HINSTANCE instance, DLGPROC a_configDlgProc, HWND parent);
	void closeConfigDialog(WPARAM wParam);

    //Loads/Saves settings from/to file
    void loadFromFile(const char* filename);   
    void saveConfig();

    //Updates Config Dialog
	void updateGUI();

    //Retrives values from Config Dialog
	void updateConfig();

    //Tell Graphics plugin that config has changed
	void updateGraphics();

public:

    //Get configuration
    ConfigMap* getConfig() { return &m_cfg; }

private:

	ConfigMap m_cfg;                     //!< Config map with all settings
	//ConfigGUI m_configGUI;               //!< Config Dialog 
    ConfigFile* m_configFile;            //!< Handles configuration file (used to store settings on disk)
    bool m_hasChanged;                   //!< True if configuration has changed since last check
    GraphicsPlugin* m_graphicsPlugin;    //!< Pointer to graphics plugin
    HWND m_configDlg;                    //!< Windows Config Dialog

};

#endif