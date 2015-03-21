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

#ifndef IMAGE_FORMAT_SELECTOR_H_
#define IMAGE_FORMAT_SELECTOR_H_

//Function pointer for image decoding
typedef unsigned int (*GetTexelFunc)( unsigned long long *src, unsigned short x, unsigned short i, unsigned char palette );

//Forward declarations
class CachedTexture;

//*****************************************************************************
//* ImageFormat
//! Struct used when diffining image formats and how to convert/decode/interperite them.
//*****************************************************************************
struct ImageFormat
{
    GetTexelFunc    Get16;
    unsigned int    glType16;
    int             glInternalFormat16;
    GetTexelFunc    Get32;
    unsigned int    glType32;
    int             glInternalFormat32;
    unsigned int    autoFormat, lineShift, maxTexels;
};

//*****************************************************************************
//* ImageFormatSelector
//! Class for selecting image format decoding functions depending on image format. 
//*****************************************************************************
class ImageFormatSelector
{
public:

    //Constructor / Destructor
    ImageFormatSelector();
    ~ImageFormatSelector();

    //Detect image format
    void detectImageFormat(CachedTexture* texture, unsigned int textureBitDepth, GetTexelFunc& getTexelFunc, unsigned int& internalFormat, int& imageType, unsigned int textureLUT);

public:

    //Static variables
    static ImageFormat imageFormats[4][5];    //!< Defines how to decode diffrent formats

};

#endif
