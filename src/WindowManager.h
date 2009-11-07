#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include "platform.h"

//*****************************************************************************
//* Window Manager
//! Class for manageing windows
//!
//! Responability:
//!  * Handle render window
//!  * Handle statusbar (if one exist)
//!  * Handle toolbar (if one exist)
//*****************************************************************************
class WindowManager
{
public:
	//Constructor / Destructor
	WindowManager();
	~WindowManager();

	//Initialize
	bool initialize(HWND hWnd, HWND statusBar);

	//Resize window
	void resizeWindow(int displaywidth, int displayHeight);

	//Toggle fullscreen
    void toggleFullscreen();

	//Calculate view scale
	void calcViewScale(int viWidth, int viHeight);

public:

	//! Returns height offset (eg height of status bar)
	int getHeightOffset() { return m_heightOffset; }

private:

	//Handles
	HWND m_hWnd;          //!< Handle to render window	
	HWND m_statusBar;     //!< Handle to status bar (0 if one does not exist)
    HWND m_toolBar;       //!< Handle to toolbar (emulator 1964 has one) (0 if one does not exist)
	HMENU m_menu;         //!< Handle to menu

	//Data
	RECT m_windowRect;    //!< Size and position of render window
	int m_width;          //!< Render Window Width
	int m_height;         //!< Render Window Height
	long m_windowExStyle; //!< Extended style of render window
	long m_windowStyle;   //!< Style of render window 
    bool m_fullscreen;    //!< Render in fullscreen?
	int m_heightOffset;   //!< Height offset (eg height of status bar)

};

#endif