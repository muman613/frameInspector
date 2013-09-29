///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Dialogs.h"

///////////////////////////////////////////////////////////////////////////

HashDialogBase::HashDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Luma MD5 :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_lumaText = new wxStaticText( this, wxID_ANY, wxT("DEADBEEFDEADBEEFDEADBEEFDEADBEEF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lumaText->Wrap( -1 );
	m_lumaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	fgSizer1->Add( m_lumaText, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Chroma MD5 :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_chromaText = new wxStaticText( this, wxID_ANY, wxT("DEADBEEFDEADBEEFDEADBEEFDEADBEEF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chromaText->Wrap( -1 );
	m_chromaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	fgSizer1->Add( m_chromaText, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
}

HashDialogBase::~HashDialogBase()
{
}

SizeDialogBase::SizeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Dimensions") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, wxT("Standard :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	gSizer1->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	wxString m_stdChoiceChoices[] = { wxT("custom"), wxT("sqcif (128x96)"), wxT("qcif (176x144)"), wxT("scif (256x192)"), wxT("sif (352x240)"), wxT("cif (352x288)"), wxT("VGA (640x480)"), wxT("4sif (704x480)"), wxT("4cif (704x576)"), wxT("SVGA (800x600)"), wxT("SXGA (1280x1024)"), wxT("1080p (1920x1080)") };
	int m_stdChoiceNChoices = sizeof( m_stdChoiceChoices ) / sizeof( wxString );
	m_stdChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_stdChoiceNChoices, m_stdChoiceChoices, 0 );
	m_stdChoice->SetSelection( 0 );
	gSizer1->Add( m_stdChoice, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_WidthCtl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_WidthCtl, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Height :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_HeightCtl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_HeightCtl, 0, wxALL, 5 );
	
	sbSizer1->Add( gSizer1, 1, wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer1, 1, wxALL|wxEXPAND, 8 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer1->Add( m_sdbSizer1, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_stdChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SizeDialogBase::OnChangeStandard ), NULL, this );
}

SizeDialogBase::~SizeDialogBase()
{
	// Disconnect Events
	m_stdChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SizeDialogBase::OnChangeStandard ), NULL, this );
}

GridDialogBase::GridDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Grid Dimensions") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Horizontal :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_spinH = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 128, 0 );
	gSizer1->Add( m_spinH, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Vertical :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_spinV = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 128, 0 );
	gSizer1->Add( m_spinV, 0, wxALL, 5 );
	
	sbSizer1->Add( gSizer1, 1, wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer1, 1, wxALL|wxEXPAND, 8 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Grid Color") ), wxVERTICAL );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Grid Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	gSizer4->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_gridColorControl = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer4->Add( m_gridColorControl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	sbSizer4->Add( gSizer4, 1, wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer4, 0, wxALL|wxEXPAND, 8 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer1->Add( m_sdbSizer1, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

GridDialogBase::~GridDialogBase()
{
}
