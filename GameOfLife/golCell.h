// I made this class to store information on a cell, im still having trouble with memory management here so its benched for now
// Main Problems:
// Allocating way to much memory without proper deletion
// not sure how to interface it with my wxPanel

#pragma once
#include <wx/wx.h>
class golCell
{
private:
	//golCell* p_cell = nullptr; // will store all info about a given cell
	wxSize p_cellSize; // stores size for easier cell initialization
	wxPoint p_cellPosition; // stores position for easier initialization

	bool p_cellState; // indicates whether its alive or dead
	int p_cellID; // gices the cell an id for easier comparison

	// optional may not be needed
	float m_width;
	float m_height;
	int m_xCoord;
	int m_yCoord; 

public:
	golCell(wxPoint startPoint); // default constructor
	golCell(wxSize cellSize, wxPoint cellPosition, int cellID, bool cellState = false); // ovld constructor
	golCell(const golCell& other); // copy constructor
	golCell& operator=(const golCell& other); // assign operator
	~golCell();

	wxSize getCellSize();
	void setCellSize(wxSize cellSize);
	void setCellSize(float cellWidth, float cellHeight);

	wxPoint getCellPosition();
	void setCellPosition(wxPoint cellPosition);
	void setCellPosition(int xCoord, int yCoord);

	bool getCellState();
	void setCellState(bool cellState);

	int getCellID();
	void setCellID(int idNum);
	
};

