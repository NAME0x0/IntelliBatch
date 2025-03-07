#include "mainframe.h"
#include "app.h"
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/menu.h>
#include <wx/statusbr.h>

// Event table for MainFrame
wxBEGIN_EVENT_TABLE(ITD::MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, ITD::MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ITD::MainFrame::OnAbout)
    EVT_MENU(wxID_PREFERENCES, ITD::MainFrame::OnPreferences)
    EVT_SIZE(ITD::MainFrame::OnSize)
    EVT_CLOSE(ITD::MainFrame::OnClose)
wxEND_EVENT_TABLE()

namespace ITD {

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    
    // Initialize the AUI manager
    m_auiManager.SetManagedWindow(this);
    
    // Initialize UI components
    InitMenuBar();
    InitStatusBar();
    InitComponents();
    InitLayout();
    InitKeyBindings();
    
    // Set icon and size
    SetIcon(wxICON(MAINICON)); // This assumes you have an icon resource
    SetSize(wxSize(1024, 768));
    
    // Center on screen
    CenterOnScreen();
    
    // Update the AUI manager
    m_auiManager.Update();
}

MainFrame::~MainFrame() {
    // Uninitialize the AUI manager
    m_auiManager.UnInit();
    
    // Clean up components
    delete m_terminal;
    delete m_widgetManager;
    delete m_explorer;
    delete m_taskbar;
    delete m_tilingManager;
    delete m_searchBar;
}

void MainFrame::InitMenuBar() {
    // Create menu bar
    wxMenuBar* menuBar = new wxMenuBar();
    
    // File menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "&New Terminal\tCtrl+N", "Open a new terminal");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+F4", "Quit this program");
    menuBar->Append(fileMenu, "&File");
    
    // Edit menu
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selection");
    editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste from clipboard");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_PREFERENCES, "&Preferences...\tCtrl+P", "Configure settings");
    menuBar->Append(editMenu, "&Edit");
    
    // View menu
    wxMenu* viewMenu = new wxMenu();
    viewMenu->Append(wxID_ANY, "&Toggle Explorer\tF3", "Show/hide the file explorer");
    viewMenu->Append(wxID_ANY, "Toggle &Widgets\tF4", "Show/hide widgets");
    viewMenu->Append(wxID_ANY, "Toggle &Taskbar\tF5", "Show/hide the taskbar");
    viewMenu->AppendSeparator();
    viewMenu->Append(wxID_ANY, "Full&screen\tF11", "Toggle fullscreen");
    menuBar->Append(viewMenu, "&View");
    
    // Help menu
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About", "Show about dialog");
    menuBar->Append(helpMenu, "&Help");
    
    // Set the menu bar
    SetMenuBar(menuBar);
}

void MainFrame::InitStatusBar() {
    // Create status bar
    wxStatusBar* statusBar = CreateStatusBar(3);
    
    // Set field widths
    static const int widths[] = { -1, 150, 100 };
    statusBar->SetStatusWidths(3, widths);
    
    // Set initial status text
    statusBar->SetStatusText("Ready", 0);
    statusBar->SetStatusText("", 1);
    statusBar->SetStatusText("ITD v0.1", 2);
}

void MainFrame::InitComponents() {
    // Create terminal
    m_terminal = new TerminalWx(this);
    
    // Create widget manager
    m_widgetManager = new WidgetManager(this, &m_auiManager);
    
    // Create file explorer
    m_explorer = new YaziExplorer(this);
    
    // Create taskbar
    m_taskbar = new Taskbar(this);
    
    // Create tiling manager
    m_tilingManager = new TilingManager(this);
    
    // Create search bar
    m_searchBar = new SearchBar(this, nullptr); // Need to initialize the indexer first
}

void MainFrame::InitLayout() {
    // Add terminal pane
    m_auiManager.AddPane(m_terminal, wxAuiPaneInfo()
        .Name("terminal")
        .Caption("Terminal")
        .CenterPane()
        .PaneBorder(false)
    );
    
    // Add explorer pane
    m_auiManager.AddPane(m_explorer, wxAuiPaneInfo()
        .Name("explorer")
        .Caption("Explorer")
        .Left()
        .Layer(1)
        .BestSize(wxSize(300, -1))
        .CloseButton(true)
        .MaximizeButton(true)
    );
    
    // Add taskbar
    m_taskbar->SetPosition(Taskbar::Position::Bottom);
    
    // Add the terminal to the tiling manager
    m_tilingManager->AddWindow(m_terminal, "Terminal");
}

void MainFrame::InitKeyBindings() {
    // TODO: Set up key bindings
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("Integrated Terminal Desktop");
    aboutInfo.SetVersion("0.1");
    aboutInfo.SetDescription("A Windows-based terminal designed to replace the full desktop");
    aboutInfo.SetCopyright("(C) 2025");
    
    wxAboutBox(aboutInfo);
}

void MainFrame::OnPreferences(wxCommandEvent& event) {
    // TODO: Show preferences dialog
}

void MainFrame::OnToggleFullScreen(wxCommandEvent& event) {
    ShowFullScreen(!IsFullScreen());
}

void MainFrame::OnSearch(wxCommandEvent& event) {
    m_searchBar->Show(!m_searchBar->IsShown());
    if (m_searchBar->IsShown()) {
        m_searchBar->SetFocus();
    }
}

void MainFrame::OnToggleExplorer(wxCommandEvent& event) {
    wxAuiPaneInfo& paneInfo = m_auiManager.GetPane("explorer");
    paneInfo.Show(!paneInfo.IsShown());
    m_auiManager.Update();
}

void MainFrame::OnToggleWidgets(wxCommandEvent& event) {
    // TODO: Implement widgets toggle
}

void MainFrame::OnSize(wxSizeEvent& event) {
    event.Skip();
}

void MainFrame::OnClose(wxCloseEvent& event) {
    event.Skip();
}

} // namespace ITD 