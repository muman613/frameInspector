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
#include <gcrypt.h>
#include "imageBuffer.h"
#include "YUV420ImageBufferFile.h"
#include "misc_utils.h"

/**
 *
 */

YUV420ImageBufferFile::YUV420ImageBufferFile(int x, int y)
    :	ImageBuffer(ImageBuffer::DATA_YUV420, x, y)
{
    debug("YUV420ImageBufferFile::YUV420ImageBufferFile()\n");
}

/**
 *
 */

YUV420ImageBufferFile::~YUV420ImageBufferFile()
{
    debug("YUV420ImageBufferFile::~YUV420ImageBufferFile()\n");

    m_file.Close();

    free_buffer();
//    if (m_imageData) {
//        free(m_imageData);
//        m_imageData = 0L;
//    }
}

/**
 *
 */

bool YUV420ImageBufferFile::SetFilename(const wxString& filename)
{
    bool	result = false;

    m_file.Close();

    if (m_file.Exists(filename.c_str()) && m_file.Open(filename.c_str())) {
        debug("-- file opened!\n");
        m_imageFilename = filename;
        result = true;
        m_lastError = ERROR_NO_ERROR;
    } else {
        m_lastError = ERROR_FILE_NOT_FOUND;
    }

    return result;
}

/**
 *
 */

bool YUV420ImageBufferFile::Load(size_t frame)
{
    debug("YUV420ImageBufferFile::Load(%d)\n", frame);

    int				ySize, uSize, vSize;
    int				frameSize = 0;
    wxFileOffset 	        offY, offU, offV;
    bool			result = false;

    if (frame == (size_t)-1) {
        debug("-- Load called with frame == -1\n");
        m_lastError = ERROR_OUT_OF_BOUNDS;
        return false;
    }

    ySize 		= m_width * m_height;
    uSize 		= ySize / 4;
    vSize 		= ySize / 4;
    frameSize 	        = ySize + uSize + vSize;
    offY 		= (wxFileOffset)frameSize * (wxFileOffset)frame;
    offU 		= offY + ySize;
    offV 		= offU + uSize;

    debug("framesize = %d\n", frameSize);

    /* If image data already exists, free it */
    if (m_imageData != 0L) {
        debug("-- freeing old image data (0x%08lx)\n", m_imageData);
        free_buffer();
    }

    if (!m_file.IsOpened()) {
        debug("ERROR: File is not opened!\n");
        m_lastError = ERROR_NOT_OPEN;
        return false;
    }

    /* Allocate new image data */
    m_imageData = (UBYTE *)malloc(sizeof(PIXEL) * m_width * m_height);
    assert(m_imageData != 0L);

    debug("Image data @ 0x%08lx\n", m_imageData);


    long int  		u,v,y0,y1,y2,y3,u0,u1,u2,u3,v0,v1,v2,v3;
    unsigned char  	*y1buf,*y2buf,*ubuf,*vbuf;
    register PIXEL  *pP1,*pP2;
    int             rows, cols, row, col;

    y1buf = (unsigned char *) pm_allocrow( m_width, 1 );
    y2buf = (unsigned char *) pm_allocrow( m_width, 1 );
    ubuf  = (unsigned char *) pm_allocrow( m_width, 1 );
    vbuf  = (unsigned char *) pm_allocrow( m_width, 1 );

    rows = m_height;
    cols = m_width;

    for (row = 0; row < (rows & ~1); row += 2) {
        unsigned char *y1ptr,*y2ptr,*uptr,*vptr;

        /* Read the Y values */
//		m_file.Seek(off + (row * cols));
        //printf("seeking to %lld\n", offY + (row * cols));

        m_file.Seek(offY + (row * cols));
        m_file.Read(y1buf, (cols & ~1));
        m_file.Read(y2buf, (cols & ~1));

        /* Read the U values */
//		m_file.Seek(off + (row * cols) / 4 + ySize);
        m_file.Seek(offU + (row * cols) / 4);
        m_file.Read(ubuf, (cols/2));

        /* Read the V values */
//		m_file.Seek(off + (row * cols) / 4 + ySize + uSize);
        m_file.Seek(offV + (row * cols) / 4);
        m_file.Read(vbuf, (cols/2));

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
    } // end row

    result = true;

    /* free buffers */
    if (y1buf)
        free(y1buf);
    if (y2buf)
        free(y2buf);
    if (ubuf)
        free(ubuf);
    if (vbuf)
        free(vbuf);

    m_lastError = ERROR_NO_ERROR;

    return result;
}

/**
 *
 */

void YUV420ImageBufferFile::GetImage(wxImage* pImage)
{
    debug("YUV420ImageBufferFile::GetImage(0x%08lx)\n", pImage);
    assert(pImage != 0L);

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

ssize_t YUV420ImageBufferFile::GetFrameCount()
{
	debug("YUV420ImageBufferFile::GetFrameCount(%s)\n", (const char*)m_imageFilename.c_str());
    int	         ySize, uSize, vSize;
    int	         frameSize = 0;
    wxFileOffset off;
    ssize_t	 result = -1;

    ySize = m_width * m_height;
    uSize = ySize / 4;
    vSize = ySize / 4;
    frameSize = ySize + uSize + vSize;

    debug("framesize = %d\n", frameSize);

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


PIXEL* YUV420ImageBufferFile::getPixel(int x, int y)
{
    return (PIXEL*)0L;
}

/**
 *
 */

bool YUV420ImageBufferFile::GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum)
{
    int                 ySize, uSize, vSize;
    int                 frameSize = 0;
    wxFileOffset        offY, offU, offV;
    bool                result = false;
    wxUint8             *yBuf = 0, *uBuf = 0, *vBuf = 0;
    gcry_md_hd_t        con;

    debug("YUV420ImageBufferFile::GetChecksum(%d, %08x, %08x)\n", frame, lumaSum, chromaSum);

    if (frame == (size_t)-1) {
        debug("-- Load called with frame == -1\n");
        m_lastError = ERROR_OUT_OF_BOUNDS;
        return false;
    }

    ySize 		= m_width * m_height;
    uSize 		= ySize / 4;
    vSize 		= ySize / 4;
    frameSize 	= ySize + uSize + vSize;
    offY 		= (wxFileOffset)frameSize * (wxFileOffset)frame;
    offU 		= offY + ySize;
    offV 		= offU + uSize;

    yBuf        = new wxUint8[ ySize ];
    uBuf        = new wxUint8[ uSize ];
    vBuf        = new wxUint8[ vSize ];

    debug("ySize = %d uSize = %d vSize = %d framesize = %d\n", ySize, uSize, vSize, frameSize);

    if (!m_file.IsOpened()) {
        debug("ERROR: File is not opened!\n");
        m_lastError = ERROR_NOT_OPEN;
        return false;
    }


    gcry_md_open(&con, GCRY_MD_MD5, 0);

    m_file.Seek( offY );
    m_file.Read( yBuf, ySize );

    gcry_md_write(con, yBuf, ySize);
    gcry_md_final(con);
    memcpy(lumaSum, gcry_md_read(con, GCRY_MD_MD5), 16);

    gcry_md_reset(con);

    m_file.Seek( offU );
    m_file.Read( uBuf, uSize );

    m_file.Seek( offV );
    m_file.Read( vBuf, vSize );

    for (int i = 0 ; i < uSize ; i++) {
        gcry_md_putc(con, uBuf[i]);
        gcry_md_putc(con, vBuf[i]);
    }

    gcry_md_final(con);
    memcpy(chromaSum, gcry_md_read(con, GCRY_MD_MD5), 16);
    gcry_md_close(con);

    delete [] yBuf;
    delete [] uBuf;
    delete [] vBuf;

    result = true;


    return result;
}


