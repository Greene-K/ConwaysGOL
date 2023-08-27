#include "golCell.h"
#include <vector>

//golCell* p_cell = nullptr; // will store all info about a given cell
//wxSize* p_cellSize = nullptr; // stores size for easier cell initialization
//wxPoint* p_cellPosition = nullptr; // stores position for easier initialization
//
//bool m_cellState = false; // indicates whether its alive or dead
//
//float m_width = 0;
//float m_height = 0;
//int m_xCoord = 0;
//int m_yCoord = 0;

golCell::golCell(wxPoint startPoint) {
	setCellSize(0,0);
	setCellPosition(startPoint);
	setCellState(false);
	setCellID(wxID_ANY);
}
golCell::golCell(wxSize cellSize, wxPoint cellPosition, int cellID, bool cellState)
{
	setCellSize(cellSize);
	setCellPosition(cellPosition);
	setCellState(cellState);
	setCellID(cellID);
}
golCell::golCell(const golCell& other)
{
	*this = other;
}
golCell& golCell::operator=(const golCell& other)
{
	if (this != &other)
	{
		setCellSize(other.p_cellSize);
		setCellPosition(other.p_cellPosition);
		setCellState(other.p_cellState);
		setCellID(other.p_cellID);
	}
	return *this;
}
golCell::~golCell()
{
	//delete p_cellPosition;
	//delete p_cellSize;
}
wxSize golCell::getCellSize()
{
	return p_cellSize;
}

// getters and setters for cell size.  
void golCell::setCellSize(wxSize cellSize) // this setter uses wxSize as a param
{
	p_cellSize = wxSize(cellSize);
	// optional may not be needed
	//m_width = p_cellSize->x;
	//m_height = p_cellSize->y;
}
void golCell::setCellSize(float cellWidth, float cellHeight) // this setter accepts floats (width and height) to set cell size
{
	p_cellSize = wxSize(cellWidth, cellHeight);
	// optional may not be needed
	//m_width = p_cellSize->x;
	//m_height = p_cellSize->y;
}

// getters and setters for cell position 
wxPoint golCell::getCellPosition() {
	return p_cellPosition;
}
void golCell::setCellPosition(wxPoint cellPosition) // this setter takes a wxPoint param to set position
{
	p_cellPosition = wxPoint(cellPosition);
	// optional may not be needed
	//m_xCoord = p_cellPosition->x;
	//m_yCoord = p_cellPosition->y;
}
void golCell::setCellPosition(int xCoord, int yCoord) // ovld constructor that accepts int coords to set size
{
	p_cellPosition = wxPoint(xCoord, yCoord);
	// optional may not be needed
	//m_xCoord = p_cellPosition->x;
	//m_yCoord = p_cellPosition->y;
}

// getter and setter for cell state
bool golCell::getCellState()
{
	return p_cellState;
}
void golCell::setCellState(bool cellState) {
	p_cellState = cellState;
}
int golCell::getCellID()
{
	return p_cellID;
}
void golCell::setCellID(int idNum)
{
	p_cellID = idNum;
}