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

#include "m64p.h"

#include <cmath>
#include <algorithm>
#include "OpenGLRenderer.h"
#include "OpenGLManager.h"
#include "RSP.h"
#include "RDP.h"
#include "TextureCache.h"
#include "VI.h"
#include "Logger.h"
//#include "CombinerManager.h"
#include "AdvancedCombinerManager.h"
#include "FogManager.h"
#include "ExtensionChecker.h"
#include "MultiTexturingExt.h"
#include "SecondaryColorExt.h"


using std::max;

#ifndef GL_CLAMP_TO_EDGE
    #define GL_CLAMP_TO_EDGE  0x812F
#endif

bool ARB_multitexture = false;
bool EXT_secondary_color = false;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
OpenGLRenderer::OpenGLRenderer()
{
    m_numVertices = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
OpenGLRenderer::~OpenGLRenderer()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointers and setup render OpenGl pointers to vertex data.
//-----------------------------------------------------------------------------
bool OpenGLRenderer::initialize(RSP* rsp, RDP* rdp, TextureCache* textureCache, VI* vi, FogManager* fogMgr)
{
    m_rsp          = rsp;
    m_rdp          = rdp;
    m_textureCache = textureCache;
    m_vi           = vi;
    m_fogMgr       = fogMgr;

    m_numVertices  = 0;
    m_numTriangles = 0;

    //Init multitexturing
    ARB_multitexture    = initializeMultiTexturingExtensions();
    EXT_secondary_color = initializeSecondaryColorExtension();

    //Vertices
    glVertexPointer(4, GL_FLOAT, sizeof(GLVertex), &m_vertices[0].x );
    glEnableClientState( GL_VERTEX_ARRAY );

    //Colors
    glColorPointer(4, GL_FLOAT, sizeof(GLVertex), &m_vertices[0].color.r);
    glEnableClientState( GL_COLOR_ARRAY );

    //Secondary Color
    if ( EXT_secondary_color)
    {
        glSecondaryColorPointerEXT( 3, GL_FLOAT, sizeof( GLVertex ), &m_vertices[0].secondaryColor.r );
        glEnableClientState( GL_SECONDARY_COLOR_ARRAY_EXT );
    }

    //Textureing 0
    glClientActiveTextureARB( GL_TEXTURE0_ARB ); 
    glTexCoordPointer( 2, GL_FLOAT, sizeof( GLVertex ), &m_vertices[0].s0 );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Textureing 1
    glClientActiveTextureARB( GL_TEXTURE1_ARB );
    glTexCoordPointer( 2, GL_FLOAT, sizeof( GLVertex ), &m_vertices[0].s1 );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Fog
    m_fogMgr->setFogCoordPointer(GL_FLOAT, sizeof(GLVertex), &m_vertices[0].fog);
    m_fogMgr->enableFogCoordArray();
    m_fogMgr->setLinearFog();

    return true;
}



//-----------------------------------------------------------------------------
// Add triangle
//-----------------------------------------------------------------------------
void OpenGLRenderer::addTriangle( SPVertex *vertices, int v0, int v1, int v2 )
{
    int v[] = { v0, v1, v2 };

    //Update States
    m_rdp->updateStates();

    //For each vertex in triangle
    for (int i=0; i<3; ++i)
    {
        //Set Vertex
        m_vertices[m_numVertices].x = vertices[v[i]].x;
        m_vertices[m_numVertices].y = vertices[v[i]].y;
        m_vertices[m_numVertices].z = m_rdp->getDepthSource() == G_ZS_PRIM ? m_rdp->getPrimitiveZ() * vertices[v[i]].w : vertices[v[i]].z; 
        m_vertices[m_numVertices].w = vertices[v[i]].w;

        //Set Color
        m_vertices[m_numVertices].color.r = vertices[v[i]].r;
        m_vertices[m_numVertices].color.g = vertices[v[i]].g;
        m_vertices[m_numVertices].color.b = vertices[v[i]].b;
        m_vertices[m_numVertices].color.a = vertices[v[i]].a;
        m_rdp->getCombinerMgr()->getCombinerColor( &m_vertices[m_numVertices].color.r );

        if ( EXT_secondary_color )
        {
            m_vertices[m_numVertices].secondaryColor.r = 0.0f;//lod_fraction; //vertices[v[i]].r;
            m_vertices[m_numVertices].secondaryColor.g = 0.0f;//lod_fraction; //vertices[v[i]].g;
            m_vertices[m_numVertices].secondaryColor.b = 0.0f;//lod_fraction; //vertices[v[i]].b;
            m_vertices[m_numVertices].secondaryColor.a = 1.0f;
            m_rdp->getCombinerMgr()->getSecondaryCombinerColor( &m_vertices[m_numVertices].secondaryColor.r );
        }

        //Set Fog
        if ( OpenGLManager::getSingleton().getFogEnabled()  )
        {
            if (vertices[v[i]].z < -vertices[v[i]].w)
            {
                m_vertices[m_numVertices].fog = max(0.0f, -m_fogMgr->getMultiplier() + m_fogMgr->getOffset() );
            }
            else
            {
                m_vertices[m_numVertices].fog = max(0.0f, vertices[v[i]].z / vertices[v[i]].w * m_fogMgr->getMultiplier() + m_fogMgr->getOffset());
            }
        }

        //Set TexCoords
        if ( m_rdp->getCombinerMgr()->getUsesTexture0()  )
        {
            RSPTexture& rspTexture      = m_rsp->getTexture();
            CachedTexture* cacheTexture = m_textureCache->getCurrentTexture(0);
            RDPTile* rspTile            = m_rsp->getTile(0);            
            if ( cacheTexture ) 
            {
                m_vertices[m_numVertices].s0 = (vertices[v[i]].s * cacheTexture->shiftScaleS * rspTexture.scaleS  - rspTile->fuls + cacheTexture->offsetS) * cacheTexture->scaleS; 
                m_vertices[m_numVertices].t0 = (vertices[v[i]].t * cacheTexture->shiftScaleT * rspTexture.scaleT  - rspTile->fult + cacheTexture->offsetT) * cacheTexture->scaleT;
            }
            else
            {
                m_vertices[m_numVertices].s0 = (vertices[v[i]].s * rspTexture.scaleS  - rspTile->fuls ); 
                m_vertices[m_numVertices].t0 = (vertices[v[i]].t * rspTexture.scaleT  - rspTile->fult );
            }            
        }

        if ( m_rdp->getCombinerMgr()->getUsesTexture1() )
        {
            RSPTexture& rspTexture      = m_rsp->getTexture();
            CachedTexture* cache = m_textureCache->getCurrentTexture(1);
            RDPTile* rspTile            = m_rsp->getTile(1);    
            if ( cache && rspTile ) 
            {
                m_vertices[m_numVertices].s1 = (vertices[v[i]].s * cache->shiftScaleS * rspTexture.scaleS - rspTile->fuls + cache->offsetS) * cache->scaleS; 
                m_vertices[m_numVertices].t1 = (vertices[v[i]].t * cache->shiftScaleT * rspTexture.scaleT - rspTile->fult + cache->offsetT) * cache->scaleT;    
            }
        }

        m_numVertices++;
    }
    m_numTriangles++;

    if ( m_numVertices >= 255 ) 
    {    
        
        Logger::getSingleton().printMsg("RENDER VERTICES!!! :)", M64MSG_ERROR);
        OpenGLRenderer::getSingleton().render();
    }    
}

//-----------------------------------------------------------------------------
// Render
//-----------------------------------------------------------------------------
void OpenGLRenderer::render()
{        
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    m_numTriangles = m_numVertices = 0;  
}

//-----------------------------------------------------------------------------
// Render Texture Rectangle
//-----------------------------------------------------------------------------
void OpenGLRenderer::renderTexRect( float ulx, float uly,   //Upper left vertex
                                    float lrx, float lry,   //Lower right vertex
                                    float uls, float ult,   //Upper left texcoord
                                    float lrs, float lrt,   //Lower right texcoord 
                                    bool flip)              //Flip
{
    //Initialize first vertex (upper left vertex)
    GLVertex rect[2];
    rect[0].x = ulx;
    rect[0].y = uly;
    rect[0].z = m_rdp->getDepthSource() == 1 ? m_rdp->getPrimitiveZ() : 0.0f;   //FIXME: Use viewport.nearz?
    rect[0].w = 1.0f;
    rect[0].color.r = 1.0f;   
    rect[0].color.g = 1.0f;
    rect[0].color.b = 1.0f;
    rect[0].color.a = 0.0f;
    rect[0].secondaryColor.r = 1.0f;   
    rect[0].secondaryColor.g = 1.0f;
    rect[0].secondaryColor.b = 1.0f;
    rect[0].secondaryColor.a = 1.0f;
    rect[0].s0 = uls;
    rect[0].t0 = ult;
    rect[0].s1 = uls;
    rect[0].t1 = ult;
    rect[0].fog = 0.0f;

    //Initialize second vertex (lower right vertex)
    rect[1].x = lrx;
    rect[1].y = lry;
    rect[1].z = m_rdp->getDepthSource() == 1 ? m_rdp->getPrimitiveZ() : 0.0f;   //FIXME: Use viewport.nearz? 
    rect[1].w = 1.0f;
    rect[1].color.r = 1.0f;   
    rect[1].color.g = 1.0f;
    rect[1].color.b = 1.0f;
    rect[1].color.a = 0.0f;
    rect[1].secondaryColor.r = 1.0f;   
    rect[1].secondaryColor.g = 1.0f;
    rect[1].secondaryColor.b = 1.0f;
    rect[1].secondaryColor.a = 1.0f;
    rect[1].s0 = lrs;
    rect[1].t0 = lrt;
    rect[1].s1 = lrs;
    rect[1].t1 = lrt;
    rect[1].fog = 0.0f;

    glDisable( GL_CULL_FACE );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //glOrtho( 0, m_vi->getWidth(), m_vi->getHeight(), 0, 1.0f, -1.0f );
    glOrtho( 0, m_vi->getWidth(), m_vi->getHeight(), 0, 1.0f, -1.0f );
    //glOrtho( 0, OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight(), 0, 1.0f, -1.0f );
    //glViewport( 0, 0, m_vi->getWidth(), m_vi->getHeight() );
    //glViewport( 0, 0, 320, 240 );
    //glViewport( 0, 0, OpenGLManager::getSingleton().getWidth(), OpenGLManager::getSingleton().getHeight());

    if ( m_rdp->getCombinerMgr()->getUsesTexture0() )
    {
        rect[0].s0 = rect[0].s0 * m_textureCache->getCurrentTexture(0)->shiftScaleS - m_rsp->getTile(0)->fuls;
        rect[0].t0 = rect[0].t0 * m_textureCache->getCurrentTexture(0)->shiftScaleT - m_rsp->getTile(0)->fult;
        rect[1].s0 = (rect[1].s0 + 1.0f) * m_textureCache->getCurrentTexture(0)->shiftScaleS - m_rsp->getTile(0)->fuls;
        rect[1].t0 = (rect[1].t0 + 1.0f) * m_textureCache->getCurrentTexture(0)->shiftScaleT - m_rsp->getTile(0)->fult;

        if ((m_textureCache->getCurrentTexture(0)->maskS) && (fmod( rect[0].s0, m_textureCache->getCurrentTexture(0)->width ) == 0.0f) && !(m_textureCache->getCurrentTexture(0)->mirrorS))
        {
            rect[1].s0 -= rect[0].s0;
            rect[0].s0 = 0.0f;
        }

        if ((m_textureCache->getCurrentTexture(0)->maskT) && (fmod( rect[0].t0, m_textureCache->getCurrentTexture(0)->height ) == 0.0f) && !(m_textureCache->getCurrentTexture(0)->mirrorT))
        {
            rect[1].t0 -= rect[0].t0;
            rect[0].t0 = 0.0f;
        }
//
//        if (OGL.ARB_multitexture)
            glActiveTextureARB( GL_TEXTURE0_ARB );
//
        if ((rect[0].s0 >= 0.0f) && (rect[1].s0 <= m_textureCache->getCurrentTexture(0)->width))
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

        if ((rect[0].t0 >= 0.0f) && (rect[1].t0 <= m_textureCache->getCurrentTexture(0)->height))
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

//
        rect[0].s0 *= m_textureCache->getCurrentTexture(0)->scaleS;
        rect[0].t0 *= m_textureCache->getCurrentTexture(0)->scaleT;
        rect[1].s0 *= m_textureCache->getCurrentTexture(0)->scaleS;
        rect[1].t0 *= m_textureCache->getCurrentTexture(0)->scaleT;
    }

    if ( m_rdp->getCombinerMgr()->getUsesTexture1() )
    {
        rect[0].s1 = rect[0].s1 * m_textureCache->getCurrentTexture(1)->shiftScaleS - m_rsp->getTile(1)->fuls;
        rect[0].t1 = rect[0].t1 * m_textureCache->getCurrentTexture(1)->shiftScaleT - m_rsp->getTile(1)->fult;
        rect[1].s1 = (rect[1].s1 + 1.0f) * m_textureCache->getCurrentTexture(1)->shiftScaleS - m_rsp->getTile(1)->fuls;
        rect[1].t1 = (rect[1].t1 + 1.0f) * m_textureCache->getCurrentTexture(1)->shiftScaleT - m_rsp->getTile(1)->fult;

        if ((m_textureCache->getCurrentTexture(1)->maskS) && (fmod( rect[0].s1, m_textureCache->getCurrentTexture(1)->width ) == 0.0f) && !(m_textureCache->getCurrentTexture(1)->mirrorS))
        {
            rect[1].s1 -= rect[0].s1;
            rect[0].s1 = 0.0f;
        }

        if ((m_textureCache->getCurrentTexture(1)->maskT) && (fmod( rect[0].t1, m_textureCache->getCurrentTexture(1)->height ) == 0.0f) && !(m_textureCache->getCurrentTexture(1)->mirrorT))
        {
            rect[1].t1 -= rect[0].t1;
            rect[0].t1 = 0.0f;
        }

        glActiveTextureARB( GL_TEXTURE1_ARB );

        if ((rect[0].s1 == 0.0f) && (rect[1].s1 <= m_textureCache->getCurrentTexture(1)->width))
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

        if ((rect[0].t1 == 0.0f) && (rect[1].t1 <= m_textureCache->getCurrentTexture(1)->height))
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        rect[0].s1 *= m_textureCache->getCurrentTexture(1)->scaleS;
        rect[0].t1 *= m_textureCache->getCurrentTexture(1)->scaleT;
        rect[1].s1 *= m_textureCache->getCurrentTexture(1)->scaleS;
        rect[1].t1 *= m_textureCache->getCurrentTexture(1)->scaleT;
    }


    if ( m_rdp->m_otherMode.cycleType == G_CYC_COPY ) /*&& !OGL.forceBilinear  )*/
    {
        //if (OGL.ARB_multitexture)
        glActiveTextureARB( GL_TEXTURE0_ARB );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }

//    SetConstant( rect[0].color, combiner.vertex.color, combiner.vertex.alpha );
    //rdp.updateStates();
    m_rdp->getCombinerMgr()->getCombinerColor(&rect[0].color.r);

    //if (OGL.EXT_secondary_color)
    m_rdp->getCombinerMgr()->getSecondaryCombinerColor(&rect[0].secondaryColor.r);
    //    SetConstant( rect[0].secondaryColor, combiner.vertex.secondaryColor, combiner.vertex.alpha );

    glBegin( GL_QUADS );

        glColor4f( rect[0].color.r, rect[0].color.g, rect[0].color.b, rect[0].color.a );

        //if (OGL.EXT_secondary_color)
            glSecondaryColor3fEXT( rect[0].secondaryColor.r, rect[0].secondaryColor.g, rect[0].secondaryColor.b );

        //if (OGL.ARB_multitexture)
        //{
        //    glMultiTexCoord2fARB( GL_TEXTURE0_ARB, rect[0].s0, rect[0].t0 );
        //    glMultiTexCoord2fARB( GL_TEXTURE1_ARB, rect[0].s1, rect[0].t1 );
        //    glVertex4f( rect[0].x, rect[0].y, rect[0].z, 1.0f );

        //    glMultiTexCoord2fARB( GL_TEXTURE0_ARB, rect[1].s0, rect[0].t0 );
        //    glMultiTexCoord2fARB( GL_TEXTURE1_ARB, rect[1].s1, rect[0].t1 );
        //    glVertex4f( rect[1].x, rect[0].y, rect[0].z, 1.0f );

        //    glMultiTexCoord2fARB( GL_TEXTURE0_ARB, rect[1].s0, rect[1].t0 );
        //    glMultiTexCoord2fARB( GL_TEXTURE1_ARB, rect[1].s1, rect[1].t1 );
        //    glVertex4f( rect[1].x, rect[1].y, rect[0].z, 1.0f );

        //    glMultiTexCoord2fARB( GL_TEXTURE0_ARB, rect[0].s0, rect[1].t0 );
        //    glMultiTexCoord2fARB( GL_TEXTURE1_ARB, rect[0].s1, rect[1].t1 );
        //    glVertex4f( rect[0].x, rect[1].y, rect[0].z, 1.0f );
        //}
//        else
        {

            /*
            Logger::getSingleton() << "\n\nTexRect x0=" << rect[0].x << " y0=" << rect[0].y << 
                                                 " x1=" << rect[1].x <<  "y1=" << rect[1].y <<
                                                    " t0u0=" << rect[0].s0 << " t0v0=" << rect[0].t0 << 
                                                 " t0u1=" << rect[1].s0 << " t0v1=" << rect[1].t0 << "\n";
            */

            glTexCoord2f(rect[0].s0, rect[0].t0 );
            //glTexCoord2f(rect[0].s1, rect[0].t1 );
            glVertex4f( rect[0].x, rect[0].y, rect[0].z, 1.0f );

            glTexCoord2f(rect[1].s0, rect[0].t0 );
            //glTexCoord2f(rect[1].s1, rect[0].t1 );
            glVertex4f( rect[1].x, rect[0].y, rect[0].z, 1.0f );

            glTexCoord2f(rect[1].s0, rect[1].t0 );
            //glTexCoord2f(rect[1].s1, rect[1].t1 );
            glVertex4f( rect[1].x, rect[1].y, rect[0].z, 1.0f );

            glTexCoord2f(rect[0].s0, rect[1].t0 );
            //glTexCoord2f(rect[0].s1, rect[1].t1 );
            glVertex4f( rect[0].x, rect[1].y, rect[0].z, 1.0f );



    /*        glTexCoord2f( rect[0].s0, rect[0].t0 );
            glVertex4f( rect[0].x, rect[0].y, rect[0].z, 1.0f );

            if (flip)
                glTexCoord2f( rect[1].s0, rect[0].t0 );
            else
                glTexCoord2f( rect[0].s0, rect[1].t0 );

            glVertex4f( rect[1].x, rect[0].y, rect[0].z, 1.0f );

            glTexCoord2f( rect[1].s0, rect[1].t0 );
            glVertex4f( rect[1].x, rect[1].y, rect[0].z, 1.0f );

            if (flip)
                glTexCoord2f( rect[1].s0, rect[0].t0 );
            else
                glTexCoord2f( rect[1].s0, rect[0].t0 );
            glVertex4f( rect[0].x, rect[1].y, rect[0].z, 1.0f );*/
        }
    glEnd();

    glLoadIdentity();
    //OGL_UpdateCullFace();
    //OGL_UpdateViewport();
}
