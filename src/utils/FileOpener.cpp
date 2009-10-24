#include "FileOpener.h"
#include <windows.h>    //GetCurrentDirectory()
#include <iostream>     //std::system(); 

//-----------------------------------------------------------------------------
//* Execute File
//! Will open a file using an apropriete program
//-----------------------------------------------------------------------------
void executeFile(const char* filename, const char* directory)
{
    #if 0
        //Using system()
        //-------------------
      
        //Get Directory
        char currentDir[255];
        if ( directory )
        {
            sprintf(currentDir, directory);                        
        }
        else
        {
            GetCurrentDirectory(200, currentDir);                
        }

        bool first = true;
        int counter = 0;
        char filepath[255];

        //For each character in the current directory
        for (unsigned int i=0; i<strlen(currentDir); ++i) 
        {
            //Add character to filepath
            filepath[counter] = currentDir[i];
            
            //Add '"' around all folders (so filepaths with whitespaces can be opened)
            if ( currentDir[i] == '\\' ) 
            {            
                //Citat
                if ( first ) {
                    filepath[++counter] = '\"';      //Start Citat
                    first = false;
                }            
                else {
                    filepath[counter] = '\"';        //Stop Citat
                    filepath[++counter] = '\\';
                    filepath[++counter] = '\"';      //Start New Citat
                }
            }       
            counter++;
        }

        filepath[counter++] = '\"';  //Stop last citat
        filepath[counter++] = '\\';
        strcat(filepath, filename);   

        //Execute file
        std::system(filepath); 
    #else
        //Using ShellExecute()
        //--------------------

        HINSTANCE hInst = ShellExecute( 0,          // Parent window
                                        "open",     // Operation to perform
                                        filename,   // Application name
                                        0,          // Additional parameters
                                        0,          // Default directory
                                        SW_SHOW);
    #endif

}