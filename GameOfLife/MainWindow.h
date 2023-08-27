//This code is a wxWidgets application that runs a simulation of Conways Game of Life and is reliant on wxWidgets binaries
// written by Greene-K - FullSail University - Computer Science
#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include <string>
#include "Settings.h"
#include "SettingsUI.h"
#include <stdlib.h>
#include <time.h>
#include <wx/textdlg.h> 
#include <iostream>
#include <wx/filedlg.h>
#include <cstdio>

class MainWindow :
    public wxFrame
{
private:
    DrawingPanel* p_DrawPan = nullptr; // pointer to the drawing panel wxPanel. Main body of the window
    wxToolBar* p_toolBar = nullptr; // toolbar, Buttons: play, pause, next, clear
    wxBoxSizer* p_sizer = nullptr; // sizes the window
    wxTimer* p_timer = nullptr; // timer, runs when play is pressed
    wxStatusBar* p_statusBar = nullptr; // status bar at bottom of window, displays living cells and num of generations
    wxMenuBar* p_menuBar = new wxMenuBar(); // menu bar that holds a menu option for settings. 
    wxMenu* p_settingsMenu = new wxMenu(); // holds the option to open the settings UI
    wxMenu* p_randomizeCellsMenu = new wxMenu(); // randomizes the cells when clicked
    wxMenu* p_gameSaves = new wxMenu(); // menu option for saving games
    wxMenu* p_viewMenu = new wxMenu(); // menu of clickboxes
    wxMenu* p_resetSettings = new wxMenu(); // menu to reset settings
    wxMenuItem* p_reset = nullptr; // reset settings
    wxMenuItem* p_torodial = nullptr; // set torroidal
    wxMenuItem* p_finite = nullptr; // set finite
    wxMenuItem* p_gridVisibility = nullptr; // set grid visibility
    wxMenuItem* p_showThickGrid = nullptr; // show a 10x10 grid
    wxMenuItem* p_showHUD = nullptr; // show the HUD
    
    wxString m_fileNameInUse = wxEmptyString;
    wxString m_fileDirectory = wxEmptyString;
    wxString m_defaultFile = wxEmptyString;

    Settings m_settings; // holds the settings for cell color, gridsize, windowSize.

    int m_generations = 0; // store the number of times the board has excelled a generation

    std::vector<std::vector<bool>> m_gameBoardDblVoV; // Main 2d vector that stores the states of the cells as booleans
    std::vector<std::vector<bool>> m_gameBoardPredictions; // 2d vector that holds the predictions for the next generation of cells
    std::vector<std::vector<int>> m_cellNeighborCounts; //stores the neighbor counts of all the cells on the board

    std::string livingCellString = "living cells: "; // string for status bar
    std::string generationsString = " |  Generations: "; // string for status bar

public:
    MainWindow();
    ~MainWindow();

    void onWindowSizeChange(wxSizeEvent& event);
    void onTimerTick(wxTimerEvent& event);
    void onPlayButtonClick(wxCommandEvent& event);
    void onPauseButtonClick(wxCommandEvent& event);
    void onNextButtonClick(wxCommandEvent& event);
    void onTrashButtonClick(wxCommandEvent& event);
    void onMenuSettingsOptionClick(wxCommandEvent& event);
    void onRandomizeOptionClick(wxCommandEvent& event);
    void onNewBoard(wxCommandEvent& event);
    void onSaveBoard(wxCommandEvent& event);
    void onSaveAs(wxCommandEvent& event);
    void onLoadBoard(wxCommandEvent& event);
    void onExitBoard(wxCommandEvent& event);
    void onTorodialCheck(wxCommandEvent& event);
    void onFiniteCheck(wxCommandEvent& event);
    void onResetSettings(wxCommandEvent& event);
    void onGridVisibility(wxCommandEvent& event);
    void onShowThickGrid(wxCommandEvent& event);
    void onShowHUD(wxCommandEvent& event);
    void onImport(wxCommandEvent& event);

    void loadGameBoard(const char* fileName);
    void importGameBoard(const char* fileName);
    bool saveGameBoard(const char* fileName);
    void updateDrawingPanelSettings(Settings& settings);
    void initializeGridCells(float gridSize);
    int findNeighbors(int row, int col);
    void findPerimeterNeighbors(int row, int col, int& numOfNeighbors);
    void findFiniteNeighbors(int row, int col, int&numOfNeighbors);
    void predictNextUniverse();
    void updateStatusBar();
    void makePredictions(int row, int col, int liveNeighbors);
    bool isNumber(std::string string);
    void resetBoard();
    int getGenerations();
    

    wxDECLARE_EVENT_TABLE();
};

