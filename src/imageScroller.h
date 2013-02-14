#ifndef	__IMAGESCROLLER_H__
#define __IMAGESCROLLER_H__

#include <wx/wx.h>
#include <wx/scrolwin.h>

#define		IS_IMAGE_DEFAULT			0
#define		IS_IMAGE_SCALE				1
#define		IS_IMAGE_HALF				2
#define		IS_IMAGE_DOUBLE				3

class ImageScroller : public wxScrolledWindow {
	public:
		ImageScroller() {};
		ImageScroller( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size );
		~ImageScroller();

		void OnPaint(wxPaintEvent& event);

		void set_image(wxImage* pImage, long int scaling= IS_IMAGE_DEFAULT);
		void set_image(wxImage* pImage, double scale);

        void        enable_grid() {
            m_bEnableGrid = true;
            Refresh();
        }

        void        disable_grid() {
            m_bEnableGrid = false;
            Refresh();
        }

        bool        get_grid_state()  {
            return m_bEnableGrid;
        }

        void        set_grid_dimensions(int hor, int ver) {
            m_gridH = hor; m_gridV = ver;
            Refresh();
        }

		DECLARE_DYNAMIC_CLASS(ImageScroller)
		DECLARE_EVENT_TABLE()

		void	OnSize(wxSizeEvent& event);

		inline bool fit_to_frame() const {
			return (m_scale == 0.0)?true:false;
		}


	protected:
		wxImage*	m_pImage;
		int			m_flags;

		double		m_scale;

		bool        m_bEnableGrid;
		int         m_gridH;
		int         m_gridV;
};

#endif

