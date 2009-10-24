#ifndef UCODE_SELECTOR_H_
#define UCODE_SELECTOR_H_

//Forward declarations
class Memory;

//*****************************************************************************
//* UCode Selector
//! Class for selecting a good ucode for the current game
//*****************************************************************************
class UCodeSelector
{
public:

	//Constructor / Destructor
	UCodeSelector();
	~UCodeSelector();

	//Initialize
	bool initialize(Memory* memory);

	//Check ucode
	unsigned long checkUCode( unsigned long ucStart, 
		                         unsigned long ucDStart, 
					             unsigned long ucSize, 
					             unsigned long ucDSize );

private:

	//Private functions
	bool _extractUCodeString(unsigned long ucDataStart, char out[500]);
	int _detectUCode(unsigned long crcUCodeDataSize, unsigned long crc800, const char ucodeStr[500]);
	int _detectUCodeFromString(const char ucodeStr[500]);

private:

	Memory* m_memory;   //!< Pointer to memory manager

};

#endif