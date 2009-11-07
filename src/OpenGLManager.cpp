#include "OpenGLManager.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
OpenGLManager::OpenGLManager()
{
#if 0
	m_currentDC = NULL;              
	m_rc        = NULL;   
	m_forceDisableCulling = false;

	//Save mode so we can switch back in destructor	
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &m_oldDevMode );
#endif
}

//-----------------------------------------------------------------------------
//* Initialize 
//! Initializes OpenGL. 
//
//! @param hWnd			window handle to render in
//! @param dc			device context to render to
//! @param fullscreen	will render scene in fullscreen if true
//! @param width		width of window or width of screen resolution
//! @param height		height of window or height of screen resolution
//! @param bitDepth		bitDepth to use
//! @param refreshRate	refresh frequency to use
//! @param vSync		limits frame rate to the monitor's refresh frequency
//! @param hideCursor	hides mouse coursor if true
//-----------------------------------------------------------------------------
bool OpenGLManager::initialize(HWND hWnd, HDC dc, bool fullscreen, int width, int height, int bitDepth, int refreshRate, bool vSync, bool hideCursor)
{
#if 0
	this->_setPixelFormat(dc);

	m_hWnd        = hWnd;
    m_currentDC   = dc;
	m_width       = width;
	m_height      = height;
    m_bitDepth    = bitDepth;
    m_refreshRate = refreshRate;
	m_fullscreen  = fullscreen;

    //Set Special Display Settings if fullscreen
	if ( fullscreen )
	{
		DEVMODE devModeScreen;
		memset(&devModeScreen, 0, sizeof(devModeScreen));          // clear the DEVMODE structure
		devModeScreen.dmSize             = sizeof(devModeScreen);  // size of the structure	
		devModeScreen.dmPelsWidth        = width;                  // set the width
		devModeScreen.dmPelsHeight       = height;                 // set the height	
		devModeScreen.dmBitsPerPel       = bitDepth;               // screenBpp;                 
        devModeScreen.dmDisplayFrequency = refreshRate;            // refresh rate
		devModeScreen.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if ( ChangeDisplaySettings(&devModeScreen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
		{		
			return false;   //Unable to run in fullscreen
		}
	}

    //Show cursor?
	ShowCursor( !hideCursor );	   
	
    //Create and set render context	
	if ( !(m_rc = wglCreateContext(m_currentDC))) {
		return false;
	}
    if ( !wglMakeCurrent(m_currentDC, m_rc) ) {
        return false;
    }

	//Set OpenGL Settings
    setClearColor(0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//Set render states
	setCullMode(false, true);
	setTextureing2D(false);
	setLighting(false);
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Set Viewport
//-----------------------------------------------------------------------------
void OpenGLManager::setViewport( int x, int y, int width, int height, float zNear, float zFar )
{
	glViewport(x, y, width, height); 

	//glViewport( gSP.viewport.x * OGL.scaleX, 
	//	       (VI.height - (gSP.viewport.y + gSP.viewport.height)) * OGL.scaleY + OGL.heightOffset, 
	//	        gSP.viewport.width * OGL.scaleX, 
	//			gSP.viewport.height * OGL.scaleY 
	//		 ); 

	//glDepthRange( 0.0f, 1.0f );//gSP.viewport.nearz, gSP.viewport.farz );
	glDepthRange( zNear, zFar );
}

//-----------------------------------------------------------------------------
//* Set Scissor
//! glScissor defines a rectangle, called the scissor box, in window coordinates. 
//! Only pixels inside the box are allowed to be modified.
//! glScissor(0,0,1,1) allows modification of only the lower left pixel in the window
//! glScissor(0,0,0,0) doesn't allow modification of any pixels in the window.
//!
//! @param x,y Specify the lower left corner of the box.  Defualt (0, 0).
//! @param width,height Specify the width and height of the box.
//-----------------------------------------------------------------------------
void OpenGLManager::setScissor(int x, int y, int width, int height)
{
	glScissor(x,y, width, height);
}

//-----------------------------------------------------------------------------
// Make Current
//-----------------------------------------------------------------------------
bool OpenGLManager::makeCurrent(HDC dc)
{
#if 0
    m_currentDC = dc;
    if ( !wglMakeCurrent(m_currentDC, m_rc) ) {

        LPVOID lpMsgBuf;
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       0,
                       GetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                       (LPTSTR) &lpMsgBuf,
                       0,
                       0);

        MessageBox(GetActiveWindow(), (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);

        LocalFree( lpMsgBuf );
		return false;
    }
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Resize
//-----------------------------------------------------------------------------
void OpenGLManager::resize(int width, int height, int bitDepth, int refreshRate)
{
#if 0
	dispose();
	initialize(m_hWnd, GetWindowDC(m_hWnd), m_fullscreen, width, height, bitDepth, refreshRate, true, false);
#endif
}

//-----------------------------------------------------------------------------
// Toggle Fullscreen
//-----------------------------------------------------------------------------
bool OpenGLManager::toggleFullscreen()
{
#if 0
	dispose();
	return initialize(m_hWnd, GetWindowDC(m_hWnd), !m_fullscreen, m_width, m_height, m_bitDepth, m_refreshRate,  true, !m_fullscreen);
#endif
}

//-----------------------------------------------------------------------------
//* Start Rendering 
//! Should be called before you render everything with OpenGL
//-----------------------------------------------------------------------------
void OpenGLManager::beginRendering()
{
	glDepthMask( true );
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
//* End Rendering
//! Should be called after you have rendered everything with OpenGL
//-----------------------------------------------------------------------------
void OpenGLManager::endRendering()
{
#if 0
    //glFlush();
    if ( !SwapBuffers(m_currentDC) ) {

        LPVOID lpMsgBuf;
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       0,
                       GetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                       (LPTSTR) &lpMsgBuf,
                       0,
                       0);

        //MessageBox(0, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );

        LocalFree( lpMsgBuf );
    }
#endif
}

//-----------------------------------------------------------------------------
// Set Wireframe
//-----------------------------------------------------------------------------
void OpenGLManager::setWireFrame(bool wireframe)
{
	m_wireframe = wireframe;
	if ( wireframe )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}

//-----------------------------------------------------------------------------
// Set ZBuffer Enabled
//-----------------------------------------------------------------------------
void OpenGLManager::setZBufferEnabled(bool enable)
{
	if ( enable ) 
	{
		glEnable( GL_DEPTH_TEST );
	}
	else 
	{
		glDisable( GL_DEPTH_TEST );
	}
}

//-----------------------------------------------------------------------------
// Get ZBuffer Enabled
//-----------------------------------------------------------------------------
bool OpenGLManager::getZBufferEnabled()
{
    return (glIsEnabled(GL_DEPTH_TEST) == GL_TRUE);
}

//-----------------------------------------------------------------------------
// Set Lighting
//-----------------------------------------------------------------------------
void OpenGLManager::setLighting(bool lighting)
{
	m_lighting = lighting;
    if ( lighting ) {
		//glEnable(GL_LIGHTING);  We dont use this type of lighting (Nintendo 64 specific)
    }
    else {
		glDisable(GL_LIGHTING);
    }
}

//-----------------------------------------------------------------------------
// Get Lighting
//-----------------------------------------------------------------------------
bool OpenGLManager::getLightingEnabled()
{
    return m_lighting;
    //return (glIsEnabled(GL_LIGHTING) == GL_TRUE);  We dont use this type of lighting (Nintendo 64 specific)
}

//-----------------------------------------------------------------------------
// Set Fog
//-----------------------------------------------------------------------------
void OpenGLManager::setFogEnabled(bool fog)
{
	if ( fog ) 
		glEnable(GL_FOG);
	else 
		glDisable(GL_FOG);
}

//-----------------------------------------------------------------------------
// Get Fog
//-----------------------------------------------------------------------------
bool OpenGLManager::getFogEnabled()
{
	return (glIsEnabled(GL_FOG) == GL_TRUE);
}

//-----------------------------------------------------------------------------
// Set Texturing
//-----------------------------------------------------------------------------
void OpenGLManager::setTextureing2D(bool textureing)
{
	if ( textureing ) 
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------------
// Get Texturing
//-----------------------------------------------------------------------------
bool getTextureing2DEnabled()
{
    return (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE);
}

//-----------------------------------------------------------------------------
// Set Alpha Test Enabled
//-----------------------------------------------------------------------------
void OpenGLManager::setAlphaTest(bool alphaTestEnable)
{
	if ( alphaTestEnable )
		glEnable(GL_ALPHA_TEST);
	else
		glDisable(GL_ALPHA_TEST);
}

//-----------------------------------------------------------------------------
// Get Alpha Test Enabled
//-----------------------------------------------------------------------------
bool getAlphaTestEnabled()
{
    return (glIsEnabled(GL_ALPHA_TEST) == GL_TRUE);
}

//-----------------------------------------------------------------------------
// Scissor
//-----------------------------------------------------------------------------
void OpenGLManager::setScissorEnabled(bool enable)
{
	if ( enable )
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

bool OpenGLManager::getScissorEnabled()
{
	return (glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE); 
}

//-----------------------------------------------------------------------------
// Set Cull Mode
//-----------------------------------------------------------------------------
void OpenGLManager::setCullMode(bool cullFront, bool cullBack)
{
	if( cullFront && cullBack )
	{
		glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);		
	}
	else if( cullFront )
	{
		glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);		
	}
	else if( cullBack )
	{
		glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);		
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	//Override Face Culling?
	if ( m_forceDisableCulling )
	{		
		glDisable(GL_CULL_FACE);
	}
}

//-----------------------------------------------------------------------------
//* Dispose
//! Restores old display settings and destroys the rendering context
//-----------------------------------------------------------------------------
void OpenGLManager::dispose()
{
#if 0
	//Restore older settings
	ChangeDisplaySettings( &m_oldDevMode, 0 );

    //Delete Render Context
	if( m_rc != 0 )
	{		
		//makeCurrent(m_dc, m_rc);
		//wglMakeCurrent(0, 0);
		wglMakeCurrent(0,0);
		wglDeleteContext(m_rc);
		m_rc = 0;        
	}

	//Release Device Context
	if ( m_currentDC )
	{
		ReleaseDC(m_hWnd, m_currentDC);
		m_currentDC = 0;
	}
#endif
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
OpenGLManager::~OpenGLManager()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Set Pixel Format
//! Makes a device context combatible with OpenGL
//! @param[in] dc The device context which will recive a pixel format.
//-----------------------------------------------------------------------------
bool OpenGLManager::_setPixelFormat(HDC dc)
{
#if 0
	//
	BYTE   colorBits      = 32;
	BYTE   depthBits      = 24;
	DWORD  flags          = PFD_DRAW_TO_WINDOW |     // support window
		                    PFD_SUPPORT_OPENGL |     // support OpenGL
		                    //PFD_GENERIC_ACCELERATED | PFD_SWAP_COPY | PFD_SWAP_EXCHANGE |
		                    PFD_DOUBLEBUFFER |        // double buffered
		                    PFD_TYPE_RGBA;

    //Create a Pixel Format   
    PIXELFORMATDESCRIPTOR pfd = { 
        sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
        1,                                // version number 
        flags,                            // flags
        PFD_TYPE_RGBA,                    // RGBA type 
        colorBits,                        // color depth 
        0, 0, 0, 0, 0, 0,                 // color bits ignored 
        0,                                // no alpha buffer 
        0,                                // shift bit ignored 
        0,                                // no accumulation buffer 
        0, 0, 0, 0,                       // accum bits ignored 
        depthBits,                        // z-buffer 
        0,                                // no stencil buffer 
        0,                                // no auxiliary buffer 
        PFD_MAIN_PLANE,                   // main layer 
        0,                                // reserved 
        0, 0, 0                           // layer masks ignored 
    }; 

    //Choose Pixel Format
    int format;
    if ( (format = ChoosePixelFormat(dc, &pfd))  == 0 )
    {
        return false;
    }

    //Set Pixel Format
    if ( !SetPixelFormat(dc, format, &pfd) )
    {
        return false;
    }

	//! @todo Study this function
    DescribePixelFormat(dc, format, sizeof(pfd), &pfd);
#endif
	return true;
}

//-----------------------------------------------------------------------------
//! 2D coordinats are in proportion to N64 viewport (vi), but we use
//! a viewport of another size, there for we need to scale the coordinats.
//! This function calculates that scale.
//! @param viWidth The videointerface width that defines the n64 resolution
//! @param viHeight The videointerface height that defines the n64 resolution
//-----------------------------------------------------------------------------
void OpenGLManager::calcViewScale(int viWidth, int viHeight)
{
	m_scaleX = m_width / (float)viWidth;
	m_scaleY = m_height / (float)viHeight;
}

