#ifndef SCREEN_SHOT_CREATOR_H_
#define SCREEN_SHOT_CREATOR_H_

//*****************************************************************************
//* ScreenShotCreator 
//! Class for taking and saving a screenshot as a BMP file
//*****************************************************************************
class ScreenshotCreator
{
public:

	//Constructor / Destrucotr
    ScreenshotCreator();
    ~ScreenshotCreator();

	//Function for taking and saving a screenshot as a BMP file
    void createScreenshot(const char* directory, const char* prefix, int x, int y, int width, int height);

};

#endif