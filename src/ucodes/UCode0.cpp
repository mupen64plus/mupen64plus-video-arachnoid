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

#include "UCode0.h"
#include "GBI.h"
#include "RSP.h"
#include "RDP.h"
#include "Memory.h"
#include "UCodeDefs.h"
#include "GBIDefs.h"
#include "Logger.h"
#include "DisplayListParser.h"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP* UCode0::m_rsp = 0;   //!< Pointer to Reality Signal Processor 
RDP* UCode0::m_rdp = 0;   //!< Pointer to Reality Drawing Processor 
DisplayListParser* UCode0::m_displayListParser = 0;
Memory* UCode0::m_memory = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode0::UCode0()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode0::~UCode0()
{
}

//-----------------------------------------------------------------------------
// Initialize 
//-----------------------------------------------------------------------------
void UCode0::initialize(RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp)
{
    m_rsp = rsp;
    m_rdp = rdp;
    m_memory = memory;
    m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//* Initialize GBI
//! Assigns functions to the GBI
//-----------------------------------------------------------------------------
void UCode0::initializeGBI(GBI* gbi)
{
    // Set GeometryMode flags
    GBI_InitFlags( F3D );    
    
    //GBI Command                            Command Value         Target          Command Function
    GBI_SetGBI( GBI::G_SPNOOP,               F3D_SPNOOP,           gbi->m_cmds,    F3D_SPNoOp );
    GBI_SetGBI( GBI::G_MTX,                  F3D_MTX,              gbi->m_cmds,    F3D_Mtx );
    GBI_SetGBI( GBI::G_RESERVED0,            F3D_RESERVED0,        gbi->m_cmds,    F3D_Reserved0 );
    GBI_SetGBI( GBI::G_MOVEMEM,              F3D_MOVEMEM,          gbi->m_cmds,    F3D_MoveMem );
    GBI_SetGBI( GBI::G_VTX,                  F3D_VTX,              gbi->m_cmds,    F3D_Vtx );
    GBI_SetGBI( GBI::G_RESERVED1,            F3D_RESERVED1,        gbi->m_cmds,    F3D_Reserved1 );
    GBI_SetGBI( GBI::G_DL,                   F3D_DL,               gbi->m_cmds,    F3D_DList );
    GBI_SetGBI( GBI::G_RESERVED2,            F3D_RESERVED2,        gbi->m_cmds,    F3D_Reserved2 );
    GBI_SetGBI( GBI::G_RESERVED3,            F3D_RESERVED3,        gbi->m_cmds,    F3D_Reserved3 );
    GBI_SetGBI( GBI::G_SPRITE2D_BASE,        F3D_SPRITE2D_BASE,    gbi->m_cmds,    F3D_Sprite2D_Base );
    GBI_SetGBI( GBI::G_MOVEWORD,             F3D_MOVEWORD,         gbi->m_cmds,    F3D_MoveWord );
    GBI_SetGBI( GBI::G_TRI1,                 F3D_TRI1,             gbi->m_cmds,    F3D_Tri1 );
    GBI_SetGBI( GBI::G_CULLDL,               F3D_CULLDL,           gbi->m_cmds,    F3D_CullDL );
    GBI_SetGBI( GBI::G_POPMTX,               F3D_POPMTX,           gbi->m_cmds,    F3D_PopMtx );
    GBI_SetGBI( GBI::G_TEXTURE,              F3D_TEXTURE,          gbi->m_cmds,    F3D_Texture );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,       F3D_SETOTHERMODE_H,   gbi->m_cmds,    F3D_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,       F3D_SETOTHERMODE_L,   gbi->m_cmds,    F3D_SetOtherMode_L );
    GBI_SetGBI( GBI::G_ENDDL,                F3D_ENDDL,            gbi->m_cmds,    F3D_EndDL );
    GBI_SetGBI( GBI::G_SETGEOMETRYMODE,      F3D_SETGEOMETRYMODE,  gbi->m_cmds,    F3D_SetGeometryMode );
    GBI_SetGBI( GBI::G_CLEARGEOMETRYMODE,    F3D_CLEARGEOMETRYMODE,gbi->m_cmds,    F3D_ClearGeometryMode );
    GBI_SetGBI( GBI::G_QUAD,                 F3D_QUAD,             gbi->m_cmds,    F3D_Quad );
    GBI_SetGBI( GBI::G_RDPHALF_1,            F3D_RDPHALF_1,        gbi->m_cmds,    F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_RDPHALF_2,            F3D_RDPHALF_2,        gbi->m_cmds,    F3D_RDPHalf_2 );
    GBI_SetGBI( GBI::G_RDPHALF_CONT,         F3D_RDPHALF_CONT,     gbi->m_cmds,    F3D_RDPHalf_Cont );
    GBI_SetGBI( GBI::G_TRI4,                 F3D_TRI4,             gbi->m_cmds,    F3D_Tri4 );
}

//*****************************************************************************
// Matrix Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Add Matrix
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_Mtx(MicrocodeArgument* ucode)
{    
    Logger::getSingleton().printMsg("F3D_Mtx", M64MSG_VERBOSE);
    RSPUCodeAddMatrix0* temp = (RSPUCodeAddMatrix0*)ucode;

    //Add matrix
    m_rsp->RSP_Matrix( temp->segmentAddress, //Segment adress 
                       temp->projection,     //Projection or view matrix?
                       temp->push,           //Save Current Matrix?
                       temp->load );         //Replace aka Load or Mult  
}

//-----------------------------------------------------------------------------
//* Pop Matrix
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_PopMtx(MicrocodeArgument* ucode)
{ 
    Logger::getSingleton().printMsg("F3D_PopMtx", M64MSG_VERBOSE);

    //Pop Matrix
    m_rsp->RSP_PopMatrix();
}

//*****************************************************************************
// Vertex Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Add Vertex
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_Vtx(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Vtx", M64MSG_VERBOSE);
    RSPUCodeAddVertices0* temp = (RSPUCodeAddVertices0*)ucode;

    //Add vertex
    m_rsp->RSP_Vertex(temp->segmentAddress, temp->numVertices + 1, temp->firstVertex); 
}

//-----------------------------------------------------------------------------
//* F3D Render 1 Triangle
//! @param ucode instruction from displaylist with input data
//! @todo Triangle flag?
//-----------------------------------------------------------------------------
void UCode0::F3D_Tri1(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Tri1", M64MSG_VERBOSE);
    RSPUCodeAddOneTriangleF3D* temp = (RSPUCodeAddOneTriangleF3D*)ucode;

    //Add one triangle  //TODO Flag?
    m_rsp->RSP_1Triangle(temp->index0 / 10, temp->index1 / 10, temp->index2 / 10);
}

//-----------------------------------------------------------------------------
//* F3D Render 4 Triangles
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_Tri4(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Tri4", M64MSG_VERBOSE);
    RSPUCodeAddFourTrianglesF3D* temp = (RSPUCodeAddFourTrianglesF3D*)ucode;

    //Add four triangles
    m_rsp->RSP_4Triangles( temp->v0, temp->v1, temp->v2,
                           temp->v3, temp->v4, temp->v5,
                           temp->v6, temp->v7, temp->v8,
                           temp->v9, temp->v10, temp->v11);
}

//-----------------------------------------------------------------------------
//* F3D Render 1 Quad
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_Quad(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Quad", M64MSG_VERBOSE);
    RSPUCodeAddOneQuadF3D* temp = (RSPUCodeAddOneQuadF3D*)ucode;

    //Add one Quad
    m_rsp->RSP_1Quadrangle(temp->index0 / 10, temp->index1 / 10, temp->index2 / 10, temp->index3 / 10);
}

//*****************************************************************************
// Display List Functions
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Display List
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_DList(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_DList", M64MSG_VERBOSE);
    RSPUCodeDisplayList* temp = (RSPUCodeDisplayList*)ucode;

    switch ( temp->param )
    {
        case G_DL_PUSH   : m_rsp->RSP_DisplayList( temp->segmentAddress ); break;
        case G_DL_NOPUSH : m_rsp->RSP_BranchList( temp->segmentAddress );  break;
    }
}

//-----------------------------------------------------------------------------
//* End Display List
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_EndDL(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_EndDL", M64MSG_VERBOSE);

    //End Display List
    m_rsp->RSP_EndDisplayList();    
}

//-----------------------------------------------------------------------------
// Cull Display List
//! @todo Cull Display List
//-----------------------------------------------------------------------------
void UCode0::F3D_CullDL(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_CullDL", M64MSG_VERBOSE);
    //TODO
}

//*****************************************************************************
// Texturing
//*****************************************************************************

//-----------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------
void UCode0::F3D_Texture(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Texture", M64MSG_VERBOSE);
    RSPUCodeTexture* temp = (RSPUCodeTexture*)ucode;

    float scaleS  = _FIXED2FLOAT( _SHIFTR( ucode->w1, 16, 16 ), 16 ); 
    float scaleT  = _FIXED2FLOAT( _SHIFTR( ucode->w1, 0, 16 ), 16 ); 
    int on       = _SHIFTR( ucode->w0, 0, 8 );

    m_rsp->RSP_Texture(scaleS, scaleT, (int)temp->level, (int)temp->tile, on);
}

//*****************************************************************************
// Half
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Half 1
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_RDPHalf_1(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_RDPHalf_1", M64MSG_VERBOSE);
    m_rdp->setHalf1( ucode->w1 );
}

//-----------------------------------------------------------------------------
//* Half 2
//! @param ucode instruction from displaylist with input data
//-----------------------------------------------------------------------------
void UCode0::F3D_RDPHalf_2(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_RDPHalf_2", M64MSG_VERBOSE);
    m_rdp->setHalf2( ucode->w1 );
}
//-----------------------------------------------------------------------------
// Half Cont
//-----------------------------------------------------------------------------
void UCode0::F3D_RDPHalf_Cont(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_RDPHalf_Cont", M64MSG_VERBOSE);
    //Ignore
}

//*****************************************************************************
// Geometry Mode
//*****************************************************************************

//-----------------------------------------------------------------------------
// Set Geometry Mode
//-----------------------------------------------------------------------------
void UCode0::F3D_SetGeometryMode(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_SetGeometryMode", M64MSG_VERBOSE);
    m_rsp->RSP_SetGeometryMode(ucode->w1);
}

//-----------------------------------------------------------------------------
// Clear Geometry Mode
//-----------------------------------------------------------------------------
void UCode0::F3D_ClearGeometryMode(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_ClearGeometryMode", M64MSG_VERBOSE);
    m_rsp->RSP_ClearGeometryMode(ucode->w1);
}

//*****************************************************************************
// Set Other Modes
//*****************************************************************************

//-----------------------------------------------------------------------------
// Set Other Mode H
//-----------------------------------------------------------------------------
void UCode0::F3D_SetOtherMode_H(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_SetOtherMode_H", M64MSG_VERBOSE);

    unsigned int otherModeType = _SHIFTR(ucode->w0, 8, 8);
    unsigned int w1             = ucode->w1;

    switch ( otherModeType )
    {
        case G_MDSFT_PIPELINE:
            //gDPPipelineMode( w1 >> G_MDSFT_PIPELINE );
            break;
        case G_MDSFT_CYCLETYPE:
            m_rdp->setCycleType( G_CYCLE_TYPE(w1 >> G_MDSFT_CYCLETYPE) );
            break;
        
        case G_MDSFT_TEXTPERSP:
            //gDPSetTexturePersp( w1 >> G_MDSFT_TEXTPERSP );
            break;
        case G_MDSFT_TEXTDETAIL:
            //gDPSetTextureDetail( w1 >> G_MDSFT_TEXTDETAIL );
            break;
        case G_MDSFT_TEXTLOD:
            //gDPSetTextureLOD( w1 >> G_MDSFT_TEXTLOD );
            break;        
        case G_MDSFT_TEXTLUT:
            m_rdp->setTextureLUT( w1 >> G_MDSFT_TEXTLUT );
            break;
        case G_MDSFT_TEXTFILT:
            m_rdp->setTextureFiltering(w1 >> G_MDSFT_TEXTFILT);
            break;
        case G_MDSFT_TEXTCONV:
            //gDPSetTextureConvert( w1 >> G_MDSFT_TEXTCONV );
            break;
        case G_MDSFT_COMBKEY:
            //gDPSetCombineKey( w1 >> G_MDSFT_COMBKEY );
            break;
        case G_MDSFT_RGBDITHER:
            //gDPSetColorDither( w1 >> G_MDSFT_RGBDITHER );
            break;
        case G_MDSFT_ALPHADITHER:
            //gDPSetAlphaDither( w1 >> G_MDSFT_ALPHADITHER );
            break;
        default:
            unsigned int shift = _SHIFTR( ucode->w0, 8, 8 );
            unsigned int length = _SHIFTR( ucode->w0, 0, 8 );
            unsigned int mask = ((1 << length) - 1) << shift;
            m_rdp->m_otherMode.h &= ~mask;
            m_rdp->m_otherMode.h |= w1 & mask;
            m_rdp->setUpdateCombiner(true);
            break;
    }
}

//-----------------------------------------------------------------------------
// Set Other Mode L
//-----------------------------------------------------------------------------
void UCode0::F3D_SetOtherMode_L(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_SetOtherMode_L", M64MSG_VERBOSE);
    
    switch (_SHIFTR( ucode->w0, 8, 8 ))
    {
        case G_MDSFT_ALPHACOMPARE:
            m_rdp->setAlphaCompareMode(ucode->w1 >> G_MDSFT_ALPHACOMPARE);
            break;
        case G_MDSFT_ZSRCSEL:
            m_rdp->setDepthSource( ucode->w1 >> G_MDSFT_ZSRCSEL );
            break;
        case G_MDSFT_RENDERMODE:
            m_rdp->setRenderMode(ucode->w1);
            break;
        default:
            unsigned int shift = _SHIFTR( ucode->w0, 8, 8 );
            unsigned int length = _SHIFTR( ucode->w0, 0, 8 );
            unsigned int mask = ((1 << length) - 1) << shift;

            m_rdp->m_otherMode.l &= ~mask;
            m_rdp->m_otherMode.l |= ucode->w1 & mask;
            break;
    }
}


//*****************************************************************************
// Other
//*****************************************************************************

//-----------------------------------------------------------------------------
//* Move Mem
//! @todo Add more case in switch, Like Force Matrix
//-----------------------------------------------------------------------------
void UCode0::F3D_MoveMem(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_MoveMem", M64MSG_VERBOSE);

    unsigned int type    = ((ucode->w0)>>16)&0xFF;
    unsigned int length  = ((ucode->w0))&0xFFFF;
    unsigned int segmentAddress = ucode->w1;

    switch (type) 
    {
        //Viewport
        case F3D_MV_VIEWPORT:               
            m_rsp->moveMemViewport(segmentAddress);
            break;

        case G_MV_MATRIX_1:
            m_rsp->RSP_ForceMatrix(segmentAddress);    
            m_displayListParser->increasePC(24); //// force matrix takes four commands
            break;

        //Lights
        case G_MV_L0: m_rsp->RSP_Light(0, segmentAddress); break;
        case G_MV_L1: m_rsp->RSP_Light(1, segmentAddress); break;
        case G_MV_L2: m_rsp->RSP_Light(2, segmentAddress); break;
        case G_MV_L3: m_rsp->RSP_Light(3, segmentAddress); break;
        case G_MV_L4: m_rsp->RSP_Light(4, segmentAddress); break;
        case G_MV_L5: m_rsp->RSP_Light(5, segmentAddress); break;
        case G_MV_L6: m_rsp->RSP_Light(6, segmentAddress); break;
        case G_MV_L7: m_rsp->RSP_Light(7, segmentAddress); break;

        //Look at
        case G_MV_LOOKATX:
            //IGNORE
            break;
        //Look at
        case G_MV_LOOKATY:
            //IGNORE
            break;

        //case RSP_GBI1_MV_MEM_LOOKATY:
        //    //IGNORE
        //    break;
        //case RSP_GBI1_MV_MEM_LOOKATX:
        //    //IGNORE
        //    break;

        //TODO?
        //case RSP_GBI1_MV_MEM_TXTATT:    break;
        //case RSP_GBI1_MV_MEM_MATRIX_1:  
        //  RSP_GFX_Force_Matrix(addr);
        //    break;    
        //case RSP_GBI1_MV_MEM_MATRIX_2: break;
        //case RSP_GBI1_MV_MEM_MATRIX_3: break;
        //case RSP_GBI1_MV_MEM_MATRIX_4: break;
        
        default:
            Logger::getSingleton().printMsg("F3D_MoveMem: Unknown type", M64MSG_WARNING); 
            break;
    }
}

//-----------------------------------------------------------------------------
//* F3D Sprite2D Base
//! @todo F3D Sprite2D Base
//-----------------------------------------------------------------------------
void UCode0::F3D_Sprite2D_Base(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_Sprite2D_Base - Unimplemented", M64MSG_WARNING);
    //TODO
}

//-----------------------------------------------------------------------------
// Move Word
//-----------------------------------------------------------------------------
void UCode0::F3D_MoveWord(MicrocodeArgument* ucode)
{
    Logger::getSingleton().printMsg("F3D_MoveWord");
    RSPUCodeMoveWordF3D* temp = (RSPUCodeMoveWordF3D*)ucode;

    switch ( temp->type ) 
    {
        case G_MW_MATRIX: 
            m_rsp->RSP_InsertMatrix(temp->offset, temp->value); 
            break;

        case G_MW_FOG:              
            m_rsp->RSP_FogFactor( (short)temp->fm, (short)temp->fo );
            break;

        case G_MW_NUMLIGHT: 
            m_rsp->RSP_NumLights( (unsigned int)(((ucode->w1 - 0x80000000) >> 5) - 1) );            
            break;

        case G_MW_SEGMENT:                                
            m_rsp->moveSegment((temp->offset >> 2) & 0xF, temp->value);              
            break;

        case G_MW_LIGHTCOL: 
            if ( (temp->offset & 0x7) == 0 ) 
            {
                m_rsp->RSP_LightColor(temp->offset / 0x20, temp->value);
            }
            break;

        case G_MW_POINTS:    
            m_rsp->RSP_ModifyVertex( _SHIFTR( ucode->w0, 8, 16 ) / 40, _SHIFTR( ucode->w0, 0, 8 ) % 40, ucode->w1);
            break;

        case G_MW_CLIP: 
            //gSPClipRatio( w1 );
            break;

        case G_MW_PERSPNORM: 
            //gSPPerspNormalize( w1 ); 
            break;
    }    
}

//*****************************************************************************
// Non important functions
//*****************************************************************************

void UCode0::F3D_SPNoOp(MicrocodeArgument* ucode){ 
    Logger::getSingleton().printMsg("F3D_SPNoOp", M64MSG_VERBOSE);   

    //If next command is a no-operation then skip displaylist for some ucodes. 
    //if( (ucode+1)->words.cmd == 0x00 && gRSP.ucode >= 17 )
    //{
    //  m_rsp->RSP_EndDisplayList();       
    //}
}

void UCode0::F3D_Reserved0(MicrocodeArgument* ucode) {
    Logger::getSingleton().printMsg("F3D_Reserved0", M64MSG_VERBOSE);
}

void UCode0::F3D_Reserved1(MicrocodeArgument* ucode){
    Logger::getSingleton().printMsg("F3D_Reserved1", M64MSG_VERBOSE);
}

void UCode0::F3D_Reserved2(MicrocodeArgument* ucode){
    Logger::getSingleton().printMsg("F3D_Reserved2", M64MSG_VERBOSE);
}

void UCode0::F3D_Reserved3(MicrocodeArgument* ucode){
    Logger::getSingleton().printMsg("F3D_Reserved3", M64MSG_VERBOSE);
}
