#ifndef MEMORY_H_
#define MEMORY_H_

//*****************************************************************************
//* Memory
//! Handle RDRAM, Texture Memory and Segments
//*****************************************************************************
class Memory
{
public:

	//Constructor / Destructor
	Memory();
	~Memory();

	//Initialize
	bool initialize(unsigned char* RDRAM, unsigned char* DMEM);

	//Get RDRAM
	unsigned char*    getRDRAM(int address=0)                { return &m_RDRAM[address];          }
	unsigned int* getRDRAMint32()                        { return (unsigned int*)m_RDRAM; }
	unsigned int     getRDRAMSize()                         { return m_RDRAMSize;                }

	//Get DMEM
	unsigned char* getDMEM(int address=0)                    { return &m_DMEM[address];           }

	//Get Texture memory
	static unsigned long long* getTextureMemory(int address=0) { return &m_TMEM[address];           }

	//Get Segment adress
	unsigned int getRDRAMAddress(unsigned int segmentAddress) 
	{ 		
		return (m_segments[(segmentAddress >> 24) & 0x0F] + (segmentAddress & 0x00FFFFFF)) & 0x00FFFFFF; 
	}

	void setSegment(unsigned int address, unsigned int value)
	{
		if ( address >= 16 ) {
			//ERROR
			return;
		}

		m_segments[address] = value;
	}

private:

	//Calculate size of Rambus Dynamic Random Access Memory.
	void _calculateRDRAMSize();

private:
	
	unsigned char*          m_RDRAM;          //!< Rambus Dynamic Random Access Memory
	unsigned char*          m_DMEM;           //!< RSP Data Memory
	static unsigned long long m_TMEM[512];      //!< Texture Memory		
	unsigned int        m_segments[16];   //!< Temporary memory for storing segment values
	unsigned int           m_RDRAMSize;      //!< Size of RDRAM

};

#endif