#include <wx/wx.h>
#include "Dialogs.h"
#include "hashDialog.h"
#include "misc_utils.h"

HashDialog::HashDialog(wxWindow* pParent)
:   HashDialogBase(pParent)
{

}

HashDialog::~HashDialog()
{
}

void HashDialog::SetChecksums(wxUint8* lumaSum, wxUint8* chromaSum) {
    std::string sTmp;

    utils::digest_to_string( lumaSum, sTmp );
    m_lumaText->SetLabel( sTmp.c_str() );

    utils::digest_to_string( chromaSum, sTmp );
    m_chromaText->SetLabel( sTmp.c_str() );

}
