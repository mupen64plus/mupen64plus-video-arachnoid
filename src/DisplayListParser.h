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

#ifndef DISPLAYLIST_PARSER_H_
#define DISPLAYLIST_PARSER_H_

//Forward declaration
class Memory;
class RSP;
class RDP;
class GBI;

#define MAX_DL_COUNT               100000        //!< Maximum display list count
#define TASK_ADDRESS_RELATIVE_DMEM 0x0FC0
//-----------------------------------------------------------------------------
//! The display list PC stack.
//-----------------------------------------------------------------------------
typedef struct 
{
    unsigned int pc;
    int countdown;
} DListStack;

//*****************************************************************************
//* DisplayListParser
//! Class for parsing and managing the displaylist
//*****************************************************************************
class DisplayListParser
{
public:
    //Constructor / Destructor
    DisplayListParser();
    ~DisplayListParser();

    //Initialize
    bool initialize(RSP* rsp, RDP* rdp, GBI* gbi, Memory* memory);

    //Process/Parse the display list
    void processDisplayList();

    //Display list funcitons
    void branchDisplayList(unsigned int dl);    
    void displayList(unsigned int segmentAddress);
    void DMADisplayList( unsigned int w0, unsigned int w1 );
    //! End display list
    void endDisplayList() { --m_DListStackPointer; }

    //! Get Program Counter
    unsigned int getPC()        { return m_DlistStack[m_DListStackPointer].pc;       }

    //! Set Program Counter
    void setPC(unsigned int pc) { m_DlistStack[m_DListStackPointer].pc = pc;         }

    //! Increase Program Counter
    void increasePC(int increment)  { m_DlistStack[m_DListStackPointer].pc += increment; }

    //Get Next Word
    unsigned int getNextWord();

    //! Get Current Display List
    DListStack& getCurrentDlist() { return m_DlistStack[m_DListStackPointer]; }

private:

    //Pointers
    RSP*    m_rsp;     //! Pointer to Reality Signal Processor
    RDP*    m_rdp;     //! Pointer to Reality Drawing Processor
    GBI*    m_gbi;     //! Pointer to Graphics Binary Interface
    Memory* m_memory;  //! Pointer to Memory
    
    //Stack used for processing the Display List
    int m_DListStackPointer;                      //!< Current size of Display List stack 
    static const int MAX_DL_STACK_SIZE = 32;      //!< Maximum size of Display List stack 
    DListStack m_DlistStack[MAX_DL_STACK_SIZE];   //!< Stack used for processing the Display List
};

#endif
