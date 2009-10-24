#ifndef STREAM_FUNCIONS_H_
#define STREAM_FUNCIONS_H_

#include <fstream> //ifstream
#include <string>

namespace StreamFunctions 
{

    void skip(std::ifstream& is, long count);

    //Important: count should be buffer size - 1
    size_t read(std::ifstream& is, char* buffer, int count);

    size_t skipLine(std::ifstream& is, const std::string& delim="\n");

}

#endif