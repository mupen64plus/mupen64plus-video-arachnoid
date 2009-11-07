//*****************************************************************************
// Common gfx plugin spec, version #1.3 maintained by zilmar (zilmar@emulation64.com)
//
// All questions or suggestions should go through the mailing list.
// http://www.egroups.com/group/Plugin64-Dev
//*****************************************************************************
//
// Notes:
// ------
//
// Setting the approprate bits in the MI_INTR_REG and calling CheckInterrupts which 
// are both passed to the DLL in InitiateGFX will generate an Interrupt from with in 
// the plugin.
//
// The Setting of the RSP flags and generating an SP interrupt  should not be done in
// the plugin
//
//*****************************************************************************
#if 0
#ifndef ZILMARS_PLUGIN_SPEC_H_
#define ZILMARS_PLUGIN_SPEC_H_

#include "platform.h"

#if defined(__cplusplus)
extern "C" {
#endif

// Plugin types 
#define PLUGIN_TYPE_GRAPHICS		2   //!< Definition for graphics plugin

#ifdef WINDOWS
	#define EXPORT						__declspec(dllexport)
	#define CALL						_cdecl
#else
	#define EXPORT
	#define CALL
#endif

//*****************************************************************************
//! Plugin Info
//! Struct used to give emulator information about plugin
//*****************************************************************************
struct PLUGIN_INFO 
{
	WORD version;        //!<  Set to 0x0103 for version 1.3
	WORD type;           //!<  Set to PLUGIN_TYPE_GRAPHICS for graphics plugin
	char name[100];      //!<  Name of the DLL 
	BOOL normalMemory;   //!<  A normal BYTE array  
	BOOL memoryBswaped;  //!< a normal BYTE array where the memory has been prebswap on a dword (32 bits) boundry 
};

//*****************************************************************************
//! Graphics Info
//! Struct used to give graphicsplugin information
//*****************************************************************************
 struct GFX_INFO
 {
	HWND hWnd;			           //!< Render window 
	HWND hStatusBar;               //!< if render window does not have a status bar then this is NULL 
	BOOL MemoryBswaped;            //!< If this is set to TRUE, then the memory has been prebswap on a dword (32 bits) boundry 
						           //  	  eg. the first 8 bytes are stored like this:
	                               //          4 3 2 1   8 7 6 5
	BYTE* HEADER;	               //!< Pointer to rom header 
	BYTE* RDRAM;                   //!< Pointer to RDRAM 
	BYTE* DMEM;                    //!< Pointer to RSP Data Memory (extract the program counter from this memory)
	BYTE* IMEM;                    //!< Pointer to RSP Instruction Memory 
	DWORD* MI_INTR_REG;            //!< Used to generate interupts
	DWORD* DPC_START_REG;
	DWORD* DPC_END_REG;
	DWORD* DPC_CURRENT_REG;
	DWORD* DPC_STATUS_REG;
	DWORD* DPC_CLOCK_REG;
	DWORD* DPC_BUFBUSY_REG;
	DWORD* DPC_PIPEBUSY_REG;
	DWORD* DPC_TMEM_REG;

	//Video Interface
	DWORD* VI_STATUS_REG;
	DWORD* VI_ORIGIN_REG;
	DWORD* VI_WIDTH_REG;
	DWORD* VI_INTR_REG;
	DWORD* VI_V_CURRENT_LINE_REG;
	DWORD* VI_TIMING_REG;
	DWORD* VI_V_SYNC_REG;
	DWORD* VI_H_SYNC_REG;
	DWORD* VI_LEAP_REG;
	DWORD* VI_H_START_REG;
	DWORD* VI_V_START_REG;
	DWORD* VI_V_BURST_REG;
	DWORD* VI_X_SCALE_REG;
	DWORD* VI_Y_SCALE_REG;

	void (*CheckInterrupts)(void);  //!< Function pointer used to generate interupts     
};

//-----------------------------------------------------------------------------
//* DllAbout
//! This function is optional function that is provided
//! to give further information about the DLL.
//! @praram[in] HWND parent a handle to the window that calls this function
//-----------------------------------------------------------------------------
EXPORT void CALL DllAbout(HWND hParent);

//-----------------------------------------------------------------------------
//* DllConfig
//! This function is optional function that is provided
//! to allow the user to configure the dll
//! @param parent A handle to the window that calls this function
//-----------------------------------------------------------------------------
EXPORT void CALL DllConfig(HWND hParent);

//-----------------------------------------------------------------------------
//* DllTest
//! This function is optional function that is provided
//! to allow the user to test the dll
//! @param parent A handle to the window that calls this function
//-----------------------------------------------------------------------------
EXPORT void CALL DllTest(HWND hParent);

//-----------------------------------------------------------------------------
//* GetDllInfo
//! This function allows the emulator to gather information
//! about the dll by filling in the PluginInfo structure.
//! @param pluginInfo A pointer to a PLUGIN_INFO stucture that needs to be
//!                   filled by the function. 
//-----------------------------------------------------------------------------
EXPORT void CALL GetDllInfo(PLUGIN_INFO * PluginInfo);

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
EXPORT BOOL CALL InitiateGFX(GFX_INFO Gfx_Info);

//-----------------------------------------------------------------------------
//* Rom Open
//! This function is called when a rom is open. (from the emulation thread)
//-----------------------------------------------------------------------------
EXPORT void CALL RomOpen();

//-----------------------------------------------------------------------------
//* Rom Closed
//! This function is called when a rom is closed.
//-----------------------------------------------------------------------------
EXPORT void CALL RomClosed();

//-----------------------------------------------------------------------------
//* Update Screen
//! This function is called in response to a vsync of the
//! screen were the VI bit in MI_INTR_REG has already been set
//-----------------------------------------------------------------------------
EXPORT void CALL UpdateScreen();

//-----------------------------------------------------------------------------
//* DrawScreen
//! This function is called when the emulator receives a
//! WM_PAINT message. This allows the gfx to fit in when
//!	it is being used in the desktop.
//-----------------------------------------------------------------------------
EXPORT void CALL DrawScreen();

//-----------------------------------------------------------------------------
//* ProcessDList
//! This function is called when there is a Dlist to be processed. (High level GFX list)
//-----------------------------------------------------------------------------
EXPORT void CALL ProcessDList();

//-----------------------------------------------------------------------------
//* ProcessRDPList
//! This function is called when there is a Dlist to be processed. (Low level GFX list)
//----------------------------------------------------------------------------- 
EXPORT void CALL ProcessRDPList();

//-----------------------------------------------------------------------------
//* Show CFB
//! Useally once Dlists are started being displayed, cfb is
//! ignored. This function tells the dll to start displaying
//! them again.
//-----------------------------------------------------------------------------
EXPORT void CALL ShowCFB();

//-----------------------------------------------------------------------------
//* CloseDLL
//! This function is called when the emulator is closing down 
//! allowing the dll to de-initialise.
//----------------------------------------------------------------------------- 
EXPORT void CALL CloseDLL();

//-----------------------------------------------------------------------------
//* ViStatusChanged
//! This function is called to notify the dll that the
//! ViStatus registers value has been changed.
//-----------------------------------------------------------------------------
EXPORT void CALL ViStatusChanged();

//-----------------------------------------------------------------------------
//* ViWidthChanged
//! This function is called to notify the dll that the
//! ViWidth registers value has been changed.
//-----------------------------------------------------------------------------
EXPORT void CALL ViWidthChanged();

//-----------------------------------------------------------------------------
//* MoveScreen
//! This function is called in response to the emulator
//! receiving a WM_MOVE passing the xpos and ypos passed
//! from that message.
//! @param xpos The x-coordinate of the upper-left corner of the 
//!             client area of the window.
//! @param ypos The y-coordinate of the upper-left corner of the
//!             client area of the window. 
//----------------------------------------------------------------------------- 
EXPORT void CALL MoveScreen(int xpos, int ypos);

//-----------------------------------------------------------------------------
//* ChangeWindow
//! Toggle between fullscreen and window mode
//----------------------------------------------------------------------------- 
EXPORT void CALL ChangeWindow();

//-----------------------------------------------------------------------------
//* CaptureScreen
//! This function dumps the current frame to a file (takes a screenshot)
//! param[in] Pointer to the directory to save the file to.
//-----------------------------------------------------------------------------
EXPORT void CALL CaptureScreen ( char * Directory );

#if defined(__cplusplus)
}
#endif
#endif
#endif