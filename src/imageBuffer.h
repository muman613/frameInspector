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
#include "dbgutils.h"


#define limit(x) (((x>0xffffff)?0xff0000:((x<=0xffff)?0:x&0xff0000))>>16)
#define PPM_ASSIGN(p,red,grn,blu) do { (p).r = (red); (p).g = (grn); (p).b = (blu); } while ( 0 )



/// Class which encapsulates a buffer of pixels.
class ImageBuffer {
	public:
		/// Image data representation
		enum dataType {
			DATA_UNKNOWN,	//!< Data format unknown
			DATA_YUV420,	//!< Data in YUV420 format
			DATA_YUV422, 	//!< Data in YUV422 format
			DATA_YUV444,    //!< Data in YUV444 format
			DATA_RGB32,		//!< Data in RGB32 format
		};

        enum eError {
            ERROR_NO_ERROR,
            ERROR_OUT_OF_BOUNDS,
            ERROR_FILE_NOT_FOUND,
            ERROR_NOT_OPEN,
        };

		ImageBuffer()
		:	m_bufType(DATA_UNKNOWN),
			m_width(0L),
			m_height(0L),
			m_imageData(0L),
			m_ccir601(false),
			m_lastError(ERROR_NO_ERROR)
		{
		}

		/// Initialize an ImageBuffer with type and size.
		ImageBuffer(dataType type, int width, int height)
		:	m_bufType(type),
			m_width(width),
			m_height(height),
			m_imageData(0L),
			m_ccir601(false),
			m_lastError(ERROR_NO_ERROR)
		{
		}

		virtual ~ImageBuffer() {}

		dataType	type() const 	{ return m_bufType; }
		int			width() const 	{ return m_width; }
		int			height() const 	{ return m_height; }
		void*		data() const 	{ return m_imageData; }
        eError      lastError() const { return m_lastError; }

		dataType	type(const dataType& type) {
			m_bufType = type;
			return m_bufType;
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

        virtual bool        GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum) {
            debug("ImageBuffer::GetChecksum(%ld, %08x, %08x)\n", frame, lumaSum, chromaSum);

            if (chromaSum)
                *chromaSum = 0;

            if (lumaSum)
                *lumaSum = 0;

            return false;
        }

        /* Pure virtual functions must be implemented in derived classes */

		virtual ssize_t		GetFrameCount()				= 0;
		virtual void		GetImage(wxImage* pImage) 	= 0;
		virtual bool		Load(size_t frame) 			= 0;
		virtual PIXEL*		getPixel(int x, int y)		= 0;

        void                enable_ccir601(bool bEnable = true) {
            m_ccir601 = bEnable;
        }

	protected:
		UBYTE*		pm_allocrow(int const cols, int const size) {
			return (UBYTE*)malloc( cols * size );
		}

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
			debug("ImageBuffer::free_buffer()\n");

			if (m_imageData) {
				free(m_imageData);
				m_imageData = 0;
			}

			return;
		}

		dataType	m_bufType;		//!< Type of buffer
		int			m_width;		//!< Width of buffer
		int			m_height;		//!< Height of buffer
		UBYTE*		m_imageData;	//!< Pointer to RGB data
		bool		m_ccir601;      //!< Enable CCIR 601
		eError      m_lastError;    //!< Last error code
};

typedef ImageBuffer::dataType	BUFFER_TYPE;



#endif	// __IMAGEBUFFER_H__


