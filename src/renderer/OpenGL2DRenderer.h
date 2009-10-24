#ifndef OPEN_GL_2D_RENDERER_H_
#define OPEN_GL_2D_RENDERER_H_

//Forward declarations
class VI;

//*****************************************************************************
//* OpenGL 2D Renderer
//! Class used to render HUD objects.
//! @details Renders 2D quads, textured 2D quads and flipped textures 2D quads.
//*****************************************************************************
class OpenGL2DRenderer
{
public:

	//Initialize
    bool initialize(VI* vi);

    //Render Quad
    void renderQuad( const float color[4], 
                     float x0, float y0, 
                     float x1, float y1,            
                     float depth );


    //Render Textured Quad
    void renderTexturedQuad( const float color[4], 
                             const float secondaryColor[4],
                             float x0, float y0,            
                             float x1, float y1,    
                             float depth,
                             float t0s0, float t0t0,  
                             float t0s1, float t0t1,
                             float t1s0, float t1t0, 
                             float t1s1, float t1t1 );
	
    //Render Flipped Textured Quad
    void renderFlippedTexturedQuad( const float color[4], 
                                    const float secondaryColor[4],
                                    float x0, float y0,            
                                    float x1, float y1,    
                                    float depth,
                                    float t0s0, float t0t0,  
                                    float t0s1, float t0t1,
                                    float t1s0, float t1t0, 
                                    float t1s1, float t1t1 );


private:

    VI* m_vi;  //!< Video interface
 
};

#endif