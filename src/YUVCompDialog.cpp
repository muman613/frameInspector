#include <wx/wx.h>
#include <wx/regex.h>
#include "YUVCompDialog.h"
#include "misc_utils.h"
//#include "dbgutils.h"

YUVCompDialog::YUVCompDialog( wxWindow* parent )
    :   YUVCompDialogBase( parent )
{
    utils::PopulateStandardDropDown(m_stdChoice);

    Layout();
    bSizer1->Fit(this);
}

//YUVCompDialog::YUVCompDialog( wxWindow* parent, int width, int height, wxString sPath )
//:   YUVCompDialogBase( parent )
//{
//    utils::PopulateStandardDropDown(m_stdChoice);
//
//    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), width) );
//    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), height) );
//    m_filePicker->SetInitialDirectory( sPath );
//
//    Layout();
//    bSizer1->Fit(this);
//
//    SetStandardToDimensions( width, height );
//}

//YUVCompDialog::YUVCompDialog( wxWindow* parent, const wxSize& imageSize, const wxString& sPath )
//:   YUVCompDialogBase( parent )
//{
//    wxLogDebug("YUVCompDialog::YUVCompDialog(%p, wxSize(%d, %d), %s)\n",
//          parent, imageSize.GetWidth(), imageSize.GetHeight(),
//          (const char*)sPath.c_str());
//
//    utils::PopulateStandardDropDown(m_stdChoice);
//
//    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), imageSize.GetWidth()) );
//    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), imageSize.GetHeight()) );
//
//
//    Layout();
//    bSizer1->Fit(this);
//
//    m_filePicker->SetInitialDirectory( sPath );
//    //m_filePicker->SetPath( "/tmp/uncut.txt" );
//
//    SetStandardToDimensions( imageSize.GetWidth(), imageSize.GetHeight() );
//}

void YUVCompDialog::SetDialogOptions(const wxSize               frameSize,
                                     const wxString             sPath,
                                     const wxString             sLastFile,
                                     const wxUint16             bits,
                                     const formatEndian         endian,
                                     const dataType             yuvFmt)
{
    wxLogDebug("YUVCompDialog::SetDialogOptions(%d, %d, %s, %s, %d, %s)",
               frameSize.GetWidth(),
               frameSize.GetHeight(),
               (const char*)sPath.c_str(),
               (const char*)sLastFile.c_str(),
               bits,
               (endian == endian_little)?"little":"big");

    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), frameSize.GetWidth()) );
    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), frameSize.GetHeight()) );

    m_filePicker->SetPath( sLastFile );

    m_filePicker->SetInitialDirectory( sPath  );

    SetStandardToDimensions( frameSize.GetWidth(), frameSize.GetHeight() );

    switch (bits) {
    case 8:
        m_bitsChoice->SetSelection(0);
        break;
    case 10:
        m_bitsChoice->SetSelection(1);
        break;
    case 16:
        m_bitsChoice->SetSelection(2);
        break;
    }

    switch (endian) {
    case endian_little:
        m_bitsEndian->SetSelection(0);
        break;
    case endian_big:
        m_bitsEndian->SetSelection(1);
        break;
    default:
        break;
    }

    m_fmtChoice->SetSelection(yuvFmt - DATA_YUV420);

    return;
}

/**
 *
 */

void YUVCompDialog::OnChangeStandard( wxCommandEvent& event )
{
    int                     selection   = event.GetSelection();
    utils::vidstd_entry*    pEntry      = (utils::vidstd_entry*)event.GetClientData();

    wxLogDebug("YUVCompDialog::OnChangeStandard()\n");

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

void YUVCompDialog::SetStandardToDimensions(int width, int height) {
    wxLogDebug("YUVCompDialog::SetStandardToDimensions(%d, %d)", width, height);

    int standard = (int)utils::GetStandardFromSize(width, height);
    m_stdChoice->SetSelection( standard );

    return;
}

/**
 *
 */

void YUVCompDialog::GetFrameDimensions(long& width, long& height) {
    m_WidthCtl->GetValue().ToLong(&width);
    m_HeightCtl->GetValue().ToLong(&height);

    return;
}

/**
 *
 */

void YUVCompDialog::GetFrameDimensions(wxSize& frameSize) {
    long w, h;

    m_WidthCtl->GetValue().ToLong(&w);
    m_HeightCtl->GetValue().ToLong(&h);

    frameSize = wxSize(w, h);

    return;
}

/**
 *
 */

void YUVCompDialog::GetFilePath(wxString& sFilePath) {
    sFilePath = m_filePicker->GetPath();
    return;
}

/**
 *  Handle user selection of source file.
 */

void YUVCompDialog::OnFileSelected(wxFileDirPickerEvent& event) {
    wxString            sFilename;
    utils::imageParms   parms;

    wxLogDebug("YUVCompDialog::OnFileSelected()");
    wxLogDebug("File %s", event.GetPath());

    sFilename = event.GetPath();

    if (utils::ParseFilePath( sFilename, parms )) {
        if ((parms.width != 0) && (parms.height != 0)) {
            m_WidthCtl->SetValue( wxString::Format(wxT("%d"), parms.width) );
            m_HeightCtl->SetValue( wxString::Format(wxT("%d"), parms.height) );
            SetStandardToDimensions( parms.width, parms.height);
        }

        wxLogDebug("Image depth = %d", parms.depth);

        if (parms.depth == 8) {
            m_bitsChoice->SetSelection(0);
        } else if (parms.depth == 10) {
            m_bitsChoice->SetSelection(1);
        } else if (parms.depth == 16) {
            m_bitsChoice->SetSelection(2);
        }
//        else {
//            m_bitsChoice->SetSelection(0);
//        }

        if (parms.endianness == endian_little) {
            m_bitsEndian->SetSelection(0);
        } else if (parms.endianness == endian_big) {
            m_bitsEndian->SetSelection(1);
        }

        if (parms.yuvFmt != DATA_UNKNOWN) {
            m_fmtChoice->SetSelection((int)parms.yuvFmt - (int)DATA_YUV420);
        }
    }

    return;
}

/**
 *
 */

void YUVCompDialog::GetFrameBits(wxUint16& bits) {
    wxLogDebug("YUVCompDialog::GetFrameBits(...)");

    int bitSel = m_bitsChoice->GetSelection();

    wxLogDebug("bitsel = %d", bitSel);

    switch (bitSel) {
    case 0:
        bits = 8;
        break;
    case 1:
        bits = 10;
        break;
    case 2:
        bits = 16;
        break;
    }

    wxLogDebug("bits = %d", bits);
}


void YUVCompDialog::GetFrameEndianness(formatEndian& endian) {
    int sel = m_bitsEndian->GetSelection();

    switch (sel) {
    case 0:
        endian = endian_little;
        break;
    case 1:
        endian = endian_big;
        break;
    default:
        endian = endian_undefined;
        break;
    }

    return;
}

/**
 *
 */

void YUVCompDialog::GetFrameYUVFmt(dataType& type) {
    type = (dataType)((int)DATA_YUV420 + m_fmtChoice->GetSelection());

    return;
}

/**
 *
 */

void YUVCompDialog::OnUpdateUI(wxUpdateUIEvent& event) {
    switch (event.GetId()) {
    case wxID_FORMAT_ENDIAN:
    {
        int nChoice = m_bitsChoice->GetSelection();

        event.Enable( ((nChoice == 1) || (nChoice == 2))?true:false );
    }
    break;
    default:
        break;
    }
}

