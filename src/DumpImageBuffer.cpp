/**
 *  @file       DumpImageBuffer.cpp
 *  @author     Michael A. Uman
 *  @date       February 19, 2013
 *  @brief      Buffer class supporting the Sigma 'dump' format.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <wx/wx.h>
#include <stdio.h>
#ifdef  HAVE_LIBGCRYPT
#include <gcrypt.h>
#endif
#include "imageBuffer.h"
#include "misc_utils.h"
#include "DumpImageBuffer.h"
#include "dumpInternals.h"
#include "crc32.h"


dumpImageBuffer::dumpImageBuffer()
    :   ImageBuffer(DATA_YUV420, -1, -1),
        m_frameNo(-1)
{
//constructor
    wxLogDebug("dumpImageBuffer::dumpImageBuffer()");

    return;
}

dumpImageBuffer::dumpImageBuffer(int width, int height)
    :   ImageBuffer(DATA_YUV420, width, height),
        m_frameNo(-1)
{
//constructor
    wxLogDebug("dumpImageBuffer::dumpImageBuffer(%d, %d)", width, height);

    return;
}

dumpImageBuffer::dumpImageBuffer(ImageBufferParms& parms)
:   ImageBuffer(parms)
{
    wxLogDebug("dumpImageBuffer::dumpImageBuffer(ImageBufferParms& parms)");
}

/**
 *
 */

dumpImageBuffer::~dumpImageBuffer() {
//destructor
    wxLogDebug("dumpImageBuffer::~dumpImageBuffer()");

    if (m_imageData != nullptr) {
        free(m_imageData);
        m_imageData = nullptr;
    }

    return;
}

/**
 *  Scan directory looking for all pic_*.rmar files.
 */

ssize_t		dumpImageBuffer::GetFrameCount() {
    ssize_t count = 0;

    wxLogDebug("dumpImageBuffer::GetFrameCount()");

    m_frameList.clear();
    /* run through all files in directory m_imagePath */

    wxString sFullPath;
    wxString sSpec = m_imagePath + wxT("/pic_*.rmar");

    sFullPath = ::wxFindFirstFile(sSpec, wxFILE);
    while (!sFullPath.empty()) {
        count++;
//        wxLogVerbose("-- found file %s\n", sFullPath.c_str());
        m_frameList.Add( sFullPath );
        sFullPath = ::wxFindNextFile();
    }

    /* the frame list should be sorted */
    m_frameList.Sort();

#ifdef  __WXDEBUG__
    wxLogDebug("found %ld frames:", m_frameList.GetCount());

    for (size_t n = 0 ; n < m_frameList.GetCount() ; n++) {
        wxLogVerbose("\t%s", (const char*)m_frameList[n].c_str());
    }
#endif  // _WXDEBUG__

    return m_frameList.GetCount() - 1;
}

/**
 *
 */

void dumpImageBuffer::GetImage(wxImage* pImage) {
    wxLogDebug("dumpImageBuffer::GetImage(0x%p)", pImage);

    wxASSERT(pImage != nullptr);

    if (m_imageData != nullptr) {
        pImage->Destroy();
        pImage->Create(m_width, m_height);
        pImage->SetData(copy_data()); //m_imageData);
    }

    return;
}

/**
 *  Load frame from dump directory.
 */

bool dumpImageBuffer::Load(size_t frame) {
    bool        bRes = false;
    wxString    sFrameName;
    wxUint8     *pLumaBuffer    = nullptr,
                *pChromaBuffer  = nullptr;
    wxUint32    lumaW = 0, lumaH = 0;
    wxUint32    chromaW = 0, chromaH = 0;
    wxInt32     lumaTabSize = 0, chromaTabSize = 0;
    wxUint32    lumaTotalWidth = 0, chromaTotalWidth = 0;
    wxUint32    bufferW = 0, bufferH = 0;
    int         flags = 0;
    enum {
        FOUND_PICI = (1L << 0),
        FOUND_LUMA = (1L << 1),
        FOUND_CHRM = (1L << 2),
    };

    wxLogDebug("dumpImageBuffer::Load(%ld)", frame);

//    wxLogDebug("-- INFO : Sizeof EMhwlibPictureInfoV1 = %d\n", sizeof(dumpInternals::EMhwlibPictureInfoV1));

    if (frame < m_frameList.GetCount()) {
        sFrameName = m_frameList[frame];
        wxLogDebug("-- loading frame [%s]", (const char*)sFrameName.c_str());

        if (m_fp.Open( sFrameName )) {
            wxUint32 nTag, nVersion;
            wxInt64 lSize;
            /* read file header */
            if (dumpInternals::read_box(&m_fp, nTag, nVersion, lSize) && (nTag == RMAR_MAGIC)) {
                wxLogDebug("Found RMAR tag (version %d size %lld)!", nVersion, lSize);

                while (!m_fp.Eof() && (flags  != (FOUND_PICI|FOUND_LUMA|FOUND_CHRM))) {
                    if (dumpInternals::read_box(&m_fp, nTag, nVersion, lSize)) {

                        if (nTag == PICI_MAGIC) {
                            void*                                   pInfo = nullptr;

                            wxLogDebug("-- found picture info [%lld] vs [%lld] !", lSize, sizeof(dumpInternals::EMhwlibPictureInfoV1));
                            pInfo = (void*)malloc( lSize );

                            if (m_fp.Read(pInfo, lSize) != lSize) {
                                wxLogDebug("ERROR: Unable to read picture info!");
                                break;
                            }

//                            picture_info = (dumpInternals::EMhwlibPictureInfoV1*)malloc( lSize );
                            if (lSize == sizeof(dumpInternals::EMhwlibPictureInfoV1))
                            {
                                dumpInternals::EMhwlibPictureInfoV1    *picture_info;

                                picture_info = (dumpInternals::EMhwlibPictureInfoV1*)pInfo;

                                wxLogDebug("luma_total_width   = %d", picture_info->Picture.luma_total_width);
                                wxLogDebug("chroma_total_width = %d", picture_info->Picture.chroma_total_width);
                                wxLogDebug("scaled_width = %d scaled_height = %d", picture_info->Picture.scaled_width, picture_info->Picture.scaled_height);

//                              lumaX = picture_info->Picture.luma_position_in_buffer.x;
//                              lumaY = picture_info->Picture.luma_position_in_buffer.y;
                                lumaW = picture_info->Picture.luma_position_in_buffer.width;
                                lumaH = picture_info->Picture.luma_position_in_buffer.height;
                                lumaTotalWidth = picture_info->Picture.luma_total_width;

//                              chromaX = picture_info->Picture.chroma_position_in_buffer.x;
//                              chromaY = picture_info->Picture.chroma_position_in_buffer.y;
                                chromaW = picture_info->Picture.chroma_position_in_buffer.width;
                                chromaH = picture_info->Picture.chroma_position_in_buffer.height;
                                chromaTotalWidth = picture_info->Picture.chroma_total_width;

                                bufferW = ((lumaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                                bufferH = ((lumaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                                lumaTabSize = bufferW * bufferH;

                                wxLogDebug("Luma buffer Dimensions = %d X %d = %d", bufferW, bufferH, lumaTabSize);

                                bufferW = ((chromaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                                bufferH = ((chromaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                                chromaTabSize = bufferW * bufferH * 2;

                                wxLogDebug("Chroma buffer Dimensions = %d X %d = %d", bufferW, bufferH, chromaTabSize);

                                /* allocate buffers */
                                wxLogDebug("-- allocating buffers!");

                                pLumaBuffer     = (wxUint8*)malloc( lumaTabSize );
                                pChromaBuffer   = (wxUint8*)malloc( chromaTabSize );

                                wxASSERT((pLumaBuffer != nullptr) &&
                                         (pChromaBuffer != nullptr));

                                m_width  = picture_info->Picture.scaled_width;
                                m_height = picture_info->Picture.scaled_height;
                            } else {
                                dumpInternals::EMhwlibPictureInfoV2*   picture_info;

                                picture_info = (dumpInternals::EMhwlibPictureInfoV2*)pInfo;

                                wxLogDebug("luma_total_width   = %d", picture_info->Picture.luma_total_width);
                                wxLogDebug("chroma_total_width = %d", picture_info->Picture.chroma_total_width);
                                wxLogDebug("scaled_width = %d scaled_height = %d", picture_info->Picture.scaled_width, picture_info->Picture.scaled_height);

//                              lumaX = picture_info->Picture.luma_position_in_buffer.x;
//                              lumaY = picture_info->Picture.luma_position_in_buffer.y;
                                lumaW = picture_info->Picture.luma_position_in_buffer.width;
                                lumaH = picture_info->Picture.luma_position_in_buffer.height;
                                lumaTotalWidth = picture_info->Picture.luma_total_width;

//                              chromaX = picture_info->Picture.chroma_position_in_buffer.x;
//                              chromaY = picture_info->Picture.chroma_position_in_buffer.y;
                                chromaW = picture_info->Picture.chroma_position_in_buffer.width;
                                chromaH = picture_info->Picture.chroma_position_in_buffer.height;
                                chromaTotalWidth = picture_info->Picture.chroma_total_width;

                                bufferW = ((lumaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                                bufferH = ((lumaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                                lumaTabSize = bufferW * bufferH;

                                wxLogDebug("Luma buffer Dimensions = %d X %d = %d", bufferW, bufferH, lumaTabSize);

                                bufferW = ((chromaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                                bufferH = ((chromaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                                chromaTabSize = bufferW * bufferH * 2;

                                wxLogDebug("Chroma buffer Dimensions = %d X %d = %d", bufferW, bufferH, chromaTabSize);

                                /* allocate buffers */
                                wxLogDebug("-- allocating buffers!");

                                pLumaBuffer     = (wxUint8*)malloc( lumaTabSize );
                                pChromaBuffer   = (wxUint8*)malloc( chromaTabSize );

                                wxASSERT((pLumaBuffer != nullptr) &&
                                         (pChromaBuffer != nullptr));

                                m_width  = picture_info->Picture.scaled_width;
                                m_height = picture_info->Picture.scaled_height;
                            }

                            wxLogDebug("width %d height %d", m_width, m_height);

                            m_yuv.alloc_buffer( m_width, m_height, m_bits, true );

                            free(pInfo);
                            flags |= FOUND_PICI;
                        } else if (nTag == LUMA_MAGIC) {
                            wxLogDebug("-- found luma buffer [%lld]!", lSize);

                            if (lumaTabSize != lSize) {
                                wxLogDebug("WARNING: It seems the luma buffer size is incorrect!");
                            }

                            if (m_fp.Read(pLumaBuffer, lumaTabSize) != lumaTabSize) {
                                wxLogDebug("WARNING: Incomplete buffer read! aborting...");
                                break;
                            }
                            flags |= FOUND_LUMA;
                        } else if (nTag == CHROMA_MAGIC) {
                            wxLogDebug("-- found chroma buffer [%lld]!", lSize);

                            if (chromaTabSize != lSize) {
                                wxLogDebug("WARNING: It seems the chroma buffer size is incorrect!");
                            }

                            if (m_fp.Read(pChromaBuffer, chromaTabSize) != chromaTabSize) {
                                wxLogDebug("WARNING: Incomplete buffer read! aborting...");
                                break;
                            }
                            flags |= FOUND_CHRM;
                        } else {
                            wxLogDebug("-- found unknown tag [%c%c%c%c]!",
                                       ((nTag & 0xff000000) >> 24),
                                       ((nTag & 0x00ff0000) >> 16),
                                       ((nTag & 0x0000ff00) >> 8),
                                       ((nTag & 0x000000ff)));
                            m_fp.Seek(lSize, wxFromCurrent);
                        }
                    }
                }
                wxLogDebug("-- de-tiling buffers!");

                wxUint32 pvc_tw = (1 << RMTILE_WIDTH_SHIFT);
                wxUint32 pvc_th = (1 << RMTILE_HEIGHT_SHIFT);
                wxUint32 pvc_ts = pvc_tw*pvc_th;
                wxUint8  *pDesty, *pDestu, *pDestv;

                wxLogDebug("luma w X h = %d X %d", lumaW, lumaH);

                pDesty = m_yuv.m_pY;
                pDestu = m_yuv.m_pU ;
                pDestv = m_yuv.m_pV;

                wxLogDebug("-- m_lumaSize %d m_chromaSize %d",
                           m_yuv.m_lumaSize, m_yuv.m_chromaSize);

                /* save the luma buffer */
                for (wxUint32 y = 0 ; y < lumaH ; y++)
                    for (wxUint32 x = 0 ; x < lumaW ; x++) {
                        wxUint8* pixel = (pLumaBuffer +\
                                          (x/pvc_tw) * pvc_ts + (y/pvc_th) * lumaTotalWidth * pvc_th +
                                          (x % pvc_tw) + (y % pvc_th)*pvc_tw);

                        *(pDesty++) = *pixel;
                    }

                /* break chroma buffer into U & V components */
                for (wxUint32 y = 0 ; y < chromaH ; y++)
                    for (wxUint32 x = 0 ; x < chromaW*2 ; x += 2) {
                        wxUint8* pixel = (pChromaBuffer +\
                                          (x/pvc_tw) * pvc_ts + (y/pvc_th) * chromaTotalWidth * pvc_th +
                                          (x % pvc_tw) + (y % pvc_th)*pvc_tw);

                        *(pDestu++) = *pixel;
                        *(pDestv++) = *(pixel + 1);
                    }

#ifdef  ENABLE_DUMP
                /* test save the Y,U, & V output */

                FILE* fp;
                fp = fopen("/tmp/output/out_0.Y", "w");
                fwrite(m_pY, 1, (m_width * m_height), fp);
                fclose(fp);
                fp = fopen("/tmp/output/out_0.U", "w");
                fwrite(m_pU, 1, (m_width * m_height) >> 1, fp);
                fclose(fp);
                fp = fopen("/tmp/output/out_0.V", "w");
                fwrite(m_pV, 1, (m_width * m_height) >> 1, fp);
                fclose(fp);
#endif

                if (m_imageData != nullptr) {
                    free(m_imageData);
                    m_imageData = nullptr;
                }
                m_imageData = m_yuv.DoConversionToRGB();
                wxASSERT( m_imageData != nullptr );
                bRes = true;
            } else {
                wxLogDebug("ERROR: Invalid file!");
            }

            /* free buffers */
            if (pLumaBuffer != nullptr) {
                free( pLumaBuffer );
                pLumaBuffer = nullptr;
            }

            if (pChromaBuffer != nullptr) {
                free( pChromaBuffer );
                pChromaBuffer = nullptr;
            }

            m_fp.Close();

            m_frameNo = frame;
        } else {
            wxLogDebug("ERROR: Unable to open file!");
        }
    }

    return bRes;
}

PIXEL*		dumpImageBuffer::getPixel(int x, int y) {
    return (PIXEL*)nullptr;
}

/**
 *  Get the frame size.
 */

bool dumpImageBuffer::QueryFrameSize(int& width, int& height) {
    width = m_width;
    height = m_height;
    return true;
}

void dumpImageBuffer::setPath(const wxString& path) {
    wxLogDebug("dumpImageBuffer::setPath(%s)", (const char*)path.c_str());
    m_imagePath = path;
    GetFrameCount();
}

bool dumpImageBuffer::CanQueryFrameSize() const {
    return true;
}

bool dumpImageBuffer::CanChecksum() const {
    return true;
}

bool dumpImageBuffer::CanSave() const {
    return true;
}

ImageBuffer* dumpImageBuffer::Create(ImageBufferParms& parms) {
    return dynamic_cast<ImageBuffer*>(new dumpImageBuffer(parms));
}
