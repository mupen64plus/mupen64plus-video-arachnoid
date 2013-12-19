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

#include "OpenGLManager.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
OpenGLManager::OpenGLManager()
{
    m_forceDisableCulling = false;
}

//-----------------------------------------------------------------------------
//* Initialize 
//! Initializes OpenGL. 
//
//! @param fullscreen   will render scene in fullscreen if true
//! @param width        width of window or width of screen resolution
//! @param height       height of window or height of screen resolution
//! @param bitDepth     bitDepth to use
//! @param refreshRate  refresh frequency to use
//! @param vSync        limits frame rate to the monitor's refresh frequency
//! @param hideCursor   hides mouse coursor if true
//-----------------------------------------------------------------------------
bool OpenGLManager::initialize(bool fullscreen, int width, int height, int bitDepth, int refreshRate, bool vSync, bool hideCursor)
{
    m_width       = width;
    m_height      = height;
    m_bitDepth    = bitDepth;
    m_refreshRate = refreshRate;
    m_fullscreen  = fullscreen;
    m_renderingCallback = NULL;
    //Set OpenGL Settings
    setClearColor(0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Set render states
    setCullMode(false, true);
    setTextureing2D(false);
    setLighting(false);

    return true;
}

//-----------------------------------------------------------------------------
// Set Viewport
//-----------------------------------------------------------------------------
void OpenGLManager::setViewport( int x, int y, int width, int height, float zNear, float zFar )
{
    glViewport(x, y, width, height); 

    //glViewport( gSP.viewport.x * OGL.scaleX, 
    //           (VI.height - (gSP.viewport.y + gSP.viewport.height)) * OGL.scaleY + OGL.heightOffset, 
    //            gSP.viewport.width * OGL.scaleX, 
    //            gSP.viewport.height * OGL.scaleY 
    //         ); 

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
// Resize
//-----------------------------------------------------------------------------
void OpenGLManager::resize(int width, int height, int bitDepth, int refreshRate)
{
#if 0
    dispose();
    initialize(m_fullscreen, width, height, bitDepth, refreshRate, true, false);
#endif
}

//-----------------------------------------------------------------------------
// Toggle Fullscreen
//-----------------------------------------------------------------------------
bool OpenGLManager::toggleFullscreen()
{
#if 0
    dispose();
    return initialize(!m_fullscreen, m_width, m_height, m_bitDepth, m_refreshRate,  true, !m_fullscreen);
#endif
    return false;
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
    glFinish();
    if (m_renderingCallback)
        m_renderingCallback(m_drawFlag);
	m_drawFlag = 0;
    CoreVideo_GL_SwapBuffers();
    //glFlush();
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
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
OpenGLManager::~OpenGLManager()
{
    dispose();
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
