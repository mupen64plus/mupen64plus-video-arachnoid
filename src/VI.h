#ifndef VIDEO_INTERFACE_H_
#define VIDEO_INTERFACE_H_

//Forward declarations
#include "UCodeDefs.h"
struct GFX_INFO;

//*****************************************************************************
//! Video Interface
//*****************************************************************************
class VI
{
public:

	//Constructor
	VI();
	~VI();

	//Calculate height and width
	void calcSize(GFX_INFO*  graphicsInfo);

	//Get Height and Width
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

private:

	int m_width;   //!< Width of video interface viewport
	int m_height;  //!< Height of video interface viewport

};

#endif