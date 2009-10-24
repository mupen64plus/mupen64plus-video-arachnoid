
#ifndef MEMORY_LEAK_DETECTOR_H
#define MEMORY_LEAK_DETECTOR_H

#include <crtdbg.h>
#include <windows.h>
#include <cstdio>

//*****************************************************************************
//* Memory Leak Detector Class
//! Class for checking if there are any memory leaks.
//*****************************************************************************
class CMemoryLeakDetector
{
public:
	
	//Constructor
	CMemoryLeakDetector() 
	{ 
		::OutputDebugString(">>> Memory leak detection enabled <<<\n"); 
	}

	//Destructor
	~CMemoryLeakDetector()
	{ 
		if ( !_CrtDumpMemoryLeaks() )
		{
			::OutputDebugString(">>> No memory leak detected <<<\n"); 
		}
	}
};

CMemoryLeakDetector md;

#endif