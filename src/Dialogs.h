///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __Dialogs__
#define __Dialogs__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HashDialogBase
///////////////////////////////////////////////////////////////////////////////
class HashDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText9;
		wxStaticText* m_lumaText;
		wxStaticText* m_staticText11;
		wxStaticText* m_chromaText;
	
	public:
		
		HashDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Checksum Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION );
		~HashDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SizeDialogBase
///////////////////////////////////////////////////////////////////////////////
class SizeDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText44;
		wxChoice* m_stdChoice;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_WidthCtl;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_HeightCtl;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChangeStandard( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SizeDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Set Image Size"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~SizeDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GridDialogBase
///////////////////////////////////////////////////////////////////////////////
class GridDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText14;
		wxColourPickerCtrl* m_gridColorControl;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		wxSpinCtrl* m_spinH;
		wxSpinCtrl* m_spinV;
		
		GridDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Grid Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~GridDialogBase();
	
};

#endif //__Dialogs__
