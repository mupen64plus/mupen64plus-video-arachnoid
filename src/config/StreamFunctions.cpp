
#include "StreamFunctions.h"
#include <cstring>

#define STREAM_TEMP_SIZE 128

namespace StreamFunctions {

//-----------------------------------------------------------------------------
// Skip
//-----------------------------------------------------------------------------
void skip(std::ifstream& is, long count)
{
	is.clear(); //Clear fail status in case eof was set
	is.seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
}

//-----------------------------------------------------------------------------
// Read
//Important: count should be buffer size - 1
//-----------------------------------------------------------------------------
size_t read(std::ifstream& is, char* buffer, int count)
{
    is.read(buffer, count);
    int readCount = is.gcount();
    buffer[readCount] = '\0';
    return readCount;
}


//-----------------------------------------------------------------------------
// Skip Line
//-----------------------------------------------------------------------------
size_t skipLine(std::ifstream& is, const std::string& delim)
{
    char tmpBuf[STREAM_TEMP_SIZE];
    size_t total = 0;
    size_t readCount;

    // Keep looping while not hitting delimiter
    while ((readCount = read(is, tmpBuf, STREAM_TEMP_SIZE-1)) != 0)
    {
        // Find first delimiter
        size_t pos = strcspn(tmpBuf, delim.c_str());

        if (pos < readCount)
        {
            // Found terminator, reposition backwards
            skip(is, (long)(pos + 1 - readCount));
            total += pos + 1;
            break;
        }
        total += readCount;
    }
    return total;    
}

} //namespace StreamFunctions 