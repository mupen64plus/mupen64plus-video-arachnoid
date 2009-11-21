#ifndef SECONDARY_COLOR_EXTENSION_H_
#define SECONDARY_COLOR_EXTENSION_H_

#include "m64p.h"
#ifndef WIN32
	#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#ifndef WIN32
	#include <GL/glext.h>
#else
	#ifndef GL_EXT_secondary_color
	#define GL_EXT_secondary_color 1

		//Definitions
		#define GL_COLOR_SUM_EXT                     0x8458
		#define GL_CURRENT_SECONDARY_COLOR_EXT       0x8459
		#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT    0x845A
		#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT    0x845B
		#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT  0x845C
		#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
		#define GL_SECONDARY_COLOR_ARRAY_EXT         0x845E

		//Functions
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat r, GLfloat g, GLfloat b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble r, GLdouble g, GLdouble b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte r, GLbyte g, GLbyte b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort r, GLshort g, GLshort b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint r, GLint g, GLint b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3UBEXTPROC)(GLubyte r, GLubyte g, GLubyte b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3USEXTPROC)(GLushort r, GLushort g, GLushort b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3UIEXTPROC)(GLuint r, GLuint g, GLuint b);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3FVEXTPROC) (const GLfloat *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3DVEXTPROC) (const GLdouble *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3BVEXTPROC) (const GLbyte *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3SVEXTPROC) (const GLshort *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3IVEXTPROC) (const GLint *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3UBVEXTPROC)(const GLubyte *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3USVEXTPROC)(const GLushort *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLOR3UIVEXTPROC)(const GLuint *v);
		typedef void (APIENTRY * PFNGLSECONDARYCOLORPOINTEREXTPROC)(GLint size, GLenum type, GLsizei stride, void *pointer);

	#endif //GL_EXT_secondary_color 

	// EXT_secondary_color functions
	extern PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
	extern PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
	extern PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
	extern PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
	extern PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
	extern PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
	extern PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
	extern PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
	extern PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
	extern PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
	extern PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
	extern PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
	extern PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
	extern PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
	extern PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
	extern PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
	extern PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
#endif

bool initializeSecondaryColorExtension();

#endif //SECONDARY_COLOR_EXTENSION_H_