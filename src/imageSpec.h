#ifndef __IMAGESPEC_H__
#define __IMAGESPEC_H__

#include <wx/wx.h>

class YUV420ImageSpec {
public:
    YUV420ImageSpec() { };
    YUV420ImageSpec(wxString sPath, wxString sPrefix, int first, int last)
        :   m_imagePath(sPath), m_prefix(sPrefix),
            m_first(first), m_last(last)
    {

    }

    wxString        m_imagePath;
    wxString        m_prefix;
    int             m_first         =   0;
    int             m_last          =   10000;
};



#endif // __IMAGESPEC_H__
