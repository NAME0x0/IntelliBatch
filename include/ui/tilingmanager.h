#pragma once

#include <wx/wx.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ITD {

/**
 * @brief Window tiling layout manager
 * 
 * This class manages the layout of windows in a tiled arrangement,
 * similar to tiling window managers in Linux.
 */
class TilingManager : public wxEvtHandler {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     */
    explicit TilingManager(wxWindow* parent);
    
    /**
     * @brief Destructor
     */
    virtual ~TilingManager();

    /**
     * @brief Layout types
     */
    enum class LayoutType {
        Horizontal,    ///< Windows arranged horizontally
        Vertical,      ///< Windows arranged vertically
        Grid,          ///< Windows arranged in a grid
        Stacked,       ///< Windows stacked (only one visible at a time)
        Tabbed         ///< Windows arranged as tabs
    };

    /**
     * @brief Set the current layout type
     * @param type Layout type
     */
    void SetLayoutType(LayoutType type);

    /**
     * @brief Get the current layout type
     * @return Current layout type
     */
    LayoutType GetLayoutType() const { return m_currentLayout; }

    /**
     * @brief Add a window to be managed
     * @param window Window to add
     * @param name Window name
     */
    void AddWindow(wxWindow* window, const wxString& name);

    /**
     * @brief Remove a window from management
     * @param window Window to remove
     * @return True if successful, false otherwise
     */
    bool RemoveWindow(wxWindow* window);

    /**
     * @brief Focus a specific window
     * @param window Window to focus
     */
    void FocusWindow(wxWindow* window);

    /**
     * @brief Get all managed windows
     * @return Vector of window pointers
     */
    const std::vector<wxWindow*>& GetWindows() const { return m_windows; }

    /**
     * @brief Get the window name
     * @param window Window to get name for
     * @return Window name
     */
    wxString GetWindowName(wxWindow* window) const;

    /**
     * @brief Update the layout
     * 
     * Call this when the parent window size changes or when
     * windows are added/removed.
     */
    void UpdateLayout();

    /**
     * @brief Set transparency level for all managed windows
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

    /**
     * @brief Cycle focus to the next window
     */
    void CycleWindowFocus();

    /**
     * @brief Split the current window
     * @param direction Split direction (true for horizontal, false for vertical)
     */
    void SplitWindow(bool horizontal);

private:
    wxWindow* m_parent;                    ///< Parent window
    std::vector<wxWindow*> m_windows;      ///< Managed windows
    std::unordered_map<wxWindow*, wxString> m_windowNames; ///< Window names
    LayoutType m_currentLayout = LayoutType::Grid; ///< Current layout type
    wxWindow* m_focusedWindow = nullptr;   ///< Currently focused window
    unsigned char m_transparency = 255;    ///< Transparency level

    // Layout methods
    void LayoutHorizontal();
    void LayoutVertical();
    void LayoutGrid();
    void LayoutStacked();
    void LayoutTabbed();

    // Event handlers
    void OnParentSize(wxSizeEvent& event);
    void OnWindowClose(wxCloseEvent& event);
};

} // namespace ITD 