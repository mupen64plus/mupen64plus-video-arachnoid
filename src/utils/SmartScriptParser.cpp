#include "SmartScriptParser.h"
#include <fstream>
#include <iostream>
#include <vector>
    using std::vector;
#include "StringFunctions.h"
#include "StreamFunctions.h"

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
SmartScriptParser::SmartScriptParser()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
SmartScriptParser::~SmartScriptParser()
{

}

//-----------------------------------------------------------------------------
//* Open
//! Will open a text file and start parsing it.
//! @param filename The name of the file you want to parse, 
//! @param space Remove extra space, tabs and other crap
//! @param slashStar Remove multiline c++ comments 
//! @param slashSlash Remove singleline c++ comments
//! @param hash Remove # comments 
//! @param atSign Remove @ comments
//-----------------------------------------------------------------------------
bool SmartScriptParser::open( const char* filename, 
                               bool space,        //Remove Extra Space, Tabs and other crap
                               bool slashStar,    //Remove /*  */ comments 
                               bool slashSlash,   //Remove //     comments
                               bool hash,          //Remove #      comments 
                               bool atSign )      //Remove @      comments
{
    //Open file
    std::ifstream is;
    is.open(filename, std::ios::binary);
    if ( !is.is_open() )
    {
        return false;
    }

    char c = '\n';
    bool isNewLine = true;

    //Read entire file    
    while ( is.good() )     
    {
        isNewLine = ( c == '\n' );

        //Get next character from file
        c = is.get();  

        //Handle Carrage Return
        if ( c == '\r' )
        {
            if ( is.peek() == '\n' ) {
                continue;  //DOS/Windows: convert to unix
            }
            else {
                c = '\n';  //MAC: convert to unix
            }
        }   

        //Remove One Line Comments
        if ( (slashSlash && c=='/' && is.peek() == '/') || (hash && c=='#') || (atSign && c=='@')  ) 
        {            
            StreamFunctions::skipLine(is, "\n"); 

            //Add new line?
            if ( !isNewLine ) { 
                c='\n'; 
                m_string += c;  
            }
            continue;
        }

        //Remove Block Comments
        if ( slashStar && c == '/' && is.peek() == '*' )
        {
            do
            {
                StreamFunctions::skipLine(is, "*"); 
            }
            while ( !is.eof() && is.peek() != '/' );
            if ( !is.eof() ) {
                is.get();  //Read '/'
            }
            continue;
        }

        //Add Character to string
        m_string += c;
    }

    is.close();
    return true;
}

//-----------------------------------------------------------------------------
//* Peek Char
//! @todo Error control
//-----------------------------------------------------------------------------
char SmartScriptParser::peekChar()
{
    char c = '\0';
    if ( !m_string.empty() )
    {
        c = m_string.at(0);
    }
    return c;
}

//-----------------------------------------------------------------------------
// Get Char
//! @todo Error control
//-----------------------------------------------------------------------------
char SmartScriptParser::getChar()
{
    char c = this->peekChar();
    if ( !m_string.empty() )
    {
        m_string.erase(0,1);
    }
    return c;
}

//-----------------------------------------------------------------------------
// Get Token
//! @todo Error control
//-----------------------------------------------------------------------------
std::string SmartScriptParser::getToken(const std::string& delim)
{   
    string token;
#if 1
    size_t lpos = m_string.find_first_not_of(delim);
    m_string.erase(0, lpos); 
    size_t rpos = m_string.find_first_of(delim);
    if ( (rpos+1) != 0 )
    {
        token = m_string.substr(0, rpos);
        m_string.erase(0, rpos+1); 
    }
    else
    {
        token = m_string;
        m_string.erase();
    }
#else
    do
    {
        size_t pos = m_string.find_first_of(delim)+1;
        if ( pos != 0 )
        {
            token = m_string.substr(0, pos);
            StringFunctions::trim(token, true, true, delim);
            m_string.erase(0, pos);
        }
        else 
        {
            token = m_string;
            m_string.erase();        
        }
    }
    while ( token.size() == 0 && m_string.size() != 0);
#endif

    return token;
}

//-----------------------------------------------------------------------------
// Get Line
//! @todo Error control
//! @todo Const ref
//-----------------------------------------------------------------------------
std::string SmartScriptParser::getLine(const std::string& delim)
{
    size_t lenght = m_string.find_first_of(delim)+1;

    string line;
    if ( lenght != 0 )
    {
        line = m_string.substr(0, lenght);
        m_string.erase(0, lenght);
    }
    else
    {
        line = m_string;
        m_string.erase();
    }
    return line;
}

//-----------------------------------------------------------------------------
//* Get String Vector
//! Will split string into multiple and return them as a vector 
//-----------------------------------------------------------------------------
std::vector<std::string> SmartScriptParser::getStringVector(const std::string& delim, int maxTokens)
{
    return StringFunctions::split(this->getLine(), delim, maxTokens);
}


//-----------------------------------------------------------------------------
//* Print
//! Will print the remaining text to the console.
//-----------------------------------------------------------------------------
void SmartScriptParser::print()
{
    std::cout << m_string << std::endl;
}      

////-----------------------------------------------------------------------------
////* Get Float
////! Function for reading n number of floats from the file.
////! @param out Output param, Vector with the floats read from file.
////! @param n The number of floats you want to read.
////-----------------------------------------------------------------------------
//const float* CSmartScriptParser::getFloatN(float* out, size_t n) 
//{ 
//    for (size_t i=0; i<n; ++i)
//    {
//        out[i] = getFloat();
//    }    
//    return out; 
//}   