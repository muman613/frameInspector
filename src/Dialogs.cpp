///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug  3 2017)
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
	m_lumaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer1->Add( m_lumaText, 0, wxALL|wxEXPAND, 5 );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Chroma MD5 :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_chromaText = new wxStaticText( this, wxID_ANY, wxT("DEADBEEFDEADBEEFDEADBEEFDEADBEEF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chromaText->Wrap( -1 );
	m_chromaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

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

	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Dimensions") ), wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );

	m_staticText44 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Standard :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	gSizer1->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_stdChoiceChoices;
	m_stdChoice = new wxChoice( sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_stdChoiceChoices, 0 );
	m_stdChoice->SetSelection( 0 );
	gSizer1->Add( m_stdChoice, 0, wxALL|wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_WidthCtl = new wxTextCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_WidthCtl, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Height :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_HeightCtl = new wxTextCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_HeightCtl, 0, wxALL, 5 );


	sbSizer1->Add( gSizer1, 1, wxALL|wxEXPAND, 5 );


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

YUVSplitDialogBase::YUVSplitDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* imageFileSizer;
	imageFileSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Files") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText39 = new wxStaticText( imageFileSizer->GetStaticBox(), wxID_ANY, wxT("File Spec :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer2->Add( m_staticText39, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_prefix = new wxTextCtrl( imageFileSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_prefix->SetToolTip( wxT("Filename template.\nframe_%02d") );

	fgSizer2->Add( m_prefix, 0, wxALL|wxEXPAND, 5 );

	m_staticText20 = new wxStaticText( imageFileSizer->GetStaticBox(), wxID_ANY, wxT("Path :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer2->Add( m_staticText20, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_dirPicker = new wxDirPickerCtrl( imageFileSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_SMALL|wxDIRP_USE_TEXTCTRL );
	fgSizer2->Add( m_dirPicker, 0, wxALL|wxEXPAND, 5 );

	m_staticText30 = new wxStaticText( imageFileSizer->GetStaticBox(), wxID_ANY, wxT("First :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer2->Add( m_staticText30, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_firstFrame = new wxSpinCtrl( imageFileSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer2->Add( m_firstFrame, 0, wxALL, 5 );

	m_staticText31 = new wxStaticText( imageFileSizer->GetStaticBox(), wxID_ANY, wxT("Last :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer2->Add( m_staticText31, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_lastFrame = new wxSpinCtrl( imageFileSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer2->Add( m_lastFrame, 0, wxALL, 5 );


	imageFileSizer->Add( fgSizer2, 1, wxEXPAND, 5 );


	bSizer1->Add( imageFileSizer, 0, wxALL|wxEXPAND, 8 );

	wxStaticBoxSizer* imageFormatSizer;
	imageFormatSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Format") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText19 = new wxStaticText( imageFormatSizer->GetStaticBox(), wxID_ANY, wxT("Format :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer5->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_fmtChoiceChoices[] = { wxT("YUV 4:2:0"), wxT("YUV 4:2:2"), wxT("YUV 4:4:4"), wxT("NV12"), wxT("NV21") };
	int m_fmtChoiceNChoices = sizeof( m_fmtChoiceChoices ) / sizeof( wxString );
	m_fmtChoice = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_YUV, wxDefaultPosition, wxDefaultSize, m_fmtChoiceNChoices, m_fmtChoiceChoices, 0 );
	m_fmtChoice->SetSelection( 0 );
	fgSizer5->Add( m_fmtChoice, 1, wxALL|wxEXPAND, 5 );


	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText201 = new wxStaticText( imageFormatSizer->GetStaticBox(), wxID_ANY, wxT("Bits :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	fgSizer5->Add( m_staticText201, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_bitsChoiceChoices[] = { wxT("8-bit"), wxT("10-bit"), wxT("16-bit") };
	int m_bitsChoiceNChoices = sizeof( m_bitsChoiceChoices ) / sizeof( wxString );
	m_bitsChoice = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_BITS, wxDefaultPosition, wxDefaultSize, m_bitsChoiceNChoices, m_bitsChoiceChoices, 0 );
	m_bitsChoice->SetSelection( 0 );
	fgSizer5->Add( m_bitsChoice, 0, wxALL|wxEXPAND, 5 );

	wxString m_bitsEndianChoices[] = { wxT("Little Endian"), wxT("Big Endian") };
	int m_bitsEndianNChoices = sizeof( m_bitsEndianChoices ) / sizeof( wxString );
	m_bitsEndian = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_ENDIAN, wxDefaultPosition, wxDefaultSize, m_bitsEndianNChoices, m_bitsEndianChoices, 0 );
	m_bitsEndian->SetSelection( 0 );
	m_bitsEndian->Enable( false );
	m_bitsEndian->SetToolTip( wxT("Format Endianness") );

	fgSizer5->Add( m_bitsEndian, 0, wxALL, 5 );


	imageFormatSizer->Add( fgSizer5, 1, wxEXPAND, 5 );


	bSizer1->Add( imageFormatSizer, 0, wxALL|wxEXPAND, 8 );

	wxStaticBoxSizer* imageDimensionsSizer;
	imageDimensionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Dimensions") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText44 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Standard :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer3->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_stdChoiceChoices;
	m_stdChoice = new wxChoice( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_stdChoiceChoices, 0 );
	m_stdChoice->SetSelection( 0 );
	fgSizer3->Add( m_stdChoice, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_WidthCtl = new wxTextCtrl( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_WidthCtl, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Height :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_HeightCtl = new wxTextCtrl( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_HeightCtl, 0, wxALL, 5 );


	imageDimensionsSizer->Add( fgSizer3, 1, wxEXPAND, 5 );


	bSizer1->Add( imageDimensionsSizer, 1, wxALL|wxEXPAND, 8 );

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
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( YUVSplitDialogBase::OnInitDialog ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( YUVSplitDialogBase::OnUpdateUI ) );
	m_prefix->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( YUVSplitDialogBase::OnPrefixChanged ), NULL, this );
	m_dirPicker->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( YUVSplitDialogBase::OnDirChanged ), NULL, this );
	m_stdChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( YUVSplitDialogBase::OnChangeStandard ), NULL, this );
}

YUVSplitDialogBase::~YUVSplitDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( YUVSplitDialogBase::OnInitDialog ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( YUVSplitDialogBase::OnUpdateUI ) );
	m_prefix->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( YUVSplitDialogBase::OnPrefixChanged ), NULL, this );
	m_dirPicker->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( YUVSplitDialogBase::OnDirChanged ), NULL, this );
	m_stdChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( YUVSplitDialogBase::OnChangeStandard ), NULL, this );

}

YUVCompDialogBase::YUVCompDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* imageFileSizer;
	imageFileSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image File") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText20 = new wxStaticText( imageFileSizer->GetStaticBox(), wxID_ANY, wxT("YUV File :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer2->Add( m_staticText20, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_filePicker = new wxFilePickerCtrl( imageFileSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a YUV Composite File"), wxT("YUV Files (*.yuv)|*.yuv"), wxDefaultPosition, wxSize( 280,-1 ), wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	fgSizer2->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );


	imageFileSizer->Add( fgSizer2, 1, wxEXPAND, 5 );


	bSizer1->Add( imageFileSizer, 0, wxALL|wxEXPAND, 8 );

	wxStaticBoxSizer* imageFormatSizer;
	imageFormatSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Format") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText19 = new wxStaticText( imageFormatSizer->GetStaticBox(), wxID_ANY, wxT("Format :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer5->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_fmtChoiceChoices[] = { wxT("YUV 4:2:0"), wxT("YUV 4:2:2"), wxT("YUV 4:4:4"), wxT("NV12"), wxT("NV21") };
	int m_fmtChoiceNChoices = sizeof( m_fmtChoiceChoices ) / sizeof( wxString );
	m_fmtChoice = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_YUV, wxDefaultPosition, wxDefaultSize, m_fmtChoiceNChoices, m_fmtChoiceChoices, 0 );
	m_fmtChoice->SetSelection( 0 );
	fgSizer5->Add( m_fmtChoice, 1, wxALL|wxEXPAND, 5 );


	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText201 = new wxStaticText( imageFormatSizer->GetStaticBox(), wxID_ANY, wxT("Bits :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	fgSizer5->Add( m_staticText201, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_bitsChoiceChoices[] = { wxT("8-bit"), wxT("10-bit"), wxT("16-bit") };
	int m_bitsChoiceNChoices = sizeof( m_bitsChoiceChoices ) / sizeof( wxString );
	m_bitsChoice = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_BITS, wxDefaultPosition, wxDefaultSize, m_bitsChoiceNChoices, m_bitsChoiceChoices, 0 );
	m_bitsChoice->SetSelection( 0 );
	fgSizer5->Add( m_bitsChoice, 0, wxALL|wxEXPAND, 5 );

	wxString m_bitsEndianChoices[] = { wxT("Little Endian"), wxT("Big Endian") };
	int m_bitsEndianNChoices = sizeof( m_bitsEndianChoices ) / sizeof( wxString );
	m_bitsEndian = new wxChoice( imageFormatSizer->GetStaticBox(), wxID_FORMAT_ENDIAN, wxDefaultPosition, wxDefaultSize, m_bitsEndianNChoices, m_bitsEndianChoices, 0 );
	m_bitsEndian->SetSelection( 0 );
	m_bitsEndian->Enable( false );
	m_bitsEndian->SetToolTip( wxT("Format Endianness") );

	fgSizer5->Add( m_bitsEndian, 0, wxALL, 5 );


	imageFormatSizer->Add( fgSizer5, 1, wxEXPAND, 5 );


	bSizer1->Add( imageFormatSizer, 0, wxALL|wxEXPAND, 8 );

	wxStaticBoxSizer* imageDimensionsSizer;
	imageDimensionsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Dimensions") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText44 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Standard :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer3->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_stdChoiceChoices;
	m_stdChoice = new wxChoice( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_stdChoiceChoices, 0 );
	m_stdChoice->SetSelection( 0 );
	fgSizer3->Add( m_stdChoice, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_WidthCtl = new wxTextCtrl( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_WidthCtl, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxT("Height :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_HeightCtl = new wxTextCtrl( imageDimensionsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_HeightCtl, 0, wxALL, 5 );


	imageDimensionsSizer->Add( fgSizer3, 1, wxEXPAND, 5 );


	bSizer1->Add( imageDimensionsSizer, 1, wxALL|wxEXPAND, 8 );

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
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( YUVCompDialogBase::OnUpdateUI ) );
	m_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( YUVCompDialogBase::OnFileSelected ), NULL, this );
	m_stdChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( YUVCompDialogBase::OnChangeStandard ), NULL, this );
}

YUVCompDialogBase::~YUVCompDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( YUVCompDialogBase::OnUpdateUI ) );
	m_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( YUVCompDialogBase::OnFileSelected ), NULL, this );
	m_stdChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( YUVCompDialogBase::OnChangeStandard ), NULL, this );

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

	m_staticText7 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Horizontal :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinH = new wxSpinCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 128, 0 );
	gSizer1->Add( m_spinH, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Vertical :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinV = new wxSpinCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 128, 0 );
	gSizer1->Add( m_spinV, 0, wxALL, 5 );


	sbSizer1->Add( gSizer1, 1, wxEXPAND, 5 );


	bSizer1->Add( sbSizer1, 1, wxALL|wxEXPAND, 8 );

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Grid Color") ), wxVERTICAL );

	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 2, 2, 0, 0 );

	m_staticText14 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, wxT("Grid Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	gSizer4->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_gridColorControl = new wxColourPickerCtrl( sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
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

ChecksumDialogBase::ChecksumDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	m_staticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Checksum Type") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText18 = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Luma :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	m_staticText18->SetFont( wxFont( 9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer4->Add( m_staticText18, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_lumaText = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("0xdeadbeef"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lumaText->Wrap( -1 );
	m_lumaText->SetFont( wxFont( 9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Monospace") ) );

	fgSizer4->Add( m_lumaText, 0, wxALL, 5 );

	m_staticText20 = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("Chroma :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	m_staticText20->SetFont( wxFont( 9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Monospace") ) );

	fgSizer4->Add( m_staticText20, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_chromaText = new wxStaticText( m_staticBoxSizer->GetStaticBox(), wxID_ANY, wxT("0xdeadbeef"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chromaText->Wrap( -1 );
	m_chromaText->SetFont( wxFont( 9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Monospace") ) );

	fgSizer4->Add( m_chromaText, 0, wxALL, 5 );


	m_staticBoxSizer->Add( fgSizer4, 1, wxALL|wxEXPAND, 8 );


	this->SetSizer( m_staticBoxSizer );
	this->Layout();
	m_staticBoxSizer->Fit( this );
}

ChecksumDialogBase::~ChecksumDialogBase()
{
}

frameInspectorMenu::frameInspectorMenu( long style ) : wxMenuBar( style )
{
	m_fileMenu = new wxMenu();
	m_submenuFileOpen = new wxMenu();
	wxMenuItem* m_submenuFileOpenItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxT("Open"), wxEmptyString, wxITEM_NORMAL, m_submenuFileOpen );
	wxMenuItem* m_submenuOpenSplit;
	m_submenuOpenSplit = new wxMenuItem( m_submenuFileOpen, ID_FILE_OPEN_SPLIT, wxString( wxT("YUV Split...") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_submenuFileOpen->Append( m_submenuOpenSplit );

	wxMenuItem* m_submenuOpenFile;
	m_submenuOpenFile = new wxMenuItem( m_submenuFileOpen, ID_FILE_OPEN_FILE, wxString( wxT("YUV FIle...") ) + wxT('\t') + wxT("Ctrl+F"), wxEmptyString, wxITEM_NORMAL );
	m_submenuFileOpen->Append( m_submenuOpenFile );

	wxMenuItem* m_submenuOpenDump;
	m_submenuOpenDump = new wxMenuItem( m_submenuFileOpen, ID_FILE_OPEN_DUMP, wxString( wxT("Dump directory...") ) + wxT('\t') + wxT("Ctrl+D"), wxEmptyString, wxITEM_NORMAL );
	m_submenuFileOpen->Append( m_submenuOpenDump );

	wxMenuItem* m_submenuOpenY4M;
	m_submenuOpenY4M = new wxMenuItem( m_submenuFileOpen, ID_FILE_OPEN_Y4M, wxString( wxT("YUV4MPEG File...") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuFileOpen->Append( m_submenuOpenY4M );

	m_fileMenu->Append( m_submenuFileOpenItem );

	wxMenuItem* m_menuFileClose;
	m_menuFileClose = new wxMenuItem( m_fileMenu, ID_FILE_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_menuFileClose );

	wxMenuItem* m_menuFileRefresh;
	m_menuFileRefresh = new wxMenuItem( m_fileMenu, ID_FILE_REFRESH, wxString( wxT("Refresh") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_menuFileRefresh );

	m_submenuFileSave = new wxMenu();
	wxMenuItem* m_submenuFileSaveItem = new wxMenuItem( m_fileMenu, wxID_ANY, wxT("Save"), wxEmptyString, wxITEM_NORMAL, m_submenuFileSave );
	wxMenuItem* m_submenuSaveImage;
	m_submenuSaveImage = new wxMenuItem( m_submenuFileSave, ID_FILE_SAVE_IMAGE, wxString( wxT("Save frame as bitmap image...") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuFileSave->Append( m_submenuSaveImage );

	wxMenuItem* m_submenuSaveSplit;
	m_submenuSaveSplit = new wxMenuItem( m_submenuFileSave, ID_FILE_SAVE_YUV_SPLIT, wxString( wxT("Save frame as YUV split...") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuFileSave->Append( m_submenuSaveSplit );

	wxMenuItem* m_submenuSaveComp;
	m_submenuSaveComp = new wxMenuItem( m_submenuFileSave, ID_FILE_SAVE_YUV_COMP, wxString( wxT("Save frame as YUV composite...") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuFileSave->Append( m_submenuSaveComp );

	wxMenuItem* m_submenuSaveYUV4M;
	m_submenuSaveYUV4M = new wxMenuItem( m_submenuFileSave, ID_FILE_SAVE_YUV_YUV4MPEG, wxString( wxT("Save Frame as YUV4MPEG...") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuFileSave->Append( m_submenuSaveYUV4M );

	m_fileMenu->Append( m_submenuFileSaveItem );

	m_fileMenu->AppendSeparator();

	wxMenuItem* m_menuFileExit;
	m_menuFileExit = new wxMenuItem( m_fileMenu, ID_FILE_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_menuFileExit );

	Append( m_fileMenu, wxT("&File") );

	m_formatMenu = new wxMenu();
	wxMenuItem* m_menuFormatSize;
	m_menuFormatSize = new wxMenuItem( m_formatMenu, ID_FORMAT_SETSIZE, wxString( wxT("Set Size...") ) + wxT('\t') + wxT("Alt+S"), wxEmptyString, wxITEM_NORMAL );
	m_formatMenu->Append( m_menuFormatSize );

	wxMenuItem* m_menuFormatYUV;
	m_menuFormatYUV = new wxMenuItem( m_formatMenu, ID_FORMAT_YUV, wxString( wxT("YUV Format...") ) , wxEmptyString, wxITEM_NORMAL );
	m_formatMenu->Append( m_menuFormatYUV );

	m_menuFormatMask = new wxMenu();
	wxMenuItem* m_menuFormatMaskItem = new wxMenuItem( m_formatMenu, wxID_ANY, wxT("Mask"), wxEmptyString, wxITEM_NORMAL, m_menuFormatMask );
	wxMenuItem* m_menuFormatMaskY;
	m_menuFormatMaskY = new wxMenuItem( m_menuFormatMask, ID_FORMAT_MASK_Y, wxString( wxT("Y Component") ) , wxEmptyString, wxITEM_CHECK );
	m_menuFormatMask->Append( m_menuFormatMaskY );

	wxMenuItem* m_menuFormatMaskU;
	m_menuFormatMaskU = new wxMenuItem( m_menuFormatMask, ID_FORMAT_MASK_U, wxString( wxT("U Component") ) , wxEmptyString, wxITEM_CHECK );
	m_menuFormatMask->Append( m_menuFormatMaskU );

	wxMenuItem* m_menuFormatMaskV;
	m_menuFormatMaskV = new wxMenuItem( m_menuFormatMask, ID_FORMAT_MASK_V, wxString( wxT("V Component") ) , wxEmptyString, wxITEM_CHECK );
	m_menuFormatMask->Append( m_menuFormatMaskV );

	m_formatMenu->Append( m_menuFormatMaskItem );

	wxMenuItem* m_menuFormatProps;
	m_menuFormatProps = new wxMenuItem( m_formatMenu, ID_FORMAT_PROPERTIES, wxString( wxT("Image Properties...") ) , wxEmptyString, wxITEM_NORMAL );
	m_formatMenu->Append( m_menuFormatProps );
	m_menuFormatProps->Enable( false );

	Append( m_formatMenu, wxT("Format") );

	m_viewMenu = new wxMenu();
	wxMenuItem* m_menuViewNext;
	m_menuViewNext = new wxMenuItem( m_viewMenu, ID_VIEW_NEXT, wxString( wxT("View next image") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuViewNext );

	wxMenuItem* m_menuViewPrev;
	m_menuViewPrev = new wxMenuItem( m_viewMenu, ID_VIEW_PREV, wxString( wxT("View previous image") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuViewPrev );

	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_viewMenu, ID_VIEW_GO_HOME, wxString( wxT("Goto first frame") ) + wxT('\t') + wxT("Ctrl+0"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuItem15 );

	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_viewMenu, ID_VIEW_GO_LAST, wxString( wxT("Goto last frame") ) + wxT('\t') + wxT("Ctrl+L"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuItem16 );

	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( m_viewMenu, ID_VIEW_GOTO, wxString( wxT("Goto frame #....") ) + wxT('\t') + wxT("Ctrl+G"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuItem17 );

	m_submenuViewScaling = new wxMenu();
	wxMenuItem* m_submenuViewScalingItem = new wxMenuItem( m_viewMenu, wxID_ANY, wxT("Scaling"), wxEmptyString, wxITEM_NORMAL, m_submenuViewScaling );
	wxMenuItem* m_menuItem20;
	m_menuItem20 = new wxMenuItem( m_submenuViewScaling, ID_VIEW_SCALE_ACTUAL, wxString( wxT("Actual size") ) , wxEmptyString, wxITEM_CHECK );
	m_submenuViewScaling->Append( m_menuItem20 );

	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem( m_submenuViewScaling, ID_VIEW_SCALE_SCALED, wxString( wxT("Fit to window") ) , wxEmptyString, wxITEM_CHECK );
	m_submenuViewScaling->Append( m_menuItem21 );

	wxMenuItem* m_menuItem22;
	m_menuItem22 = new wxMenuItem( m_submenuViewScaling, ID_VIEW_SCALE_HALF, wxString( wxT("Half size") ) , wxEmptyString, wxITEM_CHECK );
	m_submenuViewScaling->Append( m_menuItem22 );

	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( m_submenuViewScaling, ID_VIEW_SCALE_DOUBLE, wxString( wxT("Double size") ) , wxEmptyString, wxITEM_CHECK );
	m_submenuViewScaling->Append( m_menuItem23 );

	m_viewMenu->Append( m_submenuViewScalingItem );

	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_viewMenu, ID_VIEW_STEP, wxString( wxT("Auto step") ) , wxEmptyString, wxITEM_CHECK );
	m_viewMenu->Append( m_menuItem18 );

	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( m_viewMenu, ID_VIEW_GRID, wxString( wxT("Enable Grid") ) , wxEmptyString, wxITEM_CHECK );
	m_viewMenu->Append( m_menuItem19 );

	wxMenuItem* m_menuItem33;
	m_menuItem33 = new wxMenuItem( m_viewMenu, ID_VIEW_GRID_SETTINGS, wxString( wxT("Grid Settings...") ) , wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuItem33 );

	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_viewMenu, ID_VIEW_FULLSCREEN, wxString( wxT("Fullscreen") ) + wxT('\t') + wxT("F11"), wxEmptyString, wxITEM_NORMAL );
	m_viewMenu->Append( m_menuItem29 );

	Append( m_viewMenu, wxT("View") );

	m_crcMenu = new wxMenu();
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_crcMenu, ID_CHECKSUM_NONE, wxString( wxT("No checksum") ) , wxT("Disable checksum display"), wxITEM_CHECK );
	m_crcMenu->Append( m_menuItem25 );

	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_crcMenu, ID_CHECKSUM_CRC32, wxString( wxT("CRC32") ) , wxT("Enable CRC32 checksum"), wxITEM_CHECK );
	m_crcMenu->Append( m_menuItem26 );

	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_crcMenu, ID_CHECKSUM_MD5, wxString( wxT("MD5SUM") ) , wxT("Enable MD5SUM checksum"), wxITEM_CHECK );
	m_crcMenu->Append( m_menuItem27 );

	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( m_crcMenu, ID_CHECKSUM_SHA1, wxString( wxT("SHA1") ) , wxT("Enable SHA1 checksum"), wxITEM_CHECK );
	m_crcMenu->Append( m_menuItem28 );

	Append( m_crcMenu, wxT("Checksum") );

	m_helpMenu = new wxMenu();
	wxMenuItem* m_menuHelpAbout;
	m_menuHelpAbout = new wxMenuItem( m_helpMenu, ID_HELP_ABOUT, wxString( wxT("About...") ) , wxEmptyString, wxITEM_NORMAL );
	m_helpMenu->Append( m_menuHelpAbout );

	Append( m_helpMenu, wxT("Help") );

}

frameInspectorMenu::~frameInspectorMenu()
{
}

formatDialogBase::formatDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* imageFormatSizer;
	imageFormatSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Image Format") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText19 = new wxStaticText( sbSizer8->GetStaticBox(), wxID_ANY, wxT("Format :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer5->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_fmtChoiceChoices[] = { wxT("YUV 4:2:0"), wxT("YUV 4:2:2"), wxT("YUV 4:4:4"), wxT("NV12"), wxT("NV21") };
	int m_fmtChoiceNChoices = sizeof( m_fmtChoiceChoices ) / sizeof( wxString );
	m_fmtChoice = new wxChoice( sbSizer8->GetStaticBox(), wxID_FORMAT_YUV, wxDefaultPosition, wxDefaultSize, m_fmtChoiceNChoices, m_fmtChoiceChoices, 0 );
	m_fmtChoice->SetSelection( 0 );
	fgSizer5->Add( m_fmtChoice, 1, wxALL|wxEXPAND, 5 );


	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText201 = new wxStaticText( sbSizer8->GetStaticBox(), wxID_ANY, wxT("Bits :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	fgSizer5->Add( m_staticText201, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_bitsChoiceChoices[] = { wxT("8-bit"), wxT("10-bit"), wxT("16-bit") };
	int m_bitsChoiceNChoices = sizeof( m_bitsChoiceChoices ) / sizeof( wxString );
	m_bitsChoice = new wxChoice( sbSizer8->GetStaticBox(), wxID_FORMAT_BITS, wxDefaultPosition, wxDefaultSize, m_bitsChoiceNChoices, m_bitsChoiceChoices, 0 );
	m_bitsChoice->SetSelection( 0 );
	fgSizer5->Add( m_bitsChoice, 0, wxALL|wxEXPAND, 5 );

	wxString m_bitsEndianChoices[] = { wxT("Little Endian"), wxT("Big Endian") };
	int m_bitsEndianNChoices = sizeof( m_bitsEndianChoices ) / sizeof( wxString );
	m_bitsEndian = new wxChoice( sbSizer8->GetStaticBox(), wxID_FORMAT_ENDIAN, wxDefaultPosition, wxDefaultSize, m_bitsEndianNChoices, m_bitsEndianChoices, 0 );
	m_bitsEndian->SetSelection( 0 );
	m_bitsEndian->Enable( false );
	m_bitsEndian->SetToolTip( wxT("Format Endianness") );

	fgSizer5->Add( m_bitsEndian, 0, wxALL, 5 );


	sbSizer8->Add( fgSizer5, 1, wxEXPAND, 5 );


	imageFormatSizer->Add( sbSizer8, 1, wxALL|wxEXPAND, 8 );

	m_sdbSizer5 = new wxStdDialogButtonSizer();
	m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();

	imageFormatSizer->Add( m_sdbSizer5, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( imageFormatSizer );
	this->Layout();
	imageFormatSizer->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( formatDialogBase::OnUpdateUI ) );
}

formatDialogBase::~formatDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( formatDialogBase::OnUpdateUI ) );

}
