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

/*
What is the difference between '\n' and '\r\n'?
-----------------------------------------------

There are a few characters which can indicate a new line. The usual ones are these two:

    * '\n' or '0x0A' (10 in decimal) -> This character is called "Line Feed" (LF).
    * '\r' or '0x0D' (13 in decimal) -> This one is called "Carriage return" (CR).

Different Operating Systems handle newlines in a different way. Here is a short list of the most common ones:

    * DOS and Windows
      They expect a newline to be the combination of two characters, namely '\r\n' (or 13 followed by 10).

    * Unix (and hence Linux as well)
      Unix uses a single '\n' to indicate a new line.

    * Mac
      Macs use a single '\r'.

To unify things a bit, so that writing portable C/C++ programs is possible, file streams have both a 
"translated" and an "untranslated" mode. If you open a file in translated mode, the runtime library 
will convert a '\n' to the appropriate newline character(s). If the following program is compiled under 
Unix, the file will contain a single LF to indicate the newline. If it's compiled under windows, it will 
contain a CRLF.

#include <stdio.h> 
#include <stdlib.h>   
int main() { 
    FILE *fp = fopen("testfile.txt", "w"); 
    fprintf(fp, "Hello World\n"); 
    fclose(fp); 
    return 0; 
}

Important
If you want to be able to read text files written on different operating systems, you have to open the file 
in binary (= untranslated) mode and check for the different newlines yourself.

*/


#ifndef STRING_FUNCTIONS_H_
#define STRING_FUNCTIONS_H_

#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>  //std::transform
    
namespace StringFunctions 
{
    //Split
    std::vector<std::string> split(const std::string& str, const std::string& delims="\n\t ", size_t maxSplits=std::string::npos);
    std::vector<std::string> split(const char* str, const std::string& delims="\n\t ");

    //Trim
    void trim(std::string& str, bool left=true, bool right=true, const std::string delims=" \t\r\n");

    //Trim
    char* trim(char* str, bool left=true, bool right=true); 
}

#endif
