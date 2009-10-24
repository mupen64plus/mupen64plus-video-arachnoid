#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>

//*****************************************************************************
//* Logging Detail
//! Defines of much you want to log
//*****************************************************************************
enum LoggingDetail
{
	LL_VERY_LOW = 0,
    LL_LOW      = 1,
    LL_NORMAL   = 2,
    LL_INTENSE  = 3
};

//*****************************************************************************
//* Log Message Level
//! Difines how important the message is
//*****************************************************************************
enum LogMessageLevel
{
    LML_TRIVIAL       = 1,
    LML_NORMAL        = 2,
    LML_CRITICAL      = 3,
    LML_VERY_CRITICAL = 4,
};

//*****************************************************************************
//* Log
//! Class for writing to a file and/or console 
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
    bool initialize(const char* name, bool cmdOutput=true, bool fileOutput=true);  
    void dispose();

    //Message
    void printMsg(const char* msg, LogMessageLevel lml=LML_NORMAL, bool printToCmd=true);

public:

    //! @return True if log writes to console
	bool isCmdOutput() const  { return m_cmdOutput; }

    //! @return True if log writes to file
	bool isFileOutput() const { return m_fileOutput; }

public:

    //! Sets the log detail, how much you want to log depending on how important the diffrent messages are
    void setLogDetail(LoggingDetail detail) { m_logDetail = detail; }

    //! Returns the log detail, how much you log depending on how important the diffrent messages are
    LoggingDetail getLogDetail() const { return m_logDetail; }

    //Operators
	inline Logger& operator << ( int i )              { if ( m_cmdOutput ) std::cerr << i;     if ( m_fileOutput ) { m_log << i;     m_log.flush(); } return *this; }
    inline Logger& operator << ( const char* msg )    { if ( m_cmdOutput ) std::cerr << msg;   if ( m_fileOutput ) { m_log << msg;   m_log.flush(); } return *this; }
	inline Logger& operator << ( float value )        { if ( m_cmdOutput ) std::cerr << value; if ( m_fileOutput ) { m_log << value; m_log.flush(); } return *this; }
	inline Logger& operator << ( unsigned int i )     { if ( m_cmdOutput ) std::cerr << i;     if ( m_fileOutput ) { m_log << i;     m_log.flush(); } return *this; }
	inline Logger& operator << ( unsigned long i )    { if ( m_cmdOutput ) std::cerr << i;     if ( m_fileOutput ) { m_log << i;     m_log.flush(); } return *this; }
	inline Logger& operator << ( unsigned __int64 i ) { if ( m_cmdOutput ) std::cerr << i;     if ( m_fileOutput ) { m_log << i;     m_log.flush(); } return *this; }

protected:

	//! Constructor
	Logger() {}

protected:

    std::ofstream m_log;         //! I/O-object, Used for printing to files
    LoggingDetail m_logDetail;   //! Level of detail, only messages with high enough level will be printed
    bool          m_cmdOutput;   //! Print to console?
	bool          m_fileOutput;  //! Print to text file?

};

#endif
