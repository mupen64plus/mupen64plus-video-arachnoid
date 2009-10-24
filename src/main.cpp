//*****************************************************************************
//* Arachnoid Graphics Plugin for Nintendo 64 Emulation.
//* ----------------------------------------------------
//*
//! @file main.cpp
//! @brief Arachnoid Graphics Plugin for Nintendo 64 Emulation.
//! @author Kristofer Karlsson 
//! @author Rickard Niklasson
//! @version v1.0
//! @date    2007
//!
//! Should be compatible following emulators:
//!   * Project 64 
//!   * 1964
//!
//*****************************************************************************
//
// Arachnoid Graphics Plugin for Nintendo 64 Emulation.
// Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//*****************************************************************************

//Includes
#include "GraphicsPlugin.h"        //Main class
#include "PluginInfo.h"            //Tell emulator what type of plugin this is
#include "Config.h"                //Configuration   
#include "Logger.h"                //Debug logger
#include "MemoryLeakDetector.h"    //For detecting memory leaks
#include "ConsoleWindow.h"         //Console Window for debuging

//Definitions
#define MI_INTR_DP 0x00000020      //!< RDP Interrupt signal
#define MI_INTR_SP 0x00000001      //!< RSP Interrupt signal

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

char              g_cfgFilename[] = "ConfigGraphicsPlugin.cfg";  //!< Name configuration file
GFX_INFO          g_graphicsInfo;                                //!< Information about window, memory...
GraphicsPlugin    g_graphicsPlugin;                              //!< Main class for application
Config            g_config(&g_graphicsPlugin);                   //!< Handles configuration
HINSTANCE         g_instance;                                    //!< DLL Instance
 
//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	g_instance = hinstDLL;

    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            g_graphicsPlugin.setConfig(g_config.getConfig());
            g_config.loadFromFile(g_cfgFilename);		
            break;
        case DLL_PROCESS_DETACH:
			#ifdef _DEBUG
				//Close Console
				terminateConsole();
			#endif
            break;
    }
	return true;
}

//-----------------------------------------------------------------------------
// Handle Cofig Dialog Process
//-----------------------------------------------------------------------------
BOOL CALLBACK g_configDlgProc(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	g_config.handleWindowMsg(hWndDlg, message, wParam, lParam);
    return false; 
}

//-----------------------------------------------------------------------------
//* DllAbout
//! This function is optional function that is provided
//! to give further information about the DLL.
//! @param hParent HWND parent a handle to the window that calls this function
//-----------------------------------------------------------------------------
void  DllAbout(HWND hParent)
{
    Logger::getSingleton().printMsg("DllAbout\n");
	MessageBox(hParent, "Arachnoid Graphics Plugin for Nintendo 64 Emulation by Kristofer Karlsson and Rickard Niklasson", "Arachnoid Graphics Plugin", MB_OK|MB_SETFOREGROUND );
}

//-----------------------------------------------------------------------------
//* DllConfig
//! This function is optional function that is provided
//! to allow the user to configure the dll
//! @param hParent A handle to the window that calls this function
//-----------------------------------------------------------------------------
void DllConfig(HWND hParent)
{
    Logger::getSingleton().printMsg("DllConfig\n");
	g_config.openConfigDialog(g_instance, g_configDlgProc, g_graphicsInfo.hWnd);
}

//-----------------------------------------------------------------------------
//* DllTest
//! This function is optional function that is provided
//! to allow the user to test the dll
//! @param hParent A handle to the window that calls this function
//! @todo DllTest
//----------------------------------------------------------------------------- 
void DllTest(HWND hParent)
{
    Logger::getSingleton().printMsg("DllTest\n");    
	//TODO
}

//-----------------------------------------------------------------------------
//* GetDllInfo
//! This function allows the emulator to gather information
//! about the dll by filling in the PluginInfo structure.
//! @param pluginInfo A pointer to a PLUGIN_INFO stucture that needs to be
//!                   filled by the function. 
//-----------------------------------------------------------------------------
 void  GetDllInfo(PLUGIN_INFO * pluginInfo)
{	
	#ifdef _DEBUG
		//Create Console
		createConsole();
		Logger::getSingleton().initialize("log.txt", true, true);
	#else
		Logger::getSingleton().initialize("log.txt", false, false);
	#endif
	Logger::getSingleton().setLogDetail(LL_LOW);
    Logger::getSingleton().printMsg("GetDllInfo");    
	
	//Get Plugin Information
    getPluginInfo(pluginInfo);	
}

//-----------------------------------------------------------------------------
//* InitiateGFX
//! This function is called when the DLL is started to give
//! information from the emulator that the n64 graphics
//!	uses. This is not called from the emulation thread.
//! @param[in] Gfx_Info Information about rom and emulator
//! @return true on success, FALSE on failure to initialise
//!           
//! @note on interrupts :
//!    To generate an interrupt set the appropriate bit in MI_INTR_REG
//!    and then  the function CheckInterrupts to tell the emulator
//!    that there is a waiting interrupt.
//-----------------------------------------------------------------------------
BOOL InitiateGFX(GFX_INFO Gfx_Info)
{
    Logger::getSingleton().printMsg("InitiateGFX");

	//Save Graphics Info
	memcpy(&g_graphicsInfo, &Gfx_Info, sizeof(GFX_INFO));
	return true;
}

//-----------------------------------------------------------------------------
//* Rom Open
//! This function is called when a rom is open. (from the emulation thread)
//-----------------------------------------------------------------------------
 void RomOpen()
{	
    Logger::getSingleton().printMsg("RomOpen\n");    
    
	//Initialize Graphics Plugin	        
	g_graphicsPlugin.initialize(&g_graphicsInfo);    
}

//-----------------------------------------------------------------------------
//* Rom Closed
//! This function is called when a rom is closed.
//-----------------------------------------------------------------------------
void  RomClosed()
{
    //Logger::getSingleton().printMsg("RomClosed\n");


	//Destroy 
	g_graphicsPlugin.dispose();
}

//-----------------------------------------------------------------------------
//* Update Screen
//! This function is called in response to a vsync of the
//! screen were the VI bit in MI_INTR_REG has already been set
//-----------------------------------------------------------------------------
void UpdateScreen()
{
    //logger.printMsg("UpdateScreen");	

	g_graphicsPlugin.drawScreen();	

	//Render to screen
   	//DrawScreen();
}

//-----------------------------------------------------------------------------
//* DrawScreen
//! This function is called when the emulator receives a
//! WM_PAINT message. This allows the gfx to fit in when
//!	it is being used in the desktop.
//-----------------------------------------------------------------------------
void DrawScreen(void)
{
	//logger.printMsg("DrawScreen\n");

	//Render to screen
	//g_graphicsPlugin.drawScreen();		
}

//-----------------------------------------------------------------------------
//* ProcessDList
//! This function is called when there is a Dlist to be processed. (High level GFX list)
//-----------------------------------------------------------------------------
void ProcessDList()
{
	Logger::getSingleton().printMsg("ProcessDList\n");

	try
	{
		g_graphicsPlugin.viStatusChanged();
		g_graphicsPlugin.processDisplayList();
	}
	catch (...)
	{
		Logger::getSingleton().printMsg("Unknown Error processing DisplayList", LML_CRITICAL, true); 
		//MessageBox(0, "Unknown Error processing DisplayList", "Arachnoid Graphics Plugin", MB_OK|MB_SETFOREGROUND); 

		g_graphicsPlugin.dispose();
		g_graphicsPlugin.initialize(&g_graphicsInfo);

		//Trigger Interupts
		*(g_graphicsInfo.MI_INTR_REG) |= MI_INTR_DP;
		g_graphicsInfo.CheckInterrupts();
		*(g_graphicsInfo.MI_INTR_REG) |= MI_INTR_SP;
		g_graphicsInfo.CheckInterrupts();	
	}
}


//-----------------------------------------------------------------------------
//* ProcessRDPList
//! This function is called when there is a Dlist to be processed. (Low level GFX list)
//! @todo ProcessRDPList
//-----------------------------------------------------------------------------
void ProcessRDPList()
{
	Logger::getSingleton().printMsg("ProcessRDPList\n");
    //TODO
}

//-----------------------------------------------------------------------------
//* Show CFB
//! Usally once Dlists are started being displayed, cfb is
//! ignored. This function tells the dll to start displaying
//! them again.
//-----------------------------------------------------------------------------
void ShowCFB()
{
    Logger::getSingleton().printMsg("ShowCFB\n");
}

//-----------------------------------------------------------------------------
//* CloseDLL
//! This function is called when the emulator is closing down 
//! allowing the dll to de-initialise.
//----------------------------------------------------------------------------- 
void CloseDLL()
{
	//Close Logger
    Logger::getSingleton().printMsg("CloseDLL\n");
    Logger::getSingleton().dispose();   

	//g_graphicsPlugin.dispose();   
}


//-----------------------------------------------------------------------------
//* ViStatusChanged
//! This function is called to notify the dll that the
//! ViStatus registers value has been changed.
//-----------------------------------------------------------------------------
void ViStatusChanged()
{
    Logger::getSingleton().printMsg("ViStatusChanged");

	//g_graphicsPlugin.viStatusChanged();
}

//-----------------------------------------------------------------------------
//* ViWidthChanged
//! This function is called to notify the dll that the
//! ViWidth registers value has been changed.
//-----------------------------------------------------------------------------
void ViWidthChanged()
{	
    Logger::getSingleton().printMsg("ViWidthChanged");

	//g_graphicsPlugin.viStatusChanged();
}

//-----------------------------------------------------------------------------
//* MoveScreen
//! This function is called in response to the emulator
//! receiving a WM_MOVE passing the xpos and ypos passed
//! from that message.
//! @param xpos The x-coordinate of the upper-left corner of the 
//!             client area of the window.
//! @param ypos The y-coordinate of the upper-left corner of the
//!             client area of the window. 
//! @todo MoveScreen
//----------------------------------------------------------------------------- 
void MoveScreen(int xpos, int ypos)
{
    Logger::getSingleton().printMsg("MoveScreen\n");
    //TODO
}

//-----------------------------------------------------------------------------
//* ChangeWindow
//! Toggle between fullscreen and window mode
//-----------------------------------------------------------------------------
void ChangeWindow()
{
    Logger::getSingleton().printMsg("ChangeWindow\n");

	//Toggle Fullscreen
	g_graphicsPlugin.toggleFullscreen();
}

//-----------------------------------------------------------------------------
//* CaptureScreen
//! This function dumps the current frame to a file (takes a screenshot)
//! param[in] Pointer to the directory to save the file to.
//-----------------------------------------------------------------------------
void CaptureScreen(char* directory)
{
    Logger::getSingleton().printMsg("CaptureScreen");

    //Take a screenshot
    g_graphicsPlugin.takeScreenshot(directory);
}

