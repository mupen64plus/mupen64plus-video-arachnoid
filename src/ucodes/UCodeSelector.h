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
	unsigned int checkUCode( unsigned int ucStart, 
		                         unsigned int ucDStart, 
					             unsigned int ucSize, 
					             unsigned int ucDSize );

private:

	//Private functions
	bool _extractUCodeString(unsigned int ucDataStart, char out[500]);
	int _detectUCode(unsigned int crcUCodeDataSize, unsigned int crc800, const char ucodeStr[500]);
	int _detectUCodeFromString(const char ucodeStr[500]);

private:

	Memory* m_memory;   //!< Pointer to memory manager

};

#endif