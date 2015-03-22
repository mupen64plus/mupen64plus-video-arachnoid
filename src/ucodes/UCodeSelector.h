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

#ifndef UCODE_SELECTOR_H_
#define UCODE_SELECTOR_H_

//Forward declarations
class Memory;

//*****************************************************************************
//* UCode Selector
//! Class for selecting a good ucode for the current game
//*****************************************************************************
class UCodeSelector
{
public:

    //Constructor / Destructor
    UCodeSelector();
    ~UCodeSelector();

    //Initialize
    bool initialize(Memory* memory);

    //Check ucode
    unsigned int checkUCode( unsigned int ucStart, 
                             unsigned int ucDStart, 
                             unsigned int ucSize, 
                             unsigned int ucDSize );

private:

    //Private functions
    bool _extractUCodeString(unsigned int ucDataStart, char out[500]);
    int _detectUCode(unsigned int crcUCodeDataSize, unsigned int crc800, const char ucodeStr[500]);
    int _detectUCodeFromString(const char ucodeStr[500]);

private:

    Memory* m_memory;   //!< Pointer to memory manager

};

#endif
