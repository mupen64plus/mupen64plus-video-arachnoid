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

#ifndef UCODE_DATA_H_
#define UCODE_DATA_H_

//*****************************************************************************
// UCode Definitions
//*****************************************************************************
#define F3D         0     // Super Mario 64
#define F3DEX       1
#define F3DEX2      2
#define L3D         3
#define L3DEX       4
#define L3DEX2      5
#define S2DEX       6
#define S2DEX2      7
#define F3DPD       8
#define F3DDKR      9
#define F3DWRUS    10
#define NONE       11

//*****************************************************************************
//* UCode Data
//! Used to Identify witch ucode diffrent games use
//*****************************************************************************
struct UcodeData
{
    unsigned int ucode;          //!< ID of ucode
    unsigned int crc_size;       //!< Hash value used to identify ucode
    unsigned int crc_800;        //!< Hash value used to identify ucode
    const char*  ucode_name;     //!< Name used to identify ucode
    bool         non_nearclip;   //!< Does ucode support near clipping?
    bool         reject;         //!< 
};

#endif