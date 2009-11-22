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

#ifndef CYCLIC_REDUNDANCY_CHECK_CALCULATOR_H_
#define CYCLIC_REDUNDANCY_CHECK_CALCULATOR_H_

//*****************************************************************************
//! Cyclic Redundancy Check Calculator 
//! CRC is a type of hash function which is used to produce a small, 
//! fixed-size checksum of a larger block of data.
//!
//! Often used in networks for reducing errors caused by noise.
//!
//! http://en.wikipedia.org/wiki/CRC32
//! http://en.wikipedia.org/wiki/Hash_function
//! http://en.wikipedia.org/wiki/Hash_table
//! http://www.gamedev.net/reference/articles/article1941.asp
//! http://www.codeproject.com/cpp/crc32_large.asp
//*****************************************************************************
class CRCCalculator
{
public:

    //Constructor
    CRCCalculator();

    //Functions for calculating crc values
    unsigned int calcCRC(unsigned int crc, void *buffer, unsigned int count);
    unsigned int calcPaletteCRC(unsigned int crc, void *buffer, unsigned int count);

private:

    //Help function used to build hash table
    unsigned int _reflect(unsigned int ref, char ch);

private:   

    static unsigned int m_crcTable[256];   //!< Hash table that associates keys with values
};

#endif
