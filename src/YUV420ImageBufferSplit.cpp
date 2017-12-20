//==============================================================================
//	MODULE		:	YUV420ImageBufferSplit.cpp
//  PROJECT     :   frameInspector
//	PROGRAMMER	:	Michael A. Uman
//  DATE        :   October 8, 2013
//==============================================================================

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "imageBuffer.h"
#include "YUV420ImageBufferSplit.h"
#include "yuvImageControl.h"

/**
 *
 */

YUV420ImageBufferSplit::YUV420ImageBufferSplit(int x, int y, int bits,
        formatEndian endianness)
    :   ImageBuffer(DATA_YUV420, x, y, bits, endianness)
{
    wxLogDebug("YUV420ImageBufferSplit::YUVImageBuffer(%d, %d)", x, y);

    return;
}

/**
 *
 */

YUV420ImageBufferSplit::~YUV420ImageBufferSplit()
{
    wxLogDebug("YUV420ImageBufferSplit::~YUV420ImageBufferSplit()");

    if (m_imageData) {
        free(m_imageData);
        m_imageData = nullptr;
    }

    return;
}

wxString YUV420ImageBufferSplit::GetFileBase(size_t frame) {
    return m_fileVec[frame];
}

/**
 *  Load frame # 'frame'.
 */

bool YUV420ImageBufferSplit::Load(size_t frame) {
    wxString    imageFilePath;

    /* create the image filename from the path and frame #. */
    imageFilePath = m_fileVec[frame];

    return Load(imageFilePath.c_str());
}

/**
 *  Load image from .Y, .U, & .V files...
 */

bool YUV420ImageBufferSplit::Load(const char* filename) {
    bool			result = false;
    wxString        fullName;
    wxFile          yfp, ufp, vfp;

    wxLogDebug("YUV420ImageBufferSplit::Load(%s)", filename);

    /* If image data already exists, free it */
    if (m_imageData != nullptr) {
        wxLogDebug("-- freeing old image data (0x%p)", m_imageData);

        free(m_imageData);
        m_imageData = nullptr;
    }

    m_yuv.alloc_buffer( m_width, m_height, m_bits, true, m_endianness );

    /* Read Y component file */
    if (m_pMask->bMaskY == true) {
        fullName = (wxString)filename + wxT(".Y");
        wxLogDebug("Y File = %s", (const char*)fullName.c_str());
        if (yfp.Open(fullName)) {
            if (yfp.Read(m_yuv.m_pY, m_yuv.m_lumaSize) != m_yuv.m_lumaSize) {
                wxLogDebug("ERROR: unable to read Y!");
            }
        }
    } else {
        memset(m_yuv.m_pY, 0, m_yuv.m_lumaSize);
    }

    /* Read U component file */
    if (m_pMask->bMaskU == true) {
        fullName = (wxString)filename + wxT(".U");
        wxLogDebug("U File = %s", (const char*)fullName.c_str());
        if (ufp.Open(fullName)) {
            if (ufp.Read(m_yuv.m_pU, m_yuv.m_chromaSize) != m_yuv.m_chromaSize) {
                wxLogDebug("ERROR: unable to read U!");
            }
        }
    } else {
        memset(m_yuv.m_pU, 0, m_yuv.m_chromaSize);
    }

    /* Read V component file */
    if (m_pMask->bMaskV == true) {
        fullName = (wxString)filename + wxT(".V");
        wxLogDebug("V File = %s", (const char*)fullName.c_str());
        if (vfp.Open(fullName)) {
            if (vfp.Read(m_yuv.m_pV, m_yuv.m_chromaSize) != m_yuv.m_chromaSize) {
                wxLogDebug("ERROR: unable to read V!");
            }
        }
    } else {
        memset(m_yuv.m_pV, 0, m_yuv.m_chromaSize);
    }

    m_imageData = m_yuv.DoConversionToRGB(m_bufType, m_ccir601);
    wxASSERT( m_imageData != nullptr );

    result = true;

    return result;
}

PIXEL* YUV420ImageBufferSplit::getPixel(int x, int y) {
    size_t	offset = ((y * sizeof(PIXEL) * m_width) + (x * sizeof(PIXEL)));

    return (PIXEL *)&m_imageData[offset];
}

void YUV420ImageBufferSplit::GetImage(wxImage* pImage) {
    wxLogDebug("YUV420ImageBufferSplit::GetImage(0x%p)", pImage);
    assert(pImage != nullptr);

    pImage->Destroy();
    pImage->Create(m_width, m_height);
    pImage->SetData(copy_data()); //m_imageData);

    return;
}

void YUV420ImageBufferSplit::getImageVec() {
//    utils::GetSplitBufferFrameVec( m_imagePath, m_prefix, m_fileVec );
    utils::GetSplitBufferFrameVec( m_spec, m_fileVec );
    wxLogDebug("getImageVec size = %zu", m_fileVec.size());
}

/**
 *  Get total # of frames in sequence.
 */

ssize_t YUV420ImageBufferSplit::GetFrameCount() {
//    size_t fc = 0;
//
//    utils::GetSplitBufferFrameCount(m_imagePath, m_prefix, fc);
//
//    return fc - 1;
    return m_fileVec.size() - 1;
}
