/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#include "StringFunctions.h"
#include <cstring>
using std::string;
using std::vector;

namespace StringFunctions {

//-----------------------------------------------------------------------------
// split
// @param str The string you want to split into many strings
// @param delims The character(s) that split strings
// @return Vector with the new strings
//-----------------------------------------------------------------------------
vector<string> split(const string& str, const string& delims, size_t maxSplits)
{
    size_t pos;
    size_t start = 0;
    vector<string> strings;
    size_t numSplits = 0;
       
    do 
    {
        //Find next interesting data
        start = str.find_first_not_of(delims, start);

        //Try to find delimiter
        pos = str.find_first_of(delims, start);

        if (pos == start)
        {            
            //Do nothing
            start = pos + 1;  
        }
        else if (pos == string::npos || (maxSplits!=string::npos && numSplits == maxSplits)  )
        {
            //No more spliting, copy the rest of the string
            strings.push_back( str.substr(start) );
            break;
        }
        else
        {
            //Split string and add to return 
            strings.push_back( str.substr(start, pos - start) );
            start = pos + 1;
            numSplits++;
        }

    } while (pos != string::npos);

    //Return vector with strings
    return strings;
}

//-----------------------------------------------------------------------------
// String Trim
//-----------------------------------------------------------------------------
void trim(string& str, bool left, bool right, const string delims)
{
    //Erase characters from the left
    if(left)
    {
        str.erase(0, str.find_first_not_of(delims)); 
    }

    //Erase characters from the right
    if(right) 
    {
        str.erase(str.find_last_not_of(delims)+1); 
    }    
}

//-----------------------------------------------------------------------------
// String Trim
//-----------------------------------------------------------------------------
char* trim(char* str, bool left, bool right)
{
    //Trim from the left
    if(left)
    {
        //Set pointer to string
        char* p1 = str;
        char* p2 = str;
        char* end = &str[strlen(str)-1];

        //Skip white spaces
        while ( isspace( *p1 ) && p1 != end )
        {
            ++p1;
        }

        char* newEnd = p1;

        //Copy characters to begining of string
        while ( p2 != end )
        {
            if ( p1 < newEnd )
            {
                *p2 = '\0';
            }
            else
            {
                *p2 = *p1;
            }

            ++p1;
            ++p2;
        }
    }

    //Trim from the right
    if(right) 
    {
        //Point to end of string
        char* end = str + strlen(str) - 1;

        //Remove white spaces in the end
        while( end >= str && *end == ' ' )
        {
            *end-- = '\0';
        }
    }  
    return str;
}

//-----------------------------------------------------------------------------
// String Trim
//-----------------------------------------------------------------------------
std::vector<string> split(const char* str, const std::string& delims)
{
    return split(string(str), delims);
}


} //namespace StringFunctions
