#define GRID_SIZE_SPIN_SIZER_EVT_ID 10030
#define TICK_INTERVAL_SPIN_SIZER_EVT_ID 10031
#define ALIVE_CELL_CLR_PICKER_EVT_ID 10040
#define DEAD_CELL_CLR_PICKER_EVT_ID 10041
#define CELL_NEIGHBOR_COUNT_EVT_ID 10051

#include "SettingsUI.h"

wxBEGIN_EVENT_TABLE(SettingsUI, wxDialog)
EVT_SPINCTRL(GRID_SIZE_SPIN_SIZER_EVT_ID, SettingsUI::onGridSizeChange)
EVT_SPINCTRL(TICK_INTERVAL_SPIN_SIZER_EVT_ID, SettingsUI::onTickIntervalChange)
EVT_COLOURPICKER_CHANGED(ALIVE_CELL_CLR_PICKER_EVT_ID, SettingsUI::onAliveCellClrChange)
EVT_COLOURPICKER_CHANGED(DEAD_CELL_CLR_PICKER_EVT_ID, SettingsUI::onDeadCellClrChange)
EVT_CHECKBOX(CELL_NEIGHBOR_COUNT_EVT_ID, SettingsUI::onCellNeighborCountToggle)
wxEND_EVENT_TABLE()

SettingsUI::SettingsUI(wxWindow* parent, Settings* mainSettings) : 
	wxDialog(parent, wxID_ANY, "Settings", mainSettings->windowCenterPt, mainSettings->windowSize / 2)
{
	// instantiate settings
	setSettingsPtr(*mainSettings);

	p_gridSizeSpinCtrl = new wxSpinCtrl(this, GRID_SIZE_SPIN_SIZER_EVT_ID, "Grid Size"); // gridsize
	p_tickIntervalSpinCtrl = new wxSpinCtrl(this, TICK_INTERVAL_SPIN_SIZER_EVT_ID, "Tick Interval"); // tick intervals

	p_aliveCellClrPickerCtrl = new wxColourPickerCtrl(this, ALIVE_CELL_CLR_PICKER_EVT_ID, "Alive Cell Colours"); // alive cell colour
	p_deadCellClrPickerCtrl = new wxColourPickerCtrl(this, DEAD_CELL_CLR_PICKER_EVT_ID, "Dead Cell colours"); // dead cell colour

	p_cellNeighborCountCtrl = new wxCheckBox(this, CELL_NEIGHBOR_COUNT_EVT_ID, "Show neighbor count"); // show neighbor count checkbox

	// grid size spin sizer
	p_gridSizeSpinSizer->Add(new wxStaticText(this, wxID_ANY, "Grid Size: "));
	p_gridSizeSpinSizer->Add(p_gridSizeSpinCtrl);
	p_gridSizeSpinCtrl->SetValue(p_settings->gridSize);
	p_gridSizeSpinSizer->AddSpacer(5);

	// cell color sizer
	p_cellClrSizer->Add(new wxStaticText(this, wxID_ANY, "Alive Cell Colour: "));
	p_cellClrSizer->Add(p_aliveCellClrPickerCtrl);
	p_aliveCellClrPickerCtrl->SetColour(p_settings->getAliveColor());
	p_cellClrSizer->AddSpacer(5);

	p_cellClrSizer->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Colour: "));
	p_cellClrSizer->Add(p_deadCellClrPickerCtrl);
	p_deadCellClrPickerCtrl->SetColour(p_settings->getDeadColor());
	p_cellClrSizer->AddSpacer(5);

	// tick interval sizer
	p_intervalSpinSizer->Add(new wxStaticText(this, wxID_ANY, "Timer Tick Interval: "));
	p_intervalSpinSizer->Add(p_tickIntervalSpinCtrl);
	p_tickIntervalSpinCtrl->SetValue(p_settings->ticks);
	p_intervalSpinSizer->AddSpacer(5);

	// Cell count CheckBox Sizer
	p_cellNeighborCountSizer->Add(new wxStaticText(this, wxID_ANY, " "));
	p_cellNeighborCountSizer->Add(p_cellNeighborCountCtrl);
	p_cellNeighborCountCtrl->SetValue(p_settings->showNeighborCount);
	p_cellNeighborCountSizer->AddSpacer(5);

	// settings 'ok' and 'cancel' buttons and bar
	wxSizer* settingsSaveAndExitButtons = CreateButtonSizer(wxOK | wxCANCEL);
	
	// set the dialog sizer
	this->SetSizer(p_mainDialogSizer);

	// add child sizers to main sizer
	p_mainDialogSizer->Add(p_gridSizeSpinSizer);
	p_mainDialogSizer->Add(p_cellClrSizer);
	p_mainDialogSizer->Add(p_intervalSpinSizer);
	p_mainDialogSizer->Add(p_cellNeighborCountSizer);
	p_mainDialogSizer->Add(settingsSaveAndExitButtons);
	
	// formatting
	this->Layout();
}

SettingsUI::~SettingsUI() 
{
	delete p_gridSizeSpinCtrl;
	delete p_tickIntervalSpinCtrl;
	delete p_aliveCellClrPickerCtrl;
	delete p_deadCellClrPickerCtrl;
	delete p_cellNeighborCountCtrl;
}

void SettingsUI::onCellNeighborCountToggle(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		// turn on showNeighborCount display
		p_settings->showNeighborCount = true;
	}
	else
	{
		// turn off neighborCount
		p_settings->showNeighborCount = false;
	}
}

void SettingsUI::setSettingsPtr(Settings& mainSettings)
{
	p_settings = &mainSettings;
}

void SettingsUI::onGridSizeChange(wxSpinEvent& event) // listens for the gridsize changing and sets the settings accordingly
{
	p_settings->setGridSize(event.GetValue()); // sets grid size in settings.h to the value received from the event
}

void SettingsUI::onTickIntervalChange(wxSpinEvent& event) // listens for tick interval and sets settings accordingly
{
	p_settings->ticks = event.GetValue(); 
}

void SettingsUI::onAliveCellClrChange(wxColourPickerEvent& event) // looks for an alive cell colour change and sets the settings accordingly
{
	p_settings->setAliveColor(event.GetColour());
}

void SettingsUI::onDeadCellClrChange(wxColourPickerEvent& event) // looks for an dead cell color change and sets the settings accordingly
{
	p_settings->setDeadColor(event.GetColour());
}