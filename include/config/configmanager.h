#pragma once

#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace ITD {

/**
 * @brief Configuration manager for ITD
 * 
 * This class manages application configuration, including user preferences,
 * themes, and settings.
 */
class ConfigManager {
public:
    /**
     * @brief Constructor
     */
    ConfigManager();
    
    /**
     * @brief Destructor
     */
    virtual ~ConfigManager();

    /**
     * @brief Load configuration from disk
     * @param filename Configuration file path (optional)
     * @return True if successful
     */
    bool Load(const wxString& filename = wxEmptyString);

    /**
     * @brief Save configuration to disk
     * @param filename Configuration file path (optional)
     * @return True if successful
     */
    bool Save(const wxString& filename = wxEmptyString);

    /**
     * @brief Get string value
     * @param section Configuration section
     * @param key Configuration key
     * @param defaultVal Default value if not found
     * @return String value
     */
    wxString GetString(const wxString& section, const wxString& key, const wxString& defaultVal = wxEmptyString) const;

    /**
     * @brief Set string value
     * @param section Configuration section
     * @param key Configuration key
     * @param value String value to set
     */
    void SetString(const wxString& section, const wxString& key, const wxString& value);

    /**
     * @brief Get integer value
     * @param section Configuration section
     * @param key Configuration key
     * @param defaultVal Default value if not found
     * @return Integer value
     */
    int GetInt(const wxString& section, const wxString& key, int defaultVal = 0) const;

    /**
     * @brief Set integer value
     * @param section Configuration section
     * @param key Configuration key
     * @param value Integer value to set
     */
    void SetInt(const wxString& section, const wxString& key, int value);

    /**
     * @brief Get boolean value
     * @param section Configuration section
     * @param key Configuration key
     * @param defaultVal Default value if not found
     * @return Boolean value
     */
    bool GetBool(const wxString& section, const wxString& key, bool defaultVal = false) const;

    /**
     * @brief Set boolean value
     * @param section Configuration section
     * @param key Configuration key
     * @param value Boolean value to set
     */
    void SetBool(const wxString& section, const wxString& key, bool value);

    /**
     * @brief Get color value
     * @param section Configuration section
     * @param key Configuration key
     * @param defaultVal Default value if not found
     * @return Color value
     */
    wxColour GetColor(const wxString& section, const wxString& key, const wxColour& defaultVal = *wxBLACK) const;

    /**
     * @brief Set color value
     * @param section Configuration section
     * @param key Configuration key
     * @param value Color value to set
     */
    void SetColor(const wxString& section, const wxString& key, const wxColour& value);

    /**
     * @brief Get array of strings
     * @param section Configuration section
     * @param key Configuration key
     * @return Vector of strings
     */
    std::vector<wxString> GetArrayString(const wxString& section, const wxString& key) const;

    /**
     * @brief Set array of strings
     * @param section Configuration section
     * @param key Configuration key
     * @param value Vector of strings to set
     */
    void SetArrayString(const wxString& section, const wxString& key, const std::vector<wxString>& value);

    /**
     * @brief Check if a key exists
     * @param section Configuration section
     * @param key Configuration key
     * @return True if the key exists
     */
    bool HasEntry(const wxString& section, const wxString& key) const;

    /**
     * @brief Delete a key
     * @param section Configuration section
     * @param key Configuration key
     * @return True if successful
     */
    bool DeleteEntry(const wxString& section, const wxString& key);

    /**
     * @brief Delete a section
     * @param section Configuration section
     * @return True if successful
     */
    bool DeleteSection(const wxString& section);

    /**
     * @brief Get all sections
     * @return Vector of section names
     */
    std::vector<wxString> GetSections() const;

    /**
     * @brief Get all keys in a section
     * @param section Configuration section
     * @return Vector of key names
     */
    std::vector<wxString> GetKeys(const wxString& section) const;

    /**
     * @brief Get the configuration file path
     * @return Configuration file path
     */
    wxString GetConfigFilePath() const { return m_configFilePath; }

private:
    std::unique_ptr<wxFileConfig> m_config;  ///< Configuration object
    wxString m_configFilePath;               ///< Configuration file path

    // Cache for faster access
    mutable std::unordered_map<wxString, std::unordered_map<wxString, wxString>> m_cache;

    // Clear the cache
    void ClearCache();

    // Get default configuration file path
    static wxString GetDefaultConfigPath();

    // Create default configuration
    void CreateDefaultConfig();
};

} // namespace ITD 