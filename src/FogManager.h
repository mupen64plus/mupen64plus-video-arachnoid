#ifndef FOG_MANAGER_H_
#define FOG_MANAGER_H_

//*****************************************************************************
//* Fog Manager
//! Class for setting fog using OpenGL
//! @details This manager has support for vertex based using OpenGL Extensions
//*****************************************************************************
class FogManager
{
public:

    //Constructor / Destructor
	FogManager();
	~FogManager();

    //Initialize / Dispose
	void initialize();
	void dispose();

    //Set Fog settings
	void setLinearFog(float start=0, float end=255);
	void setFogColor(float r, float g, float b, float a);

	//Extensions (for vertex based fog)
	void setFogCoordPointer(unsigned int type, int stride, const void* pointer);
	void enableFogCoordArray();
	void disableFogCoordArray();

	//Get/Set N64 Settings 
	void setFogSettings(float multiplier, float offset) { m_multiplier = multiplier; m_offset = offset; }
	float getMultiplier()                               { return m_multiplier;                          }
	float getOffset()                                   { return m_offset;                              }

public:

	//! Is fog extension supported?
	static bool fogExtensionsSupported() { return m_fogExtensionsSupported; }

private:

	static bool m_fogExtensionsSupported;  //!< Is fog extension supported
	float m_multiplier;                    //!< Fog multiplier
	float m_offset;                        //!< Fog offset

};

#endif