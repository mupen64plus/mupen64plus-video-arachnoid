

#ifndef _UCODE_DEFS_H_
#define _UCODE_DEFS_H_

//*****************************************************************************
// Matrices
//*****************************************************************************

//! Add matrix struct used for example ucode F3D
struct RSPUCodeAddMatrix0
{
	unsigned int length:16;       //!< Length
	unsigned int projection:1;    //!< Is this a projection or view matrix?
	unsigned int load:1;          //!< Replace current matrix or multiply with it?
	unsigned int push:1;          //!< Save current matrix in stack? 
	unsigned int :5;              //!< Padding
	unsigned int cmd:8;           //!< Command
	unsigned int segmentAddress;  //!< Address to register where there is an RDRAM address used to retrieve matrix
};

//! Add matrix struct used for example ucode F3DEX2
#pragma pack(push, 1)
struct RSPUCodeAddMatrixF3DEX2 
{
	union 
    {
		struct 
        {
			unsigned int param:8;         //!< Push?, Load?, Projection?
			unsigned int len:16;          //!< Length
		};
        struct
        {
	        unsigned int nopush:1;        //!< Save current matrix in stack? 
	        unsigned int load:1;          //!< Replace current matrix or multiply with it?
	        unsigned int projection:1;    //!< Is this a projection or view matrix?
	        unsigned int :5;              //!< Padding
            unsigned int lenght:16;       //!< Length
        };
    };
	unsigned int cmd:8;           //!< Command
	unsigned int segmentAddress:32;  //!< Address to register where there is an RDRAM address used to retrieve matrix
};
#pragma pack(pop)
//*****************************************************************************
// Vertices
//*****************************************************************************

//! Add vertices struct used for ucode F3D
struct RSPUCodeAddVertices0 
{
	unsigned int length:16;      //!< Length
	unsigned int firstVertex:4;  //!< Index of first vertex
	unsigned int numVertices:4;  //!< Number of vertices minus one
	unsigned int cmd:8;          //!< Command
	unsigned int segmentAddress; //!< Address to register where there is an RDRAM address used to retrieve vertices
};
//! Add vertices struct used for ucode F3DEX
struct RSPUCodeAddVertices1 
{
	unsigned int length:10;      //!< Length
	unsigned int numVertices:6;  //!< Number of vertices
	unsigned int :1;             //!< Padding
	unsigned int firstVertex:7;  //!< Index of first vertex
	unsigned int cmd:8;          //!< Command
	unsigned int segmentAddress; //!< Address to register where there is an RDRAM address used to retrieve vertices
};

//! Add vertices struct used for ucode F3DEX2
struct RSPUCodeAddVerticesF3DEX2
{
    unsigned int pad0:1;             //!< Padding
	unsigned int vertexEnd:7;        //!< Index of last vertex
	unsigned int pad1:4;             //!< Padding
	unsigned int numVertices:8;      //!< Number of vertices
	unsigned int pad2:4;             //!< Padding
	unsigned int cmd:8;              //!< Command
    unsigned int segmentAddress:32;  //!< Address to register where there is an RDRAM address used to retrieve vertices
};

//! Add vertices struct used for ucode 9 (Perfect Dark)
struct RSPUCode9AddColorIndexVertices
{
    unsigned int pad0:16;             //!< Padding
    unsigned int firstVertexIndex:4;  //!< Index of first vertex (where to place vertices in vertexbuffer)
    unsigned int numVertices:4;       //!< Add one
    unsigned int cmd:8;               //!< Command
    unsigned int segmentAddress:32;   //!< Address to register where there is an RDRAM address used to retrieve vertices
};

//! Add vertices struct used for ucode 4 (Wave Rave 64)
struct RSPUCodeAddVerticesWaveRace64
{
    unsigned int pad0:8;                //!< Padding
    unsigned int pad1:1;                //!< Padding
    unsigned int numVertices:7;         //!< Number of vertices
    unsigned int firstVertexIndex:8;    //!< Index of first vertex (where to place vertices in vertexbuffer)(divide by 5)
    unsigned int cmd:8;                 //!< Command
    unsigned int segmentAddress:32;     //!< Address to register where there is an RDRAM address used to retrieve vertices
};

//*****************************************************************************
// One triangle
//*****************************************************************************

//! Add triangle struct used for ucode F3D (and F3DEX)
struct RSPUCodeAddOneTriangleF3D 
{
	unsigned int padding:24; //!< padding
	unsigned int cmd:8;      //!< Command
	unsigned int index2:8;   //!< Third vertex index  (divide by 10 in ucode F3D and 2 in F3DEX, 5 for wave race)
	unsigned int index1:8;   //!< Second vertex index (divide by 10 in ucode F3D and 2 in F3DEX, 5 for wave race)
	unsigned int index0:8;   //!< First vertex index  (divide by 10 in ucode F3D and 2 in F3DEX, 5 for wave race)
	unsigned int flag:8;     //!< ???
};

//! Add triangle struct used for example ucode F3DEX
struct RSPUCodeAddOneTriangleF3DEX 
{
	unsigned int padding:24; //!< padding
	unsigned int cmd:8;      //!< Command
	unsigned int pad0:1;     //!< Padding
	unsigned int index2:7;   //!< Third vertex index  
	unsigned int pad1:1;     //!< Padding
	unsigned int index1:7;   //!< Second vertex index 
	unsigned int pad2:1;     //!< Padding
	unsigned int index0:7;   //!< First vertex index  
	unsigned int pad3:8;     //!< Padding
};

//! Add triangle struct used for ucode F3DEX2
struct RSPUCodeAddOneTriangleF3DEX2 
{
	unsigned int index2:8;   //!< Third vertex index  
	unsigned int index1:8;   //!< Second vertex index 
	unsigned int index0:8;   //!< First vertex index  
	unsigned int cmd:8;      //!< Command
	unsigned int pad:32;     //!< padding
};

//*****************************************************************************
// Two triangles
//*****************************************************************************

//! Add triangle struct used for adding two triangles
struct RSPUCodeAddTwoTrianglesWaveRace64
{
	unsigned int v2:8;     //!< Third vertex index for first triangle   (divide by 5 for wave race)
	unsigned int v1:8;     //!< Second vertex index for first triangle  (divide by 5 for wave race)
	unsigned int v0:8;     //!< First vertex index for first triangle	(divide by 5 for wave race) 
	unsigned int cmd:8;    //!< Command
	unsigned int v5:8;     //!< Third vertex index for second triangle  (divide by 5 for wave race)
	unsigned int v4:8;     //!< Second vertex index for second triangle (divide by 5 for wave race)
	unsigned int v3:8;     //!< First vertex index for second triangle	(divide by 5 for wave race) 
	unsigned int flag:8;   //!< ???
};

//! Add triangle struct used for adding two triangles
struct RSPUCodeAddTwoTrianglesF3DEX 
{
    unsigned int pad0:1;   //!< Padding
	unsigned int v2:7;     //!< Third vertex index for first triangle
    unsigned int pad1:1;   //!< Padding
	unsigned int v1:7;     //!< Second vertex index for first triangle
    unsigned int pad2:1;   //!< Padding
	unsigned int v0:7;     //!< First vertex index for first triangle	
	unsigned int cmd:8;    //!< Command
    unsigned int pad3:1;   //!< Padding
	unsigned int v5:7;     //!< Third vertex index for second triangle
    unsigned int pad4:1;   //!< Padding
	unsigned int v4:7;     //!< Second vertex index for second triangle
    unsigned int pad8:1;   //!< Padding
	unsigned int v3:7;     //!< First vertex index for second triangle	
	unsigned int flag:8;   //!< ???
};

//*****************************************************************************
// Four triangles
//*****************************************************************************

//! Add triangle struct used for adding four triangles, used by ucode F3D
struct RSPUCodeAddFourTrianglesF3D
{
	unsigned int v0:4;
	unsigned int v3:4;
	unsigned int v6:4;
	unsigned int v9:4;
	unsigned int pad:8;
	unsigned int cmd:8;
	unsigned int v1:4;
	unsigned int v2:4;
	unsigned int v4:4;
	unsigned int v5:4;
	unsigned int v7:4;
	unsigned int v8:4;
	unsigned int v10:4;
	unsigned int v11:4;
};

//*****************************************************************************
// One Quad
//*****************************************************************************

//! Add quad struct used for adding a rectangel, used by ucode F3D
struct RSPUCodeAddOneQuadF3D
{
	unsigned int pad0:24;  //!< Padding
	unsigned int cmd:8;    //!< Command 
	unsigned int index3:8; //!< (divide by 10, 5 for Wave Race)
	unsigned int index2:8; //!< (divide by 10, 5 for Wave Race)
	unsigned int index1:8; //!< (divide by 10, 5 for Wave Race)
	unsigned int index0:8; //!< (divide by 10, 5 for Wave Race)
};

//! Add quad struct used for adding a rectangel, used by ucode F3DEX
struct RSPUCodeAddOneQuadF3DEX
{
	unsigned int pad0:24;  //!< Padding
	unsigned int cmd:8;    //!< Command 
    unsigned int pad1:1;   //!< Padding
	unsigned int index3:7; //!< Index of fourth vertex
    unsigned int pad2:1;   //!< Padding
	unsigned int index2:7; //!< Index of third vertex
    unsigned int pad3:1;   //!< Padding
	unsigned int index1:7; //!< Index of second vertex
    unsigned int pad4:1;   //!< Padding
	unsigned int index0:7; //!< Index of first vertex
};

//! Add quad struct used for adding a rectangel, used by ucode F3DEX2
struct RSPUCodeAddOneQuadF3DEX2  //aka TwoTriangles
{
	unsigned int v2:8;
	unsigned int v1:8;
	unsigned int v0:8;
	unsigned int cmd:8;
	unsigned int v5:8;
	unsigned int v4:8;
	unsigned int v3:8;
	unsigned int pad0:8;
};

//*****************************************************************************
// Misc
//*****************************************************************************

//! Struct used to set DMA Offsets (Mainly used by Diddy Kong Racing UCode6)
struct RSPUCodeSetDMAOffsets
{
    unsigned int addressOffsetMatrix:24;
    unsigned int cmd:8;
    unsigned int addressOffsetVertex:24;
    unsigned int padding:8;
};

//! Struct used to set Vertex Color Base
struct RSPUCodeSetVertexColorBase
{
	unsigned int padding:24;       //!< Padding 
	unsigned int cmd:8;            //!< Command
	unsigned int rdramAddress:32;  //!< Address to RDRAM where vertex colors are located.
};

//! Struct used to change display list
struct RSPUCodeDisplayList 
{
	unsigned int padding:16;         //!< Padding
	unsigned int param:8;            //!< Push display list or not?
	unsigned int cmd:8;              //!< Command
	unsigned int segmentAddress:32;  //!< 
};

//! Struct used when modifying vertex data
struct RSPUCodeModifyVertex
{
	unsigned int pad0:1;          //!< Padding
	unsigned int vertexIndex:15;  //!< Vertex Index (witch vertex to modify
	unsigned int modifyType:8;    //!< How to modify vertex (set color or texcoords?)
	unsigned int cmd:8;           //!< Command
	union
	{
		unsigned int value:16;    //!< Input values for vertex
		struct
		{
			unsigned int a:8;         //!< Alpha component of color
			unsigned int b:8;         //!< Blue component of color
			unsigned int g:8;         //!< Green component of color
			unsigned int r:8;         //!< Red component of color
		};
		struct
		{
			unsigned int t:16;        //!< Second value of texturecoord
			unsigned int s:16;        //!< First value of texturecoord
		};
	};
};

//*****************************************************************************
// Move Word
//*****************************************************************************

//! Struct used to extract information in MoveWord instruction for ucode F3D
struct RSPUCodeMoveWordF3D
{
	unsigned int type:8;          //!< Type of MoveWord
    unsigned int offset:16;       //!< Offset
	unsigned int cmd:8;           //!< Command

    union
    {
        unsigned int value:32;    //!< Value
		struct
		{
			unsigned int fo:16;       //!< Fog
			unsigned int fm:16;       //!< Fog
		};
    };
};

//! Struct used to extract information in MoveWord instruction for ucode F3DEX2  
struct RSPUCodeMoveWordF3DEX2 
{
    unsigned int offset:16;     //!< Offset
	unsigned int type:8;        //!< Type of MoveWord
	unsigned int cmd:8;         //!< Command
    union
    {
        unsigned int value:32;  //!< Value
		struct
		{
			unsigned int fo:16;     //!< Fog
			unsigned int fm:16;     //!< Fog
		};
    };
};

//*****************************************************************************
//* Texture
//! Struct used when retriving texture information on RSP 
//*****************************************************************************
struct RSPUCodeTexture
{
	unsigned int	enable_gbi0:1;
	unsigned int	enable_gbi2:1;
	unsigned int	:6;
	unsigned int	tile:3;
	unsigned int	level:3;
	unsigned int	:10;
	unsigned int	cmd:8;
	unsigned int	scaleT:16;
	unsigned int	scaleS:16;
};

//*****************************************************************************
//* RSPUCodeLoadUCode
//! Struct used when changing ucode on RSP 
//*****************************************************************************
struct RSPUCodeLoadUCode
{
    unsigned int ucodeSize:16;     //!< UCode size, note: Add one
    unsigned int padding:8;        //!< Padding?
    unsigned int cmd:8;            //!< Command
    unsigned int ucodeStart:32;    //!< UCode Start
};


//*****************************************************************************
// Display List
//*****************************************************************************

//! Struct used to cull displaylist
struct RSPUCodeCullDisplayList
{
    unsigned int pad0:1;             //!< Padding
    unsigned int vertexIndex:15;     //!< First vertex index to vertices to cull
    unsigned int pad2:8;             //!< Padding
    unsigned int cmd:8;              //!< Command
    unsigned int pad3:1;             //!< Padding
    unsigned int numVerticies:15;    //!< Number of vertices to cull
    unsigned int pad4:8;             //!< Padding
};

//! Used by instruction branch display list by ucode F3DEX
struct RSPUCodeBranchZF3DEX
{
    unsigned int pad0:1;        //!< Padding
    unsigned int vertex:11;     //!< Vertex to retrive z value from
    unsigned int pad1:12;       //!< Padding
    unsigned int cmd:8;         //!< Command
    unsigned int zvalue:32;     //!< z value used to compare with
}; 

//*****************************************************************************
// Others
//*****************************************************************************

//typedef struct {
//	union {
//		unsigned int w0;
//		struct {
//			unsigned int arg0:24;
//			unsigned int cmd:8;
//		};
//	};
//	unsigned int w1;
//} Gwords;

//! Stuct used to give GBI instructions input data as function argument.
typedef union {
	struct {
		union {
			unsigned int w0;
			struct {
				unsigned int arg0:24;
				unsigned int cmd:8;
			};
		};
		unsigned int w1;
	};
	long long int force_structure_alignment;
} MicrocodeArgument; 

#endif
