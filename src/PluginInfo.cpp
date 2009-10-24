#include "PluginInfo.h"
#include <cstdio>               //sprintf
#include "ZilmarsPluginSpec.h"  //PLUGIN_INFO

//-----------------------------------------------------------------------------
//* Get Plugin Info
//! Will return information about this plugin
//! @param[out] pluginInfoOut Struct where information will be placed.
//! @see ZilmarsPluginSpec.h
//! @see PLUGIN_INFO
//-----------------------------------------------------------------------------
void getPluginInfo(PLUGIN_INFO* pluginInfoOut)
{
    //Plugin supports "Zilmars Plugin specification v1.3"
    pluginInfoOut->version = 0x0103;

    //Is a graphics plugin
	pluginInfoOut->type = PLUGIN_TYPE_GRAPHICS;

    //Sets name of plugin
	#if _DEBUG
		sprintf(pluginInfoOut->name, "Arachnoid Graphics Plugin v1.0 Debug");
	#else
		sprintf(pluginInfoOut->name, "Arachnoid Graphics Plugin v1.0");
	#endif

    //FIXME: Learn more about this
	pluginInfoOut->normalMemory = false;
	pluginInfoOut->memoryBswaped = true;
}