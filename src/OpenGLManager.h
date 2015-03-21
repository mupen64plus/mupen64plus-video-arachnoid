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

#ifndef OPEN_GL_Manager_H_
#define OPEN_GL_Manager_H_

#include "OpenGL.h"
//OpenGL includes
#include "m64p.h"

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
    bool initialize(bool fullscreen, int width, int height, int bitDepth, int refreshRate, bool vSync, bool hideCursor);
    void dispose();
    void resize(int width, int height, int bitDepth, int refreshRate);
    bool toggleFullscreen();
    void beginRendering();
    void endRendering();

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

    //Set callback from the M64P core
    void setRenderingCallback(void(*callback)(int)) { m_renderingCallback = callback; }
	
	//Set draw flag for rendering callback
	void setDrawFlag() { m_drawFlag = 1; }

public:

    //N64 Specifics
    void calcViewScale(int viWidth, int viHeight);
    float getViewScaleX() { return m_scaleX; } 
    float getViewScaleY() { return m_scaleY; }

public:

    //Gets

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    bool getFullscreen() { return m_fullscreen; }

private:

     //Constructor
    OpenGLManager();          

private:

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
    
    void (*m_renderingCallback)(int);  //Rendering callback from the core
	int m_drawFlag;
};

#endif
