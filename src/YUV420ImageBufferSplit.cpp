//==============================================================================
//	MODULE		:	YUV420ImageBufferSplit.cpp
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
#include "imageBuffer.h"
#include "YUV420ImageBufferSplit.h"

YUV420ImageBufferSplit::YUV420ImageBufferSplit(int x, int y)
    :	ImageBuffer(ImageBuffer::DATA_YUV420, x, y)
{
    debug("YUV420ImageBufferSplit::YUVImageBuffer(%d, %d)\n", x, y);

#ifdef	OLDWAY
    m_imageData = (UBYTE *)malloc(sizeof(PIXEL) * x * y);
    assert(m_imageData != 0L);

    debug("Image data @ 0x%08lx\n", m_imageData);
#endif


    return;
}

YUV420ImageBufferSplit::~YUV420ImageBufferSplit()
{
    debug("YUV420ImageBufferSplit::~YUV420ImageBufferSplit()\n");

    if (m_imageData) {
        free(m_imageData);
        m_imageData = 0L;
    }

    return;
}

bool YUV420ImageBufferSplit::Load(size_t frame)
{
    /* create the image filename from the path and frame #. */
    wxString	imageFilename = wxString::Format(_T("%s/%s_%ld"),
                                m_imagePath.c_str(),
                                m_prefix.c_str(),
                                frame);

    return Load((const char *)imageFilename.c_str());
}

bool YUV420ImageBufferSplit::Load(const char* filename)
{
    bool			result = false;
    char*			fullName = 0L;
    FILE			*yfp = 0L, *ufp = 0L, *vfp = 0L;
    long int  		u,v,y0,y1,y2,y3,u0,u1,u2,u3,v0,v1,v2,v3;
    unsigned char  	*y1buf = 0L,*y2buf = 0L,*ubuf = 0L,*vbuf = 0L;
    register PIXEL  *pP1 = 0L,*pP2 = 0L;
    int             rows, cols, row, col;

    debug("YUV420ImageBufferSplit::Load(%s)\n", filename);

    /* If image data already exists, free it */
    if (m_imageData != 0L) {
        debug("-- freeing old image data (0x%08lx)\n", m_imageData);

        free(m_imageData);
        m_imageData = 0L;
    }

    /* Allocate new image data */
    m_imageData = (UBYTE *)malloc(sizeof(PIXEL) * m_width * m_height);
    assert(m_imageData != 0L);

    debug("Image data @ 0x%08lx\n", m_imageData);

    fullName = (char *)malloc(strlen(filename) + 3);

    /* open Y, U, & V files */
    strcpy(fullName, filename);
    strcat(fullName, ".Y");
    debug("Y File = %s\n", fullName);
    yfp = fopen(fullName, "rb");

    strcpy(fullName, filename);
    strcat(fullName, ".U");
    debug("U File = %s\n", fullName);
    ufp = fopen(fullName, "rb");

    strcpy(fullName, filename);
    strcat(fullName, ".V");
    debug("V File = %s\n", fullName);
    vfp = fopen(fullName, "rb");

    /* make sure the file is opened! */
    if ((yfp == 0L) || (ufp == 0L) || (vfp == 0L)) {
        debug("ERROR: Unable to open file [Y = %ld U = %ld V = %ld]\n", yfp, ufp, vfp);
        return false;
    }

    if (m_bufType == DATA_YUV444) {
        /* implement yuv444 here */
        y1buf = (unsigned char *) pm_allocrow( m_width, 1 );
        ubuf  = (unsigned char *) pm_allocrow( m_width, 1 );
        vbuf  = (unsigned char *) pm_allocrow( m_width, 1 );

        rows = m_height;
        cols = m_width;


        for (row = 0; row < (rows & ~1); row++) {
            unsigned char *y1ptr,*uptr,*vptr;
            size_t bRead;

            bRead = fread(y1buf, (cols & ~1), 1, yfp);
            if (bRead == 0) {
                debug("ERROR: Read past end of file!\n");
                goto exitLoadYUV444;
            }
            bRead = fread(ubuf, (cols & ~1), 1, ufp);
            if (bRead == 0) {
                debug("ERROR: Read past end of file!\n");
                goto exitLoadYUV444;
            }
            bRead = fread(vbuf, (cols & ~1), 1, vfp);
            if (bRead == 0) {
                debug("ERROR: Read past end of file!\n");
                goto exitLoadYUV420;
            }

            y1ptr = y1buf;
            vptr = vbuf;
            uptr = ubuf;

            pP1 = (PIXEL *)((unsigned long)m_imageData + (row * m_width * sizeof(PIXEL)));

            for (col = 0 ; col < (cols & ~1); col++) {
                long int r0,g0,b0; //,r1,g1,b1,r2,g2,b2,r3,g3,b3;

                y0 = (long int) *y1ptr++;
                u = (long int) ((*uptr++) - 128);
                v = (long int) ((*vptr++) - 128);

                r0 = 65536 * y0               + 91880 * v;
                g0 = 65536 * y0 -  22580 * u  - 46799 * v;
                b0 = 65536 * y0 + 116128 * u             ;

                r0 = limit(r0);
                g0 = limit(g0);
                b0 = limit(b0);

                PPM_ASSIGN(*pP1, r0, g0, b0);
                pP1++;

            }
        }

        result = true;

exitLoadYUV444:

        /* free buffers */
        if (y1buf)
            free(y1buf);
        if (ubuf)
            free(ubuf);
        if (vbuf)
            free(vbuf);

    } else {
        size_t bRead;

        y1buf = (unsigned char *) pm_allocrow( m_width, 1 );
        y2buf = (unsigned char *) pm_allocrow( m_width, 1 );
        ubuf  = (unsigned char *) pm_allocrow( m_width, 1 );
        vbuf  = (unsigned char *) pm_allocrow( m_width, 1 );

        rows = m_height;
        cols = m_width;

        for (row = 0; row < (rows & ~1); row += 2) {
            unsigned char *y1ptr,*y2ptr,*uptr,*vptr;

            bRead = fread(y1buf, (cols & ~1), 1, yfp);
            if (bRead == 0) {
                debug("ERROR: Read past end of file!\n");
                goto exitLoadYUV420;
            }
            bRead = fread(y2buf, (cols & ~1), 1, yfp);
            if (bRead == 0) {
                debug("ERROR: Read past end of file!\n");
                goto exitLoadYUV420;
            }

            if (m_bufType == DATA_YUV420) {
                bRead = fread(ubuf, cols/2, 1, ufp);
                wxASSERT(bRead != 0L);
                bRead = fread(vbuf, cols/2, 1, vfp);
                wxASSERT(bRead != 0L);
            } else {
                bRead = fread(ubuf, (cols & ~1), 1, ufp);
                wxASSERT(bRead != 0L);
                bRead = fread(vbuf, (cols & ~1), 1, vfp);
                wxASSERT(bRead != 0L);
            }

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

exitLoadYUV420:

        /* free buffers */
        if (y1buf)
            free(y1buf);
        if (y2buf)
            free(y2buf);
        if (ubuf)
            free(ubuf);
        if (vbuf)
            free(vbuf);
    }

    /* close all files */
    fclose(vfp);
    fclose(ufp);
    fclose(yfp);

    if (fullName)
        free(fullName);

    if (result == false) {
        debug("-- freeing image data!\n");
        free(m_imageData);
        m_imageData = 0L;
    }

    return result;
}

PIXEL* YUV420ImageBufferSplit::getPixel(int x, int y)
{
    size_t	offset = ((y * sizeof(PIXEL) * m_width) + (x * sizeof(PIXEL)));

    return (PIXEL *)&m_imageData[offset];
}

void YUV420ImageBufferSplit::GetImage(wxImage* pImage)
{
    debug("YUV420ImageBufferSplit::GetImage(0x%08lx)\n", pImage);
    assert(pImage != 0L);

    pImage->Destroy();
    pImage->Create(m_width, m_height);
    pImage->SetData(copy_data()); //m_imageData);

    return;
}

ssize_t YUV420ImageBufferSplit::GetFrameCount()
{
    size_t	 		frameID = 0L;
    wxFileName		framePath;
    bool			bDone = false;

    debug("GetFrameCount()\n");

    while (!bDone) {
        wxString	name;
        int			count = 0;

        name = wxString::Format( _T("%s/%s_%ld.Y"),
                                 m_imagePath.c_str(),
                                 m_prefix.c_str(),
                                 frameID );

        framePath.Assign(name);

        if (framePath.FileExists()) {
            count++;
        }

        name = wxString::Format( _T("%s/%s_%ld.U"),
                                 m_imagePath.c_str(),
                                 m_prefix.c_str(),
                                 frameID );

        framePath.Assign(name);

        if (framePath.FileExists()) {
            count++;
        }

        name = wxString::Format( _T("%s/%s_%ld.V"),
                                 m_imagePath.c_str(),
                                 m_prefix.c_str(),
                                 frameID );

        framePath.Assign(name);

        if (framePath.FileExists()) {
            count++;
        }

//		debug("%d %d\n", frameID, count);

        if (count == 3) {
            frameID++;
        } else {
            bDone = true;
            frameID--;
        }
    }

    return frameID;
}
