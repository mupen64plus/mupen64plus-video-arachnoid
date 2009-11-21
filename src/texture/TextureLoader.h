#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include "GBIDefs.h"

//Forward declarations
class Memory;
class RDP;

//*****************************************************************************
//* TextureImage
//! Used to store information about texture image
//*****************************************************************************
struct TextureImage
{
	unsigned int format;
	unsigned int size;
	unsigned int width;
	unsigned int bpl;
	unsigned int address;

	//! Constructor
	TextureImage()
	{
		format = size = width = bpl = 0;
		address = 0;
	}
};

//*****************************************************************************
//* RDP Tile
//! Struct used to store information used then loading textures.
//*****************************************************************************
struct RDPTile
{
	unsigned int format, size, line, tmem, palette;

	union
	{
		struct
		{
			unsigned	mirrort	: 1;
			unsigned	clampt	: 1;
			unsigned	pad0	: 30;
			unsigned	mirrors	: 1;
			unsigned	clamps	: 1;
			unsigned	pad1	: 30;
		};
		struct
		{
			unsigned int cmt, cms;
		};
	};

	//FrameBuffer *frameBuffer;
	unsigned int maskt, masks;   
	unsigned int shiftt, shifts;
	float fuls, fult, flrs, flrt; 
	unsigned int uls, ult, lrs, lrt;   

	unsigned int getWidth() { return lrs - uls + 1; }
	unsigned int getHeight() { return lrt - ult + 1; }

	//! Constructor
	RDPTile()
	{
		format = size = line = tmem = palette = 0;
		maskt = masks = 0;   
		shiftt = shifts = 0;
		fuls = fult = flrs = flrt = 0; 
		uls = ult = lrs = lrt = 0;  
	}
};

//*****************************************************************************
//* Texture Loader
//! Class for loading texturs from RDRAM to Texture Memory
//*****************************************************************************
class TextureLoader
{
public:

	//Constructor / Destructor
	TextureLoader();
	~TextureLoader();

	bool initialize(RDP* rdp, Memory* memory);

	//Set Texture Image
	void setTextureImage(unsigned int format, unsigned int size, unsigned int width, unsigned int segmentAddress);

	//Set Tile
	void setTile(int format, int size, int line, int tmem, int tile, int palette, 
		                  int clampS, int clampT, int mirrorS, int mirrorT, int maskS, 
						  int maskT, int shiftS, int shiftT);

	//Set Tile Size
	void setTileSize(int tile, unsigned int s0, unsigned int t0, unsigned int s1, unsigned int t1);

	//Load Tile
	void loadTile(int tile, int s0, int t0, int s1, int t1);

	//Load Block
	void loadBlock(int tile, int s0, int t0, int s1, int t1);

	//Load Texture Look up table
	void loadTLUT(int tile, int s0, int t0, int s1, int t1);

public:

	//! Returns information about current texture image
	TextureImage* getTextureImage()          { return &m_textureImage; }

	//! Returns previusly loaded tile
	RDPTile*      getCurrentTile()           { return m_currentTile;   }

	//! Returns a tile
	//! @param tile which of the eight tiles to return. 
	RDPTile*      getTile(unsigned int tile) { return &m_tiles[tile];  }

private:

	Memory* m_memory;                //!< Pointer to Memory Manager
	RDP*    m_rdp;                   //!< Pointer to Reality Signal Processor    

	//Tiles
	RDPTile      m_tiles[8];         //!< Eight tiles
	RDPTile*     m_currentTile;      //!< Previusly loaded tile
	TextureImage m_textureImage;     //!< Texture Image

};

#endif