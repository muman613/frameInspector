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
	wxLogDebug("YUV420ImageBufferY4M::Load(%ld)", frame);

    if (frame == (size_t)-1) {
		wxLogDebug("-- Load called with frame == -1");
		m_lastError = IB_ERROR_OUT_OF_BOUNDS;
		return false;
	}

    if ((frame >= 0) && (frame <= m_frameOffsets.size())) {
        uint8_t*        yuv_data[3] = { nullptr, nullptr, nullptr };

        yuv_data[0] = m_yuv.m_pY;
        yuv_data[1] = m_yuv.m_pU;
        yuv_data[2] = m_yuv.m_pV;

        wxFileOffset offs = m_frameOffsets[frame];
        wxLogDebug("-- seeking to offset %ld...", offs);
        m_file.Seek(offs);

        if (y4m_read_frame(m_file.fd(), &m_strInfo,
                           &m_frmInfo, yuv_data) == Y4M_OK)
        {
            m_imageData = m_yuv.DoConversionToRGB(m_bufType, m_ccir601);
            wxLogDebug("Image data @ 0x%p", m_imageData);

            m_lastError = IB_ERROR_NO_ERROR;
        } else {
            m_lastError = IB_ERROR_SYSTEM;
            return false;
        }
    } else {
		m_lastError = IB_ERROR_OUT_OF_BOUNDS;
		return false;
    }


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
	wxLogDebug("YUV420ImageBufferY4M::GetFrameCount(%s)", m_imageFilename);
    return (ssize_t)(m_frameOffsets.size() - 1);
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

    yuv_data[0] = m_yuv.m_pY;
    yuv_data[1] = m_yuv.m_pU;
    yuv_data[2] = m_yuv.m_pV;


    while (Y4M_OK == y4m_read_frame(m_file.fd(), &m_strInfo,
                                    &m_frmInfo, yuv_data))
    {
        m_frameOffsets.push_back(currOffset);
        wxLogDebug("Frame %-3ld @ offset %ld", fnum++, currOffset);

        currOffset = m_file.Tell();
    }

    wxLogDebug("-- found %ld frames!", m_frameOffsets.size());

    return (size_t)m_frameOffsets.size();
}
