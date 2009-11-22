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

#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_


//*****************************************************************************
//* FrameBuffer
//! A Texture that we can render to.
//*****************************************************************************
class FrameBuffer
{
public:

    //Get Singleton
    //static FrameBuffer& getSingleton() {
    //    static FrameBuffer instance;
    //    return instance;
    //}

    //Constructor
    FrameBuffer();

    //Destructor
    ~FrameBuffer();

    void initialize(int width, int height);
    void dispose();
    void resize(int width, int height);
    void beginRendering();
    void endRendering();
    void render();
    void render2();

protected:

    void _activate();
    void _deactivate();

private:



protected:

    unsigned int m_id;       //!< TextureID, Used mainly for OpenGL (so it can keep track of textures)
    int m_width;             //!< Width of framebuffer
    int m_height;            //!< Height of framebuffer 

    int m_oldViewport[4];


};

#endif
