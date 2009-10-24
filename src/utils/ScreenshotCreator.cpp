#include "ScreenshotCreator.h"
#include <windows.h> //BMP
#include <GL/gl.h>   //ReadPixels
#include <cstdio>    //sprintf
#include "StringFunctions.h"  //trim

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
ScreenshotCreator::ScreenshotCreator()
{

}

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
ScreenshotCreator::~ScreenshotCreator()
{
}

//-----------------------------------------------------------------------------
//! Create Screenshot
//! Function for taking and saving a screenshot as a BMP file
//! @param directory Where you want to place the screenshot, if directory does 
//!                  not exist then it will be created.
//! @param x,y       Where in window to begin reading image
//! @param width     Width of screenshot
//! @param height    Height of screenshot
//-----------------------------------------------------------------------------
void ScreenshotCreator::createScreenshot(const char* directory, const char* prefix, int x, int y, int width, int height)
{
    const int imageSize = width * height * 3;
		
    //Get Image 
    unsigned char* image = new unsigned char[imageSize];
	glReadBuffer(GL_FRONT);
	glReadPixels(x, y, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
	glReadBuffer(GL_BACK);

    //Create fileheader
    BITMAPFILEHEADER fileHeader;
	fileHeader.bfType      = 19778;   //Magic number (identifier for BMP)
	fileHeader.bfSize      = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + imageSize;
	fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits   = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );

    //Create Info header
    BITMAPINFOHEADER infoHeader;
	infoHeader.biSize          = sizeof( BITMAPINFOHEADER );
	infoHeader.biWidth         = width;
	infoHeader.biHeight        = height;
	infoHeader.biPlanes        = 1;
	infoHeader.biBitCount      = 24;
	infoHeader.biCompression   = BI_RGB;
	infoHeader.biSizeImage     = imageSize;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed       = 0;
	infoHeader.biClrImportant  = 0;

    //Create directory if it does not exist
	CreateDirectory(directory, 0);

    //Try to create BMP-file, if one already exist => Change filename and try again    
    HANDLE bitmapFile;
    char filename[256];
	int i = 0;
	do
	{
        //Create a new filename
		sprintf(filename, "%s%s - %02i.bmp", directory, prefix, i);
		if (++i > 99) return;

        //Create File
		bitmapFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	}
	while ( bitmapFile == INVALID_HANDLE_VALUE );
	
    //Write image to file
	DWORD numBytesWritten = 0;
	WriteFile(bitmapFile, &fileHeader, sizeof( BITMAPFILEHEADER ), &numBytesWritten, 0 );
    WriteFile(bitmapFile, &infoHeader, sizeof( BITMAPINFOHEADER ), &numBytesWritten, 0 );
    WriteFile(bitmapFile, image, imageSize, &numBytesWritten, 0 );

    //Clean up and return
 	CloseHandle(bitmapFile);
    delete[] image;
}