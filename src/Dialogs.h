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

#endif //__Dialogs__
