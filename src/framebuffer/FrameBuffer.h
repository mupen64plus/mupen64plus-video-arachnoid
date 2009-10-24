#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_


//*****************************************************************************
//* FrameBuffer
//! A Texture that we can render to.
//*****************************************************************************
class FrameBuffer
{
public:

	//Get Singleton
	//static FrameBuffer& getSingleton() {
	//	static FrameBuffer instance;
	//	return instance;
	//}

	//Constructor
	FrameBuffer();

	//Destructor
	~FrameBuffer();

	void initialize(int width, int height);
	void dispose();
	void resize(int width, int height);
	void beginRendering();
	void endRendering();
	void render();
    void render2();

protected:

	void _activate();
	void _deactivate();

private:



protected:

	unsigned int m_id;       //!< TextureID, Used mainly for OpenGL (so it can keep track of textures)
	int m_width;             //!< Width of framebuffer
	int m_height;            //!< Height of framebuffer 

	int m_oldViewport[4];


};

#endif