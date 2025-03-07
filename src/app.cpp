#include "app.h"
#include "mainframe.h"
#include <wx/wx.h>
#include <wx/splash.h>
#include <wx/image.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

namespace ITD {

bool App::OnInit() {
    // Initialize the wxWidgets framework
    if (!wxApp::OnInit())
        return false;

    // Initialize image handlers
    wxInitAllImageHandlers();

    // Load configuration
    if (!m_configManager.Load()) {
        wxLogError("Failed to load configuration");
    }

    // Create the main application window
    m_mainFrame = new MainFrame(
        "Integrated Terminal Desktop",
        wxDefaultPosition, 
        wxDefaultSize
    );

    // Show the main frame
    m_mainFrame->Show(true);
    SetTopWindow(m_mainFrame);

    return true;
}

int App::OnExit() {
    // Save configuration
    m_configManager.Save();

    return wxApp::OnExit();
}

} // namespace ITD 