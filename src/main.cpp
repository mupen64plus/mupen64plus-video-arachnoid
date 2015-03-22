/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

//Includes
#define M64P_PLUGIN_PROTOTYPES 1
#include <stdio.h>
#include <string.h>

#include "ConfigMap.h"
#include "GraphicsPlugin.h"        //Main class
#include "Logger.h"                //Debug logger
#include "MemoryLeakDetector.h"    //For detecting memory leaks
#include "OpenGLManager.h"
#include "config/Config.h"         //Configuration   
#include "m64p.h"
#include "m64p_common.h"
#include "m64p_config.h"
#include "m64p_plugin.h"
#include "m64p_types.h"
#include "m64p_vidext.h"
#include "osal_dynamiclib.h"

//Definitions
#define PLUGIN_NAME "Arachnoid Video Plugin"
#define PLUGIN_VERSION           0x020000
#define VIDEO_PLUGIN_API_VERSION 0x020200
#define CONFIG_API_VERSION       0x020000
#define VIDEXT_API_VERSION       0x030000

#define VERSION_PRINTF_SPLIT(x) (((x) >> 16) & 0xffff), (((x) >> 8) & 0xff), ((x) & 0xff)

#define MI_INTR_DP 0x00000020      //!< RDP Interrupt signal
#define MI_INTR_SP 0x00000001      //!< RSP Interrupt signal

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

char              g_cfgFilename[] = "ConfigGraphicsPlugin.cfg";  //!< Name configuration file
GFX_INFO          g_graphicsInfo;                                //!< Information about window, memory...
GraphicsPlugin    g_graphicsPlugin;                              //!< Main class for application
Config            g_config(&g_graphicsPlugin);                   //!< Handles configuration

void (*renderCallback)(int) = NULL;


/* definitions of pointers to Core config functions */
ptr_ConfigOpenSection      ConfigOpenSection = NULL;
ptr_ConfigSetParameter     ConfigSetParameter = NULL;
ptr_ConfigGetParameter     ConfigGetParameter = NULL;
ptr_ConfigGetParameterHelp ConfigGetParameterHelp = NULL;
ptr_ConfigSetDefaultInt    ConfigSetDefaultInt = NULL;
ptr_ConfigSetDefaultFloat  ConfigSetDefaultFloat = NULL;
ptr_ConfigSetDefaultBool   ConfigSetDefaultBool = NULL;
ptr_ConfigSetDefaultString ConfigSetDefaultString = NULL;
ptr_ConfigGetParamInt      ConfigGetParamInt = NULL;
ptr_ConfigGetParamFloat    ConfigGetParamFloat = NULL;
ptr_ConfigGetParamBool     ConfigGetParamBool = NULL;
ptr_ConfigGetParamString   ConfigGetParamString = NULL;

ptr_ConfigGetSharedDataFilepath ConfigGetSharedDataFilepath = NULL;
ptr_ConfigGetUserConfigPath     ConfigGetUserConfigPath = NULL;
ptr_ConfigGetUserDataPath       ConfigGetUserDataPath = NULL;
ptr_ConfigGetUserCachePath      ConfigGetUserCachePath = NULL;

/* definitions of pointers to Core video extension functions */
ptr_VidExt_Init                  CoreVideo_Init = NULL;
ptr_VidExt_Quit                  CoreVideo_Quit = NULL;
ptr_VidExt_ListFullscreenModes   CoreVideo_ListFullscreenModes = NULL;
ptr_VidExt_SetVideoMode          CoreVideo_SetVideoMode = NULL;
ptr_VidExt_SetCaption            CoreVideo_SetCaption = NULL;
ptr_VidExt_ToggleFullScreen      CoreVideo_ToggleFullScreen = NULL;
ptr_VidExt_ResizeWindow          CoreVideo_ResizeWindow = NULL;
ptr_VidExt_GL_GetProcAddress     CoreVideo_GL_GetProcAddress = NULL;
ptr_VidExt_GL_SetAttribute       CoreVideo_GL_SetAttribute = NULL;
ptr_VidExt_GL_SwapBuffers        CoreVideo_GL_SwapBuffers = NULL;

//-----------------------------------------------------------------------------
// Mupen64plus 2.0 Common Plugin API Functions
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context,
                                   void (*DebugCallback)(void *, int, const char *))
{
    char logMsg[530];
    Logger::getSingleton().initialize(DebugCallback, Context);
    Logger::getSingleton().printMsg("PluginStartup");

    /* attach and call the CoreGetAPIVersions function, check Config and Video Extension API versions for compatibility */
    ptr_CoreGetAPIVersions CoreAPIVersionFunc;
    CoreAPIVersionFunc = (ptr_CoreGetAPIVersions) osal_dynlib_getproc(CoreLibHandle, "CoreGetAPIVersions");
    if (CoreAPIVersionFunc == NULL)
    {
        sprintf(logMsg, "Core emulator broken; no CoreAPIVersionFunc() function found.");
        Logger::getSingleton().printMsg(logMsg, M64MSG_ERROR);
        return M64ERR_INCOMPATIBLE;
    }
    int ConfigAPIVersion, DebugAPIVersion, VidextAPIVersion;
    (*CoreAPIVersionFunc)(&ConfigAPIVersion, &DebugAPIVersion, &VidextAPIVersion, NULL);
    if ((ConfigAPIVersion & 0xffff0000) != (CONFIG_API_VERSION & 0xffff0000))
    {
        sprintf(logMsg, "Emulator core Config API (v%i.%i.%i) incompatible with plugin (v%i.%i.%i)",
                VERSION_PRINTF_SPLIT(ConfigAPIVersion), VERSION_PRINTF_SPLIT(CONFIG_API_VERSION));
        Logger::getSingleton().printMsg(logMsg, M64MSG_ERROR);
        return M64ERR_INCOMPATIBLE;
    }
    if ((VidextAPIVersion & 0xffff0000) != (VIDEXT_API_VERSION & 0xffff0000))
    {
        sprintf(logMsg, "Emulator core Video Extension API (v%i.%i.%i) incompatible with plugin (v%i.%i.%i)",
                VERSION_PRINTF_SPLIT(VidextAPIVersion), VERSION_PRINTF_SPLIT(VIDEXT_API_VERSION));
        Logger::getSingleton().printMsg(logMsg, M64MSG_ERROR);
        return M64ERR_INCOMPATIBLE;
    }

    /* Get the core config function pointers from the library handle */
    ConfigOpenSection = (ptr_ConfigOpenSection) osal_dynlib_getproc(CoreLibHandle, "ConfigOpenSection");
    ConfigSetParameter = (ptr_ConfigSetParameter) osal_dynlib_getproc(CoreLibHandle, "ConfigSetParameter");
    ConfigGetParameter = (ptr_ConfigGetParameter) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParameter");
    ConfigSetDefaultInt = (ptr_ConfigSetDefaultInt) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultInt");
    ConfigSetDefaultFloat = (ptr_ConfigSetDefaultFloat) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultFloat");
    ConfigSetDefaultBool = (ptr_ConfigSetDefaultBool) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultBool");
    ConfigSetDefaultString = (ptr_ConfigSetDefaultString) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultString");
    ConfigGetParamInt = (ptr_ConfigGetParamInt) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamInt");
    ConfigGetParamFloat = (ptr_ConfigGetParamFloat) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamFloat");
    ConfigGetParamBool = (ptr_ConfigGetParamBool) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamBool");
    ConfigGetParamString = (ptr_ConfigGetParamString) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamString");

    ConfigGetSharedDataFilepath = (ptr_ConfigGetSharedDataFilepath) osal_dynlib_getproc(CoreLibHandle, "ConfigGetSharedDataFilepath");
    ConfigGetUserConfigPath = (ptr_ConfigGetUserConfigPath) osal_dynlib_getproc(CoreLibHandle, "ConfigGetUserConfigPath");
    ConfigGetUserDataPath = (ptr_ConfigGetUserDataPath) osal_dynlib_getproc(CoreLibHandle, "ConfigGetUserDataPath");
    ConfigGetUserCachePath = (ptr_ConfigGetUserCachePath) osal_dynlib_getproc(CoreLibHandle, "ConfigGetUserCachePath");

    if (!ConfigOpenSection   || !ConfigSetParameter    || !ConfigGetParameter ||
        !ConfigSetDefaultInt || !ConfigSetDefaultFloat || !ConfigSetDefaultBool || !ConfigSetDefaultString ||
        !ConfigGetParamInt   || !ConfigGetParamFloat   || !ConfigGetParamBool   || !ConfigGetParamString ||
        !ConfigGetSharedDataFilepath || !ConfigGetUserConfigPath || !ConfigGetUserDataPath || !ConfigGetUserCachePath)
    {
        Logger::getSingleton().printMsg("Couldn't connect to Core configuration functions", M64MSG_ERROR);
        return M64ERR_INCOMPATIBLE;
    }

    /* Get the core Video Extension function pointers from the library handle */
    CoreVideo_Init = (ptr_VidExt_Init) osal_dynlib_getproc(CoreLibHandle, "VidExt_Init");
    CoreVideo_Quit = (ptr_VidExt_Quit) osal_dynlib_getproc(CoreLibHandle, "VidExt_Quit");
    CoreVideo_ListFullscreenModes = (ptr_VidExt_ListFullscreenModes) osal_dynlib_getproc(CoreLibHandle, "VidExt_ListFullscreenModes");
    CoreVideo_SetVideoMode = (ptr_VidExt_SetVideoMode) osal_dynlib_getproc(CoreLibHandle, "VidExt_SetVideoMode");
    CoreVideo_SetCaption = (ptr_VidExt_SetCaption) osal_dynlib_getproc(CoreLibHandle, "VidExt_SetCaption");
    CoreVideo_ToggleFullScreen = (ptr_VidExt_ToggleFullScreen) osal_dynlib_getproc(CoreLibHandle, "VidExt_ToggleFullScreen");
    CoreVideo_ResizeWindow = (ptr_VidExt_ResizeWindow) osal_dynlib_getproc(CoreLibHandle, "VidExt_ResizeWindow");
    CoreVideo_GL_GetProcAddress = (ptr_VidExt_GL_GetProcAddress) osal_dynlib_getproc(CoreLibHandle, "VidExt_GL_GetProcAddress");
    CoreVideo_GL_SetAttribute = (ptr_VidExt_GL_SetAttribute) osal_dynlib_getproc(CoreLibHandle, "VidExt_GL_SetAttribute");
    CoreVideo_GL_SwapBuffers = (ptr_VidExt_GL_SwapBuffers) osal_dynlib_getproc(CoreLibHandle, "VidExt_GL_SwapBuffers");

    if (!CoreVideo_Init || !CoreVideo_Quit || !CoreVideo_ListFullscreenModes || !CoreVideo_SetVideoMode ||
        !CoreVideo_SetCaption || !CoreVideo_ToggleFullScreen || !CoreVideo_GL_GetProcAddress ||
        !CoreVideo_GL_SetAttribute || !CoreVideo_GL_SwapBuffers || !CoreVideo_ResizeWindow)
    {
        Logger::getSingleton().printMsg("Couldn't connect to Core video functions", M64MSG_ERROR);
        return M64ERR_INCOMPATIBLE;
    }

    //Read configuration
    if (g_config.initialize())
    {
        g_config.load();
        g_graphicsPlugin.setConfig(g_config.getConfig());
    }

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
    //Close Logger
    Logger::getSingleton().printMsg("CloseDLL\n");
    Logger::getSingleton().dispose();   

    //g_graphicsPlugin.dispose();  
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    /* set version info */
    if (PluginType != NULL)
        *PluginType = M64PLUGIN_GFX;

    if (PluginVersion != NULL)
        *PluginVersion = PLUGIN_VERSION;

    if (APIVersion != NULL)
        *APIVersion = VIDEO_PLUGIN_API_VERSION;
    
    if (PluginNamePtr != NULL)
        *PluginNamePtr = PLUGIN_NAME;

    if (Capabilities != NULL)
    {
        *Capabilities = 0;
    }
                    
    return M64ERR_SUCCESS;
}



//-----------------------------------------------------------------------------
// Mupen64plus 2.0 Video Plugin API Functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//* InitiateGFX
//! This function is called when the DLL is started to give
//! information from the emulator that the n64 graphics
//!    uses. This is not called from the emulation thread.
//! @param[in] Gfx_Info Information about rom and emulator
//! @return true on success, FALSE on failure to initialise
//!           
//! @note on interrupts :
//!    To generate an interrupt set the appropriate bit in MI_INTR_REG
//!    and then  the function CheckInterrupts to tell the emulator
//!    that there is a waiting interrupt.
//-----------------------------------------------------------------------------
EXPORT BOOL CALL InitiateGFX(GFX_INFO Gfx_Info)
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
EXPORT int CALL RomOpen()
{
    Logger::getSingleton().printMsg("RomOpen\n");
    return g_graphicsPlugin.initialize(&g_graphicsInfo);
}

//-----------------------------------------------------------------------------
//* Resize Video Output
//! This function is called to force us to resize our output OpenGL window.
//! This is currently unsupported, and should never be called because we do
//! not pass the RESIZABLE flag to VidExt_SetVideoMode when initializing.
//-----------------------------------------------------------------------------
EXPORT void CALL ResizeVideoOutput(int Width, int Height)
{
}

//-----------------------------------------------------------------------------
//* Rom Closed
//! This function is called when a rom is closed.
//-----------------------------------------------------------------------------
EXPORT void CALL RomClosed()
{
    //Logger::getSingleton().printMsg("RomClosed\n");
    //Destroy 
    g_graphicsPlugin.dispose();
}

//-----------------------------------------------------------------------------
//* Update Screen
//! This function is called in response to a vsync of the
//! screen where the VI bit in MI_INTR_REG has already been set
//-----------------------------------------------------------------------------
EXPORT void CALL UpdateScreen()
{
    if (g_config.getConfig()->screenUpdateSetting == SCREEN_UPDATE_VI)
        g_graphicsPlugin.drawScreen();
    else if (g_config.getConfig()->screenUpdateSetting == SCREEN_UPDATE_CI)
        g_graphicsPlugin.setDrawScreenSignal();
    else
    {
        Logger::getSingleton().printMsg("Invalid screen update setting!", M64MSG_WARNING);
        g_graphicsPlugin.drawScreen();
    }
}


//-----------------------------------------------------------------------------
//* ProcessDList
//! This function is called when there is a Dlist to be processed. (High level GFX list)
//-----------------------------------------------------------------------------
EXPORT void CALL ProcessDList()
{
    Logger::getSingleton().printMsg("ProcessDList\n");

    try
    {
        g_graphicsPlugin.viStatusChanged();
        g_graphicsPlugin.processDisplayList();
    }
    catch (...)
    {
        Logger::getSingleton().printMsg("Unknown Error processing DisplayList", M64MSG_WARNING); 
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
EXPORT void CALL ProcessRDPList()
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
EXPORT void CALL ShowCFB()
{
    Logger::getSingleton().printMsg("ShowCFB\n");
}

//-----------------------------------------------------------------------------
//* ViStatusChanged
//! This function is called to notify the dll that the
//! ViStatus registers value has been changed.
//-----------------------------------------------------------------------------
EXPORT void CALL ViStatusChanged()
{
    Logger::getSingleton().printMsg("ViStatusChanged");

    //g_graphicsPlugin.viStatusChanged();
}

//-----------------------------------------------------------------------------
//* ViWidthChanged
//! This function is called to notify the dll that the
//! ViWidth registers value has been changed.
//-----------------------------------------------------------------------------
EXPORT void CALL ViWidthChanged()
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
EXPORT void CALL MoveScreen(int xpos, int ypos)
{
    Logger::getSingleton().printMsg("MoveScreen\n");
    //TODO
}

//-----------------------------------------------------------------------------
//* ChangeWindow
//! Toggle between fullscreen and window mode
//-----------------------------------------------------------------------------
EXPORT void CALL ChangeWindow()
{
    Logger::getSingleton().printMsg("ChangeWindow\n");
    //Toggle Fullscreen
    g_graphicsPlugin.toggleFullscreen();
}

//-----------------------------------------------------------------------------
//* ReadScreen2
//! This function reads the pixels of the currently displayed screen
//-----------------------------------------------------------------------------
EXPORT void CALL ReadScreen2(void *dest, int *width, int *height, int front)
{
    g_graphicsPlugin.takeScreenshot(dest, width, height, front);
}

//-----------------------------------------------------------------------------
//* SetRenderingCallback
//! Allows the core to register a callback function that will be called by the 
//! graphics plugin just before the the frame buffers are swapped.
//-----------------------------------------------------------------------------
EXPORT void CALL SetRenderingCallback(void (*callback)(int))
{
    OpenGLManager::getSingleton().setRenderingCallback(callback);
}

//-----------------------------------------------------------------------------
//* FBRead
//! Read data from frame buffer into emulated RAM space 
//-----------------------------------------------------------------------------
EXPORT void CALL FBRead(unsigned int addr)
{
    //TODO
}

//-----------------------------------------------------------------------------
//* FBWrite
//! Write data from emulated RAM space into frame buffer
//-----------------------------------------------------------------------------
EXPORT void CALL FBWrite(unsigned int addr, unsigned int size)
{
    //TODO
}

//-----------------------------------------------------------------------------
//* FBWrite
//! Get some information about the frame buffer 
//-----------------------------------------------------------------------------
EXPORT void FBGetFrameBufferInfo(void *p)
{
    //TODO
}

#ifdef __cplusplus
}
#endif
