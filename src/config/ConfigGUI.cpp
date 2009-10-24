#include "ConfigGUI.h"
#include <cstdio>
#include <vector>

//! Definition of the number of Window modes that this graphics plugin support
#define NUM_WINDOW_MODES 12
struct
{
	int width;
	int height;
	char *description;
} g_windowedModes[NUM_WINDOW_MODES] = { { 320, 240, "320 x 240" },
						                { 400, 300, "400 x 300" },
						                { 480, 360, "480 x 360" },
						                { 640, 480, "640 x 480" },
						                { 800, 600, "800 x 600" },
						                { 960, 720, "960 x 720" },
						                { 1024, 768, "1024 x 768" },
						                { 1152, 864, "1152 x 864" },
						                { 1280, 960, "1280 x 960" },
						                { 1280, 1024, "1280 x 1024" },
						                { 1440, 1080, "1440 x 1080" },
						                { 1600, 1200, "1600 x 1200" } };

//! Definition of the maximum number of fullscreen modes that are allowed.
#define MAX_NUM_FULLSCREEN_MODES 200

//! Struct used to store fullscreen modes
struct FullscreenMode
{
	int width;
	int height;
    int bitDepth;
    int refreshRate;
};

//! Global variable used to store all supported fullscreen modes
std::vector<FullscreenMode> g_fullscreenModes;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
ConfigGUI::ConfigGUI()
{
    //Save old device mode
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &m_oldDevMode);
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
ConfigGUI::~ConfigGUI()
{

}

//-----------------------------------------------------------------------------
//! Initialize
//! @param[in] hWndDlg Windows handle to config dialog
//-----------------------------------------------------------------------------
bool ConfigGUI::initialize(HWND hWndDlg)
{
	m_hWnd = hWndDlg;
    g_fullscreenModes.resize(MAX_NUM_FULLSCREEN_MODES);
	return true;
}

//-----------------------------------------------------------------------------
//! Set Fullscreen
//! @param[in] width Wanted display width in fullscreen mode
//! @param[in] height Wanted display height in fullscreen mode
//! @param[in] bitDepth Wanted bitdepth in fullscreen mode
//!     \arg \c 16 Bitdepth of 16
//!     \arg \c 32 Bitdepth of 32
//! @param[in] refreshRate Wanted screen refreshrate in fullscreen mode
//-----------------------------------------------------------------------------
void ConfigGUI::setFullscreen(int width, int height, int bitDepth, int refreshRate)
{
    //Set Bit Depth
    int i = 0;
    int counter = 0;
    DEVMODE deviceMode;
    char text[256];

    //For each fullscreen display setting
	while ( EnumDisplaySettings(0, i++, &deviceMode) != 0)
	{        
        //Only save fullscreen modes with bitDepth bigger then 8 like 16 or 32
        if ( deviceMode.dmBitsPerPel > 8 && counter < MAX_NUM_FULLSCREEN_MODES )
        {
            //Add BitDepth to Combobox
            sprintf( text, "%i x %i, %i bit, %i Hz", deviceMode.dmPelsWidth, deviceMode.dmPelsHeight, deviceMode.dmBitsPerPel, deviceMode.dmDisplayFrequency );
            SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_ADDSTRING, 0, (LPARAM)text );

            g_fullscreenModes.at(counter).width       = deviceMode.dmPelsWidth;
            g_fullscreenModes.at(counter).height      = deviceMode.dmPelsHeight;
            g_fullscreenModes.at(counter).bitDepth    = deviceMode.dmBitsPerPel;
            g_fullscreenModes.at(counter).refreshRate = deviceMode.dmDisplayFrequency;

            //Found Bit Depth?
            if ( deviceMode.dmBitsPerPel == bitDepth && 
                 deviceMode.dmPelsWidth == width && 
                 deviceMode.dmPelsHeight == height && 
                 deviceMode.dmDisplayFrequency == refreshRate )
            {
                //Set Selected
				SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_SETCURSEL, counter, 0 );
            }
            counter++;
        }
    }

    //Look for error
	if ( SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0) == CB_ERR )
    {
        //Set Last
		SendDlgItemMessage( m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_SETCURSEL, SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCOUNT, 0, 0) - 1, 0 );
    }   
}

//-----------------------------------------------------------------------------
//! Get Fullscreen Width
//! @retval int Returns the display width in fullscreen mode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getFullscreenWidth()
{
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0);
	return g_fullscreenModes.at(index).width;
}

//-----------------------------------------------------------------------------
//! Get Fullscreen Height
//! @retval int Returns the display height in fullscreen mode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getFullscreenHeight()
{
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0);
	return g_fullscreenModes.at(index).height;
}

//-----------------------------------------------------------------------------
//! Get Fullscreen Bit Depth
//! @retval int Returns the bitdepth in fullscreen mode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getFullscreenBitDepth() 
{ 
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0);
	return g_fullscreenModes.at(index).bitDepth;
}

//-----------------------------------------------------------------------------
//! Get Fullscreen Refresh Rate
//! @retval int Returns the refresh in fullscreen mode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getFullscreenRefreshRate() 
{
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_FULLSCREEN_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0);
    return g_fullscreenModes.at(index).refreshRate;
}

//-----------------------------------------------------------------------------
//! Set Window Mode
//! @param[in] width Wanted width of render window in window mode
//! @param[in] height Wanted height of render window in window mode
//-----------------------------------------------------------------------------
void ConfigGUI::setWindowMode(int width, int height)
{
	for (int i=0; i<NUM_WINDOW_MODES; ++i)
	{
        if ( m_oldDevMode.dmPelsWidth > g_windowedModes[i].width && m_oldDevMode.dmPelsHeight > g_windowedModes[i].height )
        {
		    SendDlgItemMessage(m_hWnd, IDC_RESOLUTION_COMBOBOX, CB_ADDSTRING, 0, (LPARAM)g_windowedModes[i].description );
		    if ( width == g_windowedModes[i].width && height == g_windowedModes[i].height)
		    {
			    SendDlgItemMessage(m_hWnd, IDC_RESOLUTION_COMBOBOX, CB_SETCURSEL, i, 0);
		    }
        }
	}
}

//-----------------------------------------------------------------------------
//! Get Window Width
//! @retval int Returns the width of renderwindow in windowmode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getWindowWidth()
{
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_RESOLUTION_COMBOBOX, CB_GETCURSEL, 0, 0);
	return g_windowedModes[index].width;	
}

//-----------------------------------------------------------------------------
//! Get Window Height
//! @retval int Returns the height of renderwindow in windowmode from config dialog
//-----------------------------------------------------------------------------
int ConfigGUI::getWindowHeight()
{
	int index  = (int)SendDlgItemMessage(m_hWnd, IDC_RESOLUTION_COMBOBOX, CB_GETCURSEL, 0, 0);
    return g_windowedModes[index].height;
}

//-----------------------------------------------------------------------------
//! Set Texture Cache Size
//! @param[in] sizeInBytes How big in bytes the texture cache should be
//-----------------------------------------------------------------------------
void ConfigGUI::setTextureCacheSize(int sizeInBytes)
{
	//Set Texture Cache Size
	char text[256];
	int sizeInMegaBytes = sizeInBytes / (1024 * 1024);
	itoa(sizeInMegaBytes, text, 10);
	SendDlgItemMessage(m_hWnd, IDC_TEXTURE_CACHE_EDITBOX, WM_SETTEXT, 0, (LPARAM)text);
}

//-----------------------------------------------------------------------------
//! Get Texture Cache Size
//! @retval int Returns how big in bytes the texture cache should be
//-----------------------------------------------------------------------------
int ConfigGUI::getTextureCacheSize()
{
	char text[256];
	SendDlgItemMessage(m_hWnd, IDC_TEXTURE_CACHE_EDITBOX, WM_GETTEXT, 4, (LPARAM)text);
	return atol(text) * 1024 * 1024;
}

//-----------------------------------------------------------------------------
//! Set Wireframe
//! @param wireframe True if user wants to check the wireframe-check-box in config dialog
//-----------------------------------------------------------------------------
void ConfigGUI::setWireframe(bool wireframe)
{
    SendDlgItemMessage(m_hWnd, IDC_WIREFRAME_CHECKBOX, BM_SETCHECK, wireframe ? (LPARAM)BST_CHECKED : (LPARAM)BST_UNCHECKED, 0);
}

//-----------------------------------------------------------------------------
//! Get Wireframe
//! @retval int Returns true if the wireframe-check-box is checked
//-----------------------------------------------------------------------------
bool ConfigGUI::getWireframe()
{
    return (SendDlgItemMessage(m_hWnd, IDC_WIREFRAME_CHECKBOX, BM_GETCHECK, 0, 0) == BST_CHECKED);
}

//-----------------------------------------------------------------------------
//! Set Fog
//! @param fog True if user wants to check the fog-check-box in config dialog
//-----------------------------------------------------------------------------
void ConfigGUI::setFog(bool fog)
{
    SendDlgItemMessage(m_hWnd, IDC_FOG_CHECKBOX, BM_SETCHECK, fog ? (LPARAM)BST_CHECKED : (LPARAM)BST_UNCHECKED, 0);
}

//-----------------------------------------------------------------------------
//! Get Fog
//! @retval int Returns true if the enable-fog-check-box is checked
//-----------------------------------------------------------------------------
bool ConfigGUI::getFog()
{
    return (SendDlgItemMessage(m_hWnd, IDC_FOG_CHECKBOX, BM_GETCHECK, 0, 0) == BST_CHECKED);
}


