#include "Memory.h"

//-----------------------------------------------------------------------------
//* Static variables
//-----------------------------------------------------------------------------
unsigned long long Memory::m_TMEM[512] = {0};

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
Memory::Memory()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Memory::~Memory()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//! Saves pointers to memory areas, resets segment register, and detects
//! size of RDRAM.
//! @param RDRAM Pointer to "Rambus Dynamic Random Access Memory"
//! @param DMEM Pointer to "RSP Data Memory"
//-----------------------------------------------------------------------------
bool Memory::initialize(unsigned char* RDRAM, unsigned char* DMEM)
{
	//Save pointer to memory
	m_RDRAM = RDRAM;
	m_DMEM = DMEM;

    //Reset Segment
    for (int i=0; i<16; ++i) 
    {
        m_segments[i] = 0;   
    }

    //Reset Texture Memory
    //for (int i=0; i<512; ++i)
    //{
    //    m_TMEM[i] = 0;
    //}    

	_calculateRDRAMSize();
	return true;
}

//-----------------------------------------------------------------------------
//* Calculate RDRAM Size
//! Detect how much Rambus Dynamic RAM there is
//-----------------------------------------------------------------------------
void Memory::_calculateRDRAMSize()
{
	unsigned char test;
		
	#if 1
		try
		{
			test = m_RDRAM[0x400000];
			test = m_RDRAM[0x500000];
			test = m_RDRAM[0x600000];
			test = m_RDRAM[0x700000];
			test = m_RDRAM[0x7FFFFC];
			m_RDRAMSize = 0x800000;
		}
		catch(...)
		{
			m_RDRAMSize = 0x400000;
		}
	
    #else
        unsigned int testAddress;

		__try
		{
			testAddress = 0;
			while ( true )
			{
				test = m_RDRAM[testAddress];
				testAddress++;
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			m_RDRAMSize = testAddress;
		}
	#endif
}