#ifndef CACHED_TEXTURE_H_
#define CACHED_TEXTURE_H_

//*****************************************************************************
//* Cached Texture
//! Struct used by Texture Cache to store used textures.
//*****************************************************************************
class CachedTexture
{
public:

	//Constructor / Destrucotr
	CachedTexture();
	~CachedTexture();

	//Activate / Deactivate
	void activate();
	void deactivate();

	//Get texture size
	unsigned long getTextureSize() { return m_textureSize; }

	//Assignment operator
	CachedTexture& operator = (const CachedTexture& v);

	//Equal operator
	bool operator == (const CachedTexture& t) const;

public:

	unsigned int   m_id;                      //!< id used by OpenGL to identify texture
	unsigned long  m_textureSize;             //!< Size of texture in bytes

	unsigned long  address;
	unsigned long  crc;                       //!< A CRC "checksum" (Cyclic redundancy check)
//	float          fulS, fulT;
//	unsigned short ulS, ulT, lrS, lrT;
	float	       offsetS, offsetT;
	unsigned long  maskS, maskT;
	unsigned long  clampS, clampT;
	unsigned long  mirrorS, mirrorT;
	unsigned long  line;
    unsigned long  size;
	unsigned long  format;                    //!< Texture format
	unsigned long  tMem;
	unsigned long  palette;                   //!< What Texture Look Up Table to use
	unsigned long  width, height;			  //!< N64 width and height
	unsigned long  clampWidth, clampHeight;   //!< Size to clamp to
	unsigned long  realWidth, realHeight;	  //!< Actual texture size
	float		   scaleS, scaleT;			  //!< Scale to map to 0.0-1.0
	float		   shiftScaleS, shiftScaleT;  //!< Scale to shift
//	unsigned long lastDList;
//	unsigned long frameBufferTexture;

};

#endif