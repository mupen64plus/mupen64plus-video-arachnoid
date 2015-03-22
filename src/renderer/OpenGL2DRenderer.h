/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
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

#ifndef OPEN_GL_2D_RENDERER_H_
#define OPEN_GL_2D_RENDERER_H_

//Forward declarations
class VI;

//*****************************************************************************
//* OpenGL 2D Renderer
//! Class used to render HUD objects.
//! @details Renders 2D quads, textured 2D quads and flipped textures 2D quads.
//*****************************************************************************
class OpenGL2DRenderer
{
public:

    //Initialize
    bool initialize(VI* vi);

    //Render Quad
    void renderQuad( const float color[4], 
                     float x0, float y0, 
                     float x1, float y1,            
                     float depth );


    //Render Textured Quad
    void renderTexturedQuad( const float color[4], 
                             const float secondaryColor[4],
                             float x0, float y0,            
                             float x1, float y1,    
                             float depth,
                             float t0s0, float t0t0,  
                             float t0s1, float t0t1,
                             float t1s0, float t1t0, 
                             float t1s1, float t1t1 );
    
    //Render Flipped Textured Quad
    void renderFlippedTexturedQuad( const float color[4], 
                                    const float secondaryColor[4],
                                    float x0, float y0,            
                                    float x1, float y1,    
                                    float depth,
                                    float t0s0, float t0t0,  
                                    float t0s1, float t0t1,
                                    float t1s0, float t1t0, 
                                    float t1s1, float t1t1 );


private:

    VI* m_vi;  //!< Video interface
 
};

#endif
