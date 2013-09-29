#include "SizeDialog.h"
#include "dbgutils.h"

//struct vidstd_entry {
//    videoStd        standard;
//    int             width;
//    int             height;
//};

struct vidstd_entry vidStd[] = {
    {   STD_CUSTOM,  -1,   -1, },
    {   STD_SQCIF,  128,   96, },
    {   STD_QCIF,   176,  144, },
    {   STD_SCIF,   256,  192, },
    {   STD_SIF,    352,  240, },
    {   STD_CIF,    352,  288, },
    {   STD_VGA,    640,  480, },
    {   STD_4SIF,   704,  480, },
    {   STD_4CIF,   704,  576, },
    {   STD_SVGA,   800,  600, },
    {   STD_SXGA,  1280, 1024, },
    {   STD_1080p, 1920, 1080, },
};

int stdCount = sizeof(vidStd)/sizeof(vidstd_entry);

SizeDialog::SizeDialog( wxWindow* parent, int width, int height )
:
SizeDialogBase( parent )
{
    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), width) );
    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), height) );

    SetStandardToDimensions( width, height );
}

void SizeDialog::OnChangeStandard( wxCommandEvent& event )
{
    debug("SizeDialog::OnChangeStandard()\n");
    int selection = event.GetSelection();

    debug("Selected %d\n", selection);

    if (selection > 0) {
        SetDimensionsToStandard((videoStd)selection);
    }

}

/**
 *
 */

void SizeDialog::SetStandardToDimensions(int width, int height) {
    int standard = STD_CUSTOM;

    debug("SizeDialog::SetStandardToDimensions(%d, %d)\n", width, height);

    for (int i = 0 ; i < stdCount ; i++) {
        if ((vidStd[i].width == width) && (vidStd[i].height == height)) {
            standard = i;
            break;
        }
    }

    m_stdChoice->SetSelection( standard );

    return;
}

void SizeDialog::SetDimensionsToStandard(videoStd standard) {
    if ((standard > 0) && (standard < stdCount)) {
        int width, height;

        width  = vidStd[standard].width;
        height = vidStd[standard].height;

        m_WidthCtl->SetValue( wxString::Format(wxT("%d"), width) );
        m_HeightCtl->SetValue( wxString::Format(wxT("%d"), height) );
    }
    return;
}

void SizeDialog::GetFrameDimensions(long& width, long& height) {
    m_WidthCtl->GetValue().ToLong(&width);
    m_HeightCtl->GetValue().ToLong(&height);

    return;
}
