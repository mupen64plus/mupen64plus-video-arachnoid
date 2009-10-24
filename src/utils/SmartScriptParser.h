#ifndef SMART_SCRIPT_PARSER_H_
#define SMART_SCRIPT_PARSER_H_

#include <string>
    using std::string;
#include <fstream> 
    using std::ifstream;       //for reading files 
#include <vector>
    using std::vector;

#include "StringFunctions.h"

//*****************************************************************************
//* Smart Script Parser
//! Parses textfiles and removes comments
//*****************************************************************************
class SmartScriptParser
{
public: 
    
    //Constructor / Destructor
    SmartScriptParser();
    ~SmartScriptParser();

    //Open
    bool open( const char* filename, 
               bool space=true,        //Remove Extra Space, Tabs and other crap
               bool slashStar=true,    //Remove /*  */ comments 
               bool slashSlash=true,   //Remove //     comments
               bool hash=true,         //Remove #      comments 
               bool atSign=true);      //Remove @      comments

    //Peek
    char peekChar();

    //Get
	//! @todo add delimiters
    char getChar(); 
    std::string getToken(const std::string& delim="\t\n =:");
    std::string getLine(const std::string& delim="\n");
    std::vector<std::string> getStringVector(const std::string& delim= "\n\t ", int maxTokens=-1);
    
    //Get
    //int          getInt()                        { return StringFunctions::strToInt(getToken());           }
    //unsigned int getUInt()                       { return StringFunctions::strToUInt(getToken());          }    
    //bool         getBool()                       { return StringFunctions::strToBool(getToken());          }
    //float        getFloat()                      { return StringFunctions::strToFloat(getToken());         }        
    const float* getFloat2(float out[3])         { return getFloatN(out, 2); }        
    const float* getFloat3(float out[3])         { return getFloatN(out, 3); }        
    const float* getFloat4(float out[4])         { return getFloatN(out, 4); }               
    const float* getFloatN(float* out, size_t n);     

    //End of File
    bool eof() { return (m_string.size() == 0); }

    //Print
    void print();

protected:

    std::string m_string; 

 };


#endif