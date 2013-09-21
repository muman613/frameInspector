#include "SizeDialog.h"

SizeDialog::SizeDialog( wxWindow* parent, int width, int height )
:
SizeDialogBase( parent )
{
    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), width) );
    m_heightCtl->SetValue( wxString::Format(wxT("%d"), height) );
}
