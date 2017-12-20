#include <wx/wx.h>
#include "ChecksumDialog.h"

ChecksumDialog::ChecksumDialog( wxWindow* parent )
    :
    ChecksumDialogBase( parent )
{
    // ctor
    wxLogDebug("ChecksumDialog::ChecksumDialog()");
}

ChecksumDialog::~ChecksumDialog()
{
    // dtor
    wxLogDebug("ChecksumDialog::~ChecksumDialog()");
}

void ChecksumDialog::SetAlgoName(const wxString& sAlgoName)
{
    m_staticBoxSizer->GetStaticBox()->SetLabel( sAlgoName );
}

void ChecksumDialog::SetChecksumValues(const wxString& sLuma,
                                       const wxString& sChroma)
{
    wxLogDebug("ChecksumDialog::SetChecksumValues(%s, %s)", sLuma, sChroma);

    m_lumaText->SetLabel( sLuma );
    m_chromaText->SetLabel( sChroma );

    Layout();
    m_staticBoxSizer->Fit(this);

    return;
}
