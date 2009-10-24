#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include <string>
    using std::string;
#include <map>
    using std::map;
#include "StringValue.h"

//*****************************************************************************
//* Config File
//! Class for managing the configuration file with all settings.
//*****************************************************************************
class ConfigFile
{
public:

    //Constructor / Destructor
    ConfigFile();
    ~ConfigFile();

    //Initialize / dispose
    bool initialize(const char* filename);
    void dispose();

    //Save configuration to file
    void save(const char* filename=0);

    //Get/Set config values
    StringValue get(const char* key, char* section=0, StringValue defualtValue=0);
    void set(const char* key, StringValue value, char* section=0);

private:
    
    //Settings types
    typedef map<string, StringValue> SettingsMap;              //! Type for storing config values
    typedef map<string, SettingsMap*> SettingsSectionMap;      //! Type for storing sections with config values

private:
    
    SettingsSectionMap m_settings;   //! Stored configuration settings
    std::string m_filename;          //! Filename of configuration file

};

#endif