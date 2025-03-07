#include "terminal/terminalwx.h"
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/textctrl.h>
#include <wx/dir.h>

// Event table for TerminalWx
wxBEGIN_EVENT_TABLE(ITD::TerminalWx, wxPanel)
    EVT_KEY_DOWN(ITD::TerminalWx::OnKeyDown)
    EVT_SIZE(ITD::TerminalWx::OnSize)
    EVT_CHAR(ITD::TerminalWx::OnChar)
wxEND_EVENT_TABLE()

namespace ITD {

TerminalWx::TerminalWx(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                   const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style),
      m_currentDirectory(wxGetCwd()) {
    
    // Create the text control
    m_textCtrl = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    
    // Configure the text control
    m_textCtrl->SetMarginWidth(0, 0);  // Hide line numbers
    m_textCtrl->SetMarginWidth(1, 0);
    m_textCtrl->SetUseHorizontalScrollBar(true);
    m_textCtrl->SetUseVerticalScrollBar(true);
    m_textCtrl->SetWrapMode(wxSTC_WRAP_WORD);
    m_textCtrl->SetViewWhiteSpace(wxSTC_WS_INVISIBLE);
    m_textCtrl->SetViewEOL(false);
    m_textCtrl->SetEdgeMode(wxSTC_EDGE_NONE);
    
    // Set monospace font
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE).FaceName("Consolas"));
    m_textCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    
    // Set colors
    m_textCtrl->StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxWHITE);
    m_textCtrl->StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxBLACK);
    m_textCtrl->StyleClearAll();
    
    // Initialize the terminal
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 0);
    SetSizer(sizer);
    
    // Display initial prompt
    m_textCtrl->AppendText(m_currentDirectory + ">");
    m_inputStart = m_textCtrl->GetLength();
    
    // Focus the text control
    m_textCtrl->SetFocus();
}

TerminalWx::~TerminalWx() {
    // Terminate the process if running
    if (m_process) {
        if (m_pid > 0) {
            wxProcess::Kill(m_pid);
        }
        delete m_process;
    }
}

bool TerminalWx::ExecuteCommand(const wxString& command) {
    if (m_isBusy)
        return false;
    
    // Display the command
    m_textCtrl->AppendText("\r\n");
    
    // Special command handling
    if (command.Lower() == "cls" || command.Lower() == "clear") {
        m_textCtrl->ClearAll();
        m_textCtrl->AppendText(m_currentDirectory + ">");
        m_inputStart = m_textCtrl->GetLength();
        return true;
    }
    
    if (command.Lower().StartsWith("cd ")) {
        wxString path = command.Mid(3).Trim();
        if (wxDir::Exists(path)) {
            m_currentDirectory = path;
            if (m_currentDirectory.Last() != '\\' && m_currentDirectory.Last() != '/')
                m_currentDirectory += "\\";
            
            m_textCtrl->AppendText("\r\n" + m_currentDirectory + ">");
            m_inputStart = m_textCtrl->GetLength();
            return true;
        } else {
            m_textCtrl->AppendText("\r\nDirectory not found: " + path);
            m_textCtrl->AppendText("\r\n" + m_currentDirectory + ">");
            m_inputStart = m_textCtrl->GetLength();
            return false;
        }
    }
    
    // Execute the command
    m_isBusy = true;
    
    // Create the process
    m_process = new wxProcess(this);
    m_process->Redirect();
    
    // Execute the command
    wxString fullCommand = "cmd.exe /c cd " + m_currentDirectory + " && " + command;
    m_pid = wxExecute(fullCommand, wxEXEC_ASYNC, m_process);
    
    if (m_pid <= 0) {
        m_textCtrl->AppendText("\r\nFailed to execute command: " + command);
        m_textCtrl->AppendText("\r\n" + m_currentDirectory + ">");
        m_inputStart = m_textCtrl->GetLength();
        m_isBusy = false;
        delete m_process;
        m_process = nullptr;
        return false;
    }
    
    // Add to command history
    m_commandHistory.push_back(command);
    m_historyIndex = m_commandHistory.size();
    
    // Read output asynchronously
    ReadProcessOutput();
    
    return true;
}

void TerminalWx::SetTerminalFont(const wxString& fontName, int fontSize) {
    wxFont font(wxFontInfo(fontSize).Family(wxFONTFAMILY_TELETYPE).FaceName(fontName));
    m_textCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    m_textCtrl->StyleClearAll();
}

void TerminalWx::SetColorScheme(const wxColour& foreground, const wxColour& background, 
                              const wxColour& selection) {
    m_textCtrl->StyleSetForeground(wxSTC_STYLE_DEFAULT, foreground);
    m_textCtrl->StyleSetBackground(wxSTC_STYLE_DEFAULT, background);
    m_textCtrl->SetSelBackground(true, selection);
    m_textCtrl->StyleClearAll();
}

void TerminalWx::SetTransparency(unsigned char alpha) {
    m_transparency = alpha;
    // On Windows, transparency can be set with the SetTransparent function
    wxTopLevelWindow* topWindow = wxDynamicCast(wxGetTopLevelParent(this), wxTopLevelWindow);
    if (topWindow) {
        topWindow->SetTransparent(alpha);
    }
}

void TerminalWx::EnableVimMode(bool enable) {
    m_vimModeEnabled = enable;
    if (enable) {
        m_vimMode = VimMode::Normal;
    } else {
        m_vimMode = VimMode::Insert;
    }
}

void TerminalWx::OnKeyDown(wxKeyEvent& event) {
    if (m_isBusy) {
        // If busy, only allow certain keys
        switch (event.GetKeyCode()) {
            case WXK_ESCAPE:
                // Terminate the process
                if (m_pid > 0) {
                    wxProcess::Kill(m_pid);
                }
                break;
            default:
                // Pass the key to the process
                if (m_process && m_process->IsInputOpened()) {
                    wxChar ch = event.GetUnicodeKey();
                    if (ch != WXK_NONE) {
                        wxString str(ch);
                        WriteProcessInput(str);
                    }
                }
                break;
        }
        return;
    }
    
    // Handle vim mode if enabled
    if (m_vimModeEnabled) {
        HandleVimModeInput(event);
        return;
    }
    
    // Normal mode
    switch (event.GetKeyCode()) {
        case WXK_RETURN:
        {
            // Get the command
            wxString command = m_textCtrl->GetTextRange(m_inputStart, m_textCtrl->GetLength());
            
            // Execute the command
            ExecuteCommand(command);
            break;
        }
        case WXK_UP:
            // Navigate command history
            if (m_historyIndex > 0) {
                m_historyIndex--;
                wxString cmd = m_commandHistory[m_historyIndex];
                m_textCtrl->SetTargetStart(m_inputStart);
                m_textCtrl->SetTargetEnd(m_textCtrl->GetLength());
                m_textCtrl->ReplaceTarget(cmd);
            }
            break;
        case WXK_DOWN:
            // Navigate command history
            if (m_historyIndex < m_commandHistory.size() - 1) {
                m_historyIndex++;
                wxString cmd = m_commandHistory[m_historyIndex];
                m_textCtrl->SetTargetStart(m_inputStart);
                m_textCtrl->SetTargetEnd(m_textCtrl->GetLength());
                m_textCtrl->ReplaceTarget(cmd);
            } else if (m_historyIndex == m_commandHistory.size() - 1) {
                m_historyIndex = m_commandHistory.size();
                m_textCtrl->SetTargetStart(m_inputStart);
                m_textCtrl->SetTargetEnd(m_textCtrl->GetLength());
                m_textCtrl->ReplaceTarget("");
            }
            break;
        case WXK_HOME:
            // Move to the beginning of the input
            m_textCtrl->GotoPos(m_inputStart);
            break;
        case WXK_BACK:
            // Prevent deleting the prompt
            if (m_textCtrl->GetCurrentPos() <= m_inputStart) {
                break;
            }
            event.Skip();
            break;
        default:
            // Pass the event to the text control
            event.Skip();
            break;
    }
}

void TerminalWx::OnChar(wxKeyEvent& event) {
    // Ensure the cursor is after the prompt
    if (m_textCtrl->GetCurrentPos() < m_inputStart) {
        m_textCtrl->GotoPos(m_textCtrl->GetLength());
    }
    
    event.Skip();
}

void TerminalWx::OnSize(wxSizeEvent& event) {
    // Resize the text control
    m_textCtrl->SetSize(GetClientSize());
    event.Skip();
}

void TerminalWx::ReadProcessOutput() {
    if (!m_process)
        return;
    
    if (m_process->IsInputAvailable()) {
        wxTextInputStream tis(*m_process->GetInputStream());
        wxString line = tis.ReadLine();
        m_textCtrl->AppendText("\r\n" + line);
        
        // Continue reading
        wxMilliSleep(10);
        ReadProcessOutput();
    } else if (m_process->IsErrorAvailable()) {
        wxTextInputStream tis(*m_process->GetErrorStream());
        wxString line = tis.ReadLine();
        m_textCtrl->AppendText("\r\n" + line);
        
        // Continue reading
        wxMilliSleep(10);
        ReadProcessOutput();
    } else {
        // Check if the process is still running
        if (!m_isBusy) {
            delete m_process;
            m_process = nullptr;
            
            // Display new prompt
            m_textCtrl->AppendText("\r\n" + m_currentDirectory + ">");
            m_inputStart = m_textCtrl->GetLength();
        } else {
            // Wait for more output
            wxMilliSleep(50);
            wxGetApp().Yield();
            ReadProcessOutput();
        }
    }
}

void TerminalWx::WriteProcessInput(const wxString& input) {
    if (!m_process || !m_process->IsInputOpened())
        return;
    
    wxTextOutputStream tos(*m_process->GetOutputStream());
    tos.WriteString(input);
}

void TerminalWx::HandleVimModeInput(wxKeyEvent& event) {
    // TODO: Implement Vim mode
    event.Skip();
}

void TerminalWx::SetVimMode(VimMode mode) {
    m_vimMode = mode;
    
    // Update UI based on mode
    switch (mode) {
        case VimMode::Normal:
            // Normal mode - cursor should be block
            break;
        case VimMode::Insert:
            // Insert mode - cursor should be line
            break;
        case VimMode::Visual:
            // Visual mode - cursor should be block and selection enabled
            break;
        case VimMode::Command:
            // Command mode - show command line at bottom
            break;
    }
}

} // namespace ITD 