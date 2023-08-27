#pragma once
#include "wx/wx.h"
#include <fstream>
struct Settings
{
	float gridSize = 30;
	int ticks = 50;
	int numOfAliveCells = 0;

	wxSize windowSize = wxSize(1080, 720); // window size 
	wxPoint windowCenterPt = wxPoint((windowSize.x / 2) - (windowSize.x / 4), (windowSize.y / 2) - (windowSize.y / 4)); // window center point skewed for a window that is half its size and centered
	
	bool showNeighborCount = false; // count of neighbors
	bool universeState = false; // if false the game is toroidal else its finite
	bool gridIsVisible = false; // if false grid is not visible else its visible
	bool showThickGrid = false; // if false it shows a thick line every 10th line else normal line
	bool showHUD = false;

	unsigned int dead_Red = 255; // all 128 == grey
	unsigned int dead_Green = 255; // white is all 255
	unsigned int dead_Blue = 255;
	unsigned int dead_Alpha = 255;

	unsigned int alive_Red = 128; // all 128 == grey
	unsigned int alive_Green = 128; // white is all 255
	unsigned int alive_Blue = 128;
	unsigned int alive_Alpha = 128;

	// sets grid size
	void setGridSize(float gridSize) {
		this->gridSize = gridSize;
	}

	float getGridSize() {
		return gridSize; 
	}
	
	// sets window size
	void setWindowSize(wxSize mainWindowSize) { 
		windowSize = mainWindowSize; 
		windowCenterPt = wxPoint((mainWindowSize.x / 2) - (mainWindowSize.x / 4), (mainWindowSize.y / 2) - (mainWindowSize.y / 4));
	}

	wxSize getWindowSize() const {
		return windowSize;
	}

	void setDeadColor(wxColor color)	//dead color
	{
		dead_Red = color.GetRed();
		dead_Green = color.GetGreen();
		dead_Blue = color.GetBlue();
		dead_Alpha = color.GetAlpha();
	}

	wxColor getDeadColor()
	{
		wxColor color(dead_Red, dead_Green, dead_Blue, dead_Alpha);
		return color;
	}

	void setAliveColor(wxColor color)	//alive color
	{
		alive_Red = color.GetRed();
		alive_Green = color.GetGreen();
		alive_Blue = color.GetBlue();
		alive_Alpha = color.GetAlpha();
	}

	wxColor getAliveColor()
	{
		wxColor color(alive_Red, alive_Green, alive_Blue, alive_Alpha);
		return color;
	}

	// loads the settings from the settings.bin file
	void loadData(const char* fileName)
	{
		std::ifstream file(fileName, std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}

	// saves the settings to the settings.bin file
	void saveData(const char* fileName)
	{
		std::ofstream file(fileName, std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}

};