#include "Logger.h"

//-----------------------------------------------------------------------------
//* Initialize
//! Initializes log with debug callback
//-----------------------------------------------------------------------------
bool Logger::initialize(void (*debugCallback)(void*, int, const char*), void *context)
{
	m_debugCallback = debugCallback;
	m_debugCallContext = context;

	return true;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Logger::~Logger()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Dispose
//! Closes log file
//-----------------------------------------------------------------------------
void Logger::dispose()
{
	m_debugCallback = NULL;
	m_debugCallContext = NULL;
}

//-----------------------------------------------------------------------------
//* Print Message
//! Writes a message to log-file and/or console.
//! @param msg The text message to write to log-file.
//! @param lml How important the message is. Error message are more important then warnings for example.
//-----------------------------------------------------------------------------
void Logger::printMsg(const char* msg, m64p_msg_level lml)
{
	if (m_debugCallback != NULL)
	{
		(*m_debugCallback)(m_debugCallContext, lml, msg);
    }	
}
