#ifndef CONFIG_MAP_H_
#define CONFIG_MAP_H_

//*****************************************************************************
//! Struct with all configuration settings are stored
//*****************************************************************************
struct ConfigMap
{
    //Configuration Settings
    int  fullscreenWidth;        //!< Display width in fullscreen mode,             default = 800
    int  fullscreenHeight;       //!< Display height in fullscreen mode,            default = 600
    int  fullscreenRefreshRate;  //!< Screen refresh-rate in fullscreen mode,       default = 60
    int  fullscreenBitDepth;     //!< Bitdepth in fullscreen mode,                  default = 32
	int  windowWidth;            //!< Width of render window in window mode,        default = 800
	int  windowHeight;           //!< Height of render window in window mode,       default = 600	
	int  textureCacheSize;       //!< Size of texture cache used to store textures, default = 15 MB
    bool wireframe;              //!< Render in wireframe?                          defualt = false
    bool fog;                    //!< Render fog?                                   default = true

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    ConfigMap() 
    { 
        reset(); 
    }

    //-----------------------------------------------------------------------------
    //* Reset
    //! Will assign defualt values to all config settings
    //-----------------------------------------------------------------------------
    void reset()
    {
        fullscreenWidth       = 800;
        fullscreenHeight      = 600;
        fullscreenBitDepth    = 32;
        fullscreenRefreshRate = 60;        
	    windowWidth           = 800; 
	    windowHeight          = 600; 
	    textureCacheSize      = 15 * (1024 * 1024);
        wireframe             = false;
        fog                   = true;
    }
};

#endif