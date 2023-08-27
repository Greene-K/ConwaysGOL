#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"
#include <vector>

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
   EVT_PAINT(DrawingPanel::OnPaint)
   EVT_LEFT_UP(DrawingPanel::onMouseClick)
wxEND_EVENT_TABLE(DrawingPanel, wxPanel)


DrawingPanel::DrawingPanel(wxWindow* parent, wxSize windowSize, std::vector<std::vector<bool>> &gameBoardDblVec, std::vector<std::vector<int>>& vecOfNeighborCounts) :
    wxPanel(parent, wxID_ANY, wxPoint(0,0), windowSize), r_gameBoardDblVec(gameBoardDblVec), r_vecOfNeighborCounts(vecOfNeighborCounts)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetDoubleBuffered(true);

    p_mainParent = parent; // stores the parent window for use in functions
}   

DrawingPanel::~DrawingPanel()
{

}

void DrawingPanel::OnPaint(wxPaintEvent& event) // Draws rectangles onto the panel
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* contextPtr = wxGraphicsContext::Create(dc);

    if (!contextPtr)
    {
        std::cout << "Context error" << std::endl;
        return;
    }
    if (p_settings->gridIsVisible == true)
    {
        contextPtr->SetPen(*wxBLACK); // set border
    } else { contextPtr->SetPen(wxTransparentColor); }
    
    // gets the position based on parent window
    m_pointX = GetPosition().x;
    m_pointY = GetPosition().y;

    // gets the size based on the panel size and divides it by the gridsize
    m_width = GetSize().x / p_settings->gridSize;
    m_height = GetSize().y / p_settings->gridSize;

    wxString displayText;
    wxDouble* width = nullptr;
    wxDouble* height = nullptr;

    if (p_settings->showNeighborCount == true)
    {
        contextPtr->SetFont(wxFontInfo(m_height / 2.00), *wxRED); // set font
        displayText = std::to_string(r_vecOfNeighborCounts[0][0]);
        width = new wxDouble((double)m_width);
        height = new wxDouble((double)m_height);
        contextPtr->GetTextExtent(displayText, width, height);
    }

    // loops through the 2d vector of the main gameboard
    for (size_t row = 0; row < p_settings->gridSize; row++)
    {
        for (size_t col = 0; col < p_settings->gridSize; col++)
        {
            MainWindow* tempWindow = (MainWindow*)p_mainParent;
            r_vecOfNeighborCounts[row][col] = tempWindow->findNeighbors(row, col);
            // sets the cell color based on the cell state stored in the game board. 
            if (r_gameBoardDblVec[row][col] == true)
            {
                contextPtr->SetBrush(p_settings->getAliveColor());
            }
            else
            {
                contextPtr->SetBrush(p_settings->getDeadColor());
            }
            // draws the rectangle
            contextPtr->DrawRectangle(m_pointX, m_pointY, m_width, m_height);

            // if neighbor count is on then draw text. 
            if (p_settings->showNeighborCount == true)
            {
                displayText = std::to_string(r_vecOfNeighborCounts[row][col]);
                contextPtr->DrawText(displayText, m_pointX + (m_width / 2.15), m_pointY + (m_height / 6));
            }

            m_pointX += m_width; // adds the cell width to the position to shift horizontally
        }
        m_pointX = GetPosition().x; // resets x position to the starting position
        m_pointY += m_height; // adds the cell height to the position to shift vertically
    }
    if (p_settings->showHUD == true) // shows HUD
    {
        MainWindow* tempWindow = (MainWindow*)p_mainParent;
        wxString universeState = wxEmptyString;
        wxString numOfAliveCells = "\tNumber of alive cells: " + std::to_string(p_settings->numOfAliveCells);
        wxString numOfGenerations = "\tGeneration: " + std::to_string(tempWindow->getGenerations());
        wxString universeSize = "\tUniverse Size: " + std::to_string((int)p_settings->gridSize * (int)p_settings->gridSize) + " Cells";
        wxDouble widthHUD = wxDouble(GetSize().x);
        wxDouble heightHUD = wxDouble(GetSize().y / 10);

        contextPtr->SetFont(wxFontInfo(m_height / 2.00), *wxRED); // set font

        if (p_settings->universeState == false)
        {
            universeState = "\tTorodial Universe";
        }
        else { universeState = "\tFinite Universe"; }

        displayText = universeState + "   " + numOfAliveCells + "   " + numOfGenerations + "   " + universeSize;

        contextPtr->GetTextExtent(displayText, &widthHUD, &heightHUD);

        contextPtr->DrawText(displayText, 0, GetSize().y - m_height);
    }
    if (p_settings->showNeighborCount == true)
    {
        delete width;
        delete height;
    }
    if (p_settings->showThickGrid == true && p_settings->gridSize > 10) // sets thick grid
    {
        m_pointY = GetPosition().y;
        int numOfLines = p_settings->gridSize / 10;
        for (size_t i = 1; i <= numOfLines; i++)
        {
            wxPoint verticalLineStart = wxPoint(m_width * (i * 10), m_pointY);
            wxPoint verticalLineEnd = wxPoint(m_width * (i * 10), GetSize().y);
            wxPoint horizontalLineStart = wxPoint(m_pointX, m_height * (i * 10));
            wxPoint horizontalLineEnd = wxPoint(GetSize().x, m_height * (i * 10));
            dc.SetPen((wxPen(*wxBLACK, 4)));
            dc.DrawLine(verticalLineStart, verticalLineEnd);
            dc.DrawLine(horizontalLineStart, horizontalLineEnd);
        }
    }

    delete contextPtr;
}

void DrawingPanel::onMouseClick(wxMouseEvent& event) // listens for a cell being clicked to flip the cells state
{
    // coords of where the mouse clicked
    float xClick = event.GetX();
    float yClick = event.GetY();
    // divides the position of the click event by the cell size to determine which cell was clicked
    float xCoordClicked = xClick / m_width;
    float yCoordClicked = yClick / m_height;
    // flips the state of the cell clicked
    if (r_gameBoardDblVec[yCoordClicked][xCoordClicked] == true)
    {
        r_gameBoardDblVec[yCoordClicked][xCoordClicked] = false;
        p_settings->numOfAliveCells--;
    }
    else
    {
        r_gameBoardDblVec[yCoordClicked][xCoordClicked] = true;
        p_settings->numOfAliveCells++;
    }
    // initializes a pointer to the main window to update status bar
    MainWindow* tempWindow = (MainWindow*)p_mainParent;
    tempWindow->updateStatusBar();

    Refresh();
}

void DrawingPanel::UpdateLivingCellCount()
{
    p_settings->numOfAliveCells = 0;
    for (size_t row = 0; row < r_gameBoardDblVec.size(); row++)
    {
        for (size_t col = 0; col < r_gameBoardDblVec[row].size(); col++)
        {
            if (r_gameBoardDblVec[row][col] == true)
            {
                p_settings->numOfAliveCells++;
            }
        }
    }
}

void DrawingPanel::setPanelSize(wxSize& size) // sets the size of this wxPanel
{
    SetSize(size);
    Refresh();
}

void DrawingPanel::setSettingsPtr(Settings& mainSettings) // points the drawing panels settings pointer to the member variable settings in main
{
    p_settings = &mainSettings;
}