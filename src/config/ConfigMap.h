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

#ifndef CONFIG_MAP_H_
#define CONFIG_MAP_H_

//*****************************************************************************
//! Struct with all configuration settings are stored
//*****************************************************************************
struct ConfigMap
{
    //Configuration Settings
    int  fullscreenWidth;        //!< Display width in fullscreen mode,             default = 800
    int  fullscreenHeight;       //!< Display height in fullscreen mode,            default = 600
    int  fullscreenRefreshRate;  //!< Screen refresh-rate in fullscreen mode,       default = 60
    int  fullscreenBitDepth;     //!< Bitdepth in fullscreen mode,                  default = 32
    bool startFullscreen;        //!< Whether to start in fullscreen mode,          default = false
    int  windowWidth;            //!< Width of render window in window mode,        default = 800
    int  windowHeight;           //!< Height of render window in window mode,       default = 600    
    int  textureCacheSize;       //!< Size of texture cache used to store textures, default = 15 MB
    bool wireframe;              //!< Render in wireframe?                          defualt = false
    bool fog;                    //!< Render fog?                                   default = true
    int  screenUpdateSetting;    //!< When to redraw the screen                     default = SCREEN_UPDATE_VI
};

#endif
