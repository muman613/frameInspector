//==============================================================================
//	MODULE		:	imageBuffer.h
//	PROJECT		:	frameInspector
//	PROGRAMMER	:	Michael A. Uman
//	DATE		:	May 5, 2006
//==============================================================================

#ifndef __IMAGEBUFFER_H__
#define __IMAGEBUFFER_H__

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/file.h>
#include "types.h"
#include "checksumObjects.h"
#include "misc_utils.h"

#define limit(x) (((x>0xffffff)?0xff0000:((x<=0xffff)?0:x&0xff0000))>>16)
#define PPM_ASSIGN(p,red,grn,blu) do { (p).r = (red); (p).g = (grn); (p).b = (blu); } while ( 0 )

struct yuvMask;

#include "types.h"


class ImageBuffer;

/// Buffer encapsulates a YUV frame
class yuvBuffer {
public:
    yuvBuffer();
    virtual ~yuvBuffer();

    wxUint32                m_yuvWidth;
    wxUint32                m_yuvHeight;
    wxUint32                m_lumaSize;
    wxUint32                m_chromaSize;
    wxUint32                m_bits;
    formatEndian            m_endianness;
    wxUint8*                m_pY;
    wxUint8*                m_pU;
    wxUint8*                m_pV;

    wxUint8*        DoConversionToRGB(dataType bufType = DATA_YUV420, bool ccir601 = false);
    void            alloc_buffer(wxUint32 width, wxUint32 height,
                                 wxUint32 bits,
                                 bool bClear = false,
                                 formatEndian endian = endian_little);
    void            free_buffer();
};

/**
 * Class used to pass parameters to the ImageBuffer class.
 */

class ImageBufferParms {
public:
    ImageBufferParms();
    ImageBufferParms(dataType type, int width, int height,
                     int bits = 8, formatEndian endian = endian_little)
    {
        m_dataType = type;
        m_width    = width;
        m_height   = height;
        m_bits     = bits;
        m_endian   = endian;
    }

    dataType        m_dataType          = DATA_UNKNOWN;
    int             m_width             = 320;
    int             m_height            = 240;
    int             m_bits              = 8;
    formatEndian    m_endian            = endian_little;

    wxString        m_sFileName;
    wxString        m_sSplitPath;
    wxString        m_sFilePattern;

    int             m_first             = 0;
    int             m_last              = 0;
};


typedef ImageBuffer* (*IBCreateFunc)(ImageBufferParms& parms);


template<typename T> ImageBuffer* Create(ImageBufferParms& parms) {
    return dynamic_cast<ImageBuffer*>(new T(parms));
}

/// Class which encapsulates a buffer of pixels.
class ImageBuffer {
public:


    enum eError {
        IB_ERROR_NO_ERROR,
        IB_ERROR_OUT_OF_BOUNDS,
        IB_ERROR_FILE_NOT_FOUND,
        IB_ERROR_NOT_OPEN,
        IB_ERROR_SYSTEM,
    };

    enum eSaveType {
        SAVE_YUV_SPLIT,
        SAVE_YUV_COMP,
        SAVE_YUV_YUV4MPEG
    };

    ImageBuffer();
    /// Initialize an ImageBuffer with type and size.
    ImageBuffer(dataType type, int width, int height, int bits = 8,
                formatEndian endianness = endian_little);
    ImageBuffer(ImageBufferParms& parms);

    virtual ~ImageBuffer(); // {}

    dataType	type() const 	{
        return m_bufType;
    }
    int			width() const 	{
        return m_width;
    }
    int			height() const 	{
        return m_height;
    }
    void*		data() const 	{
        return m_imageData;
    }
    int         bits() const    {
        return m_bits;
    }
    formatEndian endian() const {
        return m_endianness;
    }

    eError      lastError() const {
        return m_lastError;
    }

    dataType	type(const dataType type) {
        m_bufType = type;
        return m_bufType;
    }

    int         bits(const int bits) {
        m_bits = bits;
        return m_bits;
    }

    formatEndian endian(formatEndian endianness) {
        m_endianness = endianness;
        return m_endianness;
    }

    virtual bool        CanQueryFrameSize() const {
        return false;
    }

    virtual bool        QueryFrameSize(int& width, int& height) {
        width = height = -1;
        return false;
    }

    virtual bool        CanChecksum() const {
        return false;
    }
    virtual bool        CanSave() const {
        return false;
    }

    bool                GetChecksum(checksumAlgoBase* pAlgo, checksumEntry& entry);

    virtual bool        SaveYUV(wxString sFilename, eSaveType type);

    /* Pure virtual functions must be implemented in derived classes */

    virtual ssize_t     GetFrameCount()				= 0;
    virtual void        GetImage(wxImage* pImage) 	= 0;
    virtual bool        Load(size_t frame) 			= 0;
    virtual PIXEL*      getPixel(int x, int y)		= 0;

    void                enable_ccir601(bool bEnable = true) {
        m_ccir601 = bEnable;
    }

    bool                SaveCurrentFrame(const wxString& sFilename);

    static wxString     YUVTypeToString(dataType type);
    static wxString     BufTypeToString(yuvBufType type);

//        void                setmaskYUV(bool ymask, bool umask, bool vmask);
//        void                setmaskYUV(const yuvMask& mask);
//        void                getmaskYUV(yuvMask& mask);

    void                setYUVMask(yuvMask* pMask);

protected:
    /// Make a copy of the image data
    UBYTE*		copy_data() {
        UBYTE*	newData = nullptr;
        size_t	bufSize = (sizeof(PIXEL) * m_width * m_height);

        wxASSERT(m_imageData != nullptr);

        if (bufSize != 0) {
            newData = (UBYTE*)malloc(bufSize);
            if (newData) {
                memcpy(newData, m_imageData, bufSize);
            }
        }

        return newData;
    }

    inline void free_buffer() {
        wxLogDebug("ImageBuffer::free_buffer() [buffer @ %p]", m_imageData);

        if (m_imageData) {
            free(m_imageData);
            m_imageData = 0;
        }

        return;
    }

    dataType			m_bufType;		//!< Type of buffer
    int					m_width;		//!< Width of buffer
    int					m_height;		//!< Height of buffer
    int         		m_bits;         //!< 8 or 10 bits per component
    formatEndian 		m_endianness;
    UBYTE*				m_imageData;	//!< Pointer to RGB data
    bool				m_ccir601;      //!< Enable CCIR 601
    eError      		m_lastError;    //!< Last error code

    yuvBuffer   		m_yuv;          //!< Buffer stores YUV data.
    yuvMask*            m_pMask;         //!< YUV Mask
};

#endif	// __IMAGEBUFFER_H__


