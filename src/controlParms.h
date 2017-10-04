#ifndef __CONTROL_PARMS_H__
#define __CONTROL_PARMS_H__

#include "imageBuffer.h"
#include "yuvImageControl.h"

class wxConfig;

class controlParms {
public:
    controlParms();
    virtual ~controlParms();

    bool            LoadParmsFromConfig(wxConfig* pConfig, const wxString sSection);
//  bool            SaveParmsToConfig(wxConfig* pConfig, const wxString sSection);
    void            reset();

    yuvBufType          m_bufType;
    dataType            m_yuvType;
    scaleVal            m_scale;
    wxSize              m_imageSize;
    wxInt32             m_curFrame;
    wxString            m_imagePath;
    wxString            m_imageFilename;
    wxString            m_prefix;
    bool                m_bEnableGrid;
    wxInt32             m_gridH;
    wxInt32             m_gridV;
    wxColor             m_gridColor;
    wxInt32             m_bits;
    formatEndian        m_endianness;
    YUV420ImageSpec     m_spec;
};


#endif // __CONTROL_PARMS_H__
