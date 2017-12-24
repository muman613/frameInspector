#ifndef __DUMPIMAGEBUFFER_H__
#define __DUMPIMAGEBUFFER_H__

#include <wx/wx.h>
#include "imageBuffer.h"

/**
 *  ImageBuffer to read a Sigma rmar file.
 */
class dumpImageBuffer : public ImageBuffer {
public:
    dumpImageBuffer();
    dumpImageBuffer(int width, int height);
    virtual ~dumpImageBuffer();

    ssize_t		GetFrameCount();
    void		GetImage(wxImage* pImage);
    bool		Load(size_t frame);
    PIXEL*		getPixel(int x, int y);

    void		setPath(const wxString& path) {
        wxLogDebug("dumpImageBuffer::setPath(%s)", (const char*)path.c_str());
        m_imagePath = path;
        GetFrameCount();
    }

    bool        CanQueryFrameSize() const {
        return true;
    }

    bool        CanChecksum() const {
        return true;
    }

    bool        CanSave() const {
        return true;
    }

    bool        QueryFrameSize(int& width, int& height);

protected:
    bool DoYUVConversion(/*int rows, int cols, */ wxUint8* pY, wxUint8* pU, wxUint8* pV);

private:
    wxString        m_imagePath;
    wxArrayString   m_frameList;
    wxFile          m_fp;
    size_t          m_frameNo;
};

#endif  // __DUMPIMAGEBUFFER_H__

