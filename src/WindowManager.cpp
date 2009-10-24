#include "WindowManager.h"
#include "OpenGLManager.h"
#include "FrameBuffer.h"
#include "Commctrl.h"   //RBS_VARHEIGHT

//-----------------------------------------------------------------------------
//* Find ToolBarProc
//! Procedure used to find toolbar (used by emulator 1964)
//-----------------------------------------------------------------------------
HWND hToolBar = 0;
BOOL CALLBACK findToolBarProc(HWND hWnd, LPARAM lParam)
{
	if (GetWindowLong( hWnd, GWL_STYLE ) & RBS_VARHEIGHT)
	{
		hToolBar = hWnd;   //Found toolbar!
		return FALSE;      //No need to continue finding children, return false
	}
	return TRUE;           //Not correct child, continue looking
}

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
WindowManager::WindowManager()
{
    m_toolBar = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
WindowManager::~WindowManager()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves handles and tries to find a toolbar
//! @param hWnd Windows handle to render window
//! @param statusBar Windows handle to statusbar, 0 if one does not exsist
//-----------------------------------------------------------------------------
bool WindowManager::initialize(HWND hWnd, HWND statusBar)
{
	m_hWnd       = hWnd;
	m_statusBar  = statusBar;
	m_fullscreen = false;

    //Try to find toolbar
    hToolBar = 0;
    EnumChildWindows(hWnd, findToolBarProc, 0);
    m_toolBar = hToolBar;

	return true;
}

//-----------------------------------------------------------------------------
//* Resize Window
//! Sets a new size to render window
//! @param[in] width New width of render window
//! @param[in] height New height of render window
//-----------------------------------------------------------------------------
void WindowManager::resizeWindow(int width, int height)
{
	RECT windowRect, statusRect, toolRect;
	m_width  = width;
	m_height = height;

    //Get Size of Render window
	GetClientRect(m_hWnd, &windowRect);

    //Get Size of Statusbar
	GetWindowRect(m_statusBar, &statusRect);

    //Get Size of Toolbar
    if ( m_toolBar )
    {
        GetWindowRect(m_toolBar, &toolRect);
    }
    else
    {
    	toolRect.bottom = toolRect.top = 0;
    }

    //Calculate Height Offset
	m_heightOffset = (statusRect.bottom - statusRect.top);

	windowRect.right = windowRect.left + m_width - 1; 
	windowRect.bottom = windowRect.top + m_height - 1 + m_heightOffset;

	AdjustWindowRect( &windowRect, GetWindowLong( m_hWnd, GWL_STYLE ), GetMenu( m_hWnd ) != NULL );
	
	int cx = windowRect.right - windowRect.left + 1;
	int cy = windowRect.bottom - windowRect.top + 1 + toolRect.bottom - toolRect.top + 1;
	SetWindowPos( m_hWnd, NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE );
}

//-----------------------------------------------------------------------------
//* Toggle Fullscreen
//! Goes to fullscreen-mode or window-mode
//-----------------------------------------------------------------------------
void WindowManager::toggleFullscreen()
{
    m_fullscreen = !m_fullscreen;

    if ( m_fullscreen )
	{	
		ShowCursor( false );
		
		//Hide menu
		m_menu = GetMenu(m_hWnd);
		if (m_menu) {
			SetMenu(m_hWnd, 0);
		}

		//Hide status bar
		if ( m_statusBar ) {
			ShowWindow(m_statusBar, SW_HIDE);
		}

		//Set Style (remove window frame)
		m_windowExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		m_windowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);

		//Save window position and size
		GetWindowRect(m_hWnd, &m_windowRect);
		
		//Move window to top-left corner
        m_heightOffset = 0;
		SetWindowPos(m_hWnd, 0, 0, 0,	m_width, m_height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }
    else
    {
		ShowCursor(true);

		//Show Menu
		if ( m_menu ) {
			SetMenu(m_hWnd, m_menu);
		}

		//Show statusbar
		if ( m_statusBar ) {
			ShowWindow(m_statusBar, SW_SHOW);
		}

		//Restore window style
		SetWindowLong(m_hWnd, GWL_STYLE, m_windowStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_windowExStyle);

		//Restore Window position and size
		resizeWindow(m_width, m_height);
		SetWindowPos(m_hWnd, 0, m_windowRect.left, m_windowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);       
    }
}

