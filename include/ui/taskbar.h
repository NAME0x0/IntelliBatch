#pragma once

#include <wx/wx.h>
#include <wx/timer.h>
#include <vector>
#include <unordered_map>

namespace ITD {

/**
 * @brief Task information structure
 */
struct TaskInfo {
    wxString name;           ///< Task name
    wxIcon icon;             ///< Task icon
    wxWindow* window;        ///< Associated window
    bool isActive;           ///< True if task is active
    
    /**
     * @brief Constructor
     */
    TaskInfo() : window(nullptr), isActive(false) {}
};

/**
 * @brief Custom taskbar component for ITD
 * 
 * This class provides a taskbar for managing running applications
 * and system status, similar to the Windows taskbar but customized
 * for the Integrated Terminal Desktop.
 */
class Taskbar : public wxPanel {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param id Window ID
     * @param pos Window position
     * @param size Window size
     * @param style Window style
     */
    Taskbar(wxWindow* parent, wxWindowID id = wxID_ANY,
          const wxPoint& pos = wxDefaultPosition,
          const wxSize& size = wxDefaultSize,
          long style = wxBORDER_NONE);
    
    /**
     * @brief Destructor
     */
    virtual ~Taskbar();

    /**
     * @brief Position enum for taskbar placement
     */
    enum class Position {
        Top,      ///< Taskbar at the top
        Bottom,   ///< Taskbar at the bottom
        Left,     ///< Taskbar at the left
        Right     ///< Taskbar at the right
    };

    /**
     * @brief Set taskbar position
     * @param position Taskbar position
     */
    void SetPosition(Position position);

    /**
     * @brief Get taskbar position
     * @return Current taskbar position
     */
    Position GetPosition() const { return m_position; }

    /**
     * @brief Add a task to the taskbar
     * @param name Task name
     * @param icon Task icon
     * @param window Associated window
     * @return True if successful
     */
    bool AddTask(const wxString& name, const wxIcon& icon, wxWindow* window);

    /**
     * @brief Remove a task from the taskbar
     * @param window Associated window
     * @return True if successful
     */
    bool RemoveTask(wxWindow* window);

    /**
     * @brief Set active task
     * @param window Associated window
     */
    void SetActiveTask(wxWindow* window);

    /**
     * @brief Set transparency level
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

    /**
     * @brief Show/hide the taskbar
     * @param show True to show, false to hide
     */
    void Show(bool show = true) override;

    /**
     * @brief Check if the taskbar is visible
     * @return True if visible
     */
    bool IsShown() const override { return m_isVisible; }

private:
    std::vector<TaskInfo> m_tasks;    ///< List of tasks
    Position m_position = Position::Bottom;  ///< Taskbar position
    unsigned char m_transparency = 255;  ///< Transparency level
    bool m_isVisible = true;         ///< Visibility state
    wxSize m_itemSize;               ///< Size of each task item
    wxTimer m_clockTimer;            ///< Timer for clock updates
    wxString m_clockText;            ///< Current clock text

    // UI components
    wxBoxSizer* m_mainSizer = nullptr;  ///< Main sizer
    wxBoxSizer* m_tasksSizer = nullptr;  ///< Sizer for tasks
    wxStaticText* m_clockLabel = nullptr;  ///< Clock label

    // Event handlers
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnTaskSelected(wxCommandEvent& event);
    void OnClockTimer(wxTimerEvent& event);
    void OnRightClick(wxContextMenuEvent& event);

    // UI update methods
    void UpdateLayout();
    void UpdateTaskList();
    void UpdateClock();

    // Helper methods
    wxRect GetTaskRect(size_t index) const;

    wxDECLARE_EVENT_TABLE();
};

} // namespace ITD 