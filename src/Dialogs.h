///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug  3 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_FORMAT_YUV 1000
#define wxID_FORMAT_BITS 1001
#define wxID_FORMAT_ENDIAN 1002
#define ID_FILE_OPEN_SPLIT 1003
#define ID_FILE_OPEN_FILE 1004
#define ID_FILE_OPEN_DUMP 1005
#define ID_FILE_OPEN_Y4M 1006
#define ID_FILE_CLOSE 1007
#define ID_FILE_REFRESH 1008
#define ID_FILE_SAVE_IMAGE 1009
#define ID_FILE_SAVE_YUV_SPLIT 1010
#define ID_FILE_SAVE_YUV_COMP 1011
#define ID_FILE_SAVE_YUV_YUV4MPEG 1012
#define ID_FILE_EXIT 1013
#define ID_FORMAT_SETSIZE 1014
#define ID_FORMAT_YUV 1015
#define ID_FORMAT_MASK_Y 1016
#define ID_FORMAT_MASK_U 1017
#define ID_FORMAT_MASK_V 1018
#define ID_FORMAT_PROPERTIES 1019
#define ID_VIEW_NEXT 1020
#define ID_VIEW_PREV 1021
#define ID_VIEW_GO_HOME 1022
#define ID_VIEW_GO_LAST 1023
#define ID_VIEW_GOTO 1024
#define ID_VIEW_SCALE_ACTUAL 1025
#define ID_VIEW_SCALE_SCALED 1026
#define ID_VIEW_SCALE_HALF 1027
#define ID_VIEW_SCALE_DOUBLE 1028
#define ID_VIEW_STEP 1029
#define ID_VIEW_GRID 1030
#define ID_VIEW_GRID_SETTINGS 1031
#define ID_VIEW_FULLSCREEN 1032
#define ID_CHECKSUM_NONE 1033
#define ID_CHECKSUM_CRC32 1034
#define ID_CHECKSUM_MD5 1035
#define ID_CHECKSUM_SHA1 1036
#define ID_HELP_ABOUT 1037

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
		wxBoxSizer* bSizer1;
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
/// Class YUVSplitDialogBase
///////////////////////////////////////////////////////////////////////////////
class YUVSplitDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* bSizer1;
		wxStaticText* m_staticText39;
		wxTextCtrl* m_prefix;
		wxStaticText* m_staticText20;
		wxDirPickerCtrl* m_dirPicker;
		wxStaticText* m_staticText30;
		wxSpinCtrl* m_firstFrame;
		wxStaticText* m_staticText31;
		wxSpinCtrl* m_lastFrame;
		wxStaticText* m_staticText19;
		wxChoice* m_fmtChoice;
		wxStaticText* m_staticText201;
		wxChoice* m_bitsChoice;
		wxChoice* m_bitsEndian;
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
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnPrefixChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDirChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnChangeStandard( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		YUVSplitDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Open YUV Split File"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~YUVSplitDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class YUVCompDialogBase
///////////////////////////////////////////////////////////////////////////////
class YUVCompDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* bSizer1;
		wxStaticText* m_staticText20;
		wxFilePickerCtrl* m_filePicker;
		wxStaticText* m_staticText19;
		wxChoice* m_fmtChoice;
		wxStaticText* m_staticText201;
		wxChoice* m_bitsChoice;
		wxChoice* m_bitsEndian;
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
		virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnFileSelected( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnChangeStandard( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		YUVCompDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Open YUV Composite File"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~YUVCompDialogBase();
	
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
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		wxSpinCtrl* m_spinH;
		wxSpinCtrl* m_spinV;
		wxColourPickerCtrl* m_gridColorControl;
		
		GridDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Grid Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~GridDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ChecksumDialogBase
///////////////////////////////////////////////////////////////////////////////
class ChecksumDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticBoxSizer* m_staticBoxSizer;
		wxStaticText* m_staticText18;
		wxStaticText* m_lumaText;
		wxStaticText* m_staticText20;
		wxStaticText* m_chromaText;
	
	public:
		
		ChecksumDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Checksum Info"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION ); 
		~ChecksumDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class frameInspectorMenu
///////////////////////////////////////////////////////////////////////////////
class frameInspectorMenu : public wxMenuBar 
{
	private:
	
	protected:
		wxMenu* m_fileMenu;
		wxMenu* m_submenuFileOpen;
		wxMenu* m_submenuFileSave;
		wxMenu* m_formatMenu;
		wxMenu* m_menuFormatMask;
		wxMenu* m_viewMenu;
		wxMenu* m_submenuViewScaling;
		wxMenu* m_crcMenu;
		wxMenu* m_helpMenu;
	
	public:
		
		frameInspectorMenu( long style = 0 );
		~frameInspectorMenu();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class formatDialogBase
///////////////////////////////////////////////////////////////////////////////
class formatDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText19;
		wxChoice* m_fmtChoice;
		wxStaticText* m_staticText201;
		wxChoice* m_bitsChoice;
		wxChoice* m_bitsEndian;
		wxStdDialogButtonSizer* m_sdbSizer5;
		wxButton* m_sdbSizer5OK;
		wxButton* m_sdbSizer5Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		
	
	public:
		
		formatDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("YUV Format"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~formatDialogBase();
	
};

#endif //__DIALOGS_H__
