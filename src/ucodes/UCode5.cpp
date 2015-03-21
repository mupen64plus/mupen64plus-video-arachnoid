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
#include "Logger.h"
#include "Memory.h"
#include "RDP.h"
#include "RSP.h"
#include "UCode0.h"
#include "UCode5.h"
#include "UCodeDefs.h"


#define F3DEX2_MTX_STACKSIZE       18

#define F3DEX2_MTX_MODELVIEW       0x00
#define F3DEX2_MTX_PROJECTION      0x04
#define F3DEX2_MTX_MUL             0x00
#define F3DEX2_MTX_LOAD            0x02
#define F3DEX2_MTX_NOPUSH          0x00
#define F3DEX2_MTX_PUSH            0x01

#define F3DEX2_TEXTURE_ENABLE      0x00000000
#define F3DEX2_SHADING_SMOOTH      0x00200000
#define F3DEX2_CULL_FRONT          0x00000200
#define F3DEX2_CULL_BACK           0x00000400
#define F3DEX2_CULL_BOTH           0x00000600
#define F3DEX2_CLIPPING            0x00800000

#define F3DEX2_MV_VIEWPORT         8

#define F3DEX2_MWO_aLIGHT_1        0x00
#define F3DEX2_MWO_bLIGHT_1        0x04
#define F3DEX2_MWO_aLIGHT_2        0x18
#define F3DEX2_MWO_bLIGHT_2        0x1c
#define F3DEX2_MWO_aLIGHT_3        0x30
#define F3DEX2_MWO_bLIGHT_3        0x34
#define F3DEX2_MWO_aLIGHT_4        0x48
#define F3DEX2_MWO_bLIGHT_4        0x4c
#define F3DEX2_MWO_aLIGHT_5        0x60
#define F3DEX2_MWO_bLIGHT_5        0x64
#define F3DEX2_MWO_aLIGHT_6        0x78
#define F3DEX2_MWO_bLIGHT_6        0x7c
#define F3DEX2_MWO_aLIGHT_7        0x90
#define F3DEX2_MWO_bLIGHT_7        0x94
#define F3DEX2_MWO_aLIGHT_8        0xa8
#define F3DEX2_MWO_bLIGHT_8        0xac


#define    F3DEX2_RDPHALF_2        0xF1
#define    F3DEX2_SETOTHERMODE_H   0xE3
#define    F3DEX2_SETOTHERMODE_L   0xE2
#define    F3DEX2_RDPHALF_1        0xE1
#define    F3DEX2_SPNOOP           0xE0
#define    F3DEX2_ENDDL            0xDF
#define    F3DEX2_DL               0xDE
#define    F3DEX2_LOAD_UCODE       0xDD
#define    F3DEX2_MTX              0xDA
#define F3DEX2_GEOMETRYMODE        0xD9
#define    F3DEX2_POPMTX           0xD8
#define    F3DEX2_TEXTURE          0xD7
#define    F3DEX2_DMA_IO           0xD6
#define    F3DEX2_SPECIAL_1        0xD5
#define    F3DEX2_SPECIAL_2        0xD4
#define    F3DEX2_SPECIAL_3        0xD3

#define    F3DEX2_VTX              0x01
#define    F3DEX2_MODIFYVTX        0x02
#define    F3DEX2_CULLDL           0x03
#define    F3DEX2_BRANCH_Z         0x04
#define    F3DEX2_TRI1             0x05
#define F3DEX2_TRI2                0x06
#define F3DEX2_QUAD                0x07
#define F3DEX2_LINE3D              0x08


//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
RSP*    UCode5::m_rsp    = 0;   //!< Pointer to Reality Signal Processor 
RDP*    UCode5::m_rdp    = 0;   //!< Pointer to Reality Drawing Processor 
Memory* UCode5::m_memory = 0;
GBI*    UCode5::m_gbi    = 0;
DisplayListParser* UCode5::m_displayListParser = 0;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
UCode5::UCode5()
{
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
UCode5::~UCode5()
{
}

//-----------------------------------------------------------------------------
//! Initialize
//-----------------------------------------------------------------------------
void UCode5::initialize(GBI* gbi, RSP* rsp, RDP* rdp, Memory* memory, DisplayListParser* dlp)
{
    m_gbi = gbi;
    m_rsp = rsp;
    m_rdp = rdp;
    m_memory = memory;
    m_displayListParser = dlp;
}

//-----------------------------------------------------------------------------
//! Initialize GBI
//-----------------------------------------------------------------------------
void UCode5::initializeGBI()
{
    //Set flags
    GBI_InitFlags( F3DEX2 );


    // GBI Command                        Command Value                Command Function
    GBI_SetGBI( GBI::G_RDPHALF_2,         F3DEX2_RDPHALF_2,            m_gbi->m_cmds,    UCode0::F3D_RDPHalf_2 );
    GBI_SetGBI( GBI::G_SETOTHERMODE_H,    F3DEX2_SETOTHERMODE_H,       m_gbi->m_cmds,    F3DEX2_SetOtherMode_H );
    GBI_SetGBI( GBI::G_SETOTHERMODE_L,    F3DEX2_SETOTHERMODE_L,       m_gbi->m_cmds,    F3DEX2_SetOtherMode_L );
    GBI_SetGBI( GBI::G_RDPHALF_1,         F3DEX2_RDPHALF_1,            m_gbi->m_cmds,    UCode0::F3D_RDPHalf_1 );
    GBI_SetGBI( GBI::G_SPNOOP,            F3DEX2_SPNOOP,               m_gbi->m_cmds,    UCode0::F3D_SPNoOp );
    GBI_SetGBI( GBI::G_ENDDL,             F3DEX2_ENDDL,                m_gbi->m_cmds,    UCode0::F3D_EndDL );
    GBI_SetGBI( GBI::G_DL,                F3DEX2_DL,                   m_gbi->m_cmds,    UCode0::F3D_DList );
    GBI_SetGBI( GBI::G_LOAD_UCODE,        F3DEX2_LOAD_UCODE,           m_gbi->m_cmds,    UCode1::F3DEX_Load_uCode );
    GBI_SetGBI( GBI::G_MOVEMEM,           F3DEX2_MOVEMEM,              m_gbi->m_cmds,    F3DEX2_MoveMem );
    GBI_SetGBI( GBI::G_MOVEWORD,          F3DEX2_MOVEWORD,             m_gbi->m_cmds,    F3DEX2_MoveWord );
    GBI_SetGBI( GBI::G_MTX,               F3DEX2_MTX,                  m_gbi->m_cmds,    F3DEX2_Mtx );
    GBI_SetGBI( GBI::G_GEOMETRYMODE,      F3DEX2_GEOMETRYMODE,         m_gbi->m_cmds,    F3DEX2_GeometryMode );
    GBI_SetGBI( GBI::G_POPMTX,            F3DEX2_POPMTX,               m_gbi->m_cmds,    F3DEX2_PopMtx );
    GBI_SetGBI( GBI::G_TEXTURE,           F3DEX2_TEXTURE,              m_gbi->m_cmds,    F3DEX2_Texture );
    GBI_SetGBI( GBI::G_DMA_IO,            F3DEX2_DMA_IO,               m_gbi->m_cmds,    F3DEX2_DMAIO );
    GBI_SetGBI( GBI::G_SPECIAL_1,         F3DEX2_SPECIAL_1,            m_gbi->m_cmds,    F3DEX2_Special_1 );
    GBI_SetGBI( GBI::G_SPECIAL_2,         F3DEX2_SPECIAL_2,            m_gbi->m_cmds,    F3DEX2_Special_2 );
    GBI_SetGBI( GBI::G_SPECIAL_3,         F3DEX2_SPECIAL_3,            m_gbi->m_cmds,    F3DEX2_Special_3 );
    GBI_SetGBI( GBI::G_VTX,               F3DEX2_VTX,                  m_gbi->m_cmds,    F3DEX2_Vtx );
    GBI_SetGBI( GBI::G_MODIFYVTX,         F3DEX2_MODIFYVTX,            m_gbi->m_cmds,    UCode1::F3DEX_ModifyVtx );
    GBI_SetGBI(    GBI::G_CULLDL,         F3DEX2_CULLDL,               m_gbi->m_cmds,    UCode1::F3DEX_CullDL );
    GBI_SetGBI( GBI::G_BRANCH_Z,          F3DEX2_BRANCH_Z,             m_gbi->m_cmds,    UCode1::F3DEX_Branch_Z );
    GBI_SetGBI( GBI::G_TRI1,              F3DEX2_TRI1,                 m_gbi->m_cmds,    F3DEX2_Tri1 );
    GBI_SetGBI( GBI::G_TRI2,              F3DEX2_TRI2,                 m_gbi->m_cmds,    UCode1::F3DEX_Tri2 );
    GBI_SetGBI( GBI::G_QUAD,              F3DEX2_QUAD,                 m_gbi->m_cmds,    F3DEX2_Quad );
    GBI_SetGBI( GBI::G_LINE3D,            F3DEX2_LINE3D,               m_gbi->m_cmds,    F3DEX2_Line3D );
}

//*****************************************************************************
// Matrices
//*****************************************************************************

//-----------------------------------------------------------------------------
//! Add Matrix
//! @todo Bomberman2 hack
//! @todo South park rally hack
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Mtx( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Mtx", M64MSG_VERBOSE);
    RSPUCodeAddMatrixF3DEX2* temp = (RSPUCodeAddMatrixF3DEX2*)ucode;

    //TODO Bomberman2 hack
    if( temp->param == 0 && temp->lenght == 0 )
    {
        //Bomberman2TextRect(ucode);
        Logger::getSingleton().printMsg("F3DEX2_Mtx - Bomberman2TextRect - Unimplemented", M64MSG_WARNING);
        return;
    }

    //Add Matrix
    m_rsp->RSP_Matrix( temp->segmentAddress, temp->projection, !temp->nopush, temp->load );

    //For Conker Bad Fur Day
    m_rsp->getVertexMgr()->setConkerAddress(0);

    //TODO South park rally hack
}

//-----------------------------------------------------------------------------
//! Pop Matrix
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_PopMtx( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_PopMtx", M64MSG_VERBOSE);

    m_rsp->RSP_PopMatrixN( ucode->w1 >> 6 );
}

//*****************************************************************************
//Textures
//*****************************************************************************

//-----------------------------------------------------------------------------
//! Set Texture
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Texture( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Texture", M64MSG_VERBOSE);

    float scaleS  = _FIXED2FLOAT( _SHIFTR( ucode->w1, 16, 16 ), 16 ); 
    float scaleT  = _FIXED2FLOAT( _SHIFTR( ucode->w1, 0, 16 ), 16 ); 
    int level    = _SHIFTR( ucode->w0, 11, 3 );
    int tile     = _SHIFTR( ucode->w0, 8, 3 );
    int on       = _SHIFTR( ucode->w0, 1, 7 );

    m_rsp->RSP_Texture(scaleS, scaleT, level, tile, on);
}

//*****************************************************************************
// Vertices and indices
//*****************************************************************************

//-----------------------------------------------------------------------------
// Vertices
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Vtx( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Vtx", M64MSG_VERBOSE);
    RSPUCodeAddVerticesF3DEX2* temp = (RSPUCodeAddVerticesF3DEX2*)ucode;

    //Add Vertices
    m_rsp->RSP_Vertex(temp->segmentAddress, temp->numVertices, temp->vertexEnd - temp->numVertices);
}

//-----------------------------------------------------------------------------
// Add 1 Triangle
//! @todo Add triangle flag
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Tri1( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Tri1", M64MSG_VERBOSE);
    RSPUCodeAddOneTriangleF3DEX2* temp = (RSPUCodeAddOneTriangleF3DEX2*)ucode;

    //Add one triangle
    m_rsp->RSP_1Triangle( temp->index0 / 2, temp->index1 / 2, temp->index2 / 2); //TODO flag
}

//-----------------------------------------------------------------------------
// Add 1 Quad
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Quad( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Quad", M64MSG_VERBOSE);
    RSPUCodeAddOneQuadF3DEX2* temp = (RSPUCodeAddOneQuadF3DEX2*)ucode;

    //Add two triangles
    m_rsp->RSP_2Triangles( temp->v0 / 2, temp->v1 / 2, temp->v2 / 2, 0,
                           temp->v3 / 2, temp->v4 / 2, temp->v5 / 2, 0 );
}

//-----------------------------------------------------------------------------
// Render Line 3D
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_Line3D( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_Line3D - Unimplemented", M64MSG_WARNING);
}


//*****************************************************************************
// Misc
//*****************************************************************************

//-----------------------------------------------------------------------------
// MoveMem
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_MoveMem( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_MoveMem", M64MSG_VERBOSE);

    switch (_SHIFTR( ucode->w0, 0, 8 ))
    {
        case F3DEX2_MV_VIEWPORT:
            m_rsp->moveMemViewport( ucode->w1 );
            break;
        case G_MV_MATRIX:
            m_rsp->RSP_ForceMatrix( ucode->w1 );
            m_displayListParser->increasePC(8);        // force matrix takes two commands
            break;
        case G_MV_LIGHT:
            /*
            unsigned int dwOffset2 = ((ucode->w0) >> 5) & 0x3FFF;

            switch (dwOffset2)
            {
            case 0x00:
            {
                s8 * pcBase = g_pRDRAMs8 + addr;
                LOG_UCODE("    RSP_GBI1_MV_MEM_LOOKATX %f %f %f",
                    (float)pcBase[8 ^ 0x3],
                    (float)pcBase[9 ^ 0x3],
                    (float)pcBase[10 ^ 0x3]);

            }
            break;
            case 0x18:
                {
                    s8 * pcBase = g_pRDRAMs8 + addr;
                    LOG_UCODE("    RSP_GBI1_MV_MEM_LOOKATY %f %f %f",
                        (float)pcBase[8 ^ 0x3],
                        (float)pcBase[9 ^ 0x3],
                        (float)pcBase[10 ^ 0x3]);
                }
                break;
            default:        //0x30/48/60
                {
                    uint32 dwLight = (dwOffset2 - 0x30)/0x18;
                    LOG_UCODE("    Light %d:", dwLight);
                        RSP_MoveMemLight(dwLight, addr);
                }
                break;
            }
            break;
            */
            unsigned int offset = _SHIFTR( ucode->w0, 8, 8 ) << 3;

            if (offset >= 48)
            {
                m_rsp->RSP_Light( ((offset - 24) / 24) - 1, ucode->w1);
            }
            else
            {
                static bool warned = false;
                if ( !warned ) {
                    Logger::getSingleton().printMsg("F3DEX2_MoveMem - Light - Unimplemented", M64MSG_WARNING);
                    warned = true;
                }
            }
            
            break;
    }
}

//-----------------------------------------------------------------------------
// Move Word
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_MoveWord(MicrocodeArgument* ucode) 
{
    Logger::getSingleton().printMsg("F3DEX2_MoveWord", M64MSG_VERBOSE);
    RSPUCodeMoveWordF3DEX2* temp = (RSPUCodeMoveWordF3DEX2*)ucode;

    switch ( temp->type )
    {
        case G_MW_FORCEMTX:
            Logger::getSingleton().printMsg("ForceMatrix - Unimplemented", M64MSG_WARNING);  // Handled in movemem???            
            break;

        case G_MW_MATRIX:
            m_rsp->RSP_InsertMatrix( _SHIFTR( ucode->w0, 0, 16 ), ucode->w1 );            
            break;

        case G_MW_NUMLIGHT: 
            m_rsp->RSP_NumLights(temp->value / 24); 
            break;
    
        case G_MW_SEGMENT:
            m_rsp->moveSegment(temp->offset>>2, temp->value & 0x00FFFFFF);
            break;

        case G_MW_CLIP:
            //gSPClipRatio( ucode->w1 );
            break;

        case G_MW_FOG:
            m_rsp->RSP_FogFactor(  (short)temp->fm, (short)temp->fo);
            break;

        case G_MW_PERSPNORM:
            //gSPPerspNormalize( w1 );
            break;

        case G_MW_LIGHTCOL:
            if ( (temp->offset & 0x7) == 0 )
            {
                m_rsp->RSP_LightColor(temp->offset / 0x18, temp->value);
            }
            break;
    }
}

//-----------------------------------------------------------------------------
// Geometry Mode
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_GeometryMode( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_GeometryMode", M64MSG_VERBOSE);
                             //clear                              set
    m_rsp->RSP_GeometryMode( ~_SHIFTR( ucode->w0, 0, 24 ), ucode->w1 );
}

//-----------------------------------------------------------------------------
//* Set Other Mode H
//! @todo more case in switch
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_SetOtherMode_H( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_SetOtherMode_H", M64MSG_VERBOSE);

    switch (32 - _SHIFTR( ucode->w0, 8, 8 ) - (_SHIFTR( ucode->w0, 0, 8 ) + 1))
    {
        case G_MDSFT_CYCLETYPE:  m_rdp->setCycleType( ucode->w1 >> G_MDSFT_CYCLETYPE );    break;
        case G_MDSFT_TEXTLUT:    m_rdp->setTextureLUT( ucode->w1 >> G_MDSFT_TEXTLUT );     break;

        //TODO
        case G_MDSFT_PIPELINE:     //m_rdp->setPiplineMode( w1 >> G_MDSFT_PIPELINE ); 
            break;
        case G_MDSFT_TEXTPERSP:    //m_rdp->setTexturePerspective( w1 >> G_MDSFT_TEXTPERSP );
            break;
        case G_MDSFT_TEXTDETAIL:   //m_rdp->setTextureDetail( w1 >> G_MDSFT_TEXTDETAIL );
            break;
        case G_MDSFT_TEXTLOD:      //gDPSetTextureLOD( w1 >> G_MDSFT_TEXTLOD );
            break;        
        case G_MDSFT_TEXTFILT:     //gDPSetTextureFilter( w1 >> G_MDSFT_TEXTFILT );
            break;
        case G_MDSFT_TEXTCONV:     //gDPSetTextureConvert( w1 >> G_MDSFT_TEXTCONV );
            break;
        case G_MDSFT_COMBKEY:      //gDPSetCombineKey( w1 >> G_MDSFT_COMBKEY );
            break;
        case G_MDSFT_RGBDITHER:    //gDPSetColorDither( w1 >> G_MDSFT_RGBDITHER );
            break;
        case G_MDSFT_ALPHADITHER:  //gDPSetAlphaDither( w1 >> G_MDSFT_ALPHADITHER );
            break;
        default:
            Logger::getSingleton().printMsg("F3DEX2_SetOtherMode_L - Unknown type", M64MSG_WARNING);

            unsigned int length = _SHIFTR( ucode->w0, 0, 8 ) + 1;
            unsigned int shift = 32 - _SHIFTR( ucode->w0, 8, 8 ) - length;
            unsigned int mask = ((1 << length) - 1) << shift;

            m_rdp->m_otherMode.h &= ~mask;
            m_rdp->m_otherMode.h |= ucode->w1 & mask;

            m_rdp->setUpdateCombiner(true);
            break;
    }
}

//-----------------------------------------------------------------------------
// Set Other Mode L
//-----------------------------------------------------------------------------
void UCode5::F3DEX2_SetOtherMode_L( MicrocodeArgument* ucode ) 
{
    Logger::getSingleton().printMsg("F3DEX2_SetOtherMode_L", M64MSG_VERBOSE);

    switch (32 - _SHIFTR( ucode->w0, 8, 8 ) - (_SHIFTR( ucode->w0, 0, 8 ) + 1))
    {
        case G_MDSFT_ALPHACOMPARE:
            m_rdp->setAlphaCompareMode(ucode->w1 >> G_MDSFT_ALPHACOMPARE);
            break;
        case G_MDSFT_ZSRCSEL:
            m_rdp->setDepthSource( ucode->w1 >> G_MDSFT_ZSRCSEL );
            break;
        case G_MDSFT_RENDERMODE:
            m_rdp->setRenderMode(  ucode->w1 );
            break;
        default:
            unsigned int length = _SHIFTR( ucode->w0, 0, 8 ) + 1;
            unsigned int shift = 32 - _SHIFTR( ucode->w0, 8, 8 ) - length;
            unsigned int mask = ((1 << length) - 1) << shift;
            m_rdp->m_otherMode.l &= ~mask;
            m_rdp->m_otherMode.l |= ucode->w1 & mask;
            break;
    }
}

//*****************************************************************************
// Other
//*****************************************************************************

void UCode5::F3DEX2_DMAIO( MicrocodeArgument* ucode ) {
    Logger::getSingleton().printMsg("F3DEX2_DMAIO", M64MSG_VERBOSE);
    //Ignore?
}
void UCode5::F3DEX2_Special_1( MicrocodeArgument* ucode ) {
    Logger::getSingleton().printMsg("F3DEX2_Special_1", M64MSG_VERBOSE);
    //Ignore?
}
void UCode5::F3DEX2_Special_2( MicrocodeArgument* ucode ) {
    Logger::getSingleton().printMsg("F3DEX2_Special_2", M64MSG_VERBOSE);
    //Ignore?
}
void UCode5::F3DEX2_Special_3( MicrocodeArgument* ucode ) {
    Logger::getSingleton().printMsg("F3DEX2_Special_3", M64MSG_VERBOSE);
    //Ignore?
}

//*****************************************************************************
// Unimportant Functions
//*****************************************************************************
void UCode5::F3DEX2_Reserved1(MicrocodeArgument* ucode) {
    Logger::getSingleton().printMsg("F3DEX2_Reserved1", M64MSG_VERBOSE);
    //Ignore
}
