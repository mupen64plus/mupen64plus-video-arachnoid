#ifndef CONFIG_GUI_H_
#define CONFIG_GUI_H_

#include "resource.h"
#include <windows.h>

//*****************************************************************************
//* Config GUI
//! Manages configuration dialog
//!
//! Responability:
//!  * Set values in config dialog
//!  * Retrive values from config dialog
//*****************************************************************************
class ConfigGUI
{
public:

    //Constructor / Destructor 
	ConfigGUI();
	~ConfigGUI();

    //Initalize
	bool initialize(HWND hWndDlg);

    //Set
    void setFullscreen(int width, int height, int bitDepth, int refreshRate);
	void setWindowMode(int width, int height);
	void setTextureCacheSize(int sizeInBytes);
    void setWireframe(bool wireframe);
    void setFog(bool fog);

    //Get booleans
    bool getWireframe();
    bool getFog();

    //Get Integers
    int getFullscreenWidth();
    int getFullscreenHeight();
    int getFullscreenBitDepth();
    int getFullscreenRefreshRate();
	int getTextureCacheSize();
	int getWindowWidth();
    int getWindowHeight();

private:

	HWND m_hWnd;           //!< Pointer to windows handle for dialog
	DEVMODE m_oldDevMode;  //!< The device mode active when starting application

};

#endif