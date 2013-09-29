/**
 *  @file       DumpImageBuffer.cpp
 *  @author     Michael A. Uman
 *  @date       February 19, 2013
 *  @brief      Buffer class supporting the Sigma 'dump' format.
 */

#include <stdio.h>
#include <gcrypt.h>
#include "imageBuffer.h"
#include "DumpImageBuffer.h"
#include "dumpInternals.h"
#include "crc32.h"
#include "dbgutils.h"

dumpImageBuffer::dumpImageBuffer(int width, int height)
:   ImageBuffer(ImageBuffer::DATA_YUV420, width, height),
    m_frameNo(-1),
    m_lumaSize(0),
    m_chromaSize(0),
    m_pY(0),
    m_pU(0),
    m_pV(0)
{
//constructor
    debug("dumpImageBuffer::dumpImageBuffer(%d, %d)\n", width, height);
    return;
}

dumpImageBuffer::~dumpImageBuffer()
{
//destructor
    debug("dumpImageBuffer::~dumpImageBuffer()\n");

    if (m_pY) {
        free(m_pY);
        m_pY = 0;
    }

    if (m_pU) {
        free(m_pU);
        m_pU = 0;
    }

    if (m_pV) {
        free(m_pV);
        m_pV = 0;
    }

    if (m_imageData) {
        free(m_imageData);
        m_imageData = 0L;
    }

    return;
}

/**
 *  Scan directory looking for all pic_*.rmar files.
 */

ssize_t		dumpImageBuffer::GetFrameCount()
{
    debug("dumpImageBuffer::GetFrameCount()\n");
    ssize_t count = 0;

    m_frameList.clear();
    /* run through all files in directory m_imagePath */

    wxString sFullPath;
    wxString sSpec = m_imagePath + wxT("/pic_*.rmar");

    sFullPath = ::wxFindFirstFile(sSpec, wxFILE);
    while (!sFullPath.empty()) {
        count++;
//      debug("-- found file %s\n", sFullPath.c_str());
        m_frameList.Add( sFullPath );
        sFullPath = ::wxFindNextFile();
    }

    /* the frame list should be sorted */
    m_frameList.Sort();

#ifdef  _ENABLE_DEBUG
    debug("found %d frames:\n", count);
    for (size_t n = 0 ; n < m_frameList.GetCount() ; n++) {
        debug("\t%s\n", (const char*)m_frameList[n].c_str());
    }
#endif  // _ENABLE_DEBUG

    return count;
}

void dumpImageBuffer::GetImage(wxImage* pImage)
{
    debug("dumpImageBuffer::GetImage(0x%08lx)\n", pImage);
    assert(pImage != 0L);

    if (m_imageData != 0L) {
        pImage->Destroy();
        pImage->Create(m_width, m_height);
        pImage->SetData(copy_data()); //m_imageData);
    }

    return;
}

/**
 *  Load frame from dump directory.
 */

bool		dumpImageBuffer::Load(size_t frame)
{
    wxString    sFrameName;
    wxUint8     *pLumaBuffer = 0L,*pChromaBuffer = 0L;
    wxUint32    lumaX = 0 , lumaY = 0, lumaW = 0, lumaH = 0;
    wxUint32    chromaX = 0, chromaY = 0, chromaW = 0, chromaH = 0;
    wxInt32     lumaTabSize = 0, chromaTabSize = 0;
    wxUint32    lumaTotalWidth = 0, chromaTotalWidth = 0;
    wxUint32    bufferW = 0, bufferH = 0;
#if 1
#else
    wxUint8     *pY = 0, *pU = 0, *pV = 0;  /* YUV buffers */
#endif
    enum {
        FOUND_PICI = (1L << 0),
        FOUND_LUMA = (1L << 1),
        FOUND_CHRM = (1L << 2),
    };
    int flags = 0;

    debug("dumpImageBuffer::Load(%ld)\n", frame);

//    debug("-- INFO : Sizeof EMhwlibPictureInfoV1 = %d\n", sizeof(dumpInternals::EMhwlibPictureInfoV1));

    if (frame < m_frameList.GetCount()) {
        sFrameName = m_frameList[frame];
        debug("-- loading frame [%s]\n", (const char*)sFrameName.c_str());

        if (m_fp.Open( sFrameName )) {
            wxUint32 nTag, nVersion;
            wxInt64 lSize;
            /* read file header */
            if (dumpInternals::read_box(&m_fp, nTag, nVersion, lSize) && (nTag == RMAR_MAGIC)) {
                debug("Found RMAR tag!\n");

                while (!m_fp.Eof() && (flags  != (FOUND_PICI|FOUND_LUMA|FOUND_CHRM))) {
                    if (dumpInternals::read_box(&m_fp, nTag, nVersion, lSize)) {

                        if (nTag == PICI_MAGIC) {
                            dumpInternals::EMhwlibPictureInfoV1    *picture_info;

                            debug("-- found picture info [%lld] vs [%lld] !\n", lSize, sizeof(*picture_info));
                            picture_info = (dumpInternals::EMhwlibPictureInfoV1*)malloc( lSize );

                            if (m_fp.Read(picture_info, lSize) != lSize) {
                                debug("ERROR: Unable to read picture info!\n");
                                break;
                            }

                            debug("luma_total_width = %d\n", picture_info->Picture.luma_total_width);
                            debug("chroma_total_width = %d\n", picture_info->Picture.chroma_total_width);
                            debug("scaled_width = %d scaled_height = %d\n", picture_info->Picture.scaled_width, picture_info->Picture.scaled_height);

                            lumaX = picture_info->Picture.luma_position_in_buffer.x;
                            lumaY = picture_info->Picture.luma_position_in_buffer.y;
                            lumaW = picture_info->Picture.luma_position_in_buffer.width;
                            lumaH = picture_info->Picture.luma_position_in_buffer.height;
                            lumaTotalWidth = picture_info->Picture.luma_total_width;

                            chromaX = picture_info->Picture.chroma_position_in_buffer.x;
                            chromaY = picture_info->Picture.chroma_position_in_buffer.y;
                            chromaW = picture_info->Picture.chroma_position_in_buffer.width;
                            chromaH = picture_info->Picture.chroma_position_in_buffer.height;
                            chromaTotalWidth = picture_info->Picture.chroma_total_width;

                            bufferW = ((lumaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                            bufferH = ((lumaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                            lumaTabSize = bufferW * bufferH;

                            debug("Luma buffer Dimensions = %d X %d = %d\n", bufferW, bufferH, lumaTabSize);

                            bufferW = ((chromaW + (1 << RMTILE_WIDTH_SHIFT) -1) >> RMTILE_WIDTH_SHIFT) << RMTILE_WIDTH_SHIFT;
                            bufferH = ((chromaH + (1 << RMTILE_HEIGHT_SHIFT) -1) >> RMTILE_HEIGHT_SHIFT) << RMTILE_HEIGHT_SHIFT;
                            chromaTabSize = bufferW * bufferH * 2;

                            debug("Chroma buffer Dimensions = %d X %d = %d\n", bufferW, bufferH, chromaTabSize);

                            /* allocate buffers */
                            debug("-- allocating buffers!\n");

                            pLumaBuffer     = (wxUint8*)malloc( lumaTabSize );
                            pChromaBuffer   = (wxUint8*)malloc( chromaTabSize );

                            wxASSERT((pLumaBuffer != 0L) && (pChromaBuffer != 0L));

                            m_width  = picture_info->Picture.scaled_width;
                            m_height = picture_info->Picture.scaled_height;

                            debug("width %d height %d\n", m_width, m_height);

#if 1
                            m_pY = (wxUint8*)realloc((void*)m_pY, (m_width * m_height));
                            m_pU = (wxUint8*)realloc((void*)m_pU, (m_width * m_height) >> 2);
                            m_pV = (wxUint8*)realloc((void*)m_pV, (m_width * m_height) >> 2);
#else
                            pY = (wxUint8*)malloc( (m_width) * (m_height) );
                            pU = (wxUint8*)malloc( (m_width * m_height) /* >> 1 */ );
                            pV = (wxUint8*)malloc( (m_width * m_height) /* >> 1 */ );
#endif

                            /* Allocate new image data */
                            m_imageData = (UBYTE *)realloc(m_imageData, (sizeof(PIXEL) * m_width * m_height));
                            wxASSERT(m_imageData != 0L);

                            free(picture_info);
                            flags |= FOUND_PICI;
                        } else if (nTag == LUMA_MAGIC) {
                            debug("-- found luma buffer [%lld]!\n", lSize);

                            if (lumaTabSize != lSize) {
                                debug("WARNING: It seems the luma buffer size is incorrect!\n");
                            }

                            if (m_fp.Read(pLumaBuffer, lumaTabSize) != lumaTabSize) {
                                debug("WARNING: Incomplete buffer read! aborting...\n");
                                break;
                            }
                            flags |= FOUND_LUMA;
                        } else if (nTag == CHROMA_MAGIC) {
                            debug("-- found chroma buffer [%lld]!\n", lSize);

                            if (chromaTabSize != lSize) {
                                debug("WARNING: It seems the chroma buffer size is incorrect!\n");
                            }

                            if (m_fp.Read(pChromaBuffer, chromaTabSize) != chromaTabSize) {
                                debug("WARNING: Incomplete buffer read! aborting...\n");
                                break;
                            }
                            flags |= FOUND_CHRM;
                        } else {
                            debug("-- found unknown tag [%c%c%c%c]!\n",
                                  ((nTag & 0xff000000) >> 24),
                                  ((nTag & 0x00ff0000) >> 16),
                                  ((nTag & 0x0000ff00) >> 8),
                                  ((nTag & 0x000000ff)));
                            m_fp.Seek(lSize, wxFromCurrent);
                        }
                    }
                }
                debug("-- de-tiling buffers!\n");

                wxUint32 pvc_tw = (1 << RMTILE_WIDTH_SHIFT);
                wxUint32 pvc_th = (1 << RMTILE_HEIGHT_SHIFT);
                wxUint32 pvc_ts = pvc_tw*pvc_th;
                wxUint8  *pDesty, *pDestu, *pDestv;

                debug("luma w X h = %d X %d\n", lumaW, lumaH);

#if 1
                pDesty = m_pY; pDestu = m_pU; pDestv = m_pV;
                m_lumaSize = (m_width * m_height);
                m_chromaSize = (m_width * m_height) >> 2;

                debug("-- m_lumaSize %d m_chromaSize %d\n", m_lumaSize, m_chromaSize);
#else
                pDesty = pY; pDestu = pU; pDestv = pV;
#endif

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


                /* test save the Y,U, & V output */

#ifdef  ENABLE_DUMP
                FILE* fp;
                fp = fopen("/tmp/output/out_0.Y", "w");
                fwrite(pY, 1, (m_width * m_height), fp);
                fclose(fp);
                fp = fopen("/tmp/output/out_0.U", "w");
                fwrite(pU, 1, (m_width * m_height) >> 1, fp);
                fclose(fp);
                fp = fopen("/tmp/output/out_0.V", "w");
                fwrite(pV, 1, (m_width * m_height) >> 1, fp);
                fclose(fp);
#endif

#if 1
                if (DoYUVConversion(m_width, m_height, m_pY, m_pU, m_pV)) {
                    debug("OK!\n");
#else
                if (DoYUVConversion(m_width, m_height, pY, pU, pV)) {
                    debug("OK!\n");
#endif
                }
            } else {
                debug("ERROR: Invalid file!\n");
            }

            /* free buffers */
            if (pLumaBuffer) {
                free( pLumaBuffer );
                pLumaBuffer = 0L;
            }
            if (pChromaBuffer) {
                free( pChromaBuffer );
                pChromaBuffer = 0L;
            }

#if 1
//            if (m_pY) {
//                free( m_pY );
//                m_pY = 0L;
//            }
//            if (m_pU) {
//                free( m_pU );
//                m_pU = 0L;
//            }
//            if (m_pV) {
//                free( m_pV );
//                m_pV = 0L;
//            }

#else
            if (pY) {
                free( pY );
                pY = 0L;
            }
            if (pU) {
                free( pU );
                pU = 0L;
            }
            if (pV) {
                free( pV );
                pV = 0L;
            }
#endif

            m_fp.Close();

            m_frameNo = frame;
        } else {
            debug("ERROR: Unable to open file!\n");
        }
    }

    return true;
}

PIXEL*		dumpImageBuffer::getPixel(int x, int y)
{
    return (PIXEL*)0L;
}


bool dumpImageBuffer::DoYUVConversion(int rows, int cols, wxUint8* pY, wxUint8* pU, wxUint8* pV)
{
    long int  		u,v,y0,y1,y2,y3,u0,u1,u2,u3,v0,v1,v2,v3;
    register unsigned char  	*y1buf,*y2buf,*ubuf,*vbuf;
    register PIXEL  *pP1,*pP2;
    int             row, col;
    //bool            result = false;

    debug("DoYUVConversion()\n");

    rows = m_height;
    cols = m_width;

    y1buf = pY;
    y2buf = pY + cols;
    ubuf  = pU;
    vbuf  = pV;

    for (row = 0; row < (rows & ~1); row += 2) {
        unsigned char *y1ptr,*y2ptr,*uptr,*vptr;

        y1ptr = y1buf;
        y2ptr = y2buf;
        vptr = vbuf;
        uptr = ubuf;

        pP1 = (PIXEL *)((unsigned long)m_imageData + (row * m_width * sizeof(PIXEL)));
        pP2 = (PIXEL *)((unsigned long)m_imageData + ((row + 1) * m_width * sizeof(PIXEL)));

        for (col = 0 ; col < (cols & ~1); col += 2) {
            long int r0,g0,b0,r1,g1,b1,r2,g2,b2,r3,g3,b3;

            y0 = (long int) *y1ptr++;
            y1 = (long int) *y1ptr++;
            y2 = (long int) *y2ptr++;
            y3 = (long int) *y2ptr++;

            u = (long int) ((*uptr++) - 128);
            v = (long int) ((*vptr++) - 128);

            if (m_bufType == DATA_YUV422) {
                uptr++;
                vptr++;
            }

            if (m_ccir601) {
                y0 = ((y0-16)*255)/219;
                y1 = ((y1-16)*255)/219;
                y2 = ((y2-16)*255)/219;
                y3 = ((y3-16)*255)/219;

                u  = (u*255)/224 ;
                v  = (v*255)/224 ;
            }

            /* mean the chroma for subsampling */

            u0=u1=u2=u3=u;
            v0=v1=v2=v3=v;

            /* The inverse of the JFIF RGB to YUV Matrix for $00010000 = 1.0

            [Y]   [65496        0   91880][R]
            [U] = [65533   -22580  -46799[G]
            [V]   [65537   116128      -8][B]

            */

            r0 = 65536 * y0               + 91880 * v0;
            g0 = 65536 * y0 -  22580 * u0 - 46799 * v0;
            b0 = 65536 * y0 + 116128 * u0             ;

            r1 = 65536 * y1               + 91880 * v1;
            g1 = 65536 * y1 -  22580 * u1 - 46799 * v1;
            b1 = 65536 * y1 + 116128 * u1             ;

            r2 = 65536 * y2               + 91880 * v2;
            g2 = 65536 * y2 -  22580 * u2 - 46799 * v2;
            b2 = 65536 * y2 + 116128 * u2             ;

            r3 = 65536 * y3               + 91880 * v3;
            g3 = 65536 * y3 -  22580 * u3 - 46799 * v3;
            b3 = 65536 * y3 + 116128 * u3             ;

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

#ifdef	DUMP_RGB
            debug("r0 = 0x%02x g0 = 0x%02x b0 = 0x%02x\n", r0, g0, b0);
            debug("r1 = 0x%02x g1 = 0x%02x b1 = 0x%02x\n", r1, g1, b1);
            debug("r2 = 0x%02x g2 = 0x%02x b2 = 0x%02x\n", r2, g2, b2);
            debug("r3 = 0x%02x g3 = 0x%02x b3 = 0x%02x\n", r3, g3, b3);
#endif	// DUMP_RGB

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

        y1buf += (cols * 2);
        y2buf += (cols * 2);
        ubuf += (cols >> 1);
        vbuf += (cols >> 1);
    } // end row

    return true;
}

/**
 *  Get the frame size.
 */

bool dumpImageBuffer::QueryFrameSize(int& width, int& height)
{
    width = m_width;
    height = m_height;
    return true;
}

/**
 *  Calculate the MD5 checksum for the buffer.
 */

bool dumpImageBuffer::GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum) {
    debug("dumpImageBuffer::GetChecksum(%d, %p, %p)\n", frame, lumaSum, chromaSum);

    //debug("m_pY %p m_pU %p m_pV %p\n", m_pY, m_pU, m_pV);

    memset(lumaSum, 0xff, 16);
    memset(chromaSum, 0xff, 16);

    if (frame != m_frameNo) {
        debug("-- loading frame %d\n", frame);
        Load(frame);
    }

    if ((m_pY != 0) && (m_pU != 0) && (m_pV != 0)) {
        gcry_md_hd_t    con;

        gcry_md_open(&con, GCRY_MD_MD5, 0);

        gcry_md_write(con, m_pY, m_lumaSize);
        gcry_md_final(con);

        memcpy(lumaSum, gcry_md_read(con, GCRY_MD_MD5), 16);

        gcry_md_reset(con);

        for (wxUint32 i = 0 ; i < m_chromaSize ; i++) {
            gcry_md_putc(con, m_pU[i]);
            gcry_md_putc(con, m_pV[i]);
        }

        gcry_md_final(con);
        memcpy(chromaSum, gcry_md_read(con, GCRY_MD_MD5), 16);

        gcry_md_close(con);
    } else {
        memset(lumaSum, 0xff, 16);
        memset(chromaSum, 0xff, 16);
    }

#if 1
    wxUint32 crcLuma = crc32_be(m_pY, m_lumaSize);
    wxUint32 crcChroma = crc32_be(m_pU, m_chromaSize, m_pV, m_chromaSize);

    printf("sigma crc32:\n");
    printf("crcLuma  = %08x\n", crcLuma);
    printf("crcChoma = %08x\n", crcChroma);

#endif

    return true;
}

bool dumpImageBuffer::SaveYUV(wxString sFilename, eSaveType type) {
    bool bRes = false;
    debug("dumpImageBuffer::SaveYUV(%s, %d)\n", (const char*)sFilename.c_str(), (int)type);

    wxASSERT( (m_pY != 0) && (m_pU != 0) && (m_pV != 0) );

    if (type == SAVE_YUV_COMP) {
        wxFile      fp;

        if (fp.Open( sFilename, wxFile::write )) {
            fp.Write( m_pY, m_lumaSize );
            fp.Write( m_pU, m_chromaSize );
            fp.Write( m_pV, m_chromaSize );
        }
    } else if (type == SAVE_YUV_SPLIT) {

    } else {
        debug("ERROR: Invalid save type!\n");
    }

    return bRes;
}

/*


#if 0
    {
        gcry_md_hd_t    con;
        wxUint32        gc_lumaCRC, gc_chromaCRC;

        gcry_md_open(&con, GCRY_MD_CRC32_RFC1510, 0);
        gcry_md_write(con, m_pY, m_lumaSize);
        gcry_md_final(con);

        memcpy(&gc_lumaCRC, gcry_md_read(con, GCRY_MD_CRC32_RFC1510), 4);

        gcry_md_reset(con);

        for (wxUint32 i = 0 ; i < m_chromaSize ; i++) {
            gcry_md_putc(con, m_pU[i]);
            gcry_md_putc(con, m_pV[i]);
        }

        gcry_md_final(con);

        memcpy(&gc_chromaCRC, gcry_md_read(con, GCRY_MD_CRC32_RFC1510), 4);

        gcry_md_close(con);

        printf("gcrypt crc32:\n");
        printf("gc_lumaCRC   = %08x\n", gc_lumaCRC);
        printf("gc_chromaCRC = %08x\n", gc_chromaCRC);
    }
#endif

*/
