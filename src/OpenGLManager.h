
#ifndef OPEN_GL_Manager_H_
#define OPEN_GL_Manager_H_

//OpenGL linking and includes 
#pragma comment(lib,"opengl32.lib")  
#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "glaux.lib")

//OpenGL includes
#include <windows.h>
#include <gl/gl.h> 
#include <gl/glu.h>
#include <gl/glaux.h>	

//*****************************************************************************
//* OpenGL Manager Class                                                        
//! Singelton class for initializing OpenGL and contolling OpenGL states. 
//*****************************************************************************
class OpenGLManager
{
public: 

	//Destructor
	~OpenGLManager();

    //Singleton Instance
	static OpenGLManager& getSingleton()
	{
		static OpenGLManager instance;
		return instance;
	}

	//Functions
	bool initialize(HWND hWnd, HDC dc, bool fullscreen, int width, int height, int bitDepth, int refreshRate, bool vSync, bool hideCursor);
    void dispose();
	void resize(int width, int height, int bitDepth, int refreshRate);
    bool toggleFullscreen();
	void beginRendering();
	void endRendering();
    bool makeCurrent(HDC dc);

	//Fog
	void setFogEnabled(bool fog);	
    bool getFogEnabled();

    //Light
	void setLighting(bool lighting);
    bool getLightingEnabled();

    //Textureing
    void setTextureing2D(bool textureing);
    bool getTextureing2DEnabled();

    //Depth Testing
	void setZBufferEnabled(bool enable);
    bool getZBufferEnabled();	

    //Alpha Test
    void setAlphaTest(bool alphaTestEnable);
    bool getAlphaTestEnabled();

    //Wireframe
	void setWireFrame(bool wireframe);	

    //Culling
	void setCullMode(bool cullFront, bool cullBack);
	void setForceDisableCulling(bool force) { m_forceDisableCulling = force; }
         
    //Set Viewport
	void setViewport(int x, int y, int width, int height, float zNear=0.0f, float zFar=1.0f);

    //Set Scissor
	void setScissorEnabled(bool enable);
    bool getScissorEnabled();	
	void setScissor(int x, int y, int width, int height);
 
    //! Sets the backround color of OpenGL viewport 
    void setClearColor(float r, float g, float b) { glClearColor(r, g, b, 1.0f); }

public:

    //N64 Specifics
	void calcViewScale(int viWidth, int viHeight);
	float getViewScaleX() { return m_scaleX; } 
	float getViewScaleY() { return m_scaleY; }

public:

	//Gets
    inline HDC& getCurrentDC() { return m_currentDC; }
    inline HGLRC getRC() { return m_rc; }

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
    bool getFullscreen() { return m_fullscreen; }

private:

	bool _setPixelFormat(HDC dc);

private:

	 //Constructor
	OpenGLManager();          

private:

	HWND m_hWnd;                 //!< Window to render in
	bool m_wireframe;            //!< Wireframe mode enabled?
	bool m_lighting;             //!< Lighting enabled?
	int m_width;                 //!< Display widht
	int m_height;                //!< Display height
    int m_bitDepth;              //!< Fullscreen bitdepth
    int m_refreshRate;           //!< Fullscreen refresh rate 
	float m_scaleX;              //!< DisplayWidth aka WindowWidth / viWidth (n64 specific)
	float m_scaleY;              //!< DisplayHeight aka WindowHeight / viHeight (n64 specific)
	bool m_fullscreen;           //!< Fullscreen mode or window mode?
	bool m_forceDisableCulling;  //!< Culling cant be enabled if this is true

    //Variables	
	HGLRC m_rc;             //!< Render context
    HDC m_currentDC;        //!< Device context     
	DEVMODE m_oldDevMode;   //!< Previus device mode

};

#endif
