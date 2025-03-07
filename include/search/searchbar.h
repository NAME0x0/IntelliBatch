#pragma once

#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <vector>
#include "search/indexer.h"

namespace ITD {

/**
 * @brief Search bar component
 * 
 * This class provides a search interface for quickly finding files,
 * applications, and executing commands.
 */
class SearchBar : public wxPanel {
public:
    /**
     * @brief Constructor
     * @param parent Parent window
     * @param indexer File indexer instance
     * @param id Window ID
     * @param pos Window position
     * @param size Window size
     * @param style Window style
     */
    SearchBar(wxWindow* parent, Indexer* indexer, wxWindowID id = wxID_ANY,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxBORDER_NONE);
    
    /**
     * @brief Destructor
     */
    virtual ~SearchBar();

    /**
     * @brief Show the search bar
     * @param show True to show, false to hide
     */
    void Show(bool show = true) override;

    /**
     * @brief Check if search bar is visible
     * @return True if visible
     */
    bool IsShown() const override;

    /**
     * @brief Set focus to the search control
     */
    void SetFocus() override;

    /**
     * @brief Clear the search text
     */
    void Clear();

    /**
     * @brief Set search text
     * @param text Text to set
     */
    void SetSearchText(const wxString& text);

    /**
     * @brief Get search text
     * @return Current search text
     */
    wxString GetSearchText() const;

    /**
     * @brief Set transparency level
     * @param alpha Alpha value (0-255, where 0 is fully transparent and 255 is opaque)
     */
    void SetTransparency(unsigned char alpha);

private:
    wxSearchCtrl* m_searchCtrl = nullptr;  ///< Search control
    wxListBox* m_resultsList = nullptr;    ///< Results list box
    Indexer* m_indexer = nullptr;          ///< File indexer instance
    std::vector<SearchResult> m_results;   ///< Search results
    unsigned char m_transparency = 255;    ///< Transparency level
    bool m_isVisible = false;              ///< Visibility state

    // Event handlers
    void OnSearchText(wxCommandEvent& event);
    void OnSearchCancel(wxCommandEvent& event);
    void OnResultSelected(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);

    // UI update methods
    void UpdateResults(const wxString& query);
    void UpdateResultsList();

    wxDECLARE_EVENT_TABLE();
};

} // namespace ITD 