/**
 * @file    yuvImageControl.cpp
 * @author  Michael A. Uman
 * @date    December 20, 2017
 *
 * @brief   Implementation of the yuvImageControl component.
 */

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <wx/wx.h>
#include <wx/config.h>
#include <typeinfo>

#include "yuvImageControl.h"
#include "imageBuffer.h"
#include "YUV420ImageBufferSplit.h"
#include "YUV420ImageBufferFile.h"
#include "DumpImageBuffer.h"
#ifdef  HAVE_LIBMJPEGTOOLS
    #include "YUV420ImageBufferY4M.h"
#endif
#include "controlParms.h"

IMPLEMENT_DYNAMIC_CLASS( yuvImageControl, wxScrolledWindow )

BEGIN_EVENT_TABLE( yuvImageControl, wxScrolledWindow )
    EVT_PAINT( yuvImageControl::OnPaint )
    EVT_SIZE( yuvImageControl::OnSize )
END_EVENT_TABLE()

/**
 *
 */

yuvImageControl::yuvImageControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    :   wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER, _T("canvas")),
        m_bValid(false),
        m_pImage(0L),
        m_pScaledImage(0L),
        m_buffer(0L),
        m_bEnableGrid(false),
        m_gridColor(*wxBLUE),
        m_gridH(16),
        m_gridV(16),
        m_imageSize(-1, -1),
        m_flags(0),
        m_scale(IS_IMAGE_DEFAULT),
        m_yuvType(DATA_YUV420),
        m_bufType(YUV_FILE_UNKNOWN),
        m_bits(8),
        m_endianness(endian_little),
        m_lastFrame(0),
        m_curFrame(0)
{
    //ctor
    wxLogDebug("yuvImageControl::yuvImageControl()");
    SetScrollRate(1, 1);
    SetBackgroundColour( wxT("LIGHT STEEL BLUE") );

    m_mask.bMaskY = m_mask.bMaskU = m_mask.bMaskV = true;
}

/**
 *
 */

yuvImageControl::~yuvImageControl()
{
    wxLogDebug("yuvImageControl::~yuvImageControl()");

    CloseImage();
}

bool yuvImageControl::IsValid() const {
#ifdef  _DEBUG
    if (m_bValid) {
        wxASSERT( m_pImage != 0 );
        wxASSERT( m_pScaledImage != 0 );
        wxASSERT( m_buffer != 0 );
    }
#endif
    return m_bValid;
}

/**
 * @brief Return the image path.
 */

wxString yuvImageControl::GetImagePath() const {
    return m_imagePath;
}

wxString yuvImageControl::GetImagePrefix() const {
    return m_prefix;
}


wxString yuvImageControl::GetImageName() const {
    return m_imageFilename;
}

wxUint16 yuvImageControl::GetImageBits() const {
    return m_bits;
}

formatEndian yuvImageControl::GetEndianness() const {
    return m_endianness;
}

/**
 *  @brief Set the image scale.
 */

void yuvImageControl::SetImageScale(scaleVal imageScale) {
    wxLogDebug("yuvImageControl::SetImageScale(%d)", (int)imageScale);

    m_scale = imageScale;

    if (m_bValid) {
        GetImage();
        Refresh();
    } else {
        wxLogDebug("WARNING: Setting image scale on invalid control!");
    }
}

/**
 *
 */

scaleVal yuvImageControl::GetImageScale() const {
    return m_scale;
}

/**
 *  @brief Set the image size.
 */

void yuvImageControl::SetImageSize(const wxSize& size) {
    wxLogDebug("yuvImageControl::SetImageSize(%d,%d)", size.GetWidth(), size.GetHeight());

    m_imageSize = size;
    allocate_image_buffer();
    GetImage();
    Refresh();
}

/**
 *  @brief Get the image size.
 */

wxSize yuvImageControl::GetImageSize() const {
    wxLogDebug("yuvImageControl::GetImageSize() %d, %d",
               m_imageSize.GetWidth(), m_imageSize.GetHeight());
    return m_imageSize;
}

/**
 *
 */

void yuvImageControl::enable_grid() {
    m_bEnableGrid = true;
    Refresh();
}

/**
 *
 */

void yuvImageControl::get_grid_settings(int& hor, int& ver, wxColor& color, bool& state) {
    wxLogDebug("yuvImageControl::get_grid_settings()");

    hor     = m_gridH;
    ver     = m_gridV;
    color   = m_gridColor;
    state   = m_bEnableGrid;

    return;
}

/**
 *
 */

size_t yuvImageControl::GetImageCount() const {
    return m_lastFrame;
}

/**
 *
 */

size_t yuvImageControl::GetCurrentFrame() const {
    return m_curFrame;
}
/**
 *
 */

void yuvImageControl::disable_grid() {
    m_bEnableGrid = false;
    Refresh();
}

/**
 *
 */

bool yuvImageControl::get_grid_state()  {
    return m_bEnableGrid;
}

/**
 *  @brief Get the grid dimensions.
 */

void yuvImageControl::set_grid_dimensions(int hor, int ver, const wxColor& color) {
    m_gridH = hor;
    m_gridV = ver;
    m_gridColor = color;
    Refresh();
}

/**
 *  @brief  Allocate the image buffer object.
 */

bool yuvImageControl::allocate_image_buffer() {
    wxLogDebug("yuvImageControl::allocate_image_buffer()");

    free_image_buffer();                /* delete any existing image buffer */

    switch (m_bufType) {
    case YUV_FILE_SPLIT:
        {
            YUV420ImageBufferSplit*	newBuf = new YUV420ImageBufferSplit(m_imageSize.GetWidth(),
                    m_imageSize.GetHeight(), m_bits, m_endianness);

            wxASSERT(newBuf != 0L);

            newBuf->setImageSpec(m_imageSpec);
            newBuf->getImageVec();

            m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
        }
        break;
    case YUV_FILE_COMP:
        {
            YUV420ImageBufferFile* newBuf = new YUV420ImageBufferFile(m_imageSize.GetWidth(),
                    m_imageSize.GetHeight(), m_bits, m_endianness);

            wxASSERT(newBuf != 0L);

            newBuf->SetFilename(m_imageFilename);

            m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
        }
        break;
    case YUV_FILE_DUMP:
        {
            dumpImageBuffer* newBuf = new dumpImageBuffer(m_imageSize.GetWidth(), m_imageSize.GetHeight());

            wxASSERT(newBuf != nullptr);

            newBuf->setPath(m_imagePath);

            m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
        }
        break;

#ifdef  HAVE_LIBMJPEGTOOLS
    case YUV_FILE_Y4M:
        {
            YUV420ImageBufferY4M* newBuf = new YUV420ImageBufferY4M();
            wxASSERT(newBuf != nullptr);

            newBuf->SetFilename(m_imageFilename);

            m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
        }
        break;
#endif  // HAVE_LIBMJPEGTOOLS

    default:
        wxLogDebug("ERROR: Buffer type not recognized!");
        return false;
    }

    m_buffer->setYUVMask(&m_mask);

    wxASSERT( m_buffer != nullptr );

    m_lastFrame = m_buffer->GetFrameCount();

    wxLogDebug("m_buffer = 0x%p Type %s frameCount %ld",
               m_buffer, typeid(m_buffer).name(), m_lastFrame);

//     (%s) framecount = %ld", m_buffer,
//          (const char*)typeid(m_buffer).name(), m_lastFrame);


    if (m_buffer->lastError() != ImageBuffer::IB_ERROR_NO_ERROR) {
        wxLogDebug("ERROR: last error = %d", m_buffer->lastError());
        return false;
    }

    return (m_buffer != nullptr);
}

/**
 *
 */

void yuvImageControl::free_image_buffer() {
    wxLogDebug("yuvImageControl::free_image_buffer()");

    if (m_buffer) {
        delete m_buffer;
        m_buffer = 0L;
    }

    return;
}

/**
 *
 */

void yuvImageControl::OnSize(wxSizeEvent& event) {
#ifdef  __WXDEBUG__
    wxSize  newSize = event.GetSize();
    wxLogDebug("yuvImageControl::OnSize() %d x %d", newSize.GetWidth(), newSize.GetHeight());
#endif // __WXDEBUG__

    if (m_pImage) {
        set_image( m_pImage, m_scale );
    } else {
        SetVirtualSize( -1, -1 );
        Refresh(true);
    }
}

/**
 *
 */

void yuvImageControl::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc( this );

#if (wxDEBUG_LEVEL == 2)
    wxLogDebug("yuvImageControl::OnPaint()");
#endif

    PrepareDC( dc );

    wxSize		size;
    wxBitmap 	bmp;
    int			xoff = 0, yoff = 0;

    size = GetClientSize();

    if (m_pScaledImage) {
        bmp = wxBitmap( *m_pScaledImage );

        /* calculate offsets to center image in window */
        if (size.GetWidth() > bmp.GetWidth()) {
            xoff = (size.GetWidth() - bmp.GetWidth()) / 2;
        }

        if (size.GetHeight() > bmp.GetHeight()) {
            yoff = (size.GetHeight() - bmp.GetHeight()) / 2;
        }

        dc.DrawBitmap( bmp, xoff, yoff );

        if (m_bEnableGrid) {
            int inc;

            if (m_scale == 0.0) {
                inc = m_gridH;
            } else {
                inc = (int)(m_scale * m_gridH);
            }

            dc.SetPen(m_gridColor);
            for (int x = xoff ; x < (xoff + bmp.GetWidth()) ; x += inc) {
                dc.DrawLine(x, yoff, x, yoff + bmp.GetHeight());
//              wxLogDebug("x=%d", x);
            }

            if (m_scale == 0.0) {
                inc = m_gridV;
            } else {
                inc = (int)(m_scale * m_gridV);
            }

            for (int y = yoff ; y < (yoff + bmp.GetHeight()) ; y += inc) {
                dc.DrawLine(xoff, y, xoff + bmp.GetWidth(), y);
            }
        }
    } else {
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine( 0, 0, size.GetWidth(), size.GetHeight());
        dc.DrawLine( 0, size.GetHeight(), size.GetWidth(), 0);
    }
}

/**
 *
 */

bool yuvImageControl::GetImage() {
    wxLogDebug("GetImage()");

    bool    result = false;

    if (m_buffer == nullptr)
        return false;

    if (m_buffer->lastError() != ImageBuffer::IB_ERROR_NO_ERROR) {
        wxLogDebug("ERROR: m_buffer contains last error %d", m_buffer->lastError());
        return false;
    }

#ifdef  ENABLE_BUSY_CURSOR
    ::wxBeginBusyCursor();
#endif

    /* delete image if already loaded. */
    if (m_pImage) {
        delete m_pImage;
        m_pImage = nullptr;
    }

    m_buffer->type(m_yuvType);

#ifdef  ENABLE_PROFILE_LOAD
    wxStopWatch swTimer;
#endif

    wxLogDebug("-- loading frame # %ld", m_curFrame);

    if (m_buffer->Load(m_curFrame)) {
#ifdef  ENABLE_PROFILE_LOAD
        swTimer.Pause();
        wxLogDebug("PROFILE : load time %ld", swTimer.Time());
#endif

        /* create an empty image. */
        m_pImage = new wxImage(m_imageSize.GetWidth(), m_imageSize.GetHeight());
        wxASSERT(m_pImage != nullptr);
        m_buffer->GetImage(m_pImage);
        set_image( m_pImage, m_scale );
        result = true;
    } else {

#ifdef  ENABLE_PROFILE_LOAD
        swTimer.Pause();
        wxLogDebug("PROFILE : load failed time %ld", swTimer.Time());
#endif

        wxMessageBox(wxT("Unable to load image!\n"
                         "Read past end of file..."), wxT("frameInspector"));
        set_image( (wxImage*)nullptr );
    }

#ifdef	ENABLE_BUSY_CURSOR
    ::wxEndBusyCursor();
#endif

    return result;
}

/**
 *
 */

bool yuvImageControl::set_image(wxImage* pImage, long scaling) {
    wxSize			size = GetClientSize();
    double          scaleFactor = 0;

    wxLogDebug("yuvImageControl::set_image(%p, %ld)", pImage, scaling);
    wxLogDebug("client size = %d x %d", size.GetWidth(), size.GetHeight());

    /* make sure the image pointer is set */
    if (pImage == 0L) {
        wxLogDebug("ERROR: set_image called with NULL pointer!");

        delete m_pImage;
        delete m_pScaledImage;

        m_pImage       = 0L;
        m_pScaledImage = 0L;

        return false;
    }

    /* if image exists, delete it... */
    if (m_pScaledImage) {
        delete m_pScaledImage;
        m_pScaledImage = 0L;
    }

    double	scaleVector[] = {
        1.0,	/* IS_IMAGE_DEFAULT */
        0.0,	/* IS_IMAGE_SCALE   */
        0.5,	/* IS_IMAGE_HALF    */
        2.0,	/* IS_IMAGE_DOUBLE  */
    };

    scaleFactor = scaleVector[scaling];

    if (scaleFactor == 0.0) {
        int image_w 	= pImage->GetWidth(),
               image_h 	= pImage->GetHeight();
        int window_w	= size.GetWidth();
        int window_h 	= size.GetHeight();
        int scale_w, scale_h;

        scale_w = (int)(((double)window_h / (double)image_h) * (double)image_w);
        scale_h = (int)(((double)window_w / (double)image_w) * (double)image_h);

        if (scale_w > window_w) {
            scale_w = window_w;
        }
        if (scale_h > window_h) {
            scale_h = window_h;
        }

        m_pScaledImage = new wxImage(pImage->Scale(scale_w, scale_h));
    } else {
        int image_w 	= (int)((double)pImage->GetWidth() * scaleFactor),
               image_h 	= (int)((double)pImage->GetHeight() * scaleFactor);

        m_pScaledImage = new wxImage(pImage->Scale(image_w, image_h));
    }

    wxLogDebug("-- calling SetVirtualSize(%d, %d)", m_pScaledImage->GetWidth(), m_pScaledImage->GetHeight());
    SetVirtualSize( m_pScaledImage->GetWidth(), m_pScaledImage->GetHeight() );

#ifdef  __WXDEBUG__
    wxSize	wndSize = GetClientSize();
    wxLogDebug("Scroller Size = %d x %d", wndSize.GetWidth(), wndSize.GetHeight());
#endif // __WXDEBUG__

    Refresh(/* false */);

    return true;
}

/**
 *
 */

bool yuvImageControl::RefreshImage() {
    bool bRes = false;

    wxLogDebug("yuvImageControl::RefreshImage()");

    if (allocate_image_buffer()) {
        if (GetImage()) {
            Refresh(false);
            bRes = true;
            m_bValid = true;
        } else {
            wxLogDebug("ERROR: GetImage() returned false!");
        }
    } else {
        wxLogDebug("ERROR: allocate_image_buffer() returned false!");
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::OpenYUVSplit(const wxString& sPath,
                                   const wxString& sPrefix,
                                   const wxSize& imgSize,
                                   const wxUint16 bits,
                                   dataType type,
                                   formatEndian endianness)
{
    wxLogDebug("yuvImageControl::OpenYUVSplit(%s, %s, wxSize(%d, %d), %d, %s)",
               sPath, sPrefix,
               imgSize.GetWidth(), imgSize.GetHeight(),
               bits, (endianness == endian_little)?"le":"be");

    m_imageFilename.clear();
    m_imagePath     = sPath;
    m_prefix        = sPrefix;
    m_bufType       = YUV_FILE_SPLIT;
    m_yuvType       = type;
    m_curFrame      = 0;
    m_imageSize     = imgSize;
    m_bits          = bits;
    m_endianness    = endianness;

    RefreshImage();

    return m_bValid;
}
/**
 *
 */

bool yuvImageControl::OpenYUVSplit(const YUV420ImageSpec& spec,
                                   const wxSize& imgSize,
                                   const wxUint16 bits,
                                   dataType type,
                                   formatEndian endianness)
{
    wxLogDebug("yuvImageControl::OpenYUVSplit(%s, %s, wxSize(%d, %d), %d, %s)",
               spec.m_imagePath, spec.m_prefix,
               imgSize.GetWidth(), imgSize.GetHeight(),
               bits, (endianness == endian_little)?"le":"be");

    m_imageFilename.clear();
    m_imagePath     = spec.m_imagePath; //sPath;
    m_prefix        = spec.m_prefix;
    m_bufType       = YUV_FILE_SPLIT;
    m_yuvType       = type;
    m_curFrame      = 0;
    m_imageSize     = imgSize;
    m_bits          = bits;
    m_endianness    = endianness;
    m_imageSpec     = spec;

    RefreshImage();

    return m_bValid;
}

/**
 *
 */

bool yuvImageControl::OpenYUVComposite(const wxString& sYUVCompFile,
                                       const wxSize& imgSize,
                                       const wxUint16 bits,
                                       dataType type,
                                       formatEndian endianness)
{
    wxFileName  fname(sYUVCompFile);

    wxLogDebug("yuvImageControl::OpenYUVComposite(%s, %d, %d, %d)",
               sYUVCompFile, imgSize.GetWidth(), imgSize.GetHeight(), bits);

    m_prefix.clear();
    m_imagePath     = fname.GetPath();
    m_imageFilename = sYUVCompFile;
    m_bufType       = YUV_FILE_COMP;
    m_yuvType       = type;
    m_curFrame      = 0;
    m_imageSize     = imgSize;
    m_bits          = bits;
    m_endianness    = endianness;

    RefreshImage();

    return m_bValid;
}

/**
 *
 */

#ifdef HAVE_LIBMJPEGTOOLS

bool yuvImageControl::OpenYUVY4M(const wxString& sYUVY4MFile)
{
    wxFileName  fname(sYUVY4MFile);

    wxLogDebug("yuvImageControl::OpenYUVY4M(%s)", sYUVY4MFile);

    m_prefix.clear();
    m_imagePath     = fname.GetPath();
    m_imageFilename = sYUVY4MFile;
    m_bufType       = YUV_FILE_Y4M;

    RefreshImage();

    return m_bValid;
}

#endif


/**
 *
 */

bool yuvImageControl::OpenYUVDump(const wxString& sPath) {
    wxLogDebug("yuvImageControl::OpenYUVDump(%s)", (const char*)sPath.c_str());

    m_prefix.clear();
    m_imageFilename.clear();
    m_imagePath     = sPath;
    m_bufType       = YUV_FILE_DUMP;
    m_curFrame      = 0;
    m_yuvType       = DATA_YUV420;

    RefreshImage();

    if (m_bValid) {
        int w, h;

        m_buffer->QueryFrameSize(w, h);
        m_imageSize = wxSize(w,h);
    }

    return m_bValid;
}

/**
 *
 */

void yuvImageControl::CloseImage() {
    wxLogDebug("yuvImageControl::Close()");

    /* delete the image */
    if (m_pImage) {
        delete m_pImage;
        m_pImage = 0L;
    }
    if (m_pScaledImage) {
        delete m_pScaledImage;
        m_pScaledImage = 0L;
    }
    if (m_buffer) {
        delete m_buffer;
        m_buffer = 0L;
    }

    m_yuvType   = DATA_UNKNOWN;
    m_bufType   = YUV_FILE_UNKNOWN;
    m_bValid 	= false;

    SetVirtualSize( -1, -1 );
    Refresh(true);

}

/**
 *
 */

bool yuvImageControl::NextFrame() {
    bool bRes = false;

    wxLogDebug("yuvImageControl::NextFrame()");

    if (m_curFrame < m_lastFrame) {
        m_curFrame++;
        GetImage();
        Refresh();
        bRes = true;
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::PrevFrame() {
    bool bRes = false;

    wxLogDebug("yuvImageControl::PrevFrame()");

    if (m_curFrame > 0) {
        m_curFrame--;
        GetImage();
        Refresh();
        bRes = true;
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::GotoFrame(size_t frameNo) {
    bool bRes = false;

    wxLogDebug("yuvImageControl::GotoFrame(%ld)", frameNo);

    if ((frameNo >= 0) && (frameNo <= m_lastFrame)) {
        m_curFrame = frameNo;
        GetImage();
        Refresh();
        bRes = true;
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::SaveBitmap(wxString sFilename) {
    bool bRes = false;

    wxLogDebug("yuvImageControl::SaveBitmap(%s)", (const char*)sFilename.c_str());

    if (m_bEnableGrid) {

        wxImage     imgDest;
        wxBitmap*   pBm = 0;
        wxMemoryDC  pDc;
        int         width, height;

        width = m_pImage->GetWidth();
        height = m_pImage->GetHeight();

        pBm = new wxBitmap(*m_pImage);
        pDc.SelectObject(*pBm);

        pDc.SetPen(m_gridColor);

        for (int x = 0 ; x < width; x += 16) {
            pDc.DrawLine(x, 0, x, height);
        }
        for (int y = 0 ; y < height ; y += 16) {
            pDc.DrawLine(0, y, width, y);
        }

        imgDest = pBm->ConvertToImage();

        if (imgDest.SaveFile(sFilename)) {
            bRes = true;
        } else {
            wxLogDebug("ERROR: Unable to save");
        }

        delete pBm;
    } else {
        bRes = m_pImage->SaveFile(sFilename);
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::GetState(controlParms& parms) {
    wxLogDebug("yuvImageControl::GetState(...)");

    parms.m_bufType       = m_bufType;
    parms.m_yuvType       = m_yuvType;
    parms.m_bits          = m_bits;
    parms.m_scale         = m_scale;
    parms.m_imageSize     = m_imageSize;
    parms.m_curFrame      = m_curFrame;
    parms.m_bEnableGrid   = m_bEnableGrid;
    parms.m_gridH         = m_gridH;
    parms.m_gridV         = m_gridV;
    parms.m_gridColor     = m_gridColor;
    parms.m_imagePath     = m_imagePath;
    parms.m_imageFilename = m_imageFilename;
    parms.m_prefix        = m_prefix;
    parms.m_endianness    = m_endianness;

    return true;
}

/**
 *
 */

bool yuvImageControl::SetState(controlParms& parms) {
    bool bRes = false;

    wxLogDebug("yuvImageControl::SetState(...)");

    m_bufType       = parms.m_bufType;
    m_yuvType       = parms.m_yuvType;
    m_bits          = parms.m_bits;
    m_scale         = parms.m_scale;
    m_imageSize     = parms.m_imageSize;
    m_curFrame      = parms.m_curFrame;
    m_bEnableGrid   = parms.m_bEnableGrid;
    m_gridH         = parms.m_gridH;
    m_gridV         = parms.m_gridV;
    m_gridColor     = parms.m_gridColor;
    m_imagePath     = parms.m_imagePath;
    m_imageFilename = parms.m_imageFilename;
    m_prefix        = parms.m_prefix;
    m_endianness    = parms.m_endianness;
    m_imageSpec     = parms.m_spec;

    bRes = RefreshImage();

    return bRes;
}

/**
 *
 */

bool yuvImageControl::SaveState(wxConfig* pConfig, wxString name) {
    wxString sOldPath, sPath;
    wxString sTmp;

    wxLogDebug("yuvImageControl::SaveState(%p, %s)", pConfig, name);

    wxASSERT( pConfig != 0L );

    if (pConfig == 0) {
        wxLogDebug("ERROR: Called SaveState with a NULL wxConfig!");
        return false;
    }

    sOldPath = pConfig->GetPath();
    sPath = wxString::Format("/control/%s", name);

    pConfig->SetPath(sPath);

    pConfig->Write("Type",      (int)m_bufType);
    pConfig->Write("YUVfmt",    (int)m_yuvType);
    pConfig->Write("Bits",      (int)m_bits);
    sTmp = (m_endianness == endian_little)?"le":(m_endianness == endian_big)?"be":"??";
    pConfig->Write("Endian",    sTmp);
    pConfig->Write("Scale",     (int)m_scale);
    pConfig->Write("Width",     m_imageSize.GetWidth());
    pConfig->Write("Height",    m_imageSize.GetHeight());
    pConfig->Write("CurFrame",  m_curFrame);
    pConfig->Write("Path",      m_imagePath);
    pConfig->Write("File",      m_imageFilename);
    pConfig->Write("Prefix",    m_prefix);
    pConfig->Write("SplitPath", m_imageSpec.m_imagePath);
    pConfig->Write("SplitPrefix", m_imageSpec.m_prefix);
    pConfig->Write("SplitFirst", m_imageSpec.m_first);
    pConfig->Write("SplitLast", m_imageSpec.m_last);
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

/**
 *
 */

bool yuvImageControl::LoadState(wxConfig* pConfig, wxString name) {
    wxString    sOldPath, sPath;
    long        nTmp, w, h;
    wxString    sTmp;

    wxLogDebug("yuvImageControl::LoadState(%p, %s)",
               pConfig, (const char*)name.c_str());

    wxASSERT( pConfig != 0L );

    if (pConfig == 0) {
        wxLogDebug("ERROR: Called SaveState with a NULL wxConfig!");
        return false;
    }

    sOldPath = pConfig->GetPath();
    sPath = wxString::Format("/control/%s", name);

    pConfig->SetPath(sPath);

    if (pConfig->Read("Type", &nTmp)) {
        m_bufType = (yuvBufType)nTmp;
    }

    if (pConfig->Read("YUVfmt", &nTmp)) {
        m_yuvType = (dataType)nTmp;
    }

    if (pConfig->Read("Bits", &nTmp)) {
        m_bits = (wxUint16)nTmp;
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

    pConfig->SetPath(sOldPath);

    RefreshImage();

    return true;
}

/**
 *
 */

wxString yuvImageControl::GetIdentifier() {
    wxString    sMsg;
    wxFileName  fname(m_imageFilename);
    if (IsValid()) {
        if (m_bufType == YUV_FILE_SPLIT) {
//            wxString    splitName = utils::getFrameFilename( m_imagePath, m_prefix, m_curFrame );
            wxString splitName = dynamic_cast<YUV420ImageBufferSplit*>(m_buffer)->GetFileBase(m_curFrame);
            sMsg = wxString::Format(wxT("Split : %s"), splitName);
        } else if (m_bufType == YUV_FILE_COMP) {
            sMsg = wxString::Format(wxT("File : %s"), fname.GetFullName());
        } else if (m_bufType == YUV_FILE_DUMP) {
            sMsg = wxString::Format(wxT("Dump : %s"), m_imagePath);
        } else if (m_bufType == YUV_FILE_Y4M) {
            sMsg = wxString::Format(wxT("Y4MPEG : %s"), fname.GetFullName());
        }
    } else {
        sMsg = "N/A";
    }

    return sMsg;
}

/**
 *
 */

bool yuvImageControl::CanSetSize() {
    bool bRes = false;

    wxLogDebug("yuvImageControl::CanSetSize()");

    if (m_bValid) {
        bRes = !m_buffer->CanQueryFrameSize();
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::SaveYUVData(const wxString& sFilename, ImageBuffer::eSaveType type) {
    bool bRes = false;

    wxLogDebug("yuvImageControl::SaveYUVData(%s, %d)", sFilename, (int)type);

    if (m_bValid) {
        bRes = m_buffer->SaveYUV( sFilename, type );
    }

    return bRes;
}

/**
 *
 */

bool yuvImageControl::GetChecksum(checksumAlgoBase* pAlgo, checksumEntry& entry)
{
    bool    bRes = false;

    wxLogDebug("yuvImageControl::GetChecksum(0x%p, ...)", pAlgo);

    if (m_bValid) {
        wxASSERT( m_buffer != 0L );
        bRes = m_buffer->GetChecksum( pAlgo, entry );
    }

    return bRes;
}

/**
 *  Set YUV buffer type
 */

void yuvImageControl::SetYUVFormat(dataType type,
                                   int bits,
                                   formatEndian endian)
{
    wxLogDebug("yuvImageControl::SetYUVFormat(%d, %d, %s)", (int)type, bits,
               (endian == endian_little)?"le":"be");

    m_yuvType       = type;
    m_bits          = bits;
    m_endianness    = endian;

    if (m_buffer != 0L) {
        m_buffer->type( m_yuvType );
        m_buffer->bits( m_bits );
        m_buffer->endian( m_endianness );
    }

    return;
}

wxString yuvImageControl::GetYUVFormatString() {
    wxString sFormat;

    wxASSERT(m_buffer != nullptr);

    if (m_buffer != 0L) {
        sFormat = m_buffer->YUVTypeToString( m_yuvType );
    }

    return sFormat;
}

bool yuvImageControl::setYUVMask(yuvMask mask) {
    m_mask = mask;
    return true;
}

bool yuvImageControl::getYUVMask(yuvMask& mask) {
    mask = m_mask;
    return true;
}


#if 0

    if (m_bufType == YUV_FILE_SPLIT) {
        YUV420ImageBufferSplit*	newBuf = new YUV420ImageBufferSplit(m_imageSize.GetWidth(),
                m_imageSize.GetHeight(), m_bits, m_endianness);

        wxASSERT(newBuf != 0L);

//        newBuf->setPrefix(m_prefix);
//        newBuf->setPath(m_imagePath);
        newBuf->setImageSpec(m_imageSpec);
        newBuf->getImageVec();

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
    } else if (m_bufType == YUV_FILE_COMP) {
        YUV420ImageBufferFile* newBuf = new YUV420ImageBufferFile(m_imageSize.GetWidth(),
                m_imageSize.GetHeight(), m_bits, m_endianness);
        wxASSERT(newBuf != 0L);

        newBuf->SetFilename(m_imageFilename);

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
    } else if (m_bufType == YUV_FILE_DUMP) {
        dumpImageBuffer* newBuf = new dumpImageBuffer(m_imageSize.GetWidth(), m_imageSize.GetHeight());

        wxASSERT(newBuf != 0L);

        newBuf->setPath(m_imagePath);

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);
    } else {
        wxLogDebug("ERROR: Buffer type not specified!");
        return false;
    }
#endif