#include "ConsoleWindow.h"
#include "platform.h"
#include <cstdio>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#endif
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------------
//* Create Console
//! Will create a command console window (aka dos window) 
//! and redirect all io to it.
//! link http://msdn2.microsoft.com/en-us/library/ms682425.aspx
//-----------------------------------------------------------------------------
bool createConsole()
{
#ifdef WIN32
    //Open and show a console
    if ( !AllocConsole() ) 
	{
        return false;  //Process already has a console
    }   

    //Set size of console
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    consoleInfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);

    //Redirect unbuffered STD::OUT to the console
    HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int consoleHandle  = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    FILE* file = _fdopen(consoleHandle, "w");
    *stdout = *file;
    setvbuf(stdout, NULL, _IONBF, 0 );

    //Redirect unbuffered STD::IN to the console
    stdHandle = GetStdHandle(STD_INPUT_HANDLE);
    consoleHandle = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    file = _fdopen(consoleHandle, "r");
    *stdin = *file;
    setvbuf(stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console
    stdHandle = GetStdHandle(STD_ERROR_HANDLE);
    consoleHandle = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    file = _fdopen(consoleHandle, "w");
    *stderr = *file;
    setvbuf(stderr, NULL, _IONBF, 0 );

    //Rederect IO (cout, wcout, cin, wcin, wcerr, cerr, wclog and clog)
    std::ios::sync_with_stdio();
#endif
    return true;
}

//-----------------------------------------------------------------------------
//* Terminate Console
//! http://msdn2.microsoft.com/en-us/library/ms682425.aspx
//-----------------------------------------------------------------------------
bool terminateConsole()
{
#ifdef WIN32
    //CloseHandle(console);

    //Terminate console
    if ( !FreeConsole() ) {
        return false;
    }  
#endif
    return true;
}