//==============================================================================
//	MODULE		:	imageBuffer.h
//	PROJECT		:	PlayVideoBrowser
//	PROGRAMMER	:	Michael A. Uman
//	DATE		:	May 5, 2006
//	COPYRIGHT	:	(C) 2006 Sigma Designs
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

#if 1
#include "types.h"
#else
/// Image data representation
enum dataType {
    DATA_UNKNOWN,   //!< Data format unknown
    DATA_YUV420,    //!< Data in YUV420 format
    DATA_YUV422,    //!< Data in YUV422 format
    DATA_YUV444,    //!< Data in YUV444 format
    DATA_NV12,      //!< Data in NV12 format
    DATA_NV21,      //!< Data in NV21 format
    DATA_RGB32,     //!< Data in RGB32 format
};

enum yuvBufType {
    YUV_FILE_UNKNOWN,
    YUV_FILE_SPLIT,
    YUV_FILE_COMP,
    YUV_FILE_DUMP,
};
#endif

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
    formatEndian     m_endianness;
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


/// Class which encapsulates a buffer of pixels.
class ImageBuffer {
    public:


        enum eError {
            IB_ERROR_NO_ERROR,
            IB_ERROR_OUT_OF_BOUNDS,
            IB_ERROR_FILE_NOT_FOUND,
            IB_ERROR_NOT_OPEN,
        };

        enum eSaveType {
            SAVE_YUV_SPLIT,
            SAVE_YUV_COMP,
        };

        ImageBuffer();
        /// Initialize an ImageBuffer with type and size.
        ImageBuffer(dataType type, int width, int height, int bits = 8,
                    formatEndian endianness = endian_little);

        virtual ~ImageBuffer(); // {}

        dataType	type() const 	{ return m_bufType; }
        int			width() const 	{ return m_width; }
        int			height() const 	{ return m_height; }
        void*		data() const 	{ return m_imageData; }
        int         bits() const    { return m_bits; }
        formatEndian endian() const { return m_endianness; }

        eError      lastError() const { return m_lastError; }

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

        //virtual bool        GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum);
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
            UBYTE*	newData = 0L;
            size_t	bufSize = (sizeof(PIXEL) * m_width * m_height);

            wxASSERT(m_imageData != 0L);

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
        formatEndian m_endianness;
        UBYTE*				m_imageData;	//!< Pointer to RGB data
        bool				m_ccir601;      //!< Enable CCIR 601
        eError      		m_lastError;    //!< Last error code

        yuvBuffer   		m_yuv;          //!< Buffer stores YUV data.
        yuvMask*            m_pMask;         //!< YUV Mask
};

#endif	// __IMAGEBUFFER_H__


