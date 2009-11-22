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

#include "RSPMatrixManager.h"
#include "Memory.h"
#include <cmath>      //modff
#include "GBI.h"      //SHIFT
#include "GBIDefs.h"  //_FIXED2FLOAT
//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
RSPMatrixManager::RSPMatrixManager()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
RSPMatrixManager::~RSPMatrixManager()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//-----------------------------------------------------------------------------
bool RSPMatrixManager::initialize(Memory* memory)
{
    m_memory = memory;
    m_rdramOffset = 0;
    return true;
}

//-----------------------------------------------------------------------------
//! Add Matrix
//! @param segmentAddress Later converted to RDRam address pointing to Matrix to load
//! @param projectionMatrix True if matrix is a projection matrix, 
//!                         False if matrix is a modelview matrix
//! @param push True if adding a new matrix to stack(and saving the old one)
//! @param replace True if loading matrix (glLoadMatrix), false if multiplying with the previus matrix (glMultMatrix)
//-----------------------------------------------------------------------------
void RSPMatrixManager::addMatrix(unsigned int segmentAddress, bool projectionMatrix, bool push, bool replace)
{
    unsigned int rdramAddress = m_memory->getRDRAMAddress(segmentAddress);

    if (rdramAddress + 64 > m_memory->getRDRAMSize() ) {
        return;
    }

    Matrix4 temp;
    _loadMatrix(rdramAddress, temp);
    
    if ( projectionMatrix )
    {
        _setProjection(temp, push, replace);
    }
    else
    {
        _setWorldView(temp, push, replace);
    }

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! Insert Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::insertMatrix(unsigned int where, unsigned int num) 
{
    float fraction, integer;

    _updateCombinedMatrix();

    if ((where & 0x3) || (where > 0x3C))
    {
        return;
    }

    if (where < 0x20)
    {
        fraction = modff( m_worldProject[0][where >> 1], &integer );
        m_worldProject[0][where >> 1] = (short)_SHIFTR( num, 16, 16 ) + fabs( fraction );

        fraction = modff( m_worldProject[0][(where >> 1) + 1], &integer );
        m_worldProject[0][(where >> 1) + 1] = (short)_SHIFTR( num, 0, 16 ) + fabs( fraction );
    }
    else
    {
        float newValue;

        fraction = modff( m_worldProject[0][(where - 0x20) >> 1], &integer );
        newValue = integer + _FIXED2FLOAT( _SHIFTR( num, 16, 16 ), 16);

        // Make sure the sign isn't lost
        if ((integer == 0.0f) && (fraction != 0.0f))
            newValue = newValue * (fraction / fabs( fraction ));

        m_worldProject[0][(where - 0x20) >> 1] = newValue;

        fraction = modff( m_worldProject[0][((where - 0x20) >> 1) + 1], &integer );
        newValue = integer + _FIXED2FLOAT( _SHIFTR( num, 0, 16 ), 16 );

        // Make sure the sign isn't lost
        if ((integer == 0.0f) && (fraction != 0.0f))
            newValue = newValue * (fraction / fabs( fraction ));

        m_worldProject[0][((where - 0x20) >> 1) + 1] = newValue;
    }
}
void RSPMatrixManager::ForceMatrix(unsigned int rdramAddress)
{
    _loadMatrix(rdramAddress, m_worldProject);
}
        


//-----------------------------------------------------------------------------
// Pop Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::popMatrix()
{
    if ( m_modelViewMatrixTop > 0 )
    {
        m_modelViewMatrixTop--;             //Pop Matrix from stack
    }

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
// Pop Matrix N
//-----------------------------------------------------------------------------
void RSPMatrixManager::popMatrixN(unsigned int num)
{
    if ( m_modelViewMatrixTop > num - 1)
    {
        m_modelViewMatrixTop -= num;
    }

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! DMA Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::DMAMatrix( unsigned int rdramAddress, unsigned char index, unsigned char multiply )
{
    //Get final address
    unsigned int address = m_rdramOffset + rdramAddress;

    if (address + 64 > m_memory->getRDRAMSize())
    {
        return;
    }

    //Load Matrix from Memory
    Matrix4 temp;
    _loadMatrix(rdramAddress, temp);
    
    //Set Modelview index
    m_modelViewMatrixTop = index;

    //FIXME: Other way around?
    if (multiply)
    {
        m_modelViewMatrices[m_modelViewMatrixTop] = m_modelViewMatrices[0];  
        m_modelViewMatrices[m_modelViewMatrixTop] = m_modelViewMatrices[m_modelViewMatrixTop] * temp;
    }
    else 
    {
        m_modelViewMatrices[m_modelViewMatrixTop] = temp;
    }

    //Set Projection Matrix to Identity
    m_projectionMatrices[m_projectionMatrixTop] = Matrix4::IDENTITY;

    //Update Matrices
    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! Reset Matrices
//-----------------------------------------------------------------------------
void RSPMatrixManager::resetMatrices()
{
    m_modelViewMatrices[0] = Matrix4::IDENTITY;
    m_projectionMatrices[0] = Matrix4::IDENTITY;

    m_modelViewMatrixTop = 0;
    m_projectionMatrixTop = 0;

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! Load Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::_loadMatrix(unsigned int addr, Matrix4& out)
{
    if ( addr + 64 > m_memory->getRDRAMSize() ) {
        return;
    }

    unsigned char* RDRAM = m_memory->getRDRAM();


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++) 
        {
            int             hi = *(short *)(RDRAM + ((addr+(i<<3)+(j<<1)     )^0x2));
            unsigned short  lo = *(unsigned short  *)(RDRAM + ((addr+(i<<3)+(j<<1) + 32)^0x2));
            out[i][j] = (float)((hi<<16) | (lo))/ 65536.0f;
        }
    }
}

//-----------------------------------------------------------------------------
//! Set Projection Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::_setProjection(const Matrix4& mat, bool push, bool replace) 
{
    Matrix4& oldMatrix = m_projectionMatrices[m_projectionMatrixTop];

    if (push)
    {
        m_projectionMatrixTop++;
    }

    if ( replace )
    {
        m_projectionMatrices[m_projectionMatrixTop] = mat;
    }
    else
    {
        m_projectionMatrices[m_projectionMatrixTop] = mat * oldMatrix;        
    }

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! Set World View Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::_setWorldView(const Matrix4 & mat, bool push, bool replace)
{
    Matrix4& oldMatrix = m_modelViewMatrices[m_modelViewMatrixTop];

    if (push)
    {
        m_modelViewMatrixTop++;
    }

    if ( replace )
    {
        m_modelViewMatrices[m_modelViewMatrixTop] = mat;
    }
    else
    {
        m_modelViewMatrices[m_modelViewMatrixTop] = mat * oldMatrix;        
    }    

    _updateCombinedMatrix();
}

//-----------------------------------------------------------------------------
//! Update Combined Matrix
//-----------------------------------------------------------------------------
void RSPMatrixManager::_updateCombinedMatrix()
{
    m_worldProject = m_modelViewMatrices[m_modelViewMatrixTop] * m_projectionMatrices[m_projectionMatrixTop];
}
