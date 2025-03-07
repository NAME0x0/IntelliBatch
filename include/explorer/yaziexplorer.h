#pragma once

#include <wx/wx.h>
#include <wx/process.h>
#include <string>

namespace ITD {

/**
 * @brief File explorer component using Yazi
 * 
 * This class integrates the Yazi file manager for file exploration
 * functionality within the terminal environment.
 */
class YaziExplorer : public wxPanel {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param id Window ID
     * @param pos Window position
     * @param size Window size
     * @param style Window style
     */
    YaziExplorer(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxBORDER_NONE);
    
    /**
     * @brief Destructor
     */
    virtual ~YaziExplorer();

    /**
     * @brief Navigate to a specific directory
     * @param path Directory path to navigate to
     * @return True if successful, false otherwise
     */
    bool NavigateTo(const wxString& path);

    /**
     * @brief Get the current directory
     * @return Current directory path
     */
    wxString GetCurrentDirectory() const { return m_currentPath; }

    /**
     * @brief Refresh the explorer view
     */
    void Refresh();

    /**
     * @brief Set transparency level
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

    /**
     * @brief Check if the explorer is running
     * @return True if explorer is running, false otherwise
     */
    bool IsRunning() const { return m_process != nullptr && m_pid > 0; }

private:
    wxWindow* m_yaziWindow = nullptr;  ///< Window containing Yazi
    wxProcess* m_process = nullptr;    ///< Process for running Yazi
    long m_pid = 0;                    ///< Process ID
    wxString m_currentPath;            ///< Current directory path
    unsigned char m_transparency = 255; ///< Transparency level

    // Yazi process management
    bool StartYaziProcess();
    void StopYaziProcess();
    void OnProcessTerminate(wxProcessEvent& event);

    // Event handlers
    void OnSize(wxSizeEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    wxDECLARE_EVENT_TABLE();
};

} // namespace ITD 