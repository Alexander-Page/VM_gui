#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton* m_btn1 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxTextCtrl* m_txt2 = nullptr;
	wxStaticText* regLabel = nullptr;
	wxStaticText* regTextA = nullptr;
	wxStaticText* regValA = nullptr;
	wxStaticText* regTextB = nullptr;
	wxStaticText* regValB = nullptr;
	wxStaticText* regTextC = nullptr;
	wxStaticText* regValC = nullptr;
	wxStaticText* regTextD = nullptr;
	wxStaticText* regValD = nullptr;
	wxStaticText* regTextE = nullptr;
	wxStaticText* regValE = nullptr;
	wxStaticText* regTextF = nullptr;
	wxStaticText* regValF = nullptr;
	wxStaticText* regTextL = nullptr;
	wxStaticText* regValL = nullptr;
	wxStaticText* regTextIP = nullptr;
	wxStaticText* regValIP = nullptr;
	wxStaticText* regTextSP = nullptr;
	wxStaticText* regValSP = nullptr;

	void OnButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

