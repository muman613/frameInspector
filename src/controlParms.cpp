#include <wx/wx.h>
#include <wx/config.h>
#include "controlParms.h"
#include "misc_utils.h"

/**
 *  controlParms class encapsulates several parameters used by the
 *  application.
 */

controlParms::controlParms()
    :   m_bufType(YUV_FILE_UNKNOWN),
        m_yuvType(DATA_UNKNOWN),
        m_scale(IS_IMAGE_DEFAULT),
        m_imageSize(wxSize(0,0)),
        m_curFrame(0),
        m_bEnableGrid(false),
        m_gridH(16),
        m_gridV(16),
        m_gridColor(*wxBLUE),
        m_bits(8),
        m_endianness(endian_little),
        m_spec("", "", 0, 1000)
{
    // ctor
}

controlParms::~controlParms()
{
    // dtor
}

/**
 *  Reset parameters to default.
 */

void controlParms::reset() {
    wxLogDebug("controlParms::reset()");

    m_bufType       = YUV_FILE_UNKNOWN;
    m_yuvType       = DATA_UNKNOWN;
    m_scale         = IS_IMAGE_DEFAULT;
    m_imageSize     = wxSize(0,0);
    m_curFrame      = 0;
    m_bEnableGrid   = false;
    m_gridH         = 16;
    m_gridV         = 16;
    m_gridColor     = *wxBLUE;
    m_imagePath.clear();
    m_imageFilename.clear();
    m_bits          = 8;
    m_endianness    = endian_little;

    m_spec.m_imagePath.clear();
    m_spec.m_prefix.clear();
    m_spec.m_first  = 0;
    m_spec.m_last   = 1000;

    return;
}

/**
 *  Load parameters from configuration file.
 */

bool controlParms::LoadParmsFromConfig(wxConfig* pConfig,
                                       const wxString sSection)
{
    wxString    sOldPath, sPath;
    long        nTmp, w, h;
    wxString    sTmp;

    wxLogDebug("controlParms::LoadParmsFromConfig(%p, %s)", pConfig, sSection);

    wxASSERT( pConfig != nullptr );

    if (pConfig == 0) {
        wxLogDebug("ERROR: Called SaveState with a NULL wxConfig!");
        return false;
    }

    sOldPath = pConfig->GetPath();
    sPath = wxString::Format("/control/%s", sSection);

    pConfig->SetPath(sPath);

    if (pConfig->Read("Type", &nTmp)) {
        m_bufType = (yuvBufType)nTmp;
    }

    if (pConfig->Read("YUVfmt", &nTmp)) {
        m_yuvType = (dataType)nTmp;
    }

    if (pConfig->Read("Bits", &nTmp)) {
        m_bits = (wxUint32)nTmp;
    }

    if (pConfig->Read("Endian", &sTmp)) {
        if (sTmp.CmpNoCase("le") == 0) {
            m_endianness = endian_little;
        } else if (sTmp.CmpNoCase("be") == 0) {
            m_endianness = endian_big;
        } else {
            wxLogDebug("Invalid value for Endian in config file!");
            m_endianness = endian_little;
        }
    }

    if (pConfig->Read("Scale", &nTmp)) {
        m_scale = (scaleVal)nTmp;
    }

    if (pConfig->Read("Width", &w) && pConfig->Read("Height", &h)) {
        m_imageSize = wxSize(w,h);
    }

    if (pConfig->Read("CurFrame", &nTmp)) {
        m_curFrame = nTmp;
    }

    if (pConfig->Read("Grid", &nTmp)) {
        m_bEnableGrid = (nTmp == 1);
    }

    if (pConfig->Read("GridH", &nTmp)) {
        m_gridH = nTmp;
    }
    if (pConfig->Read("GridV", &nTmp)) {
        m_gridV = nTmp;
    }
#if wxCHECK_VERSION(2,9,5)
    pConfig->Read("gridColor", &m_gridColor);
#else
    if (pConfig->Read("gridColor", &sTmp)) {
        m_gridColor = string_to_color(sTmp);
    }
#endif

    pConfig->Read("Path",   &m_imagePath);
    pConfig->Read("File",   &m_imageFilename);
    pConfig->Read("Prefix", &m_prefix);

    pConfig->Read("SplitPath",      &m_spec.m_imagePath);
    pConfig->Read("SplitPrefix",    &m_spec.m_prefix);
    pConfig->Read("SplitFirst",     &m_spec.m_first, 0);
    pConfig->Read("SplitLast",      &m_spec.m_last, 1000);

    pConfig->SetPath(sOldPath);

    return true;
}


#if 0
/**
 *
 */

bool controlParms::SaveParmsToConfig(wxConfig* pConfig,
                                     const wxString sSection)
{
    wxString sOldPath, sPath;

    wxLogDebug("controlParms::SaveParmsToConfig((%p, %s)", pConfig, sSection);

    wxASSERT( pConfig != nullptr );

    if (pConfig == nullptr) {
        wxLogDebug("ERROR: Called SaveParmsToConfig with a NULL wxConfig!");
        return false;
    }

    sOldPath = pConfig->GetPath();
    sPath = wxString::Format("/control/%s", sSection);

    pConfig->SetPath(sPath);

    pConfig->Write("Type",      (int)m_bufType);
    pConfig->Write("YUVfmt",    (int)m_yuvType);
    pConfig->Write("Scale",     (int)m_scale);
    pConfig->Write("Width",     m_imageSize.GetWidth());
    pConfig->Write("Height",    m_imageSize.GetHeight());
    pConfig->Write("CurFrame",  m_curFrame);
    pConfig->Write("Path",      m_imagePath);
    pConfig->Write("File",      m_imageFilename);
    pConfig->Write("Prefix",    m_prefix);

    /* Save grid options */

    pConfig->Write("Grid",      (int)m_bEnableGrid);
    pConfig->Write("GridH",     m_gridH);
    pConfig->Write("GridV",     m_gridV);

#if wxCHECK_VERSION(2,9,5)
    pConfig->Write("gridColor", m_gridColor);
#else
    sTmp = color_to_string(m_gridColor);
    pConfig->Write(wxT("gridColor"),   sTmp);
#endif


    pConfig->SetPath(sOldPath);

    return true;
}

#endif // 0


