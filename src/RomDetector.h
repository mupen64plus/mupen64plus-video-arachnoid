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

#ifndef ROM_DETECTOR_H_
#define ROM_DETECTOR_H_

#include "N64Games.h"

//*****************************************************************************
//* ROM Header
//! Stored in each rom, gives us information about rom.
//*****************************************************************************
//#pragma pack(push, 1)
struct ROMHeader
{
    unsigned char    x1, x2, x3, x4;
    unsigned int    clockRate;
    unsigned int    bootAddressOffset;
    unsigned int    release;
    unsigned int    CRC1;              //!< Hash value
    unsigned int    CRC2;              //!< Hash value
    unsigned long long unknown0; 
    char             romName[20];       //!< Name of rom, used to identify what rom it is.
    unsigned int    unknown1;
    unsigned short   unknown2;
    unsigned char    unknown3;
    unsigned char    nManufacturer;
    unsigned short   cartID;
    signed char      countryID;
    unsigned char    unknown4;
};
//#pragma pack(pop)
//*****************************************************************************
//* COMBINER TYPE
//! What kind of combiner to use
//*****************************************************************************
enum COMBINER_TYPE
{
    CT_ADVANCED,
    CT_SIMPLE,
    CT_DUMMY,
};

//*****************************************************************************
//* CLEAR TYPE 
//! When to clear screen
//*****************************************************************************
enum CLEAR_TYPE
{
    CT_NEVER,
    CT_AFTER_ONE_DISPLAY_LIST,
    CT_AFTER_TWO_DISPLAY_LIST,
    CT_AFTER_THREE_DISPLAY_LIST,
};

//*****************************************************************************
//* ROM Detector
//! Class for detecting which rom it is and settings states after that.
//*****************************************************************************
class ROMDetector
{
public:

    //Destructor
    ~ROMDetector();

    //Singleton Instance
    static ROMDetector& getSingleton()
    {
        static ROMDetector instance;
        return instance;
    }

    //Initialize
    void initialize(unsigned char* romHeader);

    const char* getRomName() { return m_romHeader.romName; }

public:

    //! Get Rom ID
    //! @return ID of the current running rom (game).
    N64_ROM_ID    getRomID()              { return m_currentRomID;            }

    //! Get Combiner Type
    //! @return ID of the combiner to use.
    COMBINER_TYPE getCombinerType()       { return m_combinerType;            }

    //! Get Clear Type
    //! @return when to clear screen.
    CLEAR_TYPE    getClearType()          { return m_clearType;               }

    //! Get Ignore Fill Rects
    //! @return True if we should ignore fill rect instructions.
    bool          getIgnoreFillRects()    { return m_ignoreFillRects;         }

    //! Get Disable Face Culling
    //! @return True if we never should enable Face Culling.
    bool          getDisableFaceCulling() { return m_forceDisableFaceCulling; }

    //! Get Use Multi Texturing
    //! @return True if we should use multiple textures when rendering
    bool          getUseMultiTexture()    { return m_useMultiTexture;         }

    //! Get Use Secondary Color
    //! @return True if we should secondary color when rendering
    bool          getUseSecondaryColor()  { return m_useSecondaryColor;       }

private:

    //Construtor
    ROMDetector();

    //Get rom ID
    N64_ROM_ID _getRomID(char romName[20]);  

private:

    ROMHeader     m_romHeader;                //!< Rom header with information about rom

    N64_ROM_ID    m_currentRomID;             //!< What rom is running
    COMBINER_TYPE m_combinerType;             //!< What combiner to use
    CLEAR_TYPE    m_clearType;                //!< When  to clear screen
    bool          m_ignoreFillRects;          //!< Ignore fill rectangles?
    bool          m_forceDisableFaceCulling;  //!< Disable face culling?
    bool          m_useMultiTexture;          //!< Use multitextureing?
    bool          m_useSecondaryColor;        //!< Use secondary color?

};


#endif
