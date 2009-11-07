#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdlib>
#include "m64p_types.h"

//*****************************************************************************
//*****************************************************************************
//* Log
//! Class for sending log messages to M64P
//*****************************************************************************
class Logger
{
public:
	//Destructor
	~Logger();

    //Singleton Instance
	static Logger& getSingleton()
	{
		static Logger pInstance;
		return pInstance;
	}

    //Initialize / Dispose
    bool initialize(void (*debugCallback)(void*, int, const char*), void *context);  
    void dispose();

    //Message
    void printMsg(const char* msg, m64p_msg_level lml=M64MSG_INFO);

protected:
	//! Constructor
	Logger() { m_debugCallback = NULL; m_debugCallContext = NULL; }

protected:
	void (*m_debugCallback)(void *, int, const char *);
	void *m_debugCallContext;
};

#endif
