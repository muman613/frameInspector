#ifndef __DUMPIMAGEBUFFER_H__
#define __DUMPIMAGEBUFFER_H__

#include "imageBuffer.h"
#include "dbgutils.h"

class dumpImageBuffer : public ImageBuffer
{
public:
    dumpImageBuffer(int width, int height);
    virtual ~dumpImageBuffer();

    ssize_t		GetFrameCount();
    void		GetImage(wxImage* pImage);
    bool		Load(size_t frame);
    PIXEL*		getPixel(int x, int y);

    void		setPath(const wxString& path) {
        debug("dumpImageBuffer::setPath(%s)\n", path.c_str());
        m_imagePath = path;
    }

    bool        CanQueryFrameSize() const {
        return true;
    }

    bool        QueryFrameSize(int& width, int& height);

protected:
    bool DoYUVConversion(int rows, int cols, wxUint8* pY, wxUint8* pU, wxUint8* pV);

private:
    wxString        m_imagePath;
    wxArrayString   m_frameList;
    wxFile          m_fp;
//    bool            m_ccir601;
};

#endif  // __DUMPIMAGEBUFFER_H__

