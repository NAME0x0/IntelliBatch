#pragma once

#include <wx/wx.h>
#include "mainframe.h"
#include "config/configmanager.h"

namespace ITD {

/**
 * @brief Main application class for the Integrated Terminal Desktop
 * 
 * This class manages the application lifecycle and initializes all required components.
 */
class App : public wxApp {
public:
    /**
     * @brief Called when the application is initialized
     * @return True if initialization was successful, false otherwise
     */
    virtual bool OnInit() override;
    
    /**
     * @brief Called when the application exits
     * @return Exit code
     */
    virtual int OnExit() override;

    /**
     * @brief Get the configuration manager instance
     * @return Reference to the configuration manager
     */
    ConfigManager& GetConfigManager() { return m_configManager; }

private:
    MainFrame* m_mainFrame = nullptr;  ///< Main application window
    ConfigManager m_configManager;     ///< Configuration manager
};

} // namespace ITD

// Declare the application object as externally available
wxDECLARE_APP(ITD::App); 