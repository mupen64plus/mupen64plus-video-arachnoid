#include "UCodeSelector.h"
#include "Memory.h"
#include "CRCCalculator.h"
#include "UCodeIdentificationData.h"
#include <cstdio>
#include <cstring>
#include <string>
#include "Logger.h"

#ifndef WIN32
#define strnicmp strncasecmp
#endif

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
	unsigned int ucode = _detectUCode(crc_ucDataSize, crc_800, ucodeString );

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
	for (int i=0; i<sizeof(g_UcodeData)/sizeof(UcodeData); ++i)
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
			(!strstr(ucodeStr,"S2DEX")) ? 3 : 5;
		}
	}

	return -1;
}