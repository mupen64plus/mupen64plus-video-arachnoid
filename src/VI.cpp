#include "VI.h"
#include "GBIDefs.h" //_FIXED2FLOAT, _SHIFTR
#include "m64p.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
VI::VI()
{
	m_width = 320;
	m_height = 240;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
VI::~VI()
{

}

//-----------------------------------------------------------------------------
//* Calculate Size
//! Calculates width and height of video interface
//-----------------------------------------------------------------------------
void VI::calcSize(GFX_INFO*  graphicsInfo)
{
	//Get video interface values
	unsigned int viScaleX          = *graphicsInfo->VI_X_SCALE_REG;
	unsigned int viScaleY          = *graphicsInfo->VI_X_SCALE_REG;
	unsigned int viStartHorizontal = *graphicsInfo->VI_H_START_REG;
	unsigned int viStartVertical   = *graphicsInfo->VI_V_START_REG;

	//Get Scale
	float xScale = _FIXED2FLOAT(_SHIFTR(*graphicsInfo->VI_X_SCALE_REG, 0, 12), 10);
	float yScale = _FIXED2FLOAT(_SHIFTR(*graphicsInfo->VI_Y_SCALE_REG, 0, 12), 10);

	//Get Offsets (no need for these?)
	//float xOffset = _FIXED2FLOAT(_SHIFTR(viScaleX, 16, 12), 10);
	//float yOffset = _FIXED2FLOAT(_SHIFTR(viScaleY, 16, 12), 10);

	//Get horizontal coordinats
	unsigned int hEnd = _SHIFTR( viStartHorizontal, 0, 10 );
	unsigned int hStart = _SHIFTR( viStartHorizontal, 16, 10 );

	//Get vertical coordinats
	// These are in half-lines, so shift an extra bit
	unsigned int vEnd = _SHIFTR( viStartVertical, 1, 9 );
	unsigned int vStart = _SHIFTR( viStartVertical, 17, 9 );

	//Calculate size
	m_width = (int)((hEnd - hStart) * xScale);
	m_height = (int)((vEnd - vStart) * yScale * 1.0126582f);

	//If Zero: Set to defaults
	if ( m_width == 0.0f ) m_width = 320;
	if ( m_height == 0.0f ) m_height = 240;

	//m_width = 320;
	//m_height = 240;
}