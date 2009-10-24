#include "TextureCache.h"

#include "RDP.h"
#include "RSP.h"
#include "CachedTexture.h"
#include "MathLib.h"
#include <algorithm>
#include <xutility>  //std::min
	using std::min;
#include <windows.h>
#include <GL/gl.h>
#include "Memory.h"
#include "OpenGLRenderer.h"
	
	//gSPBgRect1Cyc
//gSPBgRectCopy
#define GL_CLAMP_TO_EDGE                  0x812F

#include "Logger.h"
#include <iostream>

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
TextureCache::TextureCache()
{
    m_currentTextures[0] = 0;
    m_currentTextures[1] = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
TextureCache::~TextureCache()
{   
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool TextureCache::initialize(RSP* rsp, RDP* rdp, Memory* memory, unsigned long textureBitDepth, unsigned long cacheSize)
{
	m_rsp = rsp;
	m_rdp = rdp;
	m_memory   = memory;
	m_bitDepth = textureBitDepth;
	m_maxBytes = cacheSize;
	
	return true;
}

//-----------------------------------------------------------------------------
//* Update
//-----------------------------------------------------------------------------
void TextureCache::update(unsigned long tile)
{
	//if (cache.bitDepth != OGL.textureBitDepth)
	//{
	//	TextureCache_Destroy();
	//	TextureCache_Init();
	//}

    //Special textures?
    if ( m_rdp->getTextureMode() == TM_BGIMAGE )
    {
        return;
    }
    else if ( m_rdp->getTextureMode() == TM_FRAMEBUFFER )
    {
        return;
    }


	CachedTexture temp;	
	unsigned long maskWidth = 0, maskHeight = 0;
	_calculateTextureSize(tile, &temp, maskWidth, maskHeight);

    static int hits = 0;
    static int misses = 0;

	//For each texture in texture cache
	for (TextureList::iterator it=m_cachedTextures.begin(); it!=m_cachedTextures.end(); ++it)
  	{
		CachedTexture* temp2 = (*it);

		if ( *temp2 == temp )
		{
			_activateTexture( tile, (*it) );
			hits++;
			return;
		}
	}
	misses++;

	// If multitexturing, set the appropriate texture
	//if (OGL.ARB_multitexture)
	glActiveTextureARB( GL_TEXTURE0_ARB + tile );

	//Add new texture to cache
	m_currentTextures[tile] = addTop();
	m_currentTextures[tile]->activate();

    m_currentTextures[tile]->address     = m_rdp->getTextureImage()->address;
    m_currentTextures[tile]->crc         = temp.crc;
	m_currentTextures[tile]->width       = temp.width;
	m_currentTextures[tile]->height      = temp.height;
	m_currentTextures[tile]->clampWidth  = temp.clampWidth;
	m_currentTextures[tile]->clampHeight = temp.clampHeight;

    m_currentTextures[tile]->format = m_rsp->getTile(tile)->format;
    m_currentTextures[tile]->size =  m_rsp->getTile(tile)->size;

	m_currentTextures[tile]->palette = m_rsp->getTile(tile)->palette;
/*	m_currentTextures[tile]->fulS = rsp.getTile(tile)->fulS;
	m_currentTextures[tile]->fulT = rsp.getTile(tile)->fulT;
	m_currentTextures[tile]->ulS = rsp.getTile(tile)->ulS;
	m_currentTextures[tile]->ulT = rsp.getTile(tile)->ulT;
	m_currentTextures[tile]->lrS = rsp.getTile(tile)->lrS;
	m_currentTextures[tile]->lrT = rsp.getTile(tile)->lrT;*/
	m_currentTextures[tile]->maskS   = m_rsp->getTile(tile)->masks;
	m_currentTextures[tile]->maskT   = m_rsp->getTile(tile)->maskt;
	m_currentTextures[tile]->mirrorS = m_rsp->getTile(tile)->mirrors;
	m_currentTextures[tile]->mirrorT = m_rsp->getTile(tile)->mirrort;
 	m_currentTextures[tile]->clampS  = m_rsp->getTile(tile)->clamps;
	m_currentTextures[tile]->clampT  = m_rsp->getTile(tile)->clampt;
	m_currentTextures[tile]->line    = m_rsp->getTile(tile)->line;
	m_currentTextures[tile]->tMem    = m_rsp->getTile(tile)->tmem;


    
//	cache.current[tile]->lastDList = RSP.DList;
//	cache.current[tile]->frameBufferTexture = FALSE;

	//Calculate Real Width
	if (m_currentTextures[tile]->clampS) 
	{
		m_currentTextures[tile]->realWidth = pow2( temp.clampWidth );
	}
	else if (m_currentTextures[tile]->mirrorS)
	{
		m_currentTextures[tile]->realWidth = maskWidth << 1;
	}
	else
	{
		m_currentTextures[tile]->realWidth = pow2( temp.width );
	}

	//Calculate Real Height
	if (m_currentTextures[tile]->clampT)
	{
		m_currentTextures[tile]->realHeight = pow2( temp.clampHeight );
	}
	else if (m_currentTextures[tile]->mirrorT)
	{
		m_currentTextures[tile]->realHeight = maskHeight << 1;
	}
	else
	{
		m_currentTextures[tile]->realHeight = pow2( temp.height );
	}

	//Calculate Scale
	m_currentTextures[tile]->scaleS = 1.0f / (float)(m_currentTextures[tile]->realWidth);
	m_currentTextures[tile]->scaleT = 1.0f / (float)(m_currentTextures[tile]->realHeight);
	m_currentTextures[tile]->shiftScaleS = 1.0f;
	m_currentTextures[tile]->shiftScaleT= 1.0f;
	#if 0
		//m_currentTextures[tile]->offsetS = OGL.enable2xSaI ? 0.25f : 0.5f;
		//m_currentTextures[tile]->offsetT = OGL.enable2xSaI ? 0.25f : 0.5f;
	#else
		m_currentTextures[tile]->offsetS = 0.5f;
		m_currentTextures[tile]->offsetT = 0.5f;	
	#endif

	if (m_rsp->getTile(tile)->shifts > 10)
		m_currentTextures[tile]->shiftScaleS = (float)(1 << (16 - m_rsp->getTile(tile)->shifts));
	else if (m_rsp->getTile(tile)->shifts > 0)
		m_currentTextures[tile]->shiftScaleS /= (float)(1 << m_rsp->getTile(tile)->shifts);

	if (m_rsp->getTile(tile)->shiftt > 10)
		m_currentTextures[tile]->shiftScaleT = (float)(1 << (16 - m_rsp->getTile(tile)->shiftt));
	else if (m_rsp->getTile(tile)->shiftt > 0)
		m_currentTextures[tile]->shiftScaleT /= (float)(1 << m_rsp->getTile(tile)->shiftt);





	_loadTexture( m_currentTextures[tile] );
	_activateTexture( tile, m_currentTextures[tile] );

	m_cachedBytes += m_currentTextures[tile]->getTextureSize();

}

//-----------------------------------------------------------------------------
//* Add Top
//! Adds a texture to cache
//-----------------------------------------------------------------------------
CachedTexture* TextureCache::addTop()
{
	//If no memory left, remove old textures from cache
	while ( m_cachedBytes > m_maxBytes )
	{
		this->removeBottom();
	}

	//Allocate memory
	CachedTexture* newTexture = new CachedTexture();

    //Generate a texture
    glGenTextures(1, &newTexture->m_id);

	//Add Texture to cache
	m_cachedTextures.push_front(newTexture);

	return newTexture;
}

//-----------------------------------------------------------------------------
// Remove Bottom
//-----------------------------------------------------------------------------
void TextureCache::removeBottom() 
{
	//Get Last Texture in list
	CachedTexture* lastTexture = *(--m_cachedTextures.end());

	//Remove Texture
	m_cachedTextures.pop_back();
	m_cachedBytes -= lastTexture->getTextureSize();

	//if (cache.bottom->frameBufferTexture)
	//	FrameBuffer_RemoveBuffer( cache.bottom->address );

	//Delete texture
    glDeleteTextures(1, &lastTexture->m_id);

	delete lastTexture;
}

//-----------------------------------------------------------------------------
// Remove
//-----------------------------------------------------------------------------
void TextureCache::remove( CachedTexture *texture ) 
{

}

//-----------------------------------------------------------------------------
//Move Texture to top
//-----------------------------------------------------------------------------
void TextureCache::moveToTop( CachedTexture *newtop ) 
{
    //Get Texture
    TextureList::iterator it = std::find( m_cachedTextures.begin(), m_cachedTextures.end(), newtop);

    //Erase Texture
    if ( it != m_cachedTextures.end() )
    {
        m_cachedTextures.erase(it);
    }

	//Add texture to the front of the list
	m_cachedTextures.push_front(newtop);
}

//-----------------------------------------------------------------------------
// Dispose
//-----------------------------------------------------------------------------
void TextureCache::dispose()
{
    //For each texture
    for (TextureList::iterator it=m_cachedTextures.begin(); it!=m_cachedTextures.end(); ++it )
    {
        delete (*it);
    }
	m_cachedTextures.clear();
}

//-----------------------------------------------------------------------------
// Load Texture
//-----------------------------------------------------------------------------
void TextureCache::_loadTexture(CachedTexture* texture)
{
	//Select Image Type
	GetTexelFunc getTexelFunc;
	unsigned int internalFormat;
	int			 imageType;
	m_formatSelector.detectImageFormat(texture, m_bitDepth, getTexelFunc, internalFormat, imageType, m_rdp->getTextureLUT());

	//Allocate memory
	unsigned long* dest = new unsigned long[ texture->getTextureSize() ];

	//Get Line Size
	unsigned short line = (unsigned short)texture->line;
	if (texture->size == G_IM_SIZ_32b)
		line <<= 1;

	//
	//Work Your magic
	//

	unsigned short mirrorSBit, maskSMask, clampSClamp;
	unsigned short mirrorTBit, maskTMask, clampTClamp;

	if (texture->maskS)
	{
		clampSClamp = unsigned short(texture->clampS ? texture->clampWidth - 1 : (texture->mirrorS ? (texture->width << 1) - 1 : texture->width - 1));
		maskSMask = (1 << texture->maskS) - 1;
		mirrorSBit = texture->mirrorS ? 1 << texture->maskS : 0;
	}
	else
	{
		clampSClamp = (unsigned short)min( texture->clampWidth, texture->width ) - 1;
		maskSMask = 0xFFFF;
		mirrorSBit = 0x0000;
	}

	if (texture->maskT)
	{
		clampTClamp = unsigned short(texture->clampT ? texture->clampHeight - 1 : (texture->mirrorT ? (texture->height << 1) - 1: texture->height - 1));
		maskTMask = (1 << texture->maskT) - 1;
		mirrorTBit = texture->mirrorT ?	1 << texture->maskT : 0;
	}
	else
	{
		clampTClamp = (unsigned short)min( texture->clampHeight, texture->height ) - 1;
		maskTMask = 0xFFFF;
		mirrorTBit = 0x0000;
	}

	//
	//Retrive texture from source (TMEM) and copy it to dest
	//

	unsigned short x, y, i, j, tx, ty;

	unsigned __int64* src;

	j = 0;
	for (y = 0; y < texture->realHeight; y++)
	{
		ty = min(y, clampTClamp) & maskTMask;

		if (y & mirrorTBit) {
			ty ^= maskTMask;
		}

		src = m_memory->getTextureMemory(texture->tMem) + line * ty;

		i = (ty & 1) << 1;
		for (x = 0; x < texture->realWidth; x++)
		{
			tx = min(x, clampSClamp) & maskSMask;

			if (x & mirrorSBit)
				tx ^= maskSMask;

			if (internalFormat == GL_RGBA8)
				((unsigned long*)dest)[j++] = getTexelFunc( src, tx, i, texture->palette );
			else
				((unsigned short*)dest)[j++] = getTexelFunc( src, tx, i, texture->palette );
		}
	}

	//Send Texture to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, texture->realWidth, texture->realHeight, 0, GL_RGBA, imageType, dest );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	delete[] dest;
}


void TextureCache::_calculateTextureSize(unsigned long tile, CachedTexture* out, unsigned long& maskWidth, unsigned long& maskHeight )
{
	RDPTile* rspTile = m_rsp->getTile(tile);

	//Calculate Tile Size
	unsigned long tileWidth =  rspTile->getWidth();
	unsigned long tileHeight = rspTile->getHeight();

	//Get Mask Size
	maskWidth = 1 << rspTile->masks;
	maskHeight = 1 << rspTile->maskt;

	//Get Current Tile Size
	unsigned long loadWidth = m_rdp->getCurrentTile()->getWidth();
	unsigned long loadHeight = m_rdp->getCurrentTile()->getHeight();

	unsigned long maxTexels = ImageFormatSelector::imageFormats[rspTile->size][rspTile->format].maxTexels;

	//Get Line Width (depending on imageformat)
	unsigned long lineWidth = rspTile->line << ImageFormatSelector::imageFormats[rspTile->size][rspTile->format].lineShift;
	unsigned long lineHeight; 
	if ( lineWidth ) // Don't allow division by zero
		lineHeight = min( maxTexels / lineWidth, tileHeight );
	else
		lineHeight = 0;

	unsigned long width;
	unsigned long height;

	if ( m_rdp->getTextureMode() == TM_TEXRECT )
	{
		unsigned short texRectWidth = unsigned short(m_rdp->getTexRectWidth() - rspTile->uls);
		unsigned short texRectHeight = unsigned short(m_rdp->getTexRectHeight() - rspTile->ult);

		if (rspTile->masks && ((maskWidth * maskHeight) <= maxTexels))
			width = maskWidth;
		else if ((tileWidth * tileHeight) <= maxTexels)
			width = tileWidth;
		else if ((tileWidth * texRectHeight) <= maxTexels)
			width = tileWidth;
		else if ((texRectWidth * tileHeight) <= maxTexels)
			width = m_rdp->getTexRectWidth();
		else if ((texRectWidth * texRectHeight) <= maxTexels)
			width = m_rdp->getTexRectWidth();
		else if (m_rdp->getLoadType() == LOADTYPE_TILE)
			width = loadWidth;
		else
			width = lineWidth;

		if (rspTile->maskt && ((maskWidth * maskHeight) <= maxTexels))
			height = maskHeight;
		else if ((tileWidth * tileHeight) <= maxTexels)
			height = tileHeight;
		else if ((tileWidth * texRectHeight) <= maxTexels)
			height = m_rdp->getTexRectHeight();
		else if ((texRectWidth * tileHeight) <= maxTexels)
			height = tileHeight;
		else if ((texRectWidth * texRectHeight) <= maxTexels)
			height = m_rdp->getTexRectHeight();
		else if (m_rdp->getLoadType() == LOADTYPE_TILE)
			height = loadHeight;
		else
			height = lineHeight;
	}
	else
	{
		if (rspTile->masks && ((maskWidth * maskHeight) <= maxTexels))
			width = maskWidth; // Use mask width if set and valid
		else if ((tileWidth * tileHeight) <= maxTexels)
			width = tileWidth; // else use tile width if valid
		else if (m_rdp->getLoadType() == LOADTYPE_TILE)
			width = loadWidth; // else use load width if load done with LoadTile
		else
			width = lineWidth; // else use line-based width

		if (rspTile->maskt && ((maskWidth * maskHeight) <= maxTexels))
			height = maskHeight;
		else if ((tileWidth * tileHeight) <= maxTexels)
			height = tileHeight;
		else if (m_rdp->getLoadType() == LOADTYPE_TILE)
			height = loadHeight;
		else
			height = lineHeight;
	}

 	unsigned long clampWidth = rspTile->clamps ? tileWidth : width;
	unsigned long clampHeight = rspTile->clampt ? tileHeight : height;

	if (clampWidth > 256)
		rspTile->clamps = 0;
	if (clampHeight > 256)
		rspTile->clampt = 0;

	// Make sure masking is valid
	if (maskWidth > width) 
	{
		rspTile->masks = powof( width );
		maskWidth = 1 << rspTile->masks;
	}

	if (maskHeight > height)
	{
		rspTile->maskt = powof( height );
		maskHeight = 1 << rspTile->maskt;
	}

	//Set output data
	out->width       = width;
	out->height      = height;
	out->clampWidth  = clampWidth;
	out->clampHeight = clampHeight;
	out->maskS       = m_rsp->getTile(tile)->masks;
	out->maskT       = m_rsp->getTile(tile)->maskt;
	out->mirrorS     = m_rsp->getTile(tile)->mirrors;
	out->mirrorT     = m_rsp->getTile(tile)->mirrort;
	out->clampS      = m_rsp->getTile(tile)->clamps;
	out->clampT      = m_rsp->getTile(tile)->clampt;
	out->format      = m_rsp->getTile(tile)->format;
	out->size        = m_rsp->getTile(tile)->size; 
    out->crc         = _calculateCRC(tile, width, height );
}

unsigned long TextureCache::_calculateCRC(unsigned long t, unsigned long width, unsigned long height)
{
    RDPTile* tile = m_rsp->getTile(t);

	unsigned long crc;
	unsigned long y, bpl, lineBytes, line;
	unsigned __int64 *src;

    src = m_memory->getTextureMemory(tile->tmem);
	bpl = width << tile->size >> 1;
	lineBytes = tile->line << 3;

	line = tile->line;
 	if (tile->size == G_IM_SIZ_32b)
		line <<= 1;

	crc = 0xFFFFFFFF;
 	for (y=0; y<height; ++y)
	{
		crc = m_crcCalculator.calcCRC( crc, src, bpl );

		src += line;
	}

   	if ( tile->format == G_IM_FMT_CI )
	{
		if ( tile->size == G_IM_SIZ_4b )
			crc = m_crcCalculator.calcCRC( crc, &m_rdp->m_paletteCRC16[tile->palette], 4 );
		else if (tile->size == G_IM_SIZ_8b)
			crc = m_crcCalculator.calcCRC( crc, &m_rdp->m_paletteCRC256, 4 );
	}
	return crc;
}

void TextureCache::_activateTexture( unsigned long t, CachedTexture *texture )
{
	// If multitexturing, set the appropriate texture
	//if (OGL.ARB_multitexture)
		glActiveTextureARB( GL_TEXTURE0_ARB + t );

	// Bind the cached texture
	texture->activate();

	// Set filter mode. Almost always bilinear, but check anyways
	unsigned int textureFiltering = m_rdp->getTextureFiltering();
	if ( textureFiltering == G_TF_BILERP || textureFiltering == G_TF_AVERAGE )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}

	// Set clamping modes
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->clampS ? GL_CLAMP_TO_EDGE : GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->clampT ? GL_CLAMP_TO_EDGE : GL_REPEAT );

	//texture->lastDList = RSP.DList;

	moveToTop( texture );

	m_currentTextures[t] = texture;
}