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

#ifndef RSP_MATRIX_MANAGER_H_
#define RSP_MATRIX_MANAGER_H_

#include "Matrix4.h"

//Forward Declarations
class Memory;

//*****************************************************************************
//! RSP Matrix Manager
//*****************************************************************************
class RSPMatrixManager
{
public:

    //Constructor / Destructor
    RSPMatrixManager();
    ~RSPMatrixManager();

    bool initialize(Memory* memory);

    //Add Matrices
    void addMatrix(unsigned int segmentAddress, bool projectionMatrix, bool push, bool replace);
    void insertMatrix(unsigned int where, unsigned int num);

    //Remove Matrices
    void popMatrix();
    void popMatrixN(unsigned int num);
    void ForceMatrix( unsigned int segmentAddress );
    void selectViewMatrix(unsigned int index) { m_modelViewMatrixTop = index; _updateCombinedMatrix(); }    
    void DMAMatrix(unsigned int segmentAddress, unsigned char index, unsigned char multiply );
    //void RSP_ForceMatrix( unsigned int mptr );
    //void RSP_LookAt( unsigned int l );
    //void RSP_PerspNormalize( unsigned short scale );    

    void setRDRAMOffset(unsigned int offset) { m_rdramOffset = offset; }

    void resetMatrices();

public:

    float* getModelViewMatrix()      { return m_modelViewMatrices[m_modelViewMatrixTop]._m;   }
    float* getProjectionMatrix()     { return m_projectionMatrices[m_projectionMatrixTop]._m; }
    float* getViewProjectionMatrix() { return m_worldProject._m;                              }

private:

    void _loadMatrix(unsigned int addr, Matrix4& out);
    void _setProjection(const Matrix4& mat, bool push, bool replace);
    void _setWorldView(const Matrix4 & mat, bool push, bool replace);
    void _updateCombinedMatrix();

private:

    Memory* m_memory;    //!< Pointer to memory

    static const int NUM_STACK_MATRICES = 60;

    unsigned int m_rdramOffset;

    //Stack indices
    unsigned int m_modelViewMatrixTop;
    unsigned int m_projectionMatrixTop;

    //Matrices
    Matrix4 m_modelViewMatrices[NUM_STACK_MATRICES];   //!< Stack with projection matrices
    Matrix4 m_projectionMatrices[NUM_STACK_MATRICES];  //!< Stack with projection matrices
    Matrix4 m_worldProject;                            //!< Combined modelviewprojection matrix
};

#endif
