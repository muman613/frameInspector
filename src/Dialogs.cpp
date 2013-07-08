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

    m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Luma Hash :"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText9->Wrap( -1 );
    fgSizer1->Add( m_staticText9, 0, wxALIGN_RIGHT|wxALL, 5 );

    m_lumaText = new wxStaticText( this, wxID_ANY, wxT("DEADBEEFDEADBEEFDEADBEEFDEADBEEF"), wxDefaultPosition, wxDefaultSize, 0 );
    m_lumaText->Wrap( -1 );
    m_lumaText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );

    fgSizer1->Add( m_lumaText, 0, wxALL|wxEXPAND, 5 );

    m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Chroma Hash :"), wxDefaultPosition, wxDefaultSize, 0 );
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
