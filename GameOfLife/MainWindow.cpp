#define TOOLBAR_PLAY_ICON 10001
#define TOOLBAR_PAUSE_ICON 10002
#define TOOLBAR_NEXT_ICON 10003
#define TOOLBAR_TRASH_ICON 10004
#define SETTINGS_MENU_OPTION 10010
#define TIMER_ID 10020
#define RANDOMIZE_EVT_ID 10030
#define RANDOMIZE_WITH_SEED_EVT_ID 10031
#define IMPORT_ID 10032
#define SAVE_SETTINGS_FILE_NAME "settings.bin"
#define DEFAULT_SAVE_GAME "GameBoardSave.txt"
#define DEFAULT_SETTINGS "defaultSettings.bin"
#define TORODIAL_CHOICE_ID 10040
#define FINITE_CHOICE_ID 10041
#define GRID_VISIBILTY_ID 10042
#define SHOW_THICK_GRID_ID 10043
#define SHOW_HUD_ID 10044
#define RESET_SETTINGS_ID 10050

#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(TOOLBAR_PLAY_ICON, MainWindow::onPlayButtonClick)
EVT_MENU(TOOLBAR_PAUSE_ICON, MainWindow::onPauseButtonClick)
EVT_MENU(TOOLBAR_NEXT_ICON, MainWindow::onNextButtonClick)
EVT_MENU(TOOLBAR_TRASH_ICON, MainWindow::onTrashButtonClick)
EVT_MENU(SETTINGS_MENU_OPTION, MainWindow::onMenuSettingsOptionClick)
EVT_MENU(RANDOMIZE_EVT_ID, MainWindow::onRandomizeOptionClick)
EVT_MENU(RANDOMIZE_WITH_SEED_EVT_ID, MainWindow::onRandomizeOptionClick)
EVT_MENU(wxID_NEW, MainWindow::onNewBoard)
EVT_MENU(wxID_OPEN, MainWindow::onLoadBoard)
EVT_MENU(wxID_SAVE, MainWindow::onSaveBoard)
EVT_MENU(wxID_SAVEAS, MainWindow::onSaveAs)
EVT_MENU(wxID_EXIT, MainWindow::onExitBoard)
EVT_MENU(IMPORT_ID, MainWindow::onImport)
EVT_MENU(TORODIAL_CHOICE_ID, MainWindow::onTorodialCheck)
EVT_MENU(FINITE_CHOICE_ID, MainWindow::onFiniteCheck)
EVT_MENU(RESET_SETTINGS_ID, MainWindow::onResetSettings)
EVT_MENU(GRID_VISIBILTY_ID, MainWindow::onGridVisibility)
EVT_MENU(SHOW_THICK_GRID_ID, MainWindow::onShowThickGrid)
EVT_MENU(SHOW_HUD_ID, MainWindow::onShowHUD)
EVT_SIZE(MainWindow::onWindowSizeChange)
EVT_TIMER(TIMER_ID, MainWindow::onTimerTick)
wxEND_EVENT_TABLE()

//EVT_BUTTON(10020, MainWindow::onButtonClick)
MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "GameOfLife", wxPoint(0, 0), wxSize(1080, 720))
{
	m_settings.saveData(DEFAULT_SETTINGS);
	m_settings.loadData(SAVE_SETTINGS_FILE_NAME); // loads variables from the settings.bin and sets the variables in the Settings.h file to the values stored on the file 
	m_settings.numOfAliveCells = 0;

	p_statusBar = CreateStatusBar();
	p_statusBar->SetStatusText(livingCellString + std::to_string(m_settings.numOfAliveCells) + generationsString + std::to_string(m_generations));

	// sets menu bar
	SetMenuBar(p_menuBar);
	p_settingsMenu->Append(SETTINGS_MENU_OPTION, "Settings"); // adds settings button to menubar
	
	// adds the randomize and randomize with seed event to the menu. 
	p_randomizeCellsMenu->Append(RANDOMIZE_EVT_ID, "Randomize");
	p_randomizeCellsMenu->Append(RANDOMIZE_WITH_SEED_EVT_ID, "Randomize with Seed");

	// options for board loads and saves
	p_gameSaves->Append(wxID_NEW);
	p_gameSaves->Append(wxID_OPEN);
	p_gameSaves->Append(wxID_SAVE);
	p_gameSaves->Append(wxID_SAVEAS);
	p_gameSaves->Append(wxID_EXIT);
	p_gameSaves->Append(IMPORT_ID, "Import");

	// adds menu option for the universe type
	p_torodial = new wxMenuItem(p_viewMenu, TORODIAL_CHOICE_ID, "Toroidal", wxEmptyString, wxITEM_CHECK);
	p_finite = new wxMenuItem(p_viewMenu, FINITE_CHOICE_ID, "Finite", wxEmptyString, wxITEM_CHECK);
	p_gridVisibility = new wxMenuItem(p_viewMenu, GRID_VISIBILTY_ID, "Grid Visibility", wxEmptyString, wxITEM_CHECK);
	p_showThickGrid = new wxMenuItem(p_viewMenu, SHOW_THICK_GRID_ID, "Show thick grid", wxEmptyString, wxITEM_CHECK);
	p_showHUD = new wxMenuItem(p_viewMenu, SHOW_HUD_ID, "Show HUD", wxEmptyString, wxITEM_CHECK);

	p_torodial->SetCheckable(true);
	p_finite->SetCheckable(true);
	p_gridVisibility->SetCheckable(true);
	p_showThickGrid->SetCheckable(true);
	p_showHUD->SetCheckable(true);

	p_viewMenu->Append(p_torodial);
	p_viewMenu->Append(p_finite);
	p_viewMenu->Append(p_gridVisibility);
	p_viewMenu->Append(p_showThickGrid);
	p_viewMenu->Append(p_showHUD);

	p_showThickGrid->Check(m_settings.showThickGrid);
	p_gridVisibility->Check(m_settings.gridIsVisible);
	p_finite->Check(m_settings.universeState);
	p_torodial->Check(m_settings.universeState);
	p_showHUD->Check(m_settings.showHUD);

	// adds menu option to reset settings
	p_reset = new wxMenuItem(p_resetSettings, RESET_SETTINGS_ID, "Reset Settings", wxEmptyString, wxITEM_CHECK);
	p_reset->SetCheckable(true);
	p_resetSettings->Append(p_reset);

	// appends all menu items to the menu bar
	p_menuBar->Append(p_settingsMenu, "Edit Settings"); // gives edit settings options after main settings menu button is pressed
	p_menuBar->Append(p_randomizeCellsMenu, "Randomize Cell State");
	p_menuBar->Append(p_gameSaves, "Game Saves");
	p_menuBar->Append(p_viewMenu, "Universe Menu");
	p_menuBar->Append(p_resetSettings, "Reset Settings");

	// sets the toolbar icons to a bitmap for later resizing
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	// creates a tool bar and adds 4 icons
	p_toolBar = CreateToolBar();
	p_toolBar->AddTool(TOOLBAR_PLAY_ICON, "play", playIcon);
	p_toolBar->AddTool(TOOLBAR_PAUSE_ICON, "pause", pauseIcon);
	p_toolBar->AddTool(TOOLBAR_NEXT_ICON, "next", nextIcon);
	p_toolBar->AddTool(TOOLBAR_TRASH_ICON, "trash", trashIcon);
	p_toolBar->SetToolBitmapSize(wxSize(16, 16));
	p_toolBar->Realize();

	// Makes a new wxPanel, passes in a parent, window size, num of living cells, and the cell neighbor counts
	p_DrawPan = new DrawingPanel(this, m_settings.windowSize, m_gameBoardDblVoV, m_cellNeighborCounts);

	// instantiates a timer and assigns an id
	p_timer = new wxTimer(this, TIMER_ID);

	// sizer for the frame, adds the wxPanel to expamd and fill the window
	p_sizer = new wxBoxSizer(wxVERTICAL);
	p_sizer->Add(p_DrawPan, 1, wxEXPAND | wxALL);

	//passes in parent and sets panel size to window size
	SetSizer(p_sizer);

	// instantiates the settings pointer in the drawing panel. 
	updateDrawingPanelSettings(m_settings);

	// this->initializeGridCells();
	this->initializeGridCells(m_settings.gridSize);
	this->Layout();
}
MainWindow::~MainWindow()
{
	delete p_DrawPan;
	delete p_timer;
}

void MainWindow::updateDrawingPanelSettings(Settings& settings)
{
	if (p_DrawPan != nullptr)
	{
		p_DrawPan->setSettingsPtr(settings);
	}
}

void MainWindow::initializeGridCells(float gridSize = 0) // intializees the gridsize, overloaded to allow different sizes to be passed in. 
{//														    if no size is passed in it will initialized based on the gridsize in settings.
	if (gridSize >= 1)
	{
		m_settings.setGridSize(gridSize);
		m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
	}

	m_gameBoardDblVoV.resize(m_settings.gridSize);
	m_gameBoardPredictions.resize(m_settings.gridSize);
	m_cellNeighborCounts.resize(m_settings.gridSize);

	for (size_t row = 0; row < m_gameBoardDblVoV.size(); row++)
	{
		m_gameBoardPredictions[row].resize(m_settings.gridSize);
		m_gameBoardDblVoV[row].resize(m_settings.gridSize);
		m_cellNeighborCounts[row].resize(m_settings.gridSize);
	}
}

void MainWindow::predictNextUniverse() // calls findNeighbors on every cell stored in the 2D Vector, when done it swaps the predictions board with the main board and updates the drawing panel
{
	m_settings.numOfAliveCells = 0;
	// m_livingCells = 0; // resets living cells to zero so that they may be counted again. 

	for (size_t row = 0; row < m_gameBoardDblVoV.size(); row++)
	{
		for (size_t col = 0; col < m_gameBoardDblVoV[row].size(); col++)
		{
			m_gameBoardPredictions[row][col] = false; // resets the cell stored in predictions to dead

			makePredictions(row, col, findNeighbors(row, col)); // finds neighbors of a cell and checks if the cell is dead or alive then based on num of neigbors and cell state 
			//it determines what the cells state will be in the next generation and passes it to the predictions vector

			if (m_gameBoardPredictions[row][col] == true) // if cell is alive increment the number of living cells
			{
				m_settings.numOfAliveCells++;
				// m_livingCells++;
			}
		}
	}
	m_generations++; // increment up the number of generations
	swap(m_gameBoardDblVoV, m_gameBoardPredictions); // swap the values stored in predictions with the values stored in the main board
	p_DrawPan->Refresh(); // update the board
}

int MainWindow::findNeighbors(int row, int col) // Finds neighbors around a cell, checks to see if a cell is on the perimeter and passes it to find perimeter neighbor and 
{												 // returns true to skip the non perimeter neighbor check, if its not a perimeter cell it passes over the perimeter neighbor
	int liveNeighbors = 0;						 // check and does a neighbor count for non-perimeter cells. it then checks the current cells state and compares it to the neighbor count
	bool isPerimeter = false;                    // passing in the predicted state for the next generation into the predictions vector

	if (row >= m_settings.gridSize || col >= m_settings.gridSize || row < 0 || col < 0) // check if valid 
	{
		std::cout << "invalid index";
		return NULL;
	}

	// checks to see if the cell is on the perimiter of the board
	if (row - 1 < 0 || row + 1 >= m_settings.gridSize || col - 1 < 0 || col + 1 >= m_settings.gridSize)
	{
		if (m_settings.universeState == false)
		{
			findPerimeterNeighbors(row, col, liveNeighbors);
		}
		else
		{
			findFiniteNeighbors(row, col, liveNeighbors);
		}
		
		isPerimeter = true;
	}

	// checks 8 surronding tiles incrementing the amount of live neigbors if cell is alive. 
	if (isPerimeter == false)
	{
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			liveNeighbors++;
		}

		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			liveNeighbors++;
		}
	}
	return liveNeighbors;
}

void MainWindow::findFiniteNeighbors(int row, int col, int& numOfNeighbors) // same as perimter neighbor check except i removed any out of bounds cell from being checked.
{
	numOfNeighbors = 0;

	if (row == 0 && col == 0) // Top Left corner spot 
	{
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1 && col == m_settings.gridSize - 1) // Bottom right corner
	{
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == 0 && col == m_settings.gridSize - 1) // top right
	{
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1 && col == 0) // lower left
	{
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
	}
	else if (row == 0) // top
	{
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1) // bottom
	{
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (col == 0) // left side
	{
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
	}
	else // right side
	{
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
}

void MainWindow::findPerimeterNeighbors(int row, int col, int& numOfNeighbors)  // this function is a series of if checks for perimeter cells. it manages the cells overflowing by comparing to the 
{																				// cell on the oppposite side of the board allowing the shapes to move infinitely across the board.
	numOfNeighbors = 0;															// Add an if check to see if either point is equal to zero before passing in variable, points not on the perimeter 
																				// that are passed here will not get a neighbor count. 
	if (row == 0 && col == 0) // Top Left corner spot 
	{
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][m_settings.gridSize - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][m_settings.gridSize - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][m_settings.gridSize - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1 && col == m_settings.gridSize - 1) // Bottom right corner
	{
		if (m_gameBoardDblVoV[0][0]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][0]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][0]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == 0 && col == m_settings.gridSize - 1) // top right
	{
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][0]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][0]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][0]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1 && col == 0) // lower left
	{
		if (m_gameBoardDblVoV[row - 1][m_settings.gridSize - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][m_settings.gridSize - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][m_settings.gridSize - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == 0) // top
	{
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[m_settings.gridSize - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (row == m_settings.gridSize - 1) // bottom
	{
		if (m_gameBoardDblVoV[0][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[0][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
	else if (col == 0) // left side
	{
		if (m_gameBoardDblVoV[row - 1][m_settings.gridSize - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][m_settings.gridSize - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][m_settings.gridSize - 1]) // mid left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col + 1]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col + 1]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col + 1]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
	}
	else // right side
	{
		if (m_gameBoardDblVoV[row - 1][0]) // upper right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][0]) // mid right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][0]) // lower right
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col - 1]) // upper left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row - 1][col]) // upper mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col]) // lower mid
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row + 1][col - 1]) // lower left
		{
			numOfNeighbors++;
		}
		if (m_gameBoardDblVoV[row][col - 1]) // mid left
		{
			numOfNeighbors++;
		}
	}
}

void MainWindow::makePredictions(int row, int col, int liveNeighbors)
{
	switch (liveNeighbors) // sets the gameboard predictions vector based on live neighbor count and cell state.
	{
	case 0:
	case 1:
		if (m_gameBoardDblVoV[row][col] == true) // if alive and less than 2 neigbors dies
		{
			m_gameBoardPredictions[row][col] = false;
		}
		break;
	case 2: // if its alive with 2 neighbors stays alive and vice versa with dead cells
		if (m_gameBoardDblVoV[row][col] == true)
		{
			m_gameBoardPredictions[row][col] = true;
		}
		else { m_gameBoardPredictions[row][col] = false; }
		break;
	case 3:
		if (m_gameBoardDblVoV[row][col] == false) // if its dead with 3 neighbors then it comes alive -- if its alive it stays alive
		{
			m_gameBoardPredictions[row][col] = true;
		}
		else { m_gameBoardPredictions[row][col] = true; }
		break;
	default:
		m_gameBoardPredictions[row][col] = false;  // its its alive its dead and if its dead its dead
		break;
	}
}

void MainWindow::updateStatusBar() // allows the status bar to be updated from outside this class
{
	p_statusBar->SetStatusText(livingCellString + std::to_string(m_settings.numOfAliveCells) + generationsString + std::to_string(m_generations));
}

void MainWindow::loadGameBoard(const char* fileName)
{
	std::string fileLine; 
	std::ifstream file(fileName, std::ios::in);
	int gridRow = 0;

	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, fileLine);
			int length = fileLine.length();

			if (gridRow == length)
			{
				break;
			}
			if (fileLine[0] == '!')
			{
				continue;
			}
			if (fileLine[0] == '$')
			{
				// checks for digits and appens them to new string
				std::string digitGrabber;
				for (size_t i = 0; i < length; i++)
				{
					if (std::isdigit(fileLine[i]))
					{
						digitGrabber += fileLine[i];
					}
				}

				fileLine = digitGrabber;
				// sets gridsize
				m_settings.gridSize = std::stoi(fileLine);
				initializeGridCells();
				continue;
			}
			// sets the board
			for (size_t col = 0; col < length; col++)
			{
				if (fileLine[col] == '*')
				{
					m_gameBoardDblVoV[gridRow][col] = true;
				}
				else if (fileLine[col] == '.')
				{
					m_gameBoardDblVoV[gridRow][col] = false;
				}
			}
			gridRow++;
		}
		file.close();
	}
	p_DrawPan->UpdateLivingCellCount();
	updateStatusBar();
	Refresh();
}

bool MainWindow::saveGameBoard(const char* fileName)
{
	std::string fileLine;

	std::ofstream file(fileName);

	if (file.is_open())
	{
		file << "$" << m_settings.gridSize << std::endl;

		for (size_t row = 0; row < m_gameBoardDblVoV.size(); row++)
		{
			for (size_t col = 0; col < m_gameBoardDblVoV[row].size(); col++)
			{
				if (m_gameBoardDblVoV[row][col] == true)
				{
					file << '*';
				}
				else { file << '.'; }
			}
			file << '\n';
		}
		file << std::endl;

		file.close();
	}
	return true;
}

void MainWindow::resetBoard()
{
	for (size_t row = 0; row < m_gameBoardDblVoV.size(); row++)
	{
		for (size_t col = 0; col < m_gameBoardDblVoV[row].size(); col++)
		{
			if (m_gameBoardDblVoV[row][col] != false)
			{
				m_gameBoardDblVoV[row][col] = false;
			}
			m_cellNeighborCounts[row][col] = 0;
			m_gameBoardPredictions[row][col] = false;
		}
	}
	m_generations = 0;
	m_settings.numOfAliveCells = 0;
	if (p_timer->IsRunning())
	{
		p_timer->Stop();
	}
	updateStatusBar();
	p_DrawPan->Refresh();
	Refresh();
}

void MainWindow::onMenuSettingsOptionClick(wxCommandEvent& event) // listens for the settings menu button to be clicked
{
	// instantiates a new settings dialog passing in settings
	SettingsUI* settingsDialog = new SettingsUI(this, &m_settings);
	
	// saves if ok is pressed
	if (settingsDialog->ShowModal() == wxID_OK)
	{
		initializeGridCells(m_settings.gridSize);
		p_DrawPan->Refresh();
		m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
	}
	else // reloads the previous settings if cancel or window closed
	{
		m_settings.loadData(SAVE_SETTINGS_FILE_NAME);
	}
	
	delete settingsDialog;
}

void MainWindow::onWindowSizeChange(wxSizeEvent& event) // listens for the window resizing and sets the drawing panel accordingly. 
{
	if (p_DrawPan != nullptr)
	{
		m_settings.setWindowSize(event.GetSize());
		p_DrawPan->setPanelSize(m_settings.windowSize);
		m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
		event.Skip();
	}
}

void MainWindow::onTimerTick(wxTimerEvent& event) // event call every timer tick
{
	predictNextUniverse();
	updateStatusBar();
	p_DrawPan->Refresh();
	Refresh();
}

void MainWindow::onPauseButtonClick(wxCommandEvent& event)
{
	if (p_timer->IsRunning())
	{
		p_timer->Stop();
	}

	p_DrawPan->Refresh();
	updateStatusBar();
}

void MainWindow::onNextButtonClick(wxCommandEvent& event)
{
	predictNextUniverse();
	updateStatusBar();
	p_DrawPan->Refresh();
	Refresh();
}

void MainWindow::onTrashButtonClick(wxCommandEvent& event)
{
	resetBoard();
}

void MainWindow::onPlayButtonClick(wxCommandEvent& event) // event call when play button is pressed
{
	if (p_timer->IsRunning() != true)
	{
		p_timer->Start(m_settings.ticks);
	}
}

void MainWindow::onRandomizeOptionClick(wxCommandEvent& event)
{
	// Randomize is selected
	if (event.GetId() == RANDOMIZE_EVT_ID)
	{
		srand(time(NULL));
	}
	else // Randomize with seed is selected, prompts user for input. 
	{
		//initialize seed number
		unsigned int seed = 0;
		// dialog message
		wxString seedMessage = "Enter a seed to randomize with";
		// reference for user input string
		std::string enteredSeed;
		// get user input
		enteredSeed = wxGetTextFromUser(seedMessage);
		int length = enteredSeed.length();

		char* charArraySeed = new char[length + 1];

		strcpy(charArraySeed, enteredSeed.c_str());

		for (size_t i = 0; i < sizeof charArraySeed; i++)
		{
			seed += (int)charArraySeed[i];
		}

		srand(seed);
		// seeded random number generator
		delete[] charArraySeed;
 	}

	// Loads the game board with the random states
	for (size_t row = 0; row < m_settings.gridSize; row++)
	{
		for (size_t col = 0; col < m_settings.gridSize; col++)
		{
			int temp = rand() % 101;
			if (temp % 2 != 0)
			{
				m_gameBoardDblVoV[row][col] = true;
				m_settings.numOfAliveCells++;
			}
			else
			{   
				m_gameBoardDblVoV[row][col] = false;
			}
			m_cellNeighborCounts[row][col] = findNeighbors(row, col);
		}
	}
	updateStatusBar();
	p_DrawPan->Refresh();
}

void MainWindow::onNewBoard(wxCommandEvent& event)
{
	resetBoard();
}

void MainWindow::onSaveAs(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Close saved files", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	if (saveGameBoard(saveFileDialog.GetPath()) == false)
	{
		wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
	}


	event.Skip();
}

void MainWindow::onSaveBoard(wxCommandEvent& event)
{
	if (m_fileNameInUse != wxEmptyString)
	{
		saveGameBoard(m_fileNameInUse);
	}
	else
	{
		wxFileDialog saveFileDialog(this, "Close saved files", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
		{
			return;
		}

		if (saveGameBoard(saveFileDialog.GetPath()) == false)
		{
			wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
		}
		m_fileNameInUse = saveFileDialog.GetPath();
	}
}

void MainWindow::onLoadBoard(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open saved Files"), wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) { return; } // user cancelled
	
	m_fileNameInUse = openFileDialog.GetPath();
	loadGameBoard(openFileDialog.GetPath());
	p_DrawPan->Refresh();
}

void MainWindow::onExitBoard(wxCommandEvent& event)
{
	this->Close();
}

void MainWindow::onTorodialCheck(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.universeState = false; // toroidal is on
		p_finite->Check(false);
	}
	else
	{
		m_settings.universeState = true;
		p_finite->Check(true);
	}
	p_DrawPan->Refresh();
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
}

void MainWindow::onFiniteCheck(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.universeState = true; 
		p_torodial->Check(false);
	}
	else 
	{
		m_settings.universeState = false;
		p_torodial->Check(true);
	}
	p_DrawPan->Refresh();
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
}

void MainWindow::onResetSettings(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.loadData(DEFAULT_SETTINGS);
	}
	p_reset->Check(false);
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
	p_DrawPan->Refresh();
}

void MainWindow::onGridVisibility(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.gridIsVisible = true;
	}
	else 
	{
		m_settings.gridIsVisible = false;
	}
	p_DrawPan->Refresh();
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
}

void MainWindow::onShowThickGrid(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.showThickGrid = true;
	}
	else { m_settings.showThickGrid = false; }
	p_DrawPan->Refresh();
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
}

void MainWindow::onShowHUD(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_settings.showHUD = true;
	}
	else { m_settings.showHUD = false; }
	p_DrawPan->Refresh();
	m_settings.saveData(SAVE_SETTINGS_FILE_NAME);
}

void MainWindow::onImport(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open saved Files"), wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) { return; } // user cancelled
	
	m_fileNameInUse = openFileDialog.GetPath();

	importGameBoard(openFileDialog.GetPath());
	
	p_DrawPan->UpdateLivingCellCount();
	updateStatusBar();
	Refresh();
}

void MainWindow::importGameBoard(const char* fileName)
{
	std::string fileLine;
	std::ifstream file(fileName, std::ios::in);
	int gridRow = 0;

	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, fileLine);
			int length = fileLine.length();

			if (gridRow == length)
			{
				break;
			}
			if (fileLine[0] == '!')
			{
				continue;
			}
			if (fileLine[0] == '$')
			{
				continue;
			}
			// sets the board
			for (size_t col = 0; col < length; col++)
			{
				if (fileLine[col] == '*')
				{
					m_gameBoardDblVoV[gridRow][col] = true;
				}
				else if (fileLine[col] == '.')
				{
					m_gameBoardDblVoV[gridRow][col] = false;
				}
			}
			gridRow++;
		}
		file.close();
	}
}

int MainWindow::getGenerations()
{
	return m_generations;
}