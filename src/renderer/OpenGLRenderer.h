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

#ifndef OPEN_GL_RENDERER_H_
#define OPEN_GL_RENDERER_H_

#include "OpenGL.h"
#include "m64p.h"

//Forward Declarations
struct SPVertex;
class RSP;
class RDP;
class TextureCache;
class VI;
class FogManager;

#include "MultiTexturingExt.h"


//*****************************************************************************
//* OpenGL Vertex
//! Used in vertex buffer by OpenGLRenderer.
//*****************************************************************************
struct GLVertex
{
    float x, y, z, w;          //!< Vertex position
    struct
    {
        float r, g, b, a;
    } color, secondaryColor;   //!< Color and secondary color
    float s0, t0, s1, t1;      //!< Texture coordinats
    float fog;                 //!< Vertex fog variable
};

//*****************************************************************************
//* OpenGL Renderer
//! Class for rendering using OpenGL
//*****************************************************************************
class OpenGLRenderer
{
public:

    //Get Singleton Instance
    static OpenGLRenderer& getSingleton()
    {
        static OpenGLRenderer instance;
        return instance;
    }

    //Destructor
    ~OpenGLRenderer();

    //Initialize
    bool initialize(RSP* rsp, RDP* rdp, TextureCache* textureCache, VI* vi, FogManager* fogMgr);

    //Flush Vertex buffer
    void render();

    //Add triangle
    void addTriangle( SPVertex *vertices, int v0, int v1, int v2 );

    //Get number of vertices
    int getNumVertices() { return m_numVertices; }

    //Render Tex Rect
    void renderTexRect( float ulx, float uly,   //Upper left vertex
                        float lrx, float lry,   //Lower right vertex
                        float uls, float ult,   //Upper left texcoord
                        float lrs, float lrt,   //Lower right texcoord 
                        bool flip);             //Flip  
    
private:

    //Constructor
    OpenGLRenderer();

private:

    GLVertex m_vertices[256];              //!< Vertex buffer
    unsigned char m_triangles[300][3];     //!< Triangles used to index vertices

    int m_numVertices;                     //!< Number of vertices in vertex buffer
    int m_numTriangles;                    //!< Number of triangles

    RSP* m_rsp;                            //!< Pointer to Reality Signal Processor
    RDP* m_rdp;                            //!< Pointer to Reality Drawing Processor
    VI* m_vi;                              //!< Pointer to Video Interface
    TextureCache* m_textureCache;          //!< Pointer to texture cache
    FogManager* m_fogMgr;                  //!< Pointer to Fog Manager used to render fog. 

};

#endif
