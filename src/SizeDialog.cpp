#include <wx/wx.h>
#include "SizeDialog.h"
#include "misc_utils.h"
//#include "dbgutils.h"

/**
 *
 */

SizeDialog::SizeDialog( wxWindow* parent, int width, int height )
    : SizeDialogBase( parent )
{
    utils::PopulateStandardDropDown(m_stdChoice);

    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), width) );
    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), height) );


    Layout();
    bSizer1->Fit(this);

    SetStandardToDimensions( width, height );
}

/**
 *
 */

void SizeDialog::OnChangeStandard( wxCommandEvent& event )
{
    int                     selection   = event.GetSelection();
    utils::vidstd_entry*    pEntry      = (utils::vidstd_entry*)event.GetClientData();

    wxLogDebug("SizeDialog::OnChangeStandard()");

    wxASSERT(pEntry != nullptr);

    wxLogDebug("Selected %d (%s %d x %d)\n", selection,
               (const char*)pEntry->name.c_str(), pEntry->width, pEntry->height);

    if (selection > 0) {
        m_WidthCtl->SetValue( wxString::Format(wxT("%d"), pEntry->width) );
        m_HeightCtl->SetValue( wxString::Format(wxT("%d"), pEntry->height) );
    } else {
        m_WidthCtl->SetValue( wxEmptyString );
        m_HeightCtl->SetValue( wxEmptyString );
    }

    return;
}

/**
 *
 */

void SizeDialog::SetStandardToDimensions(int width, int height) {
    wxLogDebug("SizeDialog::SetStandardToDimensions(%d, %d)", width, height);

    int standard = (int)utils::GetStandardFromSize(width, height);
    m_stdChoice->SetSelection( standard );

    return;
}

/**
 *
 */

void SizeDialog::GetFrameDimensions(long& width, long& height) {
    m_WidthCtl->GetValue().ToLong(&width);
    m_HeightCtl->GetValue().ToLong(&height);

    return;
}

