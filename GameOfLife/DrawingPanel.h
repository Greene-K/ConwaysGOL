#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"


class DrawingPanel :
    public wxPanel
{
private:
    std::vector<std::vector<bool>>& r_gameBoardDblVec;
    std::vector<std::vector<int>>& r_vecOfNeighborCounts;

    wxWindow* p_mainParent = nullptr;
    
    float m_pointX = 0;
    float m_pointY = 0;
    float m_width = 0;
    float m_height = 0;

    Settings* p_settings = nullptr;

public:
    // constructors / destructors
    DrawingPanel(wxWindow* parent, wxSize windowSize, std::vector<std::vector<bool>>& gameBoardDblVec, std::vector<std::vector<int>>& vecOfNeighborCounts);
    ~DrawingPanel();

    // events 
    void OnPaint(wxPaintEvent& event);
    void onMouseClick(wxMouseEvent& event);

    // functions
    void setPanelSize(wxSize& size);
    void setSettingsPtr(Settings& settingsPtr);
    void UpdateLivingCellCount();

    wxDECLARE_EVENT_TABLE();
};

