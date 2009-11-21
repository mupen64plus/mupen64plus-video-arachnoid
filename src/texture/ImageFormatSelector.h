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
	GetTexelFunc	Get16;
	unsigned int    glType16;
	int  			glInternalFormat16;
	GetTexelFunc	Get32;
	unsigned int 	glType32;
	int			    glInternalFormat32;
	unsigned int   autoFormat, lineShift, maxTexels;
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