#include "CachedTexture.h"

#include <windows.h>
#include <GL/gl.h>

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
CachedTexture::CachedTexture()
{
	m_id = 0;     //!< id used by OpenGL to identify texture
	m_textureSize = 0;       //!< Size of texture in bytes
	address = 0;
	crc  = 0;
	offsetS = offsetT  = 0;
	maskS = maskT  = 0;
	clampS = clampT  = 0;
	mirrorS = mirrorT  = 0;
	line = 0;
    size = 0;
	format = 0;           //!< Texture format
	tMem = 0;
	palette = 0;          //!< What Texture Look Up Table to use
	width  = height = 0;			  // N64 width and height
	clampWidth = clampHeight = 0;  // Size to clamp to
	realWidth = realHeight = 0;	  // Actual texture size
	scaleS = scaleT  = 0;			  // Scale to map to 0.0-1.0
	shiftScaleS = shiftScaleT = 0; // Scale to shift
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
CachedTexture::~CachedTexture()
{

}

//-----------------------------------------------------------------------------
//! Activate texture
//-----------------------------------------------------------------------------
void CachedTexture::activate()
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, m_id );
}

//-----------------------------------------------------------------------------
//! Deactivate texture
//-----------------------------------------------------------------------------
void CachedTexture::deactivate()
{
	glDisable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, 0 );	
}

//-----------------------------------------------------------------------------
//! Equal operator
//-----------------------------------------------------------------------------
bool CachedTexture::operator == (const CachedTexture& t) const
{
	return( crc         == t.crc &&
			width       == t.width &&
			height      == t.height &&
			clampWidth  == t.clampWidth &&
			clampHeight == t.clampHeight &&
			maskS       == t.maskS &&
			maskT       == t.maskT &&
			mirrorS     == t.mirrorS &&
			mirrorT     == t.mirrorT &&
			clampS      == t.clampS &&
			clampT      == t.clampT &&
			format      == t.format /* &&
			size        == t.size */ );
}

//-----------------------------------------------------------------------------
//! Assign operator
//-----------------------------------------------------------------------------
CachedTexture& CachedTexture::operator = (const CachedTexture& v)
{ 	
	address =  v.address;
	crc   = v.crc;
	format  = v.format;
	size    = v.size;
	width   = v.width;
	height  = v.height;
	clampWidth = v.clampWidth;
	clampHeight = v.clampHeight;
	palette = v.palette;
/*	cache.current[tile]->fulS = gSP.textureTile[tile]->fulS;
	cache.current[tile]->fulT = gSP.textureTile[tile]->fulT;
	cache.current[tile]->ulS = gSP.textureTile[tile]->ulS;
	cache.current[tile]->ulT = gSP.textureTile[tile]->ulT;
	cache.current[tile]->lrS = gSP.textureTile[tile]->lrS;
	cache.current[tile]->lrT = gSP.textureTile[tile]->lrT;*/
	maskS = v.maskS;
	maskT = v.maskT;
	mirrorS = v.mirrorS;
	mirrorT = v.mirrorT;
	clampS = v.clampS;
	clampT = v.clampT;
	line = v.line;
	tMem = v.tMem;
	//lastDList = RSP.DList;
	//frameBufferTexture = FALSE;
	return *this;     
}