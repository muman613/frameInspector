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
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Luma Hash :"), wxDefaultPosition, wxSize( 120,-1 ), wxALIGN_RIGHT );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );
	
	m_lumaText = new wxStaticText( this, wxID_ANY, wxT("0xDEAD BEEF"), wxDefaultPosition, wxSize( 260,-1 ), 0 );
	m_lumaText->Wrap( -1 );
	m_lumaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	bSizer2->Add( m_lumaText, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, 0, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Chroma Hash :"), wxDefaultPosition, wxSize( 120,-1 ), wxALIGN_RIGHT );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer3->Add( m_staticText3, 0, wxALL, 5 );
	
	m_chromaText = new wxStaticText( this, wxID_ANY, wxT("0xDEAD BEEF"), wxDefaultPosition, wxSize( 260,-1 ), 0 );
	m_chromaText->Wrap( -1 );
	m_chromaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	bSizer3->Add( m_chromaText, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 0 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer1->Add( m_sdbSizer1, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
}

HashDialogBase::~HashDialogBase()
{
}
