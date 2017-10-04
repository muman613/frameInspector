#include "YUVSplitDialog.h"
#include "YUV420ImageBufferSplit.h"

YUVSplitDialog::YUVSplitDialog( wxWindow* parent )
:   YUVSplitDialogBase( parent )
{
    wxLogDebug("YUVSplitDialog::YUVSplitDialog()");
#if 0
    utils::PopulateStandardDropDown(m_stdChoice);

    Layout();
    bSizer1->Fit(this);
#endif
}

void    YUVSplitDialog::OnInitDialog(wxInitDialogEvent& event) {
    wxLogDebug("YUVSplitDialog::OnInitDialog()");

//    m_dirPicker->SetInitialDirectory( "/tmp/" );
//    m_dirPicker->SetPath( "/tmp/" );

    utils::PopulateStandardDropDown(m_stdChoice);

    Layout();
    bSizer1->Fit(this);

    SetDialogOptions();
}

void YUVSplitDialog::OnUpdateUI( wxUpdateUIEvent& event )
{
    switch (event.GetId()) {
    case wxID_FORMAT_ENDIAN:
        {
            int nChoice = m_bitsChoice->GetSelection();

            event.Enable( ((nChoice == 1) || (nChoice == 2))?true:false );
        }
        break;

    case wxID_OK:
        event.Enable(ValidateFileSpec());
        break;

    default:
        break;
    }
}

void YUVSplitDialog::OnPrefixChanged( wxCommandEvent& event )
{
	// TODO: Implement OnPrefixChanged
}

void YUVSplitDialog::OnDirChanged( wxFileDirPickerEvent& event )
{
	// TODO: Implement OnDirChanged
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
}

/**
 *
 */

void YUVSplitDialog::OnChangeStandard( wxCommandEvent& event )
{
    int                     selection   = event.GetSelection();
    utils::vidstd_entry*    pEntry      = (utils::vidstd_entry*)event.GetClientData();

    wxLogDebug("YUVSplitDialog::OnChangeStandard()\n");

    wxASSERT(pEntry != 0L);

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

//void YUVSplitDialog::SetDialogOptions(const wxSize  frameSize,
//                         const wxString             sPath,
//                         const wxString             sPrefix,
//                         const wxUint16             bits,
//                         const formatEndian         endian,
//                         const dataType             yuvFmt)
void YUVSplitDialog::SetDialogOptions(const wxSize  frameSize,
                         const YUV420ImageSpec&     spec,
                         const wxUint16             bits,
                         const formatEndian         endian,
                         const dataType             yuvFmt)
{
    wxLogDebug("YUVSplitDialog::SetDialogOptions(%d, %d, %s, %s, %d, %s)",
               frameSize.GetWidth(),
               frameSize.GetHeight(),
               spec.m_imagePath, spec.m_prefix,
               bits,
               (endian == endian_little)?"little":"big");
#if 1
    /* Copy values to private store. */
    m_frameSize = frameSize;
//    m_sPath     = spec.m_imagePath;
//    m_sPrefix   = spec.m_prefix;
    m_bits      = bits;
    m_endian    = endian;
    m_yuvFmt    = yuvFmt;
    m_spec      = spec;
#else
    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), frameSize.GetWidth()) );
    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), frameSize.GetHeight()) );

    //m_dirPicker->SetPath( "/local_repository/storage/monsters" );
    m_dirPicker->SetInitialDirectory( sPath );
    m_dirPicker->SetPath( sPath );
    m_prefix->SetValue( sPrefix );

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
#endif // 1
}

/**
 *
 */

void YUVSplitDialog::SetDialogOptions() {
    wxLogDebug("YUVSplitDialog::SetDialogOptions()");

    m_WidthCtl->SetValue( wxString::Format(wxT("%d"), m_frameSize.GetWidth()) );
    m_HeightCtl->SetValue( wxString::Format(wxT("%d"), m_frameSize.GetHeight()) );

    //m_dirPicker->SetPath( "/local_repository/storage/monsters" );
    m_dirPicker->SetInitialDirectory( m_spec.m_imagePath );
    m_dirPicker->SetPath( m_spec.m_imagePath );
    m_prefix->SetValue( m_spec.m_prefix );

    SetStandardToDimensions( m_frameSize.GetWidth(), m_frameSize.GetHeight() );

    switch (m_bits) {
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

    switch (m_endian) {
    case endian_little:
        m_bitsEndian->SetSelection(0);
        break;
    case endian_big:
        m_bitsEndian->SetSelection(1);
        break;
    default:
        break;
    }

    m_fmtChoice->SetSelection(m_yuvFmt - DATA_YUV420);

    m_firstFrame->SetRange( 0, 10000);
    m_lastFrame->SetRange( 0, 10000 );
    m_firstFrame->SetValue( m_spec.m_first );
    m_lastFrame->SetValue( m_spec.m_last );
}


/**
 *
 */

void YUVSplitDialog::GetFrameDimensions(wxSize& frameSize) {
    long w, h;

    GetFrameDimensions(w, h);
    frameSize = wxSize(w, h);

    return;
}

/**
 *
 */

void YUVSplitDialog::GetFrameDimensions(long& width, long& height) {
    m_WidthCtl->GetValue().ToLong(&width);
    m_HeightCtl->GetValue().ToLong(&height);

    return;
}

/**
 *
 */

void YUVSplitDialog::GetFrameBits(wxUint16& bits) {
    wxLogDebug("YUVSplitDialog::GetFrameBits(...)");

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

/**
 *
 */

void YUVSplitDialog::GetFrameEndianness(formatEndian& endian) {
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

void YUVSplitDialog::GetFileInfo(wxString& sFilePath, wxString& sFilePrefix) {
    sFilePath = m_dirPicker->GetPath();
    sFilePrefix = m_prefix->GetValue();

    return;
}

void YUVSplitDialog::GetFrameVector(utils::FRAME_FILEBASE_VEC& vec) {
    vec = m_fileVec;
    return;
}

/**
 *
 */

void YUVSplitDialog::SetStandardToDimensions(int width, int height) {
    wxLogDebug("YUVSplitDialog::SetStandardToDimensions(%d, %d)", width, height);

    int standard = (int)utils::GetStandardFromSize(width, height);
    m_stdChoice->SetSelection( standard );

    return;
}

/**
 *
 */

void YUVSplitDialog::GetFrameYUVFmt(dataType& type) {
    type = (dataType)((int)DATA_YUV420 + m_fmtChoice->GetSelection());

    return;
}

bool YUVSplitDialog::ValidateFileSpec() {
    wxString sPath      = m_dirPicker->GetPath();
    wxString sFileSpec  = m_prefix->GetValue();
    int      nFirst     = m_firstFrame->GetValue();
    int      nLast      = m_lastFrame->GetValue();

    YUV420ImageSpec spec(sPath, sFileSpec, nFirst, nLast);

    m_fileVec.clear();
    utils::GetSplitBufferFrameVec( spec, m_fileVec );

    return (m_fileVec.size() > 0)?true:false;
}

void YUVSplitDialog::GetImageSpec(YUV420ImageSpec& spec) {
    wxString sPath      = m_dirPicker->GetPath();
    wxString sFileSpec  = m_prefix->GetValue();
    int      nFirst     = m_firstFrame->GetValue();
    int      nLast      = m_lastFrame->GetValue();

    YUV420ImageSpec     newSpec(sPath, sFileSpec, nFirst, nLast);

    spec = newSpec;

    return;
}
