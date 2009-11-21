
#ifndef MEMORY_LEAK_DETECTOR_H
#define MEMORY_LEAK_DETECTOR_H

#include "m64p.h"
#include <cstdio>
#ifdef WIN32
#include <crtdbg.h>
#endif

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
#ifdef WIN32
		::OutputDebugString(">>> Memory leak detection enabled <<<\n"); 
#endif
	}

	//Destructor
	~CMemoryLeakDetector()
	{ 
#ifdef WIN32
		if ( !_CrtDumpMemoryLeaks() )
		{
			::OutputDebugString(">>> No memory leak detected <<<\n"); 
		}
#endif
	}
};

CMemoryLeakDetector md;

#endif