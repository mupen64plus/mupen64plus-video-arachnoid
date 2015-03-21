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

#include <cstdio>
#include <cstring>
#include <string>

#include "CRCCalculator.h"
#include "Logger.h"
#include "Memory.h"
#include "UCodeIdentificationData.h"
#include "UCodeSelector.h"

#ifndef WIN32
#define strnicmp strncasecmp
#endif

//*****************************************************************************
//* UCode Data
//! Used to Identify witch ucode diffrent games use
//*****************************************************************************
static UcodeData g_UcodeData[] = 
{
//  id, crc_size,    crc_800,     ucode string,
    {0, 0x150c3ce8, 0x150c3ce8, "RSP SW Version: 2.0D, 04-01-96",}, // Super Mario 64
    {4, 0x2b94276f, 0x2b94276f, "RSP SW Version: 2.0D, 04-01-96",}, // Wave Race 64 (v1.0)
    {16,0xb1870454, 0xb1870454, "RSP SW Version: 2.0D, 04-01-96",}, // Star Wars - Shadows of the Empire (v1.0), 
    {0, 0x51671ae4, 0x51671ae4, "RSP SW Version: 2.0D, 04-01-96",}, // Pilot Wings 64, 
    {0, 0x67b5ac55, 0x67b5ac55, "RSP SW Version: 2.0D, 04-01-96",}, // Wibble, 
    {0, 0x64dc8104, 0x64dc8104, "RSP SW Version: 2.0D, 04-01-96",}, // Dark Rift, 
    {0, 0x309f363d, 0x309f363d, "RSP SW Version: 2.0D, 04-01-96",}, // Killer Instinct Gold, 
    {0, 0xfcb57e57, 0xfcb57e57, "RSP SW Version: 2.0D, 04-01-96",}, // Blast Corps, 
    {0, 0xb420f35a, 0xb420f35a, "RSP SW Version: 2.0D, 04-01-96",}, // Blast Corps, 
    {0, 0x6e26c1df, 0x7c98e9c2, "RSP SW Version: 2.0D, 04-01-96",}, 
    {2, 0xc02ac7bc, 0xc02ac7bc, "RSP SW Version: 2.0G, 09-30-96",}, // GoldenEye 007, 
    {0, 0xe5fee3bc, 0xe5fee3bc, "RSP SW Version: 2.0G, 09-30-96",}, // Aero Fighters Assault, 
    {8, 0xe4bb5ad8, 0x80129845, "RSP SW Version: 2.0G, 09-30-96",}, // Puzzle Master 64, 
    {0, 0x72109ec6, 0x72109ec6, "RSP SW Version: 2.0H, 02-12-97",}, // Duke Nukem 64, 
    {0, 0xf24a9a04, 0xf24a9a04, "RSP SW Version: 2.0H, 02-12-97",}, // Tetrisphere, 
    {15,0x700de42e, 0x700de42e, "RSP SW Version: 2.0H, 02-12-97",}, // Wipeout 64 (uses GBI1 too!), 
    {15,0x1b304a74, 0x1b304a74, "RSP SW Version: 2.0H, 02-12-97",}, // Flying Dragon, 
    {15,0xe4bb5ad8, 0xa7b2f704, "RSP SW Version: 2.0H, 02-12-97",}, // Silicon Valley, 
    {15,0xe4bb5ad8, 0x88202781, "RSP SW Version: 2.0H, 02-12-97",}, // Glover, 
    {0, 0xe466b5bd, 0xe466b5bd, "Unknown 0xe466b5bd, 0xe466b5bd",}, // Dark Rift, 
    {9, 0x7064a163, 0x7064a163, "Unknown 0x7064a163, 0x7064a163",}, // Perfect Dark (v1.0), 
    {0, 0x6522df69, 0x71bd078d, "Unknown 0x6522df69, 0x71bd078d",}, // Tetris 
    {0, 0x6522df69, 0x1b0c23a8, "Unknown 0x6522df69, 0x1b0c23a8",}, // Pachinko Nichi 

    // GBI1
    
    {1, 0x45ca328e, 0x45ca328e, "RSP Gfx ucode F3DLX         0.95 Yoshitaka Yasumoto Nintendo.",}, // Mario Kart 64, 
    {1, 0x98e3b909, 0x98e3b909, "RSP Gfx ucode F3DEX         0.95 Yoshitaka Yasumoto Nintendo.",},    // Mario Kart 64
    {1, 0x5d446090, 0x5d446090, "RSP Gfx ucode F3DLP.Rej     0.96 Yoshitaka Yasumoto Nintendo.",0,1}, // Jikkyou J. League Perfect Striker, 
    {1, 0x244f5ca3, 0x244f5ca3, "RSP Gfx ucode F3DEX         1.00 Yoshitaka Yasumoto Nintendo.",}, // F-1 Pole Position 64, 
    {1, 0x6a022585, 0x6a022585, "RSP Gfx ucode F3DEX.NoN     1.00 Yoshitaka Yasumoto Nintendo.",1}, // Turok - The Dinosaur Hunter (v1.0), 
    {1, 0x150706be, 0x150706be, "RSP Gfx ucode F3DLX.NoN     1.00 Yoshitaka Yasumoto Nintendo.",1}, // Extreme-G, 
    {1, 0x503f2c53, 0x503f2c53, "RSP Gfx ucode F3DEX.NoN     1.21 Yoshitaka Yasumoto Nintendo.",1}, // Bomberman 64, 
    {1, 0xc705c37c, 0xc705c37c, "RSP Gfx ucode F3DLX         1.21 Yoshitaka Yasumoto Nintendo.",}, // Fighting Force 64, Wipeout 64
    {1, 0xa2146075, 0xa2146075, "RSP Gfx ucode F3DLX.NoN     1.21 Yoshitaka Yasumoto Nintendo.",1}, // San Francisco Rush - Extreme Racing, 
    {1, 0xb65aa2da, 0xb65aa2da, "RSP Gfx ucode L3DEX         1.21 Yoshitaka Yasumoto Nintendo.",}, // Wipeout 64, 
    {1, 0x0c8e5ec9, 0x0c8e5ec9, "RSP Gfx ucode F3DEX         1.21 Yoshitaka Yasumoto Nintendo.",}, // 
    {1, 0xe30795f2, 0xa53df3c4, "RSP Gfx ucode F3DLP.Rej     1.21 Yoshitaka Yasumoto Nintendo.",0,1},

    {1, 0xaebeda7d, 0xaebeda7d, "RSP Gfx ucode F3DLX.Rej     1.21 Yoshitaka Yasumoto Nintendo.",0,1}, // Jikkyou World Soccer 3, 
    {1, 0x0c8e5ec9, 0x0c8e5ec9, "RSP Gfx ucode F3DEX         1.23 Yoshitaka Yasumoto Nintendo" ,},  // Wave Race 64 (Rev. 2) - Shindou Rumble Edition (JAP) 
    {1, 0xc705c37c, 0xc705c37c, "RSP Gfx ucode F3DLX         1.23 Yoshitaka Yasumoto Nintendo.",}, // GT
    {1, 0x2a61350d, 0x2a61350d, "RSP Gfx ucode F3DLX         1.23 Yoshitaka Yasumoto Nintendo.",}, // Toy Story2
    {1, 0x0c8e5ec9, 0x0c8e5ec9, "RSP Gfx ucode F3DEX         1.23 Yoshitaka Yasumoto Nintendo.",}, // Wave Race 64 Shindou Edition
    {12,0xfc6529aa, 0xfc6529aa, "RSP Gfx ucode F3DEX         1.23 Yoshitaka Yasumoto Nintendo.",}, // Superman - The Animated Series, 
    {1, 0xa56cf996, 0xa56cf996, "RSP Gfx ucode L3DEX         1.23 Yoshitaka Yasumoto Nintendo.",}, // Flying Dragon, 
    {1, 0xcc83b43f, 0xcc83b43f, "RSP Gfx ucode F3DEX.NoN     1.23 Yoshitaka Yasumoto Nintendo.",1}, // AeroGauge, 
    {1, 0xca8927a0, 0xca8927a0, "RSP Gfx ucode F3DLX.Rej     1.23 Yoshitaka Yasumoto Nintendo.",0,1},    // Puzzle Bobble 64, 
    {1, 0x25689c75, 0xbe481ae8, "RSP Gfx ucode F3DLP.Rej     1.23 Yoshitaka Yasumoto Nintendo.",0,1},
    {1, 0xd2d747b7, 0xd2d747b7, "RSP Gfx ucode F3DLX.NoN     1.23 Yoshitaka Yasumoto Nintendo.",1}, // Penny Racers, 
    {1, 0xa849c858, 0x5bd32b5a, "RSP Gfx ucode F3DTEX/A      1.23 Yoshitaka Yasumoto Nintendo.",}, // Tamagotchi 

    {7, 0xecd8b772, 0xecd8b772, "RSP Gfx ucode S2DEX  1.06 Yoshitaka Yasumoto Nintendo.",}, // Yoshi's Story, 
    {7, 0xf59132f5, 0xf59132f5, "RSP Gfx ucode S2DEX  1.07 Yoshitaka Yasumoto Nintendo.",}, // Bakuretsu Muteki Bangaioh, 
    {7, 0x961dd811, 0x961dd811, "RSP Gfx ucode S2DEX  1.03 Yoshitaka Yasumoto Nintendo.",}, // GT

    {5, 0x3e083afa, 0x722f97cc, "RSP Gfx ucode F3DEX.NoN   fifo 2.03  Yoshitaka Yasumoto 1998 Nintendo.",1}, // F-Zero X, 
    {5, 0xa8050bd1, 0xa8050bd1, "RSP Gfx ucode F3DEX       fifo 2.03  Yoshitaka Yasumoto 1998 Nintendo.",}, // F-Zero X, 
    {5, 0x4e8055f0, 0x4e8055f0, "RSP Gfx ucode F3DLX.Rej   fifo 2.03  Yoshitaka Yasumoto 1998 Nintendo.",0,1}, // F-Zero X, 
    {5, 0xabf001f5, 0xabf001f5, "RSP Gfx ucode F3DFLX.Rej  fifo 2.03F Yoshitaka Yasumoto 1998 Nintendo.",0,1}, // F-Zero X, 
    {5, 0xadb4b686, 0xadb4b686, "RSP Gfx ucode F3DEX       fifo 2.04  Yoshitaka Yasumoto 1998 Nintendo.",}, // Top Gear Rally 2, 
    {5, 0x779e2a9b, 0x779e2a9b, "RSP Gfx ucode F3DEX.NoN   fifo 2.04  Yoshitaka Yasumoto 1998 Nintendo.",1}, // California Speed, 
    {5, 0xa8cb3e09, 0xa8cb3e09, "RSP Gfx ucode L3DEX       fifo 2.04  Yoshitaka Yasumoto 1998 Nintendo.",}, // In-Fisherman Bass Hunter 64, 
    {5, 0x2a1341d6, 0x2a1341d6, "RSP Gfx ucode F3DEX       fifo 2.04H Yoshitaka Yasumoto 1998 Nintendo.",}, // Kirby 64 - The Crystal Shards, 
    {5, 0x3e083afa, 0x89a8e0ed, "RSP Gfx ucode F3DEX.NoN   fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo.",1}, // Carmageddon 64 (uncensored), 
    {5, 0x4964b75d, 0x4964b75d, "RSP Gfx ucode F3DEX.NoN   fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo.",1}, 
    {5, 0x39e3e95a, 0x39e3e95a, "RSP Gfx ucode F3DEX       fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo."}, // Knife Edge - Nose Gunner, 
    {5, 0xd2913522, 0xd2913522, "RSP Gfx ucode F3DAM       fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo."}, // Hey You, Pikachu!, 
    {5, 0x3e083afa, 0xc998443f, "RSP Gfx ucode F3DEX       xbus 2.05  Yoshitaka Yasumoto 1998 Nintendo."}, //Triple play
    {5, 0xf4184a7d, 0xf4184a7d, "RSP Gfx ucode F3DEX       fifo 2.06  Yoshitaka Yasumoto 1998 Nintendo.",}, // Hey You, Pikachu!, 
    {5, 0x595a88de, 0x595a88de, "RSP Gfx ucode F3DEX.Rej   fifo 2.06  Yoshitaka Yasumoto 1998 Nintendo.",0,1}, // Bio Hazard 2, 
    {5, 0x0259f764, 0x0259f764, "RSP Gfx ucode F3DLX.Rej   fifo 2.06  Yoshitaka Yasumoto 1998 Nintendo.",0,1}, // Mario Party, 
    {5, 0xe1a5477a, 0xe1a5477a, "RSP Gfx ucode F3DEX.NoN   xbus 2.06  Yoshitaka Yasumoto 1998 Nintendo.",1}, // Command & Conquer, 
    {5, 0x4cfa0a19, 0x4cfa0a19, "RSP Gfx ucode F3DZEX.NoN  fifo 2.06H Yoshitaka Yasumoto 1998 Nintendo.",1}, // The Legend of Zelda - Ocarina of Time (v1.0), 
    {5, 0x2cbd9514, 0x5f40b9f5, "RSP Gfx ucode F3DZEX.NoN  fifo 2.06H Yoshitaka Yasumoto 1998 Nintendo.",1}, 

    {5, 0x3e083afa, 0x882680f4, "RSP Gfx ucode L3DEX       fifo 2.07  Yoshitaka Yasumoto 1998 Nintendo."},    // Polaris Sno

    {5, 0xdeb1cac0, 0xdeb1cac0, "RSP Gfx ucode F3DEX.NoN   fifo 2.07  Yoshitaka Yasumoto 1998 Nintendo.",1}, // Knockout Kings 2000, 
    {5, 0xf4184a7d, 0xf4184a7d, "RSP Gfx ucode F3DEX       fifo 2.07  Yoshitaka Yasumoto 1998 Nintendo.",}, // Xena Warrior Princess - Talisman of Fate, Army Men - Air Combat, Destruction Derby
    {5, 0x4b013e60, 0x4b013e60, "RSP Gfx ucode F3DEX       xbus 2.07  Yoshitaka Yasumoto 1998 Nintendo.",}, // Lode Runner 3-D, 
    {5, 0xd1a63836, 0xd1a63836, "RSP Gfx ucode L3DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // Hey You, Pikachu!, 
    {5, 0x97193667, 0x97193667, "RSP Gfx ucode F3DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // Top Gear Hyper-Bike, 
    {5, 0x92149ba8, 0x92149ba8, "RSP Gfx ucode F3DEX       fifo 2.08  Yoshitaka Yasumoto/Kawasedo 1999.",}, // Paper Mario, 
    {5, 0xae0fb88f, 0xae0fb88f, "RSP Gfx ucode F3DEX       xbus 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // WWF WrestleMania 2000, 
    {5, 0xc572f368, 0xc572f368, "RSP Gfx ucode F3DLX.Rej   xbus 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // WWF No Mercy, 
    {5, 0x3e083afa, 0x74252492, "RSP Gfx ucode F3DEX.NoN   xbus 2.08  Yoshitaka Yasumoto 1999 Nintendo.",1}, 

    {5, 0x9c2edb70, 0xea98e740, "RSP Gfx ucode F3DEX.NoN   fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",1}, // LEGO Racers, Fighter's Destiny 2
    {5, 0x79e004a6, 0x79e004a6, "RSP Gfx ucode F3DLX.Rej   fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",0,1}, // Mario Party 2, 
    {5, 0xaa6ab3ca, 0xaa6ab3ca, "RSP Gfx ucode F3DEX.Rej   fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",0,1}, // V-Rally Edition 99, 
    {5, 0x2c597e0f, 0x2c597e0f, "RSP Gfx ucode F3DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // Cruis'n Exotica,
    {10, 0x4e5f3e3b, 0x4e5f3e3b,"RSP Gfx ucode F3DEXBG.NoN fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",1}, // Conker The Bad Fur Day 
    {5, 0x61f31862, 0x61f31862, "RSP Gfx ucode F3DEX.NoN   fifo 2.08H Yoshitaka Yasumoto 1999 Nintendo.",1}, // Pokemon Snap, 
    {5, 0x005f5b71, 0x005f5b71, "RSP Gfx ucode F3DZEX.NoN  fifo 2.08I Yoshitaka Yasumoto/Kawasedo 1999.",1}, // The Legend of Zelda 2 - Majora's Mask, 

    {3, 0x41839d1e, 0x41839d1e, "RSP Gfx ucode S2DEX       fifo 2.05  Yoshitaka Yasumoto 1998 Nintendo.",}, // Chou Snobow Kids, 
    {3, 0x2cbd9514, 0xc639dbb9, "RSP Gfx ucode S2DEX       xbus 2.06  Yoshitaka Yasumoto 1998 Nintendo.",},
    {3, 0xec89e273, 0xec89e273, "RSP Gfx ucode S2DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // V-Rally Edition 99, 
    {3, 0x9429b7d6, 0x9429b7d6, "RSP Gfx ucode S2DEX       xbus 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // Star Craft, 
    //{14,0x5a72397b, 0xec89e273, "RSP Gfx ucode S2DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // OgreBattle Background, 
    {3, 0x2cbd9514, 0xec89e273, "RSP Gfx ucode S2DEX       fifo 2.08  Yoshitaka Yasumoto 1999 Nintendo.",}, // Zelda MM, 

    {6, 0x6aef74f8, 0x6aef74f8, "Unknown 0x6aef74f8, 0x6aef74f8",}, // Diddy Kong Racing (v1.0), 
    {6, 0x4c4eead8, 0x4c4eead8, "Unknown 0x4c4eead8, 0x4c4eead8",}, // Diddy Kong Racing (v1.1), 

    {1, 0xed421e9a, 0xed421e9a, "Unknown 0xed421e9a, 0xed421e9a",}, // Kuiki Uhabi Suigo, 
    {5, 0x37751932, 0x55c0fd25, "Unknown 0x37751932, 0x55c0fd25",}, // Bio Hazard 2, 
    {11,0xbe0b83e7, 0xbe0b83e7,"Unknown 0xbe0b83e7, 0xbe0b83e7",},  // Jet Force Gemini, 

    {17, 0x02e882cf, 0x2ad17281, "Unknown 0x02e882cf, 0x2ad17281",}, // Indiana Jones, 
    {17, 0x1f7d9118, 0xdab2199b, "Unknown 0x1f7d9118, 0xdab2199b",}, // Battle Naboo, 
    {17, 0x74583614, 0x74583614, "Unknown 0x74583614, 0x74583614",}, // Star Wars - Rogue Squadron, 
    {17, 0xe37e2f49, 0x1eb63fd8, "Unknown 0xe37e2f49, 0x1eb63fd8",}, // Star Wars - Rogue Squadron, 
    {17, 0x8ce1af3d, 0xb2760ea2, "Unknown 0x8ce1af3d, 0xb2760ea2",}, // Star Wars - Rogue Squadron, 

    {18, 0x7b685972, 0x57b8095a, "Unknown 0x7b685972, 0x57b8095a",}, // World Driver Championship
    {18, 0xe92dbb9b, 0x57b8095a, "Unknown 0xe92dbb9b, 0x57b8095a",}, // World Driver Championship
    {18, 0xe6c9acc1, 0x65f80845, "Unknown 0xe6c9acc1, 0x65f80845",}, // World Driver Championship
    {18, 0x6522df69, 0x720b88a0, "Unknown 0x6522df69, 0x720b88a0",}, // World Driver Championship
    {18, 0x6522df69, 0xf1e8ba9e, "Unknown 0x6522df69, 0xf1e8ba9e",}, // World Driver Championship

    {19, 0xa486bed3, 0xa486bed3, "Unknown 0xa486bed3, 0xa486bed3",}, // Last Legion UX, 
    {19, 0x6b519381, 0xfebacfd8, "Unknown in Toukan Road",},    // I don't know which ucode

    {20, 0x6d2a01b1, 0x6d2a01b1, "RSP Gfx ucode ZSortp 0.33 Yoshitaka Yasumoto Nintendo.",}, // Mia Hamm Soccer 64, 
};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCodeSelector::UCodeSelector()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCodeSelector::~UCodeSelector()
{
}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointer to memory
//! @param[in] memory Pointer to memory manager used to access RDRAM
//-----------------------------------------------------------------------------
bool UCodeSelector::initialize(Memory* memory)
{
    m_memory = memory;
    return true;
}

//-----------------------------------------------------------------------------
//* check UCode
//! Will select a good ucode
//! @return ID of The selected ucode
//-----------------------------------------------------------------------------
unsigned int UCodeSelector::checkUCode( unsigned int ucStart, 
                                        unsigned int ucDataStart, 
                                        unsigned int ucSize, 
                                        unsigned int ucDataSize )
{
    unsigned char* RDRAM = m_memory->getRDRAM();
    unsigned int base = ucStart & 0x1fffffff;

    //Calculate Hash values
    CRCCalculator crcCalculator;
    unsigned int crc_ucDataSize = crcCalculator.calcCRC(0, &RDRAM[base], 8); //ucDataSize
    unsigned int crc_800        = crcCalculator.calcCRC(0, &RDRAM[base], 0x800);

    //Get UCode String
    char ucodeString[500];
    bool foundString = _extractUCodeString(ucDataStart, ucodeString);

    //Try to identify ucode
    int ucode = _detectUCode(crc_ucDataSize, crc_800, ucodeString );

    //Is ucode valid?
    if ( ucode == -1 && foundString )
    {
        //We were unable to identify ucode so try from string
        ucode = _detectUCodeFromString(ucodeString);
        
        //Is ucode valid?
        if ( ucode == -5 )
        {
            Logger::getSingleton().printMsg("Unable to find UCode!", M64MSG_WARNING);
            ucode = 5; //We where unable to find ucode, so just select one and hope for the best.
        }
    }
    char logMsg[530];
    if ( foundString )
    {
        sprintf(logMsg, "Selected UCode %d String=%s", ucode, ucodeString);
        Logger::getSingleton().printMsg(logMsg, M64MSG_INFO);
    }
    else
    {
        sprintf(logMsg, "Selected UCode %d Could not find UCode String ", ucode);
        Logger::getSingleton().printMsg(logMsg, M64MSG_WARNING);
    }

    return ucode;
}

//-----------------------------------------------------------------------------
//* Extract UCode String
//! @param ucDataStart Address in RAM memory where to find UCode Data were
//!                    the string is placed.
//! @param out The string identifing what ucode to use
//! @return True if we found the string OK.
//-----------------------------------------------------------------------------
bool UCodeSelector::_extractUCodeString(unsigned int ucDataStart, char out[500])
{
    unsigned int base = ucDataStart & 0x1fffffff;
    signed char* RDRAM = (signed char*)m_memory->getRDRAM();
    
    //Check for error
    if ( base >= m_memory->getRDRAMSize()+0x1000 )
    {
        return false;
    }    

    //Extract string
    for (unsigned int i=0; i<0x1000; ++i)
    {
        //If found RSP string
        if ( RDRAM[base+((i+0)^3)] == 'R' && 
             RDRAM[base+((i+1)^3)] == 'S' && 
             RDRAM[base+((i+2)^3)] == 'P' )
        {                            
            //while there are characters in string
            char* p = out;
            while ( RDRAM[base+(i^3)] >= ' ' )
            {
                //Copy string to output
                *p++ = RDRAM[base+(i^3)];
                i++;
            }
            *p++ = 0;                    
            return true;   //Found ucode string
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
//* Detect UCode
//! Use hash values to detect ucodes
//! @return Index of detected ucode, -1 if no ucode was found
//-----------------------------------------------------------------------------
int UCodeSelector::_detectUCode(unsigned int crcUCodeDataSize, unsigned int crc800, const char ucodeStr[500])
{
    //For each ucode
    for (unsigned int i=0; i<sizeof(g_UcodeData)/sizeof(UcodeData); ++i)
    {
        if ( crc800 == g_UcodeData[i].crc_800 )
        {
            //Found ucode!!!
            //gRSP.bNearClip = !g_UcodeData[i].non_nearclip;
            //gRSP.bRejectVtx = g_UcodeData[i].reject;
            return g_UcodeData[i].ucode;
        }
    }

    //gRSP.bNearClip = false;
    //gRSP.bRejectVtx = false;
    return -1;  //Return invalid number
}

//-----------------------------------------------------------------------------
//! Detect UCode from string
//! @param ucodeStr String from ROM that tells use wich ucode it uses.
//! @return Index of detected ucode, -1 if no ucode was found
//-----------------------------------------------------------------------------
int UCodeSelector::_detectUCodeFromString(const char ucodeStr[500])
{
    //UCode F3D?
    const char tempUCode0[] = "RSP SW Version: 2.0";
    if ( strnicmp( ucodeStr, tempUCode0, strlen(tempUCode0) ) == 0 )
    {
        return F3D;
    }
    if ( strncmp(&ucodeStr[4], "SW", 2) == 0 )
    {
        return F3D;
    }

    //If String = "RSP Gfx ucode "
    const char temp[] = "RSP Gfx ucode ";
    if ( strnicmp(ucodeStr, temp, strlen(temp)) == 0 )
    {
        //If v1.x
        if( strstr(ucodeStr, "1.") != 0 )
        {
            return (!strstr(ucodeStr, "S2DEX")) ? 7  : 1;
        }
        //If v2.x
        else if( strstr(ucodeStr,"2.") != 0 )
        {
            return (!strstr(ucodeStr,"S2DEX")) ? 3 : 5;
        }
    }

    return -1;
}
