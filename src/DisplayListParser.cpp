/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
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

#include "DisplayListParser.h"

#include "GBI.h"
#include "GBIDefs.h"
#include "Memory.h"
#include "OpenGLRenderer.h"
#include "RDP.h"
#include "RSP.h"
#include "UCodeDefs.h"

//-----------------------------------------------------------------------------
//* Task
//! Definition of tasks that needs to be performed  by the RSP
//! Used to process the dlist by the RSP (Reality Signal Processor)
//! Placed in DMEM at adress 0x04000fc0
//-----------------------------------------------------------------------------
typedef struct 
{
    unsigned int type;
    unsigned int flags;
    unsigned int ucode_boot;
    unsigned int ucode_boot_size;
    unsigned int ucode;
    unsigned int ucode_size;
    unsigned int ucode_data;
    unsigned int ucode_data_size;
    unsigned int dram_stack;
    unsigned int dram_stack_size;
    unsigned int output_buff;
    unsigned int output_buff_size;
    unsigned int data_ptr;
    unsigned int data_size;
    unsigned int yield_data_ptr;
    unsigned int yield_data_size;
} Task_t;

typedef union {
    Task_t t;
    unsigned long long force_structure_alignment;
} Task;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
DisplayListParser::DisplayListParser()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
DisplayListParser::~DisplayListParser()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
bool DisplayListParser::initialize(RSP* rsp, RDP* rdp, GBI* gbi, Memory* memory)
{
    //Save pointers
    m_rsp    = rsp;
    m_rdp    = rdp;
    m_gbi    = gbi;
    m_memory = memory;

    //Reset display list
    m_DListStackPointer = 0;
    for (int i=0; i<MAX_DL_STACK_SIZE; ++i)
    {
        m_DlistStack[i].pc = 0;
        m_DlistStack[i].countdown = MAX_DL_COUNT;
    }
    return true;
}

//-----------------------------------------------------------------------------
//* Process Display List
//! Parses the display list
//-----------------------------------------------------------------------------
void DisplayListParser::processDisplayList()
{
    Task* task = (Task*)( m_memory->getDMEM(TASK_ADDRESS_RELATIVE_DMEM));


    //Select UCode
    m_gbi->selectUCode(task->t.ucode, task->t.ucode_data, task->t.ucode_size, task->t.ucode_data_size);

    //Parse DList
    m_DListStackPointer = 0;
    m_DlistStack[m_DListStackPointer].pc = (unsigned int)task->t.data_ptr;
    m_DlistStack[m_DListStackPointer].countdown = MAX_DL_COUNT;

    // The main loop
    while( m_DListStackPointer >= 0 )
    {
        //Cast memory pointer
        //uint32* RDRAMu32            = (unsigned int*)graphicsInfo.RDRAM;
        unsigned int* RDRAMu32 = m_memory->getRDRAMint32();            

        //Get ucode argument from memory (vertices, textures, matrices...)
        MicrocodeArgument* ucodeArg = (MicrocodeArgument*)&RDRAMu32[(m_DlistStack[m_DListStackPointer].pc>>2)];

        //Increment program counter
        m_DlistStack[m_DListStackPointer].pc += 8;

        //Call function to execute command
        m_gbi->m_cmds[(ucodeArg->cmd)](ucodeArg);

        //Get next command        
        MicrocodeArgument* ucodeNext =  (MicrocodeArgument*)&RDRAMu32[(m_DlistStack[m_DListStackPointer].pc>>2)];
        
        //If this was a rendering command
        if ( ucodeArg->cmd == GBI::G_TRI1 ||  
             ucodeArg->cmd == GBI::G_TRI2 || 
             ucodeArg->cmd == GBI::G_TRI4 || 
             ucodeArg->cmd == GBI::G_QUAD || 
             ucodeArg->cmd == GBI::G_DMA_TRI ) 
        {
            //If next is not a rendering command
            if (  ucodeNext->cmd != GBI::G_TRI1 &&  
                  ucodeNext->cmd != GBI::G_TRI2 && 
                  ucodeNext->cmd != GBI::G_TRI4 && 
                  ucodeNext->cmd != GBI::G_QUAD && 
                  ucodeNext->cmd != GBI::G_DMA_TRI ) 
            {
                OpenGLRenderer::getSingleton().render();
            }
        }

        //??
        if ( m_DListStackPointer >= 0 && --m_DlistStack[m_DListStackPointer].countdown < 0 )
        {
            m_DListStackPointer--;
        }
    }

    //Trigger interupts
    m_rdp->triggerInterrupt();
    m_rsp->triggerInterrupt();
}

//-----------------------------------------------------------------------------
//! Get next word
//-----------------------------------------------------------------------------
unsigned int DisplayListParser::getNextWord()
{
    unsigned int word = *(unsigned int*)m_memory->getRDRAM( this->getPC() + 4 );
    this->increasePC(8);
    return word;
}

//-----------------------------------------------------------------------------
//!< @param segmentAddress Used to retrive RDRAM address witch is used to set program counter
//-----------------------------------------------------------------------------
void DisplayListParser::displayList(unsigned int segmentAddress)
{
    unsigned int address = m_memory->getRDRAMAddress(segmentAddress);

    if ( (address + 8) > m_memory->getRDRAMSize() ) {
        return;
    }  

    if ( m_DListStackPointer < (MAX_DL_STACK_SIZE - 1))
    {
        m_DListStackPointer++;
        m_DlistStack[m_DListStackPointer].pc = address;
        m_DlistStack[m_DListStackPointer].countdown = MAX_DL_COUNT;
    }
}

//-----------------------------------------------------------------------------
//! Branch Display List
//-----------------------------------------------------------------------------
void DisplayListParser::branchDisplayList(unsigned int dl)
{
    unsigned int address = m_memory->getRDRAMAddress( dl );

    if ( (address + 8) > m_memory->getRDRAMSize() ) {
        return;
    }  

    m_DlistStack[m_DListStackPointer].pc = address;
    m_DlistStack[m_DListStackPointer].countdown = MAX_DL_COUNT;
}

//-----------------------------------------------------------------------------
//! DMA Display List
//-----------------------------------------------------------------------------
void DisplayListParser::DMADisplayList( unsigned int w0, unsigned int w1 )
{
    //unsigned int dwAddr = (w1);//RSPSegmentAddr((gfx->words.w1));
    unsigned int dwAddr = m_memory->getRDRAMAddress(w1);//RSPSegmentAddr((gfx->words.w1));

    {
        m_DListStackPointer++;
        m_DlistStack[m_DListStackPointer].pc = dwAddr;
        m_DlistStack[m_DListStackPointer].countdown = (((w0)>>16)&0xFF);
    }
}
