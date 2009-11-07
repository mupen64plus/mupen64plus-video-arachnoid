#include "ConfigFile.h"
#include "SmartScriptParser.h"

//-----------------------------------------------------------------------------
// Global variabels
//-----------------------------------------------------------------------------

//! Defualt section name for settings
const char defaultSectionName[] = "[Defualt]";   

//-----------------------------------------------------------------------------
//! Constructor
//-----------------------------------------------------------------------------
ConfigFile::ConfigFile()
{
}   

//-----------------------------------------------------------------------------
//! Destructor
//-----------------------------------------------------------------------------
ConfigFile::~ConfigFile()
{
    dispose();
}

//-----------------------------------------------------------------------------
//* Initialize
//! Loads configuration settings from file
//! @param[in] filename Name of configuration file stored on disk
//-----------------------------------------------------------------------------
bool ConfigFile::initialize(const char* filename)
{
    //Remove old settings?
    if ( m_settings.size() > 0 ) {
        dispose();
    }

    //Save pointer to filename
    m_filename = filename;

    //Open Config File
    SmartScriptParser parser;
    if ( !parser.open(filename) ) {
        return false;
    }

    //Create defualt section
    string section = defaultSectionName;
    m_settings[section] = new SettingsMap();

    //Parse File, and save all settings
    while ( !parser.eof() )
    {
        string token = parser.getToken();

        //Break on eof character
        if ( token == "ÿ" ) {
            break;
        }   

        if ( token.at(0) == '[' && token.at(token.size()-1) == ']' )
        {
            //Found new section
            section = token;
        
            //Does section exist? if not create if
            SettingsSectionMap::const_iterator it = m_settings.find( section );
            if ( it == m_settings.end() )
            {
                m_settings[section] = new SettingsMap();
            }
        }
        else
        {
            //Found setting
            string line = parser.getLine();
            StringFunctions::trim(line, true, true, " =:");
            (*m_settings[section])[token] = line;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
//* Save
//! Save settings to a file on disk
//! @param[in] filename Name of configuration file to store settings to
//-----------------------------------------------------------------------------
void ConfigFile::save(const char* filename)
{
    //Create a config file
    std::ofstream out;
    out.open(filename ? filename : m_filename.c_str(), std::ios::binary);

    //For each setting map
    ConfigFile::SettingsSectionMap::iterator tempIt = m_settings.begin();
    while ( tempIt != m_settings.end() )
    {
        //Write section name
        out << tempIt->first << "\r\n";

        //For each setting
        SettingsMap::const_iterator it=tempIt->second->begin(); 
        while ( it != tempIt->second->end() )
        {
            //Write settings
            out << it->first << " = " << (const char*)it->second << "\r\n";
            ++it;
        }  
        ++tempIt;
    }
    
    out.close();
}

//-----------------------------------------------------------------------------
//* Dispose
//! Removes all configuration settings
//-----------------------------------------------------------------------------
void ConfigFile::dispose()
{
    //Delete all settings
    ConfigFile::SettingsSectionMap::iterator tempIt = m_settings.begin();
    while ( tempIt != m_settings.end() )
    {
        delete tempIt->second;
        ++tempIt;
    }
    m_settings.clear();        
}

//-----------------------------------------------------------------------------
//* Get
//! Retrives a loaded setting.
//! @param[in] key Name of setting
//! @param[in] section Name of section where setting is located (0 to search all sections)
//! @param[in] defualtValue The value that should be returned if no value could be found
//! @retval Value of a configuration setting
//-----------------------------------------------------------------------------
StringValue ConfigFile::get(const char* key, char* section, StringValue defualtValue)
{
    SettingsMap::const_iterator keyIt;
    bool foundKey = false;

	std::string temp = m_settings[defaultSectionName]->begin()->second.str();

    if ( section )
    {
        SettingsSectionMap::const_iterator sectionIt = m_settings.find( string(section) );
        if ( sectionIt == m_settings.end() )
        {
            return StringValue(defualtValue);
        }
        
        //Get Key
        SettingsMap::const_iterator keyIt = sectionIt->second->find(key);
        if ( keyIt == sectionIt->second->end() )
        {
            return StringValue(defualtValue);
        }

        foundKey = true;
    }
    else
    {


        //For each setting map
        ConfigFile::SettingsSectionMap::iterator tempIt = m_settings.begin();
        while ( tempIt != m_settings.end() )
        {
            //For each key           
            SettingsMap::const_iterator it=tempIt->second->begin(); 
            while ( it != tempIt->second->end() )
            {
                if ( it->first == key )
                {
                    keyIt = it;
                    foundKey = true;
                    break;
                }

                ++it;
            }  

            ++tempIt;
        }
    }

    if ( !foundKey ) 
    {
        return StringValue(defualtValue);
    }

    //Return value
    return keyIt->second;
}

//-----------------------------------------------------------------------------
//* Set
//! Adds/Changes a setting to configuration
//! @param[in] key Name of setting to add
//! @param[in] value The value to assign
//! @param[in] section The section to place setting in. (0 to place setting in defualt section)
//-----------------------------------------------------------------------------
void ConfigFile::set(const char* key, StringValue value, char* section)
{
    string sectionStr = section ? section : defaultSectionName;

    //Does section exist? if not create if
    SettingsSectionMap::const_iterator it = m_settings.find( sectionStr );
    if ( it == m_settings.end() )
    {
        m_settings[sectionStr] = new SettingsMap();
    }

    //Assign value
    (*m_settings[sectionStr])[key] = value;
}