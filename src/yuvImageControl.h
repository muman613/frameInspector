#ifndef __YUVIMAGECONTROL_H__
#define __YUVIMAGECONTROL_H__

#include <wx/wx.h>
#include <wx/scrolwin.h>

#include "imageBuffer.h"
#include "checksumObjects.h"
//#include "controlParms.h"

class wxConfig;
class controlParms;

enum scaleVal {
    IS_IMAGE_DEFAULT,
    IS_IMAGE_SCALE,
    IS_IMAGE_HALF,
    IS_IMAGE_DOUBLE,
};

struct yuvMask {
    bool        bMaskY:1;
    bool        bMaskU:1;
    bool        bMaskV:1;
};


/**
 *  Image control class
 */

class yuvImageControl : public wxScrolledWindow {
public:
    /** Default constructor */
    yuvImageControl() {}
    yuvImageControl(wxWindow* parent,
                    wxWindowID id = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);
    /** Default destructor */
    virtual ~yuvImageControl();

    bool                    IsValid() const;

    void                    SetImageScale(scaleVal imageScale);
    scaleVal                GetImageScale() const;
    void                    SetImageSize(const wxSize& size);
    wxSize                  GetImageSize() const;
    size_t                  GetImageCount() const;
    wxString                GetImagePath() const;
    wxString                GetImagePrefix() const;
    size_t                  GetCurrentFrame() const;

    void                    SetYUVFormat(dataType type,
                                         int bits = 8,
                                         formatEndian endian = endian_little);
    dataType                GetYUVFormat() const {
        return m_yuvType;
    }
    wxString                GetYUVFormatString();
    yuvBufType              GetBufferType() const {
        return m_bufType;
    }
//        wxString                GetImagePath() const;
    wxString                GetImageName() const;
    wxUint16                GetImageBits() const;
    formatEndian     GetEndianness() const;

    bool                    OpenYUVSplit(const wxString& sPath,
                                         const wxString& sPrefix,
                                         const wxSize& size,
                                         const wxUint16 bits,
                                         dataType type = DATA_YUV420,
                                         formatEndian endianness = endian_little);
    bool                    OpenYUVSplit(const YUV420ImageSpec& spec,
                                         const wxSize& imgSize,
                                         const wxUint16 bits,
                                         dataType type,
                                         formatEndian endianness);
    bool                    OpenYUVComposite(const wxString& sYUVCompFile,
                                        const wxSize& size,
                                        const wxUint16 bits,
                                        dataType type = DATA_YUV420,
                                        formatEndian endianness = endian_little);
#ifdef HAVE_LIBMJPEGTOOLS
    bool                    OpenYUVY4M(const wxString& sYUVY4MFile);
#endif

    bool                    OpenYUVDump(const wxString& sPath);
    void                    CloseImage();

    bool                    SaveBitmap(wxString sFilename);

    void                    enable_grid();
    void                    disable_grid();
    bool                    get_grid_state();
    void                    set_grid_dimensions(int hor,
            int ver,
            const wxColor& color = *wxBLACK);

    void                    get_grid_settings(int& hor,
            int& ver,
            wxColor& color,
            bool& state);

    void                    OnPaint(wxPaintEvent& event);
    void                    OnSize(wxSizeEvent& event);

    bool                    NextFrame();
    bool                    PrevFrame();
    bool                    GotoFrame(size_t index);

    bool                    RefreshImage();

    bool                    SaveState(wxConfig* pConfig, wxString name);
    bool                    LoadState(wxConfig* pConfig, wxString name);

    bool                    GetState(controlParms& parms);
    bool                    SetState(controlParms& parms);

    wxString                GetIdentifier();

    bool                    CanSetSize();

    bool                    SaveYUVData(const wxString& sFilename, ImageBuffer::eSaveType type);
    bool                    GetChecksum(checksumAlgoBase* pAlgo, checksumEntry& entry);

    bool                    setYUVMask(yuvMask mask);
    bool                    getYUVMask(yuvMask& mask);

protected:
    bool                    GetImage();
    bool                    DrawGrid(wxImage* pImage);

    bool                    allocate_image_buffer();
    void                    free_image_buffer();
    bool                    set_image(wxImage* pImage, long scale = IS_IMAGE_DEFAULT);

    bool                    m_bValid;

    wxImage*	            m_pImage;
    wxImage*                m_pScaledImage;
    ImageBuffer*            m_buffer;

//#if 1
    //    controlParms            m_parms;
//#else
    bool                    m_bEnableGrid;
    wxColor                 m_gridColor;
    int                     m_gridH;
    int                     m_gridV;
    wxSize                  m_imageSize;        ///< Size of image
    int			            m_flags;
    scaleVal	            m_scale;
    dataType                m_yuvType;
    yuvBufType              m_bufType;
    wxInt32                 m_bits;             ///< 8 or 10 bits per component
    formatEndian            m_endianness;       ///< Endianness for 10bit
    wxString                m_imagePath;        ///< Current image path
    wxString                m_imageFilename;    ///< Current image filename
    wxString                m_prefix;           ///< Name prefix {i.e. out}
//#endif

    YUV420ImageSpec         m_imageSpec;

    size_t                  m_lastFrame;
    size_t                  m_curFrame;
    yuvMask                 m_mask;


private:
    DECLARE_DYNAMIC_CLASS(yuvImageControl)
    DECLARE_EVENT_TABLE()
};

#endif // __YUVIMAGECONTROL_H__
