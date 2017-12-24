//==============================================================================
//  MODULE      :   imageBuffer.cpp
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   October 7, 2013
//  LAST MOD    :   December 20, 2017
//==============================================================================

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/wx.h>
#ifdef HAVE_LIBGCRYPT
#include <gcrypt.h>
#endif
#include "component_buffer.h"
#include "imageBuffer.h"

#ifdef HAVE_LIBSWSCALE
    extern "C" {
        #include <libswscale/swscale.h>
    }
#endif // HAVE_LIBSWSCALE

/**
 *
 */

yuvBuffer::yuvBuffer()
    : m_yuvWidth(0), m_yuvHeight(0), m_lumaSize(0), m_chromaSize(0), m_bits(8),
      m_endianness(endian_little), m_pY(nullptr), m_pU(nullptr), m_pV(nullptr) {
    // ctor
    wxLogDebug("yuvBuffer::yuvBuffer()");
}

yuvBuffer::~yuvBuffer() {
    wxLogDebug("yuvBuffer::~yuvBuffer()");
    // dtor
    free_buffer();
}

/**
 *  allocate the buffer to store the YUV data in.
 */

void yuvBuffer::alloc_buffer(wxUint32 width, wxUint32 height, wxUint32 bits,
                             bool bClear, formatEndian endianness) {
    int bpc = 1;

    wxLogDebug("yuvBuffer::alloc_buffer(%d, %d, %d, %d, %s)", width, height, bits,
               (bClear == true) ? 1 : 0,
               (endianness == endian_little) ? "le" : "be");

    wxASSERT((width > 0) && (height > 0));

    switch (bits) {
    case 8:
        bpc = sizeof(wxUint8);
        break;
    case 10:
        bpc = sizeof(wxUint16);
        break;
    case 16:
        bpc = sizeof(wxUint16);
        break;
    default:
        wxLogDebug("Invalid buffer depth of %d, assuming 1 byte per component",
                   bits);
        bpc = sizeof(wxUint8);
        break;
    }

    /* Only realloc if the width or height or bits has changed... */
    if ((width != m_yuvWidth) || (height != m_yuvHeight) || (bits != m_bits)) {
        m_yuvWidth  = width;
        m_yuvHeight = height;
        m_bits      = bits;
        m_endianness = endianness;
        m_lumaSize = (width * height) * bpc;
        m_chromaSize = ((width * height) >> 2) * bpc;

        wxLogDebug("m_lumaSize = %d m_chromaSize = %d", m_lumaSize, m_chromaSize);

        wxUint8 *oldBuffer = m_pY;
        wxUint8 *newBuffer = (wxUint8 *)realloc(oldBuffer, m_lumaSize + m_chromaSize * 2);

        m_pY = newBuffer;
        m_pU = newBuffer + m_lumaSize;
        m_pV = m_pU + m_chromaSize;

        wxASSERT((m_pY != nullptr) && (m_pU != nullptr) && (m_pV != nullptr));
    }

    if (bClear) {
        wxLogDebug("-- clearing memory to zero...");
        memset(m_pY, 0, m_lumaSize + m_chromaSize * 2);
    }

    return;
}

/**
 *  Free resources used by yuv buffer...
 */

void yuvBuffer::free_buffer() {
    wxLogDebug("yuvBuffer::free_buffer()");

    if (m_pY != nullptr) {
        free(m_pY);
        m_pY = m_pU = m_pV = nullptr;

        m_yuvWidth = m_yuvHeight = m_lumaSize = m_chromaSize = 0;
        m_bits = 8;
    }

    return;
}

/**
 *  Convert YUV buffer to RGB bitmap.
 */

wxUint8 *yuvBuffer::DoConversionToRGB(dataType bufType, bool ccir601) {

    wxLogDebug("yuvBuffer::DoConversionToRGB(%d, %d)", (int)bufType,
               (int)ccir601);

#ifdef HAVE_LIBSWSCALE
    wxUint8         *imageData = nullptr;
    AVPixelFormat   srcFmt;
    int             bpc = 0;

    if (ccir601 == true) {
        wxMessageBox("ccir601 Not supported", "Warning");
        return (wxUint8*)nullptr;
    }

    switch (m_bits) {
    case 8:
        srcFmt  = AV_PIX_FMT_YUV420P;
        bpc     = sizeof(wxUint8);
        break;
    case 10:
        if (m_endianness == endian_little) {
            srcFmt = AV_PIX_FMT_YUV420P10LE;
        } else {
            srcFmt = AV_PIX_FMT_YUV420P10BE;
        }
        bpc     = sizeof(wxUint16);
        break;
    case 16:
        if (m_endianness == endian_little) {
            srcFmt = AV_PIX_FMT_YUV420P16LE;
        } else {
            srcFmt = AV_PIX_FMT_YUV420P16BE;
        }
        bpc     = sizeof(wxUint16);
        break;
    default:
        wxLogWarning("Invalid value of %d for m_bits!", m_bits);
        return (wxUint8 *)nullptr;
    }

    int rows = m_yuvHeight,
        cols = m_yuvWidth;

    imageData = (wxUint8 *)malloc(rows * cols * sizeof(PIXEL));
    wxASSERT( imageData != nullptr );

    uint8_t*        srcSlice[3]     = { m_pY, m_pU, m_pV } ;
    int             srcStride[3]    = { cols * bpc, cols * bpc / 2, cols * bpc / 2 };
    uint8_t*        dst[3]          = { imageData, nullptr, nullptr };
    int             dstStride[3]    = { cols * (int)sizeof(PIXEL), 0, 0 };
    int             res             = 0;


    struct SwsContext  *swsContext = sws_getContext(cols, rows, srcFmt,
                                                    cols, rows, AV_PIX_FMT_RGB24,
                                                    SWS_BILINEAR, NULL, NULL, NULL);

    if (swsContext != nullptr) {
        res = sws_scale(swsContext, srcSlice, srcStride, 0, rows, dst, dstStride);
        sws_freeContext( swsContext );
    } else {
        memset(imageData, 0x80, rows * cols * sizeof(PIXEL));
    }

#else // HAVE_LIBSWSCALE

    long int u, v, y0, y1, y2, y3, u0, u1, u2, u3, v0, v1, v2, v3;
    unsigned char *y1buf = nullptr, *y2buf = nullptr, *ubuf = nullptr,
                   *vbuf = nullptr;
    register PIXEL *pP1 = nullptr, *pP2 = nullptr;
    int rows, row, cols, col;
    size_t bpc = 0, shift = 0;
    wxUint8 *imageData = nullptr;


    switch (m_bits) {
    case 8:
        bpc = sizeof(wxUint8);
        shift = 0;
        break;
    case 10:
        bpc = sizeof(wxUint16);
        shift = 2;
        break;
    case 16:
        bpc = sizeof(wxUint16);
        shift = 8;
        break;
    default:
        wxLogWarning("Invalid value of %d for m_bits!", m_bits);
        return (wxUint8 *)nullptr;
    }

    wxLogDebug("bpc %zu shift %zu", bpc, shift);

    rows = m_yuvHeight;
    cols = m_yuvWidth;

    imageData = (wxUint8 *)malloc(rows * cols * sizeof(PIXEL));
    wxASSERT( imageData != nullptr );

    y1buf = m_pY;
    y2buf = m_pY + (cols * bpc);
    ubuf = m_pU;
    vbuf = m_pV;

    for (row = 0; row < (rows & ~1); row += 2) {
        component_buffer y1Buffer(y1buf, cols, bpc, m_endianness);
        component_buffer y2Buffer(y2buf, cols, bpc, m_endianness);
        component_buffer vBuffer(vbuf, cols, bpc, m_endianness);
        component_buffer uBuffer(ubuf, cols, bpc, m_endianness);

        component_buffer::component_iterator y1It = y1Buffer.begin();
        component_buffer::component_iterator y2It = y2Buffer.begin();
        component_buffer::component_iterator vIt = vBuffer.begin();
        component_buffer::component_iterator uIt = uBuffer.begin();

        pP1 = (PIXEL *)((unsigned long)imageData +
                        (row * m_yuvWidth * sizeof(PIXEL)));
        pP2 = (PIXEL *)((unsigned long)imageData +
                        ((row + 1) * m_yuvWidth * sizeof(PIXEL)));

        for (col = 0; col < (cols & ~1); col += 2) {
            long int r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;

            y0 = (long int)(*y1It >> shift);
            y1It++;
            y1 = (long int)(*y1It >> shift);
            y1It++;
            y2 = (long int)(*y2It >> shift);
            y2It++;
            y3 = (long int)(*y2It >> shift);
            y2It++;

            u = (long int)((wxUint8)(*uIt >> shift) - 128);
            uIt++;
            v = (long int)((wxUint8)(*vIt >> shift) - 128);
            vIt++;

            if (bufType == DATA_YUV422) {
                uIt++;
                vIt++;
            }

#if 0
            wxLogDebug("y0 %04x y1 %04x y2 %04x y3 %04x u %04x v %04x",
                       (wxUint16)y0,(wxUint16)y1,(wxUint16)y2,(wxUint16)y3,(wxUint16)u,(wxUint16)v);
#endif

            if (ccir601) {
                y0 = ((y0 - 16) * 255) / 219;
                y1 = ((y1 - 16) * 255) / 219;
                y2 = ((y2 - 16) * 255) / 219;
                y3 = ((y3 - 16) * 255) / 219;

                u = (u * 255) / 224;
                v = (v * 255) / 224;
            }

            /* mean the chroma for subsampling */

            u0 = u1 = u2 = u3 = u;
            v0 = v1 = v2 = v3 = v;

            /* The inverse of the JFIF RGB to YUV Matrix for $00010000 = 1.0

            [Y]   [65496        0   91880][R]
            [U] = [65533   -22580  -46799[G]
            [V]   [65537   116128      -8][B]

            */

            r0 = 65536 * y0 + 91880 * v0;
            g0 = 65536 * y0 - 22580 * u0 - 46799 * v0;
            b0 = 65536 * y0 + 116128 * u0;

            r1 = 65536 * y1 + 91880 * v1;
            g1 = 65536 * y1 - 22580 * u1 - 46799 * v1;
            b1 = 65536 * y1 + 116128 * u1;

            r2 = 65536 * y2 + 91880 * v2;
            g2 = 65536 * y2 - 22580 * u2 - 46799 * v2;
            b2 = 65536 * y2 + 116128 * u2;

            r3 = 65536 * y3 + 91880 * v3;
            g3 = 65536 * y3 - 22580 * u3 - 46799 * v3;
            b3 = 65536 * y3 + 116128 * u3;

            r0 = limit(r0);
            r1 = limit(r1);
            r2 = limit(r2);
            r3 = limit(r3);
            g0 = limit(g0);
            g1 = limit(g1);
            g2 = limit(g2);
            g3 = limit(g3);
            b0 = limit(b0);
            b1 = limit(b1);
            b2 = limit(b2);
            b3 = limit(b3);

#ifdef DUMP_RGB
            wxLogDebug("r0 = 0x%02x g0 = 0x%02x b0 = 0x%02x", r0, g0, b0);
            wxLogDebug("r1 = 0x%02x g1 = 0x%02x b1 = 0x%02x", r1, g1, b1);
            wxLogDebug("r2 = 0x%02x g2 = 0x%02x b2 = 0x%02x", r2, g2, b2);
            wxLogDebug("r3 = 0x%02x g3 = 0x%02x b3 = 0x%02x", r3, g3, b3);
#endif // DUMP_RGB

            /* first pixel */
            PPM_ASSIGN(*pP1, r0, g0, b0);
            pP1++;
            /* 2nd pixval */
            PPM_ASSIGN(*pP1, r1, g1, b1);
            pP1++;
            /* 3rd pixval */
            PPM_ASSIGN(*pP2, r2, g2, b2);
            pP2++;
            /* 4th pixval */
            PPM_ASSIGN(*pP2, r3, g3, b3);
            pP2++;
        }

        y1buf += (cols * bpc * 2);
        y2buf += (cols * bpc * 2);
        ubuf += ((cols * bpc) >> 1);
        vbuf += ((cols * bpc) >> 1);
    } // end row

#endif // HAV_LIBSWSCALE

    return imageData;
}

/******************************************************************************/

ImageBuffer::ImageBuffer()
    : m_bufType(DATA_UNKNOWN), m_width(0L), m_height(0L),
      m_endianness(endian_little), m_imageData(nullptr), m_ccir601(false),
      m_lastError(IB_ERROR_NO_ERROR), m_pMask(nullptr) {
    // ctor
    wxLogDebug("ImageBuffer::ImageBuffer() default");
}

/**
 *  Initialize an ImageBuffer with type and size.
 */

ImageBuffer::ImageBuffer(dataType type, int width, int height,
                         int bits /* = 8 */,
                         formatEndian endianness /* = endian_little */)
    : m_bufType(type), m_width(width), m_height(height), m_bits(bits),
      m_endianness(endianness), m_imageData(nullptr), m_ccir601(false),
      m_lastError(IB_ERROR_NO_ERROR), m_pMask(nullptr) {
    // ctor
    wxLogDebug("ImageBuffer::ImageBuffer(%s, %d, %d, %d, %s)",
               ImageBuffer::YUVTypeToString(type), width, height, bits,
               (m_endianness == endian_little) ? "le" : "be");
}

/**
 *  Destructor
 */

ImageBuffer::~ImageBuffer() {
    // dtor
    wxLogDebug("ImageBuffer::~ImageBuffer()");
}

/**
 *  Save YUV file.
 */

bool ImageBuffer::SaveYUV(wxString sFilename, eSaveType type) {
    bool bRes = false;
    wxFile fp;

    wxLogDebug("ImageBuffer::SaveYUV(%s, %d)", (const char *)sFilename.c_str(),
               (int)type);

    wxASSERT((m_yuv.m_pY != 0) && (m_yuv.m_pU != 0) && (m_yuv.m_pV != 0));

    if (type == SAVE_YUV_COMP) {
        if (fp.Open(sFilename, wxFile::write)) {
            fp.Write(m_yuv.m_pY, m_yuv.m_lumaSize);
            fp.Write(m_yuv.m_pU, m_yuv.m_chromaSize);
            fp.Write(m_yuv.m_pV, m_yuv.m_chromaSize);
            fp.Close();
        }
    } else if (type == SAVE_YUV_SPLIT) {
        wxString sCompFilename;

        sCompFilename = wxString::Format(wxT("%s_0.Y"), sFilename.c_str());
        if (fp.Open(sCompFilename, wxFile::write)) {
            fp.Write(m_yuv.m_pY, m_yuv.m_lumaSize);
            fp.Close();
        }

        sCompFilename = wxString::Format(wxT("%s_0.U"), sFilename.c_str());
        if (fp.Open(sCompFilename, wxFile::write)) {
            fp.Write(m_yuv.m_pU, m_yuv.m_chromaSize);
            fp.Close();
        }

        sCompFilename = wxString::Format(wxT("%s_0.V"), sFilename.c_str());
        if (fp.Open(sCompFilename, wxFile::write)) {
            fp.Write(m_yuv.m_pU, m_yuv.m_chromaSize);
            fp.Close();
        }

    } else {
        wxLogDebug("ERROR: Invalid save type!");
    }

    return bRes;
}

/**
 *
 */

bool ImageBuffer::GetChecksum(checksumAlgoBase *pAlgo, checksumEntry &entry) {
    bool bRes = false;

    wxLogDebug("ImageBuffer::GetChecksum(0x%p, ...)", pAlgo);

    wxASSERT(pAlgo != nullptr);

    bRes = pAlgo->calculate(&m_yuv, entry);

    return bRes;
}

/**
 *
 */

bool ImageBuffer::SaveCurrentFrame(const wxString &sFilename) {
    bool bRes = false;

    wxLogDebug("ImageBuffer::SaveCurrentFrame(%s)", sFilename);

    if (m_imageData != nullptr) {
        wxImage pImage;

        wxASSERT((m_width > 0) && (m_height > 0));

        pImage.Create(m_width, m_height);
        pImage.SetData(copy_data()); // m_imageData);
        bRes = pImage.SaveFile(sFilename);
    }

    return bRes;
}

/**
 *  Convert YUV format to string.
 */

wxString ImageBuffer::YUVTypeToString(dataType type) {
    wxString sBufferType;

    switch (type) {
    case DATA_UNKNOWN: //!< Data format unknown
        sBufferType = "Unknown";
        break;
    case DATA_YUV420: //!< Data in YUV420 format
        sBufferType = "YUV4:2:0";
        break;
    case DATA_YUV422: //!< Data in YUV422 format
        sBufferType = "YUV4:2:2";
        break;
    case DATA_YUV444: //!< Data in YUV444 format
        sBufferType = "YUV4:4:4";
        break;
    case DATA_NV12: //!< Data in NV12 format
        sBufferType = "NV12";
        break;
    case DATA_NV21: //!< Data in NV21 format
        sBufferType = "NV21";
        break;
    case DATA_RGB32: //!< Data in RGB32 format
        sBufferType = "RBG32";
        break;
    default:
        sBufferType = "Undefined";
        break;
    }

    return sBufferType;
}

/**
 *  Convert buffer type to string.
 */

wxString ImageBuffer::BufTypeToString(yuvBufType type) {
    wxString sBufferType;

    switch (type) {
    case YUV_FILE_UNKNOWN:
        sBufferType = "YUV_FILE_UNKNOWN";
        break;
    case YUV_FILE_SPLIT:
        sBufferType = "YUV_FILE_SPLIT";
        break;
    case YUV_FILE_COMP:
        sBufferType = "YUV_FILE_COMP";
        break;
    case YUV_FILE_DUMP:
        sBufferType = "YUV_FILE_DUMP";
        break;
    }

    return sBufferType;
}

/**
 *
 */

// void ImageBuffer::setmaskYUV(bool ymask, bool umask, bool vmask) {
//    m_mask.bMaskY = ymask;
//    m_mask.bMaskU = umask;
//    m_mask.bMaskV = vmask;
//
//    return;
//}
//
// void ImageBuffer::setmaskYUV(const yuvMask& mask) {
//    m_mask = mask;
//}
//
// void ImageBuffer::getmaskYUV(yuvMask& mask) {
//    mask = m_mask;
//}

void ImageBuffer::setYUVMask(yuvMask *pMask) {
    m_pMask = pMask;
    return;
}
