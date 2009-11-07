#include "GraphicsPlugin.h"
#include "ZilmarsPluginSpec.h"   //GFX_INFO
#include "VI.h"                  //Video interface
#include "Memory.h"              //Access to memory (RDRAM, Texture Memory)
#include "OpenGLRenderer.h"      //Renderer
#include "FrameBuffer.h"         //Framebuffer
#include "DisplayListParser.h"   //Displaylist parser
#include "WindowManager.h"       //Window manager
#include "FogManager.h"          //Fog 
#include "RSP.h"                 //Reality Signal Processor
#include "RDP.h"                 //Reality Drawing Processor
#include "GBI.h"                 //Graphics Binary Interface 
#include "ConfigMap.h"           //Configuration
#include "Logger.h"              //Debug logger
#include "ROMDetector.h"

#include <ctime>

//FrameBuffer framebuffer01;
//FrameBuffer framebuffer02;

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
GraphicsPlugin::GraphicsPlugin() 
: m_openGLMgr( OpenGLManager::getSingleton() )
{
	m_vi = 0;
	m_initialized = false;
    m_updateConfig = false;
    m_screenshotDirectory = 0;
	m_fogManager = 0;
}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
GraphicsPlugin::~GraphicsPlugin()
{
    dispose();
}

//-----------------------------------------------------------------------------
// Initialize
//-----------------------------------------------------------------------------
bool GraphicsPlugin::initialize(GFX_INFO* graphicsInfo)
{
	//Save pointer to graphics info
	m_graphicsInfo = graphicsInfo;

	m_numDListProcessed = 0;

    //Detect what rom it is
    m_romDetector = &ROMDetector::getSingleton();        
    m_romDetector->initialize( m_graphicsInfo->HEADER );

	//Start up the video
	CoreVideo_Init();
	CoreVideo_SetVideoMode(320, 240, 32, M64VIDEO_WINDOWED);

	//Initialize Video Interface
	m_vi = new VI();
	m_vi->calcSize(m_graphicsInfo);

	//Initialize Memory
	m_memory = new Memory();
	if ( !m_memory->initialize(m_graphicsInfo->RDRAM, m_graphicsInfo->DMEM) ) {
		return false;
	}
	
	m_displayListParser = new DisplayListParser();
	m_displayListParser->initialize(&m_rsp, &m_rdp, &m_gbi, m_memory);

	//Get Device Context
	/*
	//TODO: replace with m64p window stuff
    HDC dc = GetWindowDC(m_graphicsInfo->hWnd);

    int width = m_config->windowWidth;
    int height = m_config->windowHeight;

	m_windowMgr = new WindowManager();
	m_windowMgr->initialize(m_graphicsInfo->hWnd, m_graphicsInfo->hStatusBar);
	m_windowMgr->resizeWindow(width, height);

    //Init OpenGL
    if ( !m_openGLMgr.initialize(m_graphicsInfo->hWnd, dc, false, width, height, m_config->fullscreenBitDepth, m_config->fullscreenRefreshRate, true, false) ) {
		Logger::getSingleton().printMsg("ERROR: Unable, to initialize OpenGL", M64MSG_ERROR);
		return false;
    }
	*/
	m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());

	//Initialize Fog Manager
	m_fogManager = new FogManager();
	m_fogManager->initialize();



	//Initialize Texture Cache
	//! @todo Not "hardcode" TextureBitDepth.
	m_textureCache.initialize(&m_rsp, &m_rdp, m_memory, 16);


	//Initialize OpenGL Renderer
	if ( !OpenGLRenderer::getSingleton().initialize(&m_rsp, &m_rdp, &m_textureCache, m_vi, m_fogManager) ) {
		Logger::getSingleton().printMsg("ERROR: Unable to initialize OpenGL Renderer", M64MSG_ERROR);
		return false;
	}

	//Initialize Processors
	m_rdp.initialize(m_graphicsInfo, &m_rsp, m_memory, &m_gbi, &m_textureCache, m_windowMgr, m_vi, m_displayListParser, m_fogManager);
	m_rsp.initialize(m_graphicsInfo, &m_rdp, m_memory, m_windowMgr, m_vi, m_displayListParser, m_fogManager);
	m_gbi.initialize(&m_rsp, &m_rdp, m_memory, m_displayListParser);	
		

    //Set Background color
    m_openGLMgr.setClearColor(0.0f, 0.0f, 0.0f);
	m_openGLMgr.setLighting(false);
    glDisable(GL_LIGHTING);
	m_openGLMgr.setCullMode(false, true);
    m_openGLMgr.setWireFrame(m_config->wireframe);   

	//Initialize framebuffer
	//framebuffer01.initialize(width, height);
   // framebuffer02.initialize(width, height);

	m_initialized = true;
	return true;
}

//-----------------------------------------------------------------------------
// Dispose
//-----------------------------------------------------------------------------
void GraphicsPlugin::dispose()
{	
    //Dispose of Textures
	m_textureCache.dispose();

    //Dispose of member objects
	if ( m_vi )                { delete m_vi;                m_vi = 0;                }
    if ( m_memory )            { delete m_memory;            m_memory = 0;            }
    if ( m_displayListParser ) { delete m_displayListParser; m_displayListParser = 0; }
    if ( m_windowMgr )         { delete m_windowMgr;         m_windowMgr = 0;         }   
	if ( m_fogManager )        { delete m_fogManager;        m_fogManager = 0;        }
	
    m_gbi.dispose();
    m_rdp.dispose();
	m_rsp.dispose();
    
    //Dispose of OpenGL
    //framebuffer01.dispose();
   // framebuffer02.dispose();
	m_openGLMgr.dispose();

	CoreVideo_Quit();

	m_initialized = false;
}

void renderMotionBlur()
{
	///glPushMatrix();

		glDisable(GL_DEPTH_TEST);			
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
		glEnable(GL_BLEND);
        glColor4f(1, 1, 1, 0.9f);  //Alpha blending
        
//        framebuffer01.render2();

        glColor4f(1, 1, 1, 1.0f);  //Alpha blending
		glEnable(GL_DEPTH_TEST);						
		glDisable(GL_BLEND);

	//glPopMatrix();
}

bool animate(int frameRate)
{
	static clock_t lastTime;
	clock_t currentTime = clock() * 1000 / CLOCKS_PER_SEC;
	clock_t elapsedTime = currentTime - lastTime;
	if (elapsedTime > ((clock_t)1000 / frameRate))
	{
		lastTime = currentTime;
		return true;
	}
	return false;
	
#if 0
	//Todo: test against new version
	static float lastTime = 0.0f;
	float elapsedTime = 0.0;
    float currentTime = GetTickCount() * 0.001f; 
	elapsedTime = currentTime - lastTime;

    if( elapsedTime > (1.0f / frameRate) )
    {
        lastTime = currentTime;	
        return true;
    }

	return false;
#endif

}

void renderQuad()
{
    static float dt = 0;
    dt += 0.2f;
    
    //glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
   // glRotatef(dt, 0, 1, 0);
    {
        glBegin(GL_QUADS);
        {
            glColor3f(1,0,0);   glVertex3f(-1,-1,0);
            glColor3f(1,1,0);   glVertex3f( 1,-1,0);
            glColor3f(0,1,1);   glVertex3f( 1, 1,0);
            glColor3f(0,0,1);   glVertex3f(-1, 1,0);
        }
        glEnd();
    }
    glPopMatrix();
}

void renderRedBox(float x, float y, float z, float width, float height,  float length)
{
    static float dt = 0;
    dt += 10.6f;
    glRotatef(dt, 1,1,1);
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	glBegin(GL_QUADS);				
        glColor3f(1,0,0);		
		glVertex3f(x,			y,			z);
		glVertex3f(x,			y + height, z);
		glVertex3f(x + width, y + height, z); 
		glVertex3f(x + width, y,			z);
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y + height, z + length);
		glVertex3f(x + width, y + height, z + length); 
		glVertex3f(x + width, y,			z + length);		
		glVertex3f(x,			y,			z);
		glVertex3f(x,			y,			z + length);
		glVertex3f(x + width, y,			z + length); 
		glVertex3f(x + width, y,			z);
		glVertex3f(x,			y + height,	z);
		glVertex3f(x,			y + height,	z + length);
		glVertex3f(x + width, y + height, z + length); 
		glVertex3f(x + width, y + height, z);	
		glVertex3f(x,			y,			z);
		glVertex3f(x,			y,			z + length);
		glVertex3f(x,			y + height,	z + length); 
		glVertex3f(x,			y + height,	z);
		glVertex3f(x + width, y,			z);
		glVertex3f(x + width, y,			z + length);
		glVertex3f(x + width, y + height,	z + length); 
		glVertex3f(x + width, y + height,	z);
        glColor3f(1,1,1);
	glEnd();
}

void GraphicsPlugin::_motionBlur()
{

        glMatrixMode(GL_PROJECTION);					
	    glLoadIdentity();									
	    gluPerspective(45.0f,(GLfloat)800.0f/(GLfloat)600.0f, .5f ,150.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	    gluLookAt(0, 0, 10,     0, 0, 0,     0, 1, 0);		

        ////Render new stuff to the motion blur texture
        if ( animate(50) )
        {
            //Render to frame buffer
//            framebuffer01.beginRendering();
            {
		        renderMotionBlur();
        		
                glMatrixMode(GL_PROJECTION);					
	            glLoadIdentity();									
	            gluPerspective(45.0f,(GLfloat)800.0f/(GLfloat)600.0f, .5f ,150.0f);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
	            gluLookAt(0, 0, 10,     0, 0, 0,     0, 1, 0);	

		        renderRedBox(0, 0, 0,	1, 5, 1);
                //m_displayListParser->processDisplayList();

		
            }
            glFlush();
           // framebuffer01.endRendering();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
        }

        //glDisable(GL_BLEND);
        //glEnable(GL_DEPTH_TEST);

        //renderMotionBlur();
        //renderTriangle();




	    renderMotionBlur();

     //   glMatrixMode(GL_PROJECTION);						
	    //glLoadIdentity();									
	    //gluPerspective(45.0f,(GLfloat)800.0f/(GLfloat)600.0f, .5f ,150.0f);

     //   glMatrixMode(GL_MODELVIEW);
     //   glLoadIdentity();
	    //gluLookAt(0, 0, 10,     0, 0, 0,     0, 1, 0);		

        //renderRedBox(0, 0, 0,	1, 5, 1);

        m_rdp.triggerInterrupt();
        m_rsp.triggerInterrupt();
        

        //Render new stuff to frame buffer
        //framebuffer02.beginRendering();
        {
      //      m_displayListParser->processDisplayList();  
      //      RSP::getSingleton().reset();
      //      RDP::getSingleton().reset();

      //      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
      //      glDisable(GL_BLEND);
      //      glEnable(GL_DEPTH_TEST);
      //      glDisable(GL_ALPHA_TEST);
		    //m_displayListParser->processDisplayList();
        }
        //glFlush();
        //framebuffer02.endRendering();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

        //Blend old stuff and new stuff      

        //framebuffer02.render2();
        
        //FrameBuffer::getSingleton().render();

}

//-----------------------------------------------------------------------------
// ProcessDisplayList
//-----------------------------------------------------------------------------
void GraphicsPlugin::processDisplayList()
{
	if ( m_numDListProcessed == 1 && m_romDetector->getClearType() == CT_AFTER_ONE_DISPLAY_LIST ||
		 m_numDListProcessed == 2 && m_romDetector->getClearType() == CT_AFTER_TWO_DISPLAY_LIST ||
		 m_numDListProcessed == 3 && m_romDetector->getClearType() == CT_AFTER_THREE_DISPLAY_LIST )
	{
		bool scissors = OpenGLManager::getSingleton().getScissorEnabled();
		OpenGLManager::getSingleton().setScissorEnabled(false);
		glClear(GL_COLOR_BUFFER_BIT);
		m_numDListProcessed = 0;
		OpenGLManager::getSingleton().setScissorEnabled(scissors);
	}

    //Update Config?
    if ( m_updateConfig )
    {        
	    m_vi->calcSize(m_graphicsInfo);	
	    m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());
        OpenGLManager::getSingleton().setViewport(0,m_windowMgr->getHeightOffset(), m_config->windowWidth, m_config->windowHeight);
        m_openGLMgr.setWireFrame( m_config->wireframe );
        _setTextureCacheSize( m_config->textureCacheSize );
        m_updateConfig = false;

        //if ( OpenGLManager::getSingleton().getFullscreen() )
        //{
        //    //Initialize framebuffer
        //    FrameBuffer::getSingleton().dispose();
	       // FrameBuffer::getSingleton().initialize(m_config->fullscreenWidth,  m_config->fullscreenHeight);
        //}
        //else
        //{
        //    //Initialize framebuffer
        //    FrameBuffer::getSingleton().dispose();
	       // FrameBuffer::getSingleton().initialize(m_config->windowWidth,  m_config->windowHeight);
        //}


    }       

	//Get Video Interface Size
	m_vi->calcSize(m_graphicsInfo);	
	m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());
	OpenGLManager::getSingleton().setForceDisableCulling( ROMDetector::getSingleton().getDisableFaceCulling() );

    //Render Scene
    OpenGLManager::getSingleton().beginRendering();		
    OpenGLManager::getSingleton().setTextureing2D(true);        
    glEnable(GL_DEPTH_TEST);						
	{	
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		m_rsp.reset();
        m_rdp.reset();
		m_openGLMgr.setCullMode(false, true);


        if ( false )
        {
            _motionBlur();
        }
        else 
        {
            m_displayListParser->processDisplayList();
			
        }
        
	}

		//Clear Screen?
	m_numDListProcessed++;

	//glFlush();	
	//OpenGLManager::getSingleton().endRendering();

    //Take screenshot?
    if ( m_screenshotDirectory )
    {
        m_screenshotCreator.createScreenshot( m_screenshotDirectory, 
			                                  m_romDetector->getRomName(),
                                              0, m_windowMgr->getHeightOffset(), 
                                              m_openGLMgr.getWidth(), m_openGLMgr.getHeight());
        delete[] m_screenshotDirectory; 
        m_screenshotDirectory = 0;
    }
}

//-----------------------------------------------------------------------------
// Update Screen
//-----------------------------------------------------------------------------
void GraphicsPlugin::drawScreen()
{

	glFinish();
	/*
	OpenGLManager::getSingleton().beginRendering();		
	{	
		FrameBuffer::getSingleton().render();
	}
	OpenGLManager::getSingleton().endRendering();
	*/
	OpenGLManager::getSingleton().endRendering();

	
	glFinish();
}

//-----------------------------------------------------------------------------
// Video Interface Status Changed
//-----------------------------------------------------------------------------
void GraphicsPlugin::viStatusChanged()
{
	//Get new VI Size
	m_vi->calcSize( m_graphicsInfo );

	//Re Calculate Scale
	m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());
}

//-----------------------------------------------------------------------------
// Toggle Fullscreen
//-----------------------------------------------------------------------------
void GraphicsPlugin::toggleFullscreen()
{
	if ( m_initialized ) 
	{   
        if ( !OpenGLManager::getSingleton().getFullscreen() )
        {
            //Initialize framebuffer
            //framebuffer01.dispose();
            //framebuffer02.dispose();
	        //framebuffer01.initialize(m_config->fullscreenWidth,  m_config->fullscreenHeight);
            //framebuffer02.initialize(m_config->fullscreenWidth,  m_config->fullscreenHeight);

            //Goto fullscreen
            _setWindowMode(m_config->fullscreenWidth, m_config->fullscreenHeight);  
            OpenGLManager::getSingleton().resize(m_config->fullscreenWidth,  m_config->fullscreenHeight, m_config->fullscreenBitDepth, m_config->fullscreenRefreshRate);
            OpenGLManager::getSingleton().toggleFullscreen();
	        m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());
            OpenGLManager::getSingleton().setViewport(0,m_windowMgr->getHeightOffset(), m_config->windowWidth, m_config->windowHeight);        
		    m_windowMgr->toggleFullscreen();
        }
        else
        {
            //Initialize framebuffer
            //framebuffer01.dispose();
          //  framebuffer02.dispose();
	        //framebuffer01.initialize(m_config->windowWidth,  m_config->windowHeight);
           // framebuffer02.initialize(m_config->windowWidth,  m_config->windowHeight);


            //Goto Window Mode
            _setWindowMode(m_config->windowWidth, m_config->windowHeight);  
            OpenGLManager::getSingleton().resize(m_config->windowWidth,  m_config->windowHeight, m_config->fullscreenBitDepth, m_config->fullscreenRefreshRate);
            OpenGLManager::getSingleton().toggleFullscreen();
	        m_openGLMgr.calcViewScale(m_vi->getWidth(), m_vi->getHeight());
            OpenGLManager::getSingleton().setViewport(0,m_windowMgr->getHeightOffset(), m_config->windowWidth, m_config->windowHeight);
		    m_windowMgr->toggleFullscreen();
        }
    }
}

//-----------------------------------------------------------------------------
// Take Screenshot
//-----------------------------------------------------------------------------
void GraphicsPlugin::takeScreenshot(char* directory)
{
    //Copy string
    size_t length = strlen(directory);
    m_screenshotDirectory = new char[length+1];
    strncpy(m_screenshotDirectory, directory, length);
    m_screenshotDirectory[length] = '\0';
}

//-----------------------------------------------------------------------------
// Update Configuration
//-----------------------------------------------------------------------------
void GraphicsPlugin::updateConfig()
{
    m_updateConfig = true;

    //For project 64, Give config dialog time to close before continueing
#ifdef WIN32
    Sleep(300); 
#endif
    //Resize Window
    _setWindowMode(m_config->windowWidth, m_config->windowHeight);  

    //Reinitialize OpenGL
    OpenGLManager::getSingleton().resize(m_config->windowWidth, m_config->windowHeight, m_config->fullscreenBitDepth, m_config->fullscreenRefreshRate);    	
}

//-----------------------------------------------------------------------------
// Set Window Mode
//-----------------------------------------------------------------------------
void GraphicsPlugin::_setWindowMode(int width, int height)
{
	if ( m_initialized ) 
	{
		m_windowMgr->resizeWindow(width, height);			
	}	
}

//-----------------------------------------------------------------------------
// Set Texture Cache Size
//-----------------------------------------------------------------------------
void GraphicsPlugin::_setTextureCacheSize(int sizeInBytes)
{

}