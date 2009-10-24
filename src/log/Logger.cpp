#include "Logger.h"
#include <time.h>
#include <iomanip>  //setw, setfill

//-----------------------------------------------------------------------------
//* Initialize
//! Initializes log, opens log-file
//! @param name Name of the log file that will be saved to disk.
//! @param cmdOutput True if logger should write to console.
//! @param fileOutput True if logger should write to logfile.
//-----------------------------------------------------------------------------
bool Logger::initialize(const char* name, bool cmdOutput, bool fileOutput)
{
	m_logDetail = LL_NORMAL;
	m_cmdOutput = cmdOutput;
	m_fileOutput = fileOutput;

    //Open File
    if ( m_fileOutput ) { m_log.open(name); }

	//Get Local time
	time_t ctTime; 
    time(&ctTime);
	struct tm* pTime = localtime( &ctTime );

    m_log << "----------------------------------------------\n"
	      << "Log Created " 		  
          << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":"   //Hour
          << std::setw(2) << std::setfill('0') << pTime->tm_min  << ":"   //Minute
          << std::setw(2) << std::setfill('0') << pTime->tm_sec  << ": "  //Second
          << "\n---------------------------------------------\n";

	m_log.flush();	 

	return true;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
Logger::~Logger()
{
    if ( m_log.is_open() ) {
        dispose();
    }
}

//-----------------------------------------------------------------------------
//* Dispose
//! Closes log file
//-----------------------------------------------------------------------------
void Logger::dispose()
{
    //Close file
    printMsg("\n------------------ Log Closed --------------------", LML_NORMAL, false);
	if ( m_fileOutput )
	{
	    m_log.close();
	}
}

//-----------------------------------------------------------------------------
//* Print Message
//! Writes a message to log-file and/or console.
//! @param msg The text message to write to log-file.
//! @param lml How important the message is. Error message are more important then warnings for example.
//! @param printToCmd True if you wish to write message to the console.
//-----------------------------------------------------------------------------
void Logger::printMsg(const char* msg, LogMessageLevel lml, bool printToCmd)
{
	
    //Handle very critical
    if ( lml == LML_VERY_CRITICAL ) {
        printMsg("\n----------------------------------------", LML_CRITICAL, printToCmd);
        printMsg(msg, LML_CRITICAL, printToCmd);
        printMsg("----------------------------------------\n", LML_CRITICAL, printToCmd);
        return;
    }

    //Handel newlines
    if ( msg[0] == '\n' ) {
        std::cout << "\n"; 
        m_log << "\n";
        printMsg( &msg[1], lml, printToCmd);
        return;
    }

	//Intressting message?
    if ( (m_logDetail + lml) >= 4)
    {
		//Print to file (with a time stamp)
		if ( m_fileOutput )
		{
			//Get time
			time_t ctTime; 
			time(&ctTime);

			//Get Local time
			struct tm* pTime = localtime( &ctTime );

			//Print
			m_log << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":" 
					<< std::setw(2) << std::setfill('0') << pTime->tm_min  << ":" 
					<< std::setw(2) << std::setfill('0') << pTime->tm_sec  << ": " 
					<< msg << std::endl;

			m_log.flush();
		}

        //Print to command promt
        if ( m_cmdOutput && printToCmd)
        {
            std::cout << msg << std::endl;
        }
    }	
}
