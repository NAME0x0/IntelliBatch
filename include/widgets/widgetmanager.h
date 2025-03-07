#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace ITD {

// Forward declarations
class Widget;

/**
 * @brief Base class for all widgets
 * 
 * This class defines the interface for all widgets that can be managed
 * by the WidgetManager.
 */
class Widget : public wxPanel {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param id Widget ID
     * @param title Widget title
     * @param pos Widget position
     * @param size Widget size
     * @param style Widget style
     */
    Widget(wxWindow* parent, wxWindowID id = wxID_ANY,
           const wxString& title = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxBORDER_NONE);
    
    /**
     * @brief Destructor
     */
    virtual ~Widget();

    /**
     * @brief Get the widget title
     * @return Widget title
     */
    const wxString& GetTitle() const { return m_title; }

    /**
     * @brief Set the widget title
     * @param title New widget title
     */
    void SetTitle(const wxString& title) { m_title = title; }

    /**
     * @brief Get the widget type identifier
     * @return Widget type as a string
     */
    virtual wxString GetWidgetType() const = 0;

    /**
     * @brief Save widget configuration
     * @param config Configuration object to save to
     */
    virtual void SaveConfig(wxConfigBase* config) const = 0;

    /**
     * @brief Load widget configuration
     * @param config Configuration object to load from
     */
    virtual void LoadConfig(wxConfigBase* config) = 0;

    /**
     * @brief Refresh the widget content
     */
    virtual void RefreshContent() = 0;

    /**
     * @brief Set transparency level
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    virtual void SetTransparency(unsigned char alpha) { m_transparency = alpha; }

protected:
    wxString m_title;                  ///< Widget title
    unsigned char m_transparency = 255; ///< Transparency level
};

/**
 * @brief Widget manager for ITD
 * 
 * This class manages all widgets in the application, including creation,
 * destruction, and layout of widgets.
 */
class WidgetManager : public wxEvtHandler {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param auiManager AUI manager for docking
     */
    WidgetManager(wxWindow* parent, wxAuiManager* auiManager);
    
    /**
     * @brief Destructor
     */
    virtual ~WidgetManager();

    /**
     * @brief Create a new widget of the specified type
     * @param type Widget type identifier
     * @param title Widget title
     * @return Pointer to the created widget, or nullptr if creation failed
     */
    Widget* CreateWidget(const wxString& type, const wxString& title = wxEmptyString);

    /**
     * @brief Remove a widget
     * @param widget Pointer to the widget to remove
     * @return True if successful, false otherwise
     */
    bool RemoveWidget(Widget* widget);

    /**
     * @brief Get all widgets
     * @return Vector of widget pointers
     */
    const std::vector<Widget*>& GetWidgets() const { return m_widgets; }

    /**
     * @brief Get available widget types
     * @return Vector of widget type identifiers
     */
    std::vector<wxString> GetAvailableWidgetTypes() const;

    /**
     * @brief Load all widgets from configuration
     */
    void LoadFromConfig();

    /**
     * @brief Save all widgets to configuration
     */
    void SaveToConfig();

    /**
     * @brief Refresh all widgets
     */
    void RefreshAllWidgets();

    /**
     * @brief Set transparency level for all widgets
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

private:
    wxWindow* m_parent;                ///< Parent window
    wxAuiManager* m_auiManager;        ///< AUI manager for docking
    std::vector<Widget*> m_widgets;    ///< List of all widgets

    // Factory method for creating widgets
    using WidgetCreatorFunction = std::function<Widget*(wxWindow*)>;
    std::unordered_map<wxString, WidgetCreatorFunction> m_widgetFactories;

    // Initialize widget factories
    void InitializeWidgetFactories();
};

} // namespace ITD 