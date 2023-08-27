#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include <vector>

class SettingsUI :
    public wxDialog
{
private:
    Settings* p_settings = nullptr; // points to the settings in the main window
    wxBoxSizer* p_mainDialogSizer = new wxBoxSizer(wxVERTICAL); // main dialog box sizer
    wxBoxSizer* p_gridSizeSpinSizer = new wxBoxSizer(wxHORIZONTAL); // child sizer for gridsize
    wxBoxSizer* p_cellClrSizer = new wxBoxSizer(wxHORIZONTAL); // child sizer for cell color (both dead and alive)
    wxBoxSizer* p_intervalSpinSizer = new wxBoxSizer(wxHORIZONTAL); // sizeer for time tick intervals
    wxBoxSizer* p_cellNeighborCountSizer = new wxBoxSizer(wxHORIZONTAL); // sizer for cell neigbor count

    std::vector<int> v_neighborCounts; // vector for neighbor counts 
    wxCheckBox* p_cellNeighborCountCtrl = nullptr; // control for neighbor count
    wxSpinCtrl* p_gridSizeSpinCtrl = nullptr; // control for gridsize
    wxSpinCtrl* p_tickIntervalSpinCtrl = nullptr; // control for time intervals
    wxColourPickerCtrl* p_aliveCellClrPickerCtrl = nullptr; // control for alive cells
    wxColourPickerCtrl* p_deadCellClrPickerCtrl = nullptr; // control for dead cells

public:
    SettingsUI(wxWindow* parent, Settings* mainSettings);
    ~SettingsUI();

    void setSettingsPtr(Settings& mainSettings);

    void onGridSizeChange(wxSpinEvent& event);
    void onTickIntervalChange(wxSpinEvent& event);
    void onAliveCellClrChange(wxColourPickerEvent& event);
    void onDeadCellClrChange(wxColourPickerEvent& event);
    void onCellNeighborCountToggle(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

