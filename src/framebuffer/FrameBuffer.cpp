#include "FrameBuffer.h"

#include <windows.h>
#include <GL/gl.h>

//-----------------------------------------------------------------------------
//OpenGL Texture Definitions
//-----------------------------------------------------------------------------
typedef GLvoid (APIENTRY *PFNGLACTIVETEXTUREPROC) (GLenum texture);
PFNGLACTIVETEXTUREPROC      glActiveTexture = NULL;
#ifndef GL_TEXTURE0
	#define GL_TEXTURE0     0x84C0
#endif
#ifndef GL_CLAMP_TO_EDGE
	#define GL_CLAMP_TO_EDGE  0x812F
#endif

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
FrameBuffer::FrameBuffer()
{
	m_id = -1;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{

}

//-----------------------------------------------------------------------------
//* Initialize
//! @param width Width of framebuffer, usually equal to window-client-width
//! @param height Height of framebuffer, usually equal to window-client-height
//-----------------------------------------------------------------------------
void FrameBuffer::initialize(int width, int height)
{
    //Save parameters
    m_width    = width;
    m_height   = height;
    int channels = 3;	 //!< RGB=3 or RGBA=4	

    //Allocate memory
	unsigned char* data = new unsigned char[width * height * channels];
	memset(data, 0, width * height * channels * sizeof(unsigned char));	

	//Register the texture with OpenGL and bind it to the texture ID
	glGenTextures(1, &m_id);								
	glBindTexture(GL_TEXTURE_2D, m_id);					

	//Create the texture and store it on the video card
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	
		
	//No texure filtering
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );		    
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Clamp texture to edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Delete data (no need for it when it is stored in video card)
	delete[] data;
    data = 0;
}

//-----------------------------------------------------------------------------
// Dispose
//-----------------------------------------------------------------------------
void FrameBuffer::dispose()
{
	if ( m_id != -1 )
	{
		glDeleteTextures(1, &m_id);		
		m_id = -1;
	}
}

//-----------------------------------------------------------------------------
// Resize
//-----------------------------------------------------------------------------
void FrameBuffer::resize(int width, int height)
{
	dispose();
	initialize(width, height);
}

//-----------------------------------------------------------------------------
//* Begin Rendering
//! 
//-----------------------------------------------------------------------------
void FrameBuffer::beginRendering()
{
    //Get viewport
    glGetIntegerv(GL_VIEWPORT, m_oldViewport);

    //Set new viewport for texture
    //glViewport(0, 20, m_width, m_height);

	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    //Clear Buffers
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
//* End Rendering
//! Will save all rendering to a texture
//-----------------------------------------------------------------------------
void FrameBuffer::endRendering()
{
	//Activate texture
	_activate();

    //Render to Texture
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 20, m_width, m_height );

	//TODO Deactivate texture?
	//_deactivate();

	//Reset Viewport
	//glViewport(m_oldViewport[0], m_oldViewport[1], m_oldViewport[2], m_oldViewport[3]);

    //Clear Buffers
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
//* Render
//! Will render frame buffer to screen.
//-----------------------------------------------------------------------------
void FrameBuffer::render()
{
    //Push states
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
 	//glOrtho( 0, m_width, 0, m_height, -1.0f, 1.0f );
	//glViewport( 0, 0, m_width, m_height );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    //Render QUAD (using framebuffer texture)
	_activate();
    {
        glBegin(GL_QUADS);
        {
			glColor3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0,0);
            glVertex3f(-1,-1,0);
            glTexCoord2f(1,0);
            glVertex3f( 0,-1,0);
            glTexCoord2f(1,1);
            glVertex3f( 0, 0,0);
            glTexCoord2f(0,1);
            glVertex3f(-1, 0,0);
			glColor3f(1.0f, 1.0f, 1.0f);
        }
        glEnd();
    }
    _deactivate();

    //Pop states
    //glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
}

//-----------------------------------------------------------------------------
//* Render
//! Will render frame buffer to screen.
//-----------------------------------------------------------------------------
void FrameBuffer::render2()
{
    //Push states
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
 	//glOrtho( 0, m_width, 0, m_height, -1.0f, 1.0f );
	//glViewport( 0, 0, m_width, m_height );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    //Render QUAD (using framebuffer texture)
	_activate();
    {
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0,0);
            glVertex3f(-1,-1,0);
            glTexCoord2f(1,0);
            glVertex3f( 1,-1,0);
            glTexCoord2f(1,1);
            glVertex3f( 1, 1,0);
            glTexCoord2f(0,1);
            glVertex3f(-1, 1,0);
			glColor3f(1.0f, 1.0f, 1.0f);
        }
        glEnd();
    }
    _deactivate();

    //Pop states
    //glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
}

//-----------------------------------------------------------------------------
// Activate
//-----------------------------------------------------------------------------
void FrameBuffer::_activate()
{
    //Activate Texture (so we can copy to it)
    if ( glActiveTexture == 0 ) {
        glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");
    }
    glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

//-----------------------------------------------------------------------------
// Deactivate
//-----------------------------------------------------------------------------
void FrameBuffer::_deactivate()
{
    glActiveTexture((GLuint)GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
}