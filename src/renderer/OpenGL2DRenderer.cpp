/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
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

#include "OpenGL2DRenderer.h"
#include "VI.h"
#include "m64p.h"
#include "OpenGL.h"

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointer to video interface
//-----------------------------------------------------------------------------
bool OpenGL2DRenderer::initialize(VI* vi)
{
    m_vi = vi;
    return true;
}

//-----------------------------------------------------------------------------
//* Render Quad
//! Renders a 2D rectangle in HUD.
//! @todo Set Viewport
//! @todo Reset Viewport
//-----------------------------------------------------------------------------
void OpenGL2DRenderer::renderQuad( const float color[4], 
                                   float x0, float y0, 
                                   float x1, float y1,            
                                   float depth )
{
    //Get States
    GLboolean scissor = glIsEnabled(GL_SCISSOR_TEST);
    GLboolean cull    = glIsEnabled(GL_CULL_FACE);

    //Set States
    glDisable( GL_SCISSOR_TEST );
    glDisable( GL_CULL_FACE );

    //Set Othographic Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_vi->getWidth(), m_vi->getHeight(), 0, 1.0f, -1.0f);

    //TODO Set Viewport
    //glViewport(0, glheightOffset, glwidth, glheight);
    //glDepthRange( 0.0f, 1.0f );

    //Render Quad
    glColor4fv(color);
    glBegin(GL_QUADS);
        glVertex3f(x0, y0, depth);
        glVertex3f(x1, y0, depth);
        glVertex3f(x1, y1, depth);
        glVertex3f(x0, y1, depth);
    glEnd();

    //Reset Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    //Reset States
    if ( scissor ) glEnable(GL_SCISSOR_TEST);
    if ( cull ) glEnable(GL_CULL_FACE);
    
    //TODO Reset viewport?    
}


//-----------------------------------------------------------------------------
//* Render Textured Quad
//! Renders a textured 2D rectangle in HUD.
//! @todo Set Viewport
//! @todo Reset Viewport
//-----------------------------------------------------------------------------
void OpenGL2DRenderer::renderTexturedQuad( const float color[4], 
                                           const float secondaryColor[4],
                                           float x0, float y0,            
                                           float x1, float y1, 
                                           float depth,
                                           float t0s0, float t0t0,  
                                           float t0s1, float t0t1,
                                           float t1s0, float t1t0, 
                                           float t1s1, float t1t1 )
{
    //Get States
    GLboolean cull = glIsEnabled(GL_CULL_FACE);
    GLboolean fog  = glIsEnabled(GL_FOG);

    //Set States
    glDisable(GL_CULL_FACE);
    glDisable(GL_FOG);

    //Set Orthographic Projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_vi->getWidth(), m_vi->getHeight(), 0, 1.0f, -1.0f);

    //TODO Set Viewport
    //glViewport(0, glheightOffset, glwidth, glheight);
    //glDepthRange( 0.0f, 1.0f );

    //Set Color
    glColor4fv( color );

    //Render Rectangle
    glBegin(GL_QUADS);
    {    
        //Vertex 00
        glTexCoord2f(t0s0, t0t0);    
        glVertex3f(x0, y0, depth);   

        //Vertex 10
        glTexCoord2f(t0s1, t0t0);    
        glVertex3f(x1, y0, depth);

        //Vertex 11
        glTexCoord2f(t0s1, t0t1 );   
        glVertex3f(x1, y1, depth);

        //Vertex 01
        glTexCoord2f(t0s0, t0t1 );   
        glVertex3f(x0, y1, depth);
    }
    glEnd();
    
    //Reset Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    //Reset States
    if ( cull ) glEnable(GL_CULL_FACE);
    if ( fog ) glEnable(GL_FOG);

    //TODO Reset viewport?    
}


//-----------------------------------------------------------------------------
//Render Flipped Textured Quad
//! Renders a flipped textured 2D rectangle in HUD.
//! @todo Set Viewport
//! @todo Reset Viewport
//-----------------------------------------------------------------------------
void OpenGL2DRenderer::renderFlippedTexturedQuad( const float color[4], 
                                const float secondaryColor[4],
                                float x0, float y0,            
                                float x1, float y1,    
                                float depth,
                                float t0s0, float t0t0,  
                                float t0s1, float t0t1,
                                float t1s0, float t1t0, 
                                float t1s1, float t1t1 )
{
    //Get States
    GLboolean cull = glIsEnabled(GL_CULL_FACE);
    GLboolean fog  = glIsEnabled(GL_FOG);

    //Set States
    glDisable(GL_CULL_FACE);
    glDisable(GL_FOG);

    //Set Orthographic Projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_vi->getWidth(), m_vi->getHeight(), 0, 1.0f, -1.0f);

    //TODO
    //glViewport(0, glheightOffset, glwidth, glheight);
    //glDepthRange( 0.0f, 1.0f );

    //Set Color
    glColor4fv( color );

    //Render Rectangle
    glBegin(GL_QUADS);
    {    
        //Vertex 00
        glTexCoord2f(t0s0, t0t0);        //00
        glVertex3f(x0, y0, depth);   

        //Vertex 10
        glTexCoord2f(t0s0, t0t1);        //01 !
        glVertex3f(x1, y0, depth);

        //Vertex 11
        glTexCoord2f(t0s1, t0t1);        //11
        glVertex3f(x1, y1, depth);

        //Vertex 01
        glTexCoord2f(t0s1, t0t0);        //10  !
        glVertex3f(x0, y1, depth);
    }
    glEnd();
    
    //Reset Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    //Reset States
    if ( cull ) glEnable(GL_CULL_FACE);
    if ( fog ) glEnable(GL_FOG);

    //TODO Reset viewport?    
}

