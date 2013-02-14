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
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HashDialogBase
///////////////////////////////////////////////////////////////////////////////
class HashDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_lumaText;
		wxStaticText* m_staticText3;
		wxStaticText* m_chromaText;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		
		HashDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Checksum Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~HashDialogBase();
	
};

#endif //__Dialogs__
