#include <wx/wx.h>
#include "imageScroller.h"
#include "dbgutils.h"

//#define	LOAD_IMAGE		1

IMPLEMENT_DYNAMIC_CLASS( ImageScroller, wxScrolledWindow )

BEGIN_EVENT_TABLE( ImageScroller, wxScrolledWindow )
	EVT_PAINT( ImageScroller::OnPaint )
//	EVT_SIZE ( ImageScroller::OnSize )
END_EVENT_TABLE()

ImageScroller::ImageScroller(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
:	wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER, _T("canvas")),
	m_pImage(0L),
	m_bEnableGrid(false),
	m_gridH(16),
	m_gridV(16)
{
//	int xoff, yoff;
	SetScrollRate( 1,1 );
	SetBackgroundColour( wxT("LIGHT STEEL BLUE") );
}

ImageScroller::~ImageScroller() {
	/* delete the image */
	if (m_pImage) {
		delete m_pImage;
		m_pImage = 0L;
	}
}


void ImageScroller::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc( this );
    PrepareDC( dc );

//    OutputDebugString("OnPaint()\n");

	wxSize		size;
	wxBitmap 	bmp;
	int			xoff = 0, yoff = 0;

	size = GetClientSize();

	if (m_pImage) {
		bmp = wxBitmap( *m_pImage );

		/* calculate offsets to center image in window */
		if (size.GetWidth() > bmp.GetWidth()) {
			xoff = (size.GetWidth() - bmp.GetWidth()) / 2;
		}

		if (size.GetHeight() > bmp.GetHeight()) {
			yoff = (size.GetHeight() - bmp.GetHeight()) / 2;
		}

        dc.DrawBitmap( bmp, xoff, yoff );

		if (m_bEnableGrid) {
            int inc;

            if (m_scale == 0.0) {
                inc = m_gridH;
            } else {
                inc = (int)(m_scale * m_gridH);
            }

            dc.SetPen(*wxBLACK_PEN);
            for (int x = xoff ; x < (xoff + bmp.GetWidth()) ; x += inc) {
                dc.DrawLine(x, yoff, x, yoff + bmp.GetHeight());
                debug("x=%d\n", x);
            }

            if (m_scale == 0.0) {
                inc = m_gridV;
            } else {
                inc = (int)(m_scale * m_gridV);
            }

            for (int y = yoff ; y < (yoff + bmp.GetHeight()) ; y += inc) {
                dc.DrawLine(xoff, y, xoff + bmp.GetWidth(), y);
                debug("y=%d\n", y);
            }
		}
	}
}

void ImageScroller::set_image(wxImage* pImage, long scaling) {
	wxSize			size = GetClientSize();

	debug("ImageScroller::set_image()\n");
	debug("client size = %d x %d\n", size.GetWidth(), size.GetHeight());

	/* make sure the image pointer is set */
	if (pImage == 0L) {
//		debug("ERROR: set_image called with NULL pointer!\n");
		delete m_pImage;
		m_pImage = 0L;
		return;
	}

	/* if image exists, delete it... */
	if (m_pImage) {
		delete m_pImage;
		m_pImage = 0L;
	}


	double	scaleVector[] = {
		1.0,	/* IS_IMAGE_DEFAULT */
		0.0,	/* IS_IMAGE_SCALE   */
		0.5,	/* IS_IMAGE_HALF    */
		2.0,	/* IS_IMAGE_DOUBLE  */
	};

	m_scale = scaleVector[scaling];

	if (m_scale == 0.0) {
		int image_w 	= pImage->GetWidth(),
			image_h 	= pImage->GetHeight();
		int window_w	= size.GetWidth();
		int window_h 	= size.GetHeight();
		int scale_w, scale_h;

		scale_w = (int)(((double)window_h / (double)image_h) * (double)image_w);
		scale_h = (int)(((double)window_w / (double)image_w) * (double)image_h);

		if (scale_w > window_w) {
			scale_w = window_w;
		}
		if (scale_h > window_h) {
			scale_h = window_h;
		}

		m_pImage = new wxImage(pImage->Scale(scale_w, scale_h));
	} else {
		int image_w 	= (int)((double)pImage->GetWidth() * m_scale),
			image_h 	= (int)((double)pImage->GetHeight() * m_scale);

		m_pImage = new wxImage(pImage->Scale(image_w, image_h));
	}

	SetVirtualSize( m_pImage->GetWidth(), m_pImage->GetHeight() );

	wxSize	wndSize = GetClientSize();
	debug("Scroller Size = %d x %d\n", wndSize.GetWidth(), wndSize.GetHeight());

	Refresh(false);
}

void ImageScroller::set_image(wxImage* pImage, double scale) {
	wxSize			size = GetClientSize();

	debug("ImageScroller::set_image()\n");
	debug("client size = %d x %d\n", size.GetWidth(), size.GetHeight());

	/* make sure the image pointer is set */
	if (pImage == 0L) {
//		debug("ERROR: set_image called with NULL pointer!\n");
		delete m_pImage;
		m_pImage = 0L;
		return;
	}

	/* if image exists, delete it... */
	if (m_pImage) {
		delete m_pImage;
		m_pImage = 0L;
	}

	m_scale = scale;

	if (m_scale == 0.0) {
		int image_w 	= pImage->GetWidth(),
			image_h 	= pImage->GetHeight();
		int window_w	= size.GetWidth();
		int window_h 	= size.GetHeight();
		int scale_w, scale_h;

		scale_w = (int)(((double)window_h / (double)image_h) * (double)image_w);
		scale_h = (int)(((double)window_w / (double)image_w) * (double)image_h);

		if (scale_w > window_w) {
			scale_w = window_w;
		}
		if (scale_h > window_h) {
			scale_h = window_h;
		}

		m_pImage = new wxImage(pImage->Scale(scale_w, scale_h));
	} else {
		int image_w 	= (int)((double)pImage->GetWidth() * m_scale),
			image_h 	= (int)((double)pImage->GetHeight() * m_scale);

		m_pImage = new wxImage(pImage->Scale(image_w, image_h));
	}

	SetVirtualSize( m_pImage->GetWidth(), m_pImage->GetHeight() );

	wxSize	wndSize = GetClientSize();
	debug("Scroller Size = %d x %d\n", wndSize.GetWidth(), wndSize.GetHeight());

	Refresh(false);
}

void ImageScroller::OnSize(wxSizeEvent& event) {
	debug("ImageScroller::OnSize()\n");

	wxSize	wndSize = GetClientSize();
	debug("Scroller Size = %d x %d\n", wndSize.GetWidth(), wndSize.GetHeight());

}

