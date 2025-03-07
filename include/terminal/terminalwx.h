#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <vector>
#include <string>

namespace ITD {

/**
 * @brief Terminal emulation widget for ITD
 * 
 * This class provides terminal emulation functionality using the wxStyledTextCtrl
 * component, with custom enhancements for the Integrated Terminal Desktop.
 */
class TerminalWx : public wxPanel {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param id Window ID
     * @param pos Window position
     * @param size Window size
     * @param style Window style
     */
    TerminalWx(wxWindow* parent, wxWindowID id = wxID_ANY,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxBORDER_NONE);
    
    /**
     * @brief Destructor
     */
    virtual ~TerminalWx();

    /**
     * @brief Execute a command in the terminal
     * @param command Command to execute
     * @return True if successful, false otherwise
     */
    bool ExecuteCommand(const wxString& command);

    /**
     * @brief Set the terminal font and size
     * @param fontName Font name
     * @param fontSize Font size
     */
    void SetTerminalFont(const wxString& fontName, int fontSize);

    /**
     * @brief Set the terminal color scheme
     * @param foreground Foreground color
     * @param background Background color
     * @param selection Selection color
     */
    void SetColorScheme(const wxColour& foreground, const wxColour& background, const wxColour& selection);

    /**
     * @brief Set transparency level
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

    /**
     * @brief Check if terminal is busy
     * @return True if terminal is executing a command, false otherwise
     */
    bool IsBusy() const { return m_isBusy; }

    /**
     * @brief Get the current directory of the terminal
     * @return Current working directory
     */
    wxString GetCurrentDirectory() const { return m_currentDirectory; }

    /**
     * @brief Enable/disable Vim mode
     * @param enable True to enable Vim mode, false to disable
     */
    void EnableVimMode(bool enable);

private:
    wxStyledTextCtrl* m_textCtrl = nullptr;  ///< Styled text control
    wxProcess* m_process = nullptr;          ///< Process for command execution
    long m_pid = 0;                          ///< Process ID
    bool m_isBusy = false;                   ///< Terminal busy status
    wxString m_currentDirectory;             ///< Current working directory
    bool m_vimModeEnabled = false;           ///< Vim mode status
    int m_inputStart = 0;                    ///< Start position of user input
    unsigned char m_transparency = 255;      ///< Transparency level

    // Command history
    std::vector<wxString> m_commandHistory;
    size_t m_historyIndex = 0;

    // Event handlers
    void OnTextEnter(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnProcessTerminate(wxProcessEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnChar(wxKeyEvent& event);

    // Terminal I/O
    void ReadProcessOutput();
    void WriteProcessInput(const wxString& input);

    // Vim mode related
    void HandleVimModeInput(wxKeyEvent& event);
    enum class VimMode { Normal, Insert, Visual, Command };
    VimMode m_vimMode = VimMode::Insert;
    void SetVimMode(VimMode mode);

    wxDECLARE_EVENT_TABLE();
};

} // namespace ITD 