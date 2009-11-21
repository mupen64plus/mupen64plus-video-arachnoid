#ifndef CONFIG_H_
#define CONFIG_H_

#include "m64p.h"
#include "ConfigMap.h"

//Forward declaration
class GraphicsPlugin;

//*****************************************************************************
//* Config
//! Manages configuration of graphics plugin
//!
//! Responability:
//!  * Loads config data from core
//!  * Sets default config values
//!
//! @see ConfigMap
//*****************************************************************************
class Config
{
public:

	//Constructor / Destructor
	Config(GraphicsPlugin* graphicsPlugin);
	~Config();

	bool initialize();

    //Loads settings from core
	void load();

    //Tell Graphics plugin that config has changed
	void updateGraphics();

public:

    //Get configuration
    ConfigMap* getConfig() { return &m_cfg; }

private:

	ConfigMap m_cfg;                     //!< Config map with all settings
    GraphicsPlugin* m_graphicsPlugin;    //!< Pointer to graphics plugin

	m64p_handle m_videoGeneralSection;
	m64p_handle m_videoArachnoidSection;
};

#endif