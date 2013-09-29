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
        debug("dumpImageBuffer::setPath(%s)\n", (const char*)path.c_str());
        m_imagePath = path;
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

    bool        SaveYUV(wxString sFilename, eSaveType type);
    bool        QueryFrameSize(int& width, int& height);
    bool        GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum);

protected:
    bool DoYUVConversion(int rows, int cols, wxUint8* pY, wxUint8* pU, wxUint8* pV);

private:
    wxString        m_imagePath;
    wxArrayString   m_frameList;
    wxFile          m_fp;
    size_t          m_frameNo;
    wxUint32        m_lumaSize;
    wxUint32        m_chromaSize;
    wxUint8*        m_pY;
    wxUint8*        m_pU;
    wxUint8*        m_pV;
//    bool            m_ccir601;
};

#endif  // __DUMPIMAGEBUFFER_H__

