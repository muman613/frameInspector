//==============================================================================
//	MODULE		:	YUVImageBufferFile.cpp
//	PROJECT		:	PlayVideoBrowser
//	PROGRAMMER	:	Michael A. Uman
//	DATE		:	May 25, 2010
//	COPYRIGHT	:	(C) 2006-2010 Sigma Designs
//==============================================================================

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef HAS_GCRYPT
#include <gcrypt.h>
#endif
#include "imageBuffer.h"
#include "YUV420ImageBufferY4M.h"
#include "misc_utils.h"
#include "yuvImageControl.h"

/**
 *
 */

YUV420ImageBufferY4M::YUV420ImageBufferY4M()
:   ImageBuffer(DATA_YUV420, 0, 0)
{
	wxLogDebug("YUV420ImageBufferY4M::YUV420ImageBufferY4M()");

    y4m_init_stream_info(&m_strInfo);
    y4m_init_frame_info(&m_frmInfo);
}

/**
 *
 */

YUV420ImageBufferY4M::~YUV420ImageBufferY4M()
{
	wxLogDebug("YUV420ImageBufferY4M::~YUV420ImageBufferY4M()");

	m_file.Close();

    y4m_fini_frame_info(&m_frmInfo);
    y4m_fini_stream_info(&m_strInfo);

    free_buffer();
    m_yuv.free_buffer();
}

/**
 *  Open the file specified.
 */

bool YUV420ImageBufferY4M::SetFilename(const wxString& filename)
{
	bool	result = false;

	wxLogDebug("YUV420ImageBufferY4M::SetFilename(%s)", (const char*)filename.c_str());

	m_file.Close();

	if (m_file.Exists(filename.c_str()) && m_file.Open(filename.c_str())) {
		wxLogDebug("-- file opened!");
		m_imageFilename = filename;

        if (y4m_read_stream_header(m_file.fd(), &m_strInfo) == Y4M_OK) {
            m_width     = y4m_si_get_width(&m_strInfo);
            m_height    = y4m_si_get_height(&m_strInfo);

            m_yuv.alloc_buffer(m_width, m_height, m_bits, true, m_endianness);

            generateIndex();

            result      = true;
            m_lastError = IB_ERROR_NO_ERROR;
        } else {
            m_file.Close();
            m_lastError = IB_ERROR_SYSTEM;
        }
	} else {
	    m_lastError = IB_ERROR_FILE_NOT_FOUND;
	}

	return result;
}

/**
 *  Load the YUV image from the file...
 */

bool YUV420ImageBufferY4M::Load(size_t frame)
{
    int bpc = 1; // bytes per component

	wxLogDebug("YUV420ImageBufferY4M::Load(%ld)", frame);

	if (frame == (size_t)-1) {
		wxLogDebug("-- Load called with frame == -1");
		m_lastError = IB_ERROR_OUT_OF_BOUNDS;
		return false;
	}

    if (!m_file.IsOpened()) {
        wxLogDebug("ERROR: File is not opened!");
        m_lastError = IB_ERROR_NOT_OPEN;
        return false;
    }

    wxLogDebug("-- loading YUV format %s %d bits...", ImageBuffer::YUVTypeToString(m_bufType), m_bits);

    switch (m_bits) {
        case 8:
            bpc = 1;
            break;

        case 10:
            bpc = 2;
            break;

        case 16:
            bpc = 2;
            break;
    }

    wxLogDebug("Bytes per component = %d", bpc);

    if ((m_bufType == DATA_YUV420) || (m_bufType == DATA_YUV422)) {
        int				ySize, uSize, vSize;
        int				frameSize = 0;
        wxFileOffset 	offY, offU, offV;

        ySize 		= (m_width * m_height) * bpc;
        uSize 		= ySize / 4;
        vSize 		= ySize / 4;
        frameSize 	= ySize + uSize + vSize;
        offY 		= (wxFileOffset)frameSize * (wxFileOffset)frame;
        offU 		= offY + ySize;
        offV 		= offU + uSize;

        wxLogDebug("framesize = %d", frameSize);

        /* If image data already exists, free it */
        if (m_imageData != 0L) {
            wxLogDebug("-- freeing old image data (0x%p)", m_imageData);
            free_buffer();
        }

        m_yuv.alloc_buffer(m_width, m_height, m_bits, true, m_endianness);

        if (m_pMask->bMaskY == true) {
            m_file.Seek(offY);
            if (m_file.Read(m_yuv.m_pY, m_yuv.m_lumaSize) != m_yuv.m_lumaSize) {
                wxLogDebug("ERROR: incorrect bytecount!");
                m_lastError = IB_ERROR_OUT_OF_BOUNDS;
                return false;
            }
        } else {
            memset(m_yuv.m_pY, 0, m_yuv.m_lumaSize);
        }

        if (m_pMask->bMaskU == true) {
            m_file.Seek(offU);
            if (m_file.Read(m_yuv.m_pU, m_yuv.m_chromaSize) != m_yuv.m_chromaSize) {
                wxLogDebug("ERROR: incorrect bytecount!");
                m_lastError = IB_ERROR_OUT_OF_BOUNDS;
                return false;
            }
        } else {
            memset(m_yuv.m_pU, 0, m_yuv.m_chromaSize);
        }

        if (m_pMask->bMaskV == true) {
            m_file.Seek(offV);
            if (m_file.Read(m_yuv.m_pV, m_yuv.m_chromaSize) != m_yuv.m_chromaSize) {
                wxLogDebug("ERROR: incorrect bytecount!");
                m_lastError = IB_ERROR_OUT_OF_BOUNDS;
                return false;
            }
        } else {
            memset(m_yuv.m_pV, 0, m_yuv.m_chromaSize);
        }
    } else if ((m_bufType == DATA_NV12) || (m_bufType == DATA_NV21)) {
        int				ySize, uSize, vSize, uvSize;
        int				frameSize = 0;
        wxFileOffset 	offY, offUV;
        wxUint8         *pU = 0,
                        *pV = 0;

        ySize       = m_width * m_height;
        uSize       = ySize / 4;
        vSize       = ySize / 4;
        uvSize      = ((ySize / 4) + (ySize / 4));
        frameSize   = ySize + uSize + vSize;
        offY        = (wxFileOffset)frameSize * (wxFileOffset)frame;
        offUV 		= offY + ySize;
//      offV 		= offU + uSize;

        wxLogDebug("framesize = %d", frameSize);

        /* If image data already exists, free it */
        if (m_imageData != 0L) {
            wxLogDebug("-- freeing old image data (0x%p)", m_imageData);
            free_buffer();
        }

        m_yuv.alloc_buffer(m_width, m_height, m_bits, true);

        m_file.Seek(offY);
        if (m_file.Read(m_yuv.m_pY, m_yuv.m_lumaSize) != m_yuv.m_lumaSize) {
            wxLogDebug("ERROR: incorrect bytecount!");
            m_lastError = IB_ERROR_OUT_OF_BOUNDS;
            return false;
        }

        m_file.Seek(offUV);

        /* Swap U & V components in case of NV21 */
        switch (m_bufType) {
            case DATA_NV12:
                pU = m_yuv.m_pU;
                pV = m_yuv.m_pV;
                break;
            case DATA_NV21:
                pV = m_yuv.m_pU;
                pU = m_yuv.m_pV;
                break;
            default:
                break;
        }

        /* de-interleave the U & V components */
        for (int i = 0 ; i < uvSize / 2 ; i++, pU++, pV++) {
            if (m_file.Read(pU, sizeof(wxUint8)) != sizeof(wxUint8)) {
                wxLogDebug("ERROR: incorrect bytecount!");
                m_lastError = IB_ERROR_OUT_OF_BOUNDS;
                return false;
            }
            if (m_file.Read(pV, sizeof(wxUint8)) != sizeof(wxUint8)) {
                wxLogDebug("ERROR: incorrect bytecount!");
                m_lastError = IB_ERROR_OUT_OF_BOUNDS;
                return false;
            }
        }
    } else {
        wxLogDebug("WARNING: Unsupported buffer format %d", (int)m_bufType);
    }

    m_imageData = m_yuv.DoConversionToRGB(m_bufType, m_ccir601);

	wxLogDebug("Image data @ 0x%p", m_imageData);

    m_lastError = IB_ERROR_NO_ERROR;

	return true;
}

/**
 *
 */

void YUV420ImageBufferY4M::GetImage(wxImage* pImage) {
	wxLogDebug("YUV420ImageBufferY4M::GetImage(0x%p)", pImage);

    wxASSERT(pImage != 0L);

    if (m_imageData != 0L) {
        pImage->Destroy();
        pImage->Create(m_width, m_height);
        pImage->SetData(copy_data()); //m_imageData);
    }

	return;
}

/**
 *
 */

ssize_t YUV420ImageBufferY4M::GetFrameCount() {
	int	ySize, uSize, vSize;
	int	frameSize = 0;
	wxFileOffset off;
	ssize_t	result = -1;
    int bpc = sizeof(wxUint8);

	wxLogDebug("YUV420ImageBufferY4M::GetFrameCount(%s)", (const char*)m_imageFilename.c_str());

    if (m_bits == 8) {
        bpc = sizeof(wxUint8);
    } else if (m_bits == 10) {
        bpc = sizeof(wxUint16);
    } else if (m_bits == 16) {
        bpc = sizeof(wxUint16);
    }

	ySize = (m_width * m_height * bpc);
	uSize = ySize / 4;
	vSize = ySize / 4;
	frameSize = ySize + uSize + vSize;

	wxLogDebug("framesize = %d", frameSize);

	if (m_file.IsOpened()) {
		m_file.SeekEnd();
		off = m_file.Tell();
		result = (off / frameSize) - 1;
	}

	return result;
}

/**
 *
 */

PIXEL* YUV420ImageBufferY4M::getPixel(int x, int y) {
	return (PIXEL*)0L;
}


size_t	YUV420ImageBufferY4M::generateIndex() {
    wxFileOffset    currOffset = m_file.Tell();
    uint8_t*        yuv_data[3] = { nullptr, nullptr, nullptr };
    size_t          fnum = 0;

    wxLogDebug("YUV420ImageBufferY4M::generateIndex()");

    m_frameOffsets.push_back(currOffset);
    wxLogDebug("Frame %-3ld @ offset %ld", fnum++, currOffset);

    yuv_data[0] = m_yuv.m_pY;
    yuv_data[1] = m_yuv.m_pU;
    yuv_data[2] = m_yuv.m_pV;

    while (Y4M_OK == y4m_read_frame_data(m_file.fd(), &m_strInfo, &m_frmInfo, yuv_data)) {
        currOffset = m_file.Tell();
        m_frameOffsets.push_back(currOffset);
        wxLogDebug("Frame %-3ld @ offset %ld", fnum++, currOffset);
    }

    wxLogDebug("-- found %ld frames!", m_frameOffsets.size());

    return (size_t)m_frameOffsets.size();
}
