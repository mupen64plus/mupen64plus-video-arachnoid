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

#ifndef RDP_UCODE_DEFS_H_
#define RDP_UCODE_DEFS_H_

//! Struct for setting color-, depth- and texture- image
struct RDPUCodeSetImage
{
    unsigned int width:12;        //!< Width   
    unsigned int :7;              //!< Padding
    unsigned int size:2;          //!< Size    (0=4, 1=8, 2=16, or 3=32)
    unsigned int format:3;        //!< Image format (0=RGBA, 1=YUV, 2=CI, 3=IA, 4=I, 5=?, 6=?, 7=?)
    unsigned int cmd:8;           //!< Command
    unsigned int segmentAddress;  //!< Address to register where there is an RDRAM address
};

//! Struct used by RDP to load textures
struct RDPUCodeTileSize
{
    unsigned int t0:12;      //!< t texture coordinat for first vertex
    unsigned int s0:12;      //!< s texture coordinat for first vertex
    unsigned int cmd:8;      //!< Command, usualy LoadBlock, LoadTLUT, LoadTile, or SetTileSize
    unsigned int t1:12;      //!< t texture coordinat for second vertex
    unsigned int s1:12;      //!< s texture coordinat for second vertex  
    unsigned int tile:3;     //!< Tile
    unsigned int padding:5;  //!< Unused Padding 
};

//! Struct used by RDP to set tiles
struct RDPUCodeSetTile 
{
    unsigned int    tmem:9;     //!< Address in Texture Memory
    unsigned int    line:9;
    unsigned int    pad0:1;     //!< Padding
    unsigned int    size:2;
    unsigned int    format:3;   //!< Image format of tile
    unsigned int    cmd:8;      //!< Command, usualy SetTileSize
    unsigned int    shiftS:4;
    unsigned int    maskS:4;
    unsigned int    mirrorS:1;  //!<
    unsigned int    clampS:1;
    unsigned int    shiftT:4;
    unsigned int    maskT:4;
    unsigned int    mirrorT:1;
    unsigned int    clampT:1;
    unsigned int    palette:4;
    unsigned int    tile:3;
    unsigned int    pad1:5;     //!< Padding
};

//! Struct used by RDP to set fog-, blend-, enviroment-, and prim- color (fillcolor is diffrent)
struct RDPUCodeSetColor
{
    unsigned int prim_level:8;      //!< Only used by setPrimColor
    unsigned int prim_min_level:8;  //!< Only used by setPrimColor
    unsigned int pad0:8;            //!< Padding
    unsigned int cmd:8;   //!< Command
    unsigned int a:8;     //!< Alpha (0-255)
    unsigned int b:8;     //!< Blue  (0-255)
    unsigned int g:8;     //!< Green (0-255)
    unsigned int r:8;     //!< Red   (0-255)
};

//! Struct used by RDP to set fill color
struct RDPUCodeSetFillColor
{
    unsigned int pad0:24; //!< Padding
    unsigned int cmd:8;   //!< Command
    unsigned int a:1;     //!< Alpha  (0-1) 
    unsigned int b:5;     //!< Blue   (0-255)
    unsigned int g:5;     //!< Green  (0-255)
    unsigned int r:5;     //!< Red    (0-255)
    unsigned int pad1:16; //!< Padding
};

//! Struct used by RDP to render rectangles
struct RDPUCodeRectangle
{
    unsigned int pad0 : 2;   //!< Padding
    unsigned int y1   : 10;  //!< Y coordinate of second vertex corner
    unsigned int pad1 : 2;   //!< Padding
    unsigned int x1   : 10;  //!< X coordinate of second vertex corner
    unsigned int cmd  : 8;   //!< Command
    unsigned int pad3 : 2;   //!< Padding
    unsigned int y0   : 10;  //!< Y coordinate of first vertex corner
    unsigned int pad4 : 2;   //!< Padding
    unsigned int x0   : 10;  //!< X coordinate of first vertex corner    
    unsigned int pad5 : 8;   //!< Padding
};

//! Struct used by RDP to set scissor
struct RDPUCodeScissor
{
    unsigned int    y0:12;        //!< Y coordinate of second vertex
    unsigned int    x0:12;        //!< X coordinate of second vertex    
    unsigned int    cmd:8;        //!< Command usualy         
    unsigned int    y1:12;        //!< Y coordinate of first vertex    
    unsigned int    x1:12;        //!< X coordinate of first vertex
    unsigned int    mode:2;        //!< 
    unsigned int    pad0:6;        //!< Padding TexRect or TexRectFlip    
};

//! Struct used by RDP to render textured rectangles
struct RDPUCodeTextureRectangle
{
    unsigned int    y1:12;        //!< Y coordinate of second vertex
    unsigned int    x1:12;        //!< X coordinate of second vertex    
    unsigned int    cmd:8;        //!< Command usualy         
    unsigned int    y0:12;        //!< Y coordinate of first vertex    
    unsigned int    x0:12;        //!< X coordinate of first vertex
    unsigned int    tile:3;       //!< Tile descriptor index    
    unsigned int    pad0:5;       //!< Padding TexRect or TexRectFlip            
    unsigned int    t:16;         //!< T texture coord at first vertex
    unsigned int    s:16;         //!< S texture coord at first vertex
    unsigned int    dtdy:16;      //!< Change in T per change in Y    
    unsigned int    dsdx:16;      //!< Change in S per change in X    
};

//! Struct used by RDP to set prim depth
struct RDPUCodeSetPrimDepth
{
    unsigned int pad0:24; //!< Padding
    unsigned int cmd:8;   //!< Command
    unsigned int dz:16;   
    unsigned int z:16;    //!< Depth value
};

#endif
