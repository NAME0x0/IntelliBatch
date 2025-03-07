#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include "terminal/terminalwx.h"
#include "widgets/widgetmanager.h"
#include "explorer/yaziexplorer.h"
#include "ui/taskbar.h"
#include "ui/tilingmanager.h"
#include "search/searchbar.h"

namespace ITD {

/**
 * @brief Main application window for the Integrated Terminal Desktop
 * 
 * This class represents the main window of the application, containing
 * the terminal, widgets, taskbar, and other UI elements.
 */
class MainFrame : public wxFrame {
public:
    /**
     * @brief Constructor
     * @param title Window title
     * @param pos Window position
     * @param size Window size
     */
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    /**
     * @brief Destructor
     */
    virtual ~MainFrame();

private:
    // UI Components
    wxAuiManager m_auiManager;         ///< AUI manager for dockable panels
    TerminalWx* m_terminal = nullptr;  ///< Terminal emulator widget
    WidgetManager* m_widgetManager = nullptr;  ///< Widget manager
    YaziExplorer* m_explorer = nullptr;  ///< File explorer
    Taskbar* m_taskbar = nullptr;      ///< Custom taskbar
    TilingManager* m_tilingManager = nullptr;  ///< Window tiling manager
    SearchBar* m_searchBar = nullptr;  ///< Search bar

    // Event handlers
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnToggleFullScreen(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);
    void OnToggleExplorer(wxCommandEvent& event);
    void OnToggleWidgets(wxCommandEvent& event);
    void OnPreferences(wxCommandEvent& event);

    // Initialize methods
    void InitMenuBar();
    void InitStatusBar();
    void InitComponents();
    void InitLayout();
    void InitKeyBindings();

    // Window event handlers
    void OnSize(wxSizeEvent& event);
    void OnClose(wxCloseEvent& event);

    wxDECLARE_EVENT_TABLE();
};

} // namespace ITD 