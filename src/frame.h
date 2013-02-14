//==============================================================================
//	MODULE		:	frame.h
//	PROJECT		:	PlayVideoBrowser
//	PROGRAMMER	:	Michael A. Uman
//	DATE		:	February 14, 2006
//	COPYRIGHT	:	(C) 2006 Sigma Designs
//==============================================================================

#ifndef	__FRAME_H__
#define __FRAME_H__

#define	NEW_SCALING		1

#include <wx/wx.h>
#include <wx/timer.h>

#include "dbgutils.h"
#include "imageBuffer.h"
#include "imageScroller.h"

#ifdef	HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef	HAVE_FAM
	#include "wxFAM.h"
#endif

class Frame : public wxFrame {
	public:
		Frame();
		~Frame();

		enum {
			ID_FILE_EXIT	= 0x1000,

			ID_FILE_OPEN,
			ID_FILE_OPEN_SPLIT,
			ID_FILE_OPEN_FILE,
			ID_FILE_OPEN_DUMP,
			ID_FILE_REFRESH,
			ID_FILE_SAVE_AS,
			ID_FILE_SETSIZE,
            ID_FILE_GRID_SETTINGS,

			ID_VIEW_NEXT,
			ID_VIEW_PREV,
			ID_VIEW_GOTO,
			ID_VIEW_GO_HOME,
			ID_VIEW_GO_LAST,

			ID_VIEW_SCALE,
			ID_VIEW_SCALE_ACTUAL,
			ID_VIEW_SCALE_SCALED,
			ID_VIEW_SCALE_HALF,
			ID_VIEW_SCALE_DOUBLE,
			ID_VIEW_SCALE_CUSTOM,

			ID_VIEW_STEP,
			ID_VIEW_TYPE,
			ID_VIEW_420,
			ID_VIEW_422,
			ID_VIEW_444,

            ID_VIEW_CCIR601,

            ID_VIEW_GRID,
            ID_VIEW_FULLSCREEN,

            ID_VIEW_CHECKSUM,

			ID_FRAME_SLIDER,

			ID_HELP_ABOUT,

			ID_TIMER = 0x2000,
			ID_HEART,
		};

		enum yuvType {
			YUV_SPLIT,
			YUV_FILE,
			YUV_DUMP,
		};

		/// Set current frame number.
		void	SetFrameNumber(long frameNo) {
			m_imageID = frameNo;
		}

		/// Set current image path.
		void	SetPath(const wxString& path) {
			m_imagePath = path;
		}

		/// Set image dimensions.
		void	SetDimensions(const wxSize& size) {
			m_imageSize = size;
		}

		/// Return current frame number.
		long	GetFrameNumber() const {
			return m_imageID;
		}

		/// Return the image path.
		wxString GetPath() const {
			return m_imagePath;
		}

		/// Get the image size and store in size.
		void GetDimensions(wxSize& size) {
			size = m_imageSize;
		}

		void	OnClose(wxCloseEvent& event);
		void 	OnExit(wxCommandEvent& event);
		void	OnFileSetSize(wxCommandEvent& event);
		void	OnFileSaveAs(wxCommandEvent& event);
//		void	OnFileSetPrefix(wxCommandEvent& event);
//		void	OnFileSetPath(wxCommandEvent& event);

		void	OnFileOpenSplit(wxCommandEvent& event);
		void	OnFileOpenFile(wxCommandEvent& event);
        void    OnFileOpenDump(wxCommandEvent& event);

		void	OnViewNext(wxCommandEvent& event);
		void 	OnViewPrevious(wxCommandEvent& event);
		void	OnGotoFrame(wxCommandEvent& event);
		void	OnGotoHome(wxCommandEvent& event);
		void	OnGotoLast(wxCommandEvent& event);
		void	OnViewScaleToggle(wxCommandEvent& event);
		void	OnViewScaleCustom(wxCommandEvent& event);
		void 	OnViewAutoStep(wxCommandEvent& event);
		void	OnViewFormat(wxCommandEvent& event);
        void    OnViewFullscreen(wxCommandEvent& event);
        void    OnViewChecksum(wxCommandEvent& event);

		void 	OnHelpAbout(wxCommandEvent& event);

		void	OnTimer(wxTimerEvent& event);
		void 	OnHeartbeat(wxTimerEvent& event);
		void	OnPaint(wxPaintEvent& event);
		void	OnSlider(wxScrollEvent& event);

        void    OnGridChange(wxCommandEvent& event);
        void    OnGridSettings(wxCommandEvent& event);

        void    OnUpdateUI(wxUpdateUIEvent& event);

		/// Load options from config file.
		void	LoadOptions();
		/// Save options to config file.
		void	SaveOptions();

		void	OnSize(wxSizeEvent& event);
		void	OnLoadImage(wxCommandEvent& event);

        void    SaveBitmap(wxString sFilename);

#ifdef	HAVE_FAM
		/// Handle a File Access Monitor event.
		void	OnFAMEvent(wxFAMEvent& event);
#endif
        void            OnRefresh(wxCommandEvent& event);

		/// wxWidgets command dispatch table.
		DECLARE_EVENT_TABLE();

	private:

		bool			GetImage();
		void			GetFrameRange();
		bool			GetImageYSize(size_t* size);
		void			post_process(wxBitmap& bmp);
		void			alpha_blend(wxImage* imgB, wxImage* imgA, int x, int y, wxColor& mask);
		void			UpdateStatusBar(int id);
//		void			GenerateScaledImage();

		bool			allocate_image_buffer();
		void			free_image_buffer();
		void			GetSizeFromPath();

#ifdef  HAVE_FAM
		/// Is File Alteration Monitor enabled
		bool			m_bEnableFAM;
#endif
		/// Current image index
		long			m_imageID;
		/// Current image path
		wxString		m_imagePath;
		/// Current image filename
		wxString		m_imageFilename;
		/// Name prefix {i.e. out}
		wxString		m_prefix;
		/// Size of image
		wxSize			m_imageSize;
		/// Image storage
		wxImage*		m_pImage;
        /// Scaled Image storage
		wxImage*		m_pScaledImage;

		/// Window status bar
		wxStatusBar*	m_statBar;
		/// Enable scale to window size
		long			m_nScale;
		double			m_dScale;

		/// Enable automatic timer advance
		bool			m_bAutoStep;
		/// Window timer
		wxTimer			m_timer;
		wxTimer			m_heartbeat;

		wxSlider*		m_sliderCntrl;

		/// Last frame id in sequence
		int				m_lastID;
		float			m_alpha;
		int				m_depth;
		int				m_maskMask;

		yuvType			m_type;
		ImageBuffer*	m_buffer;
		ImageBuffer::dataType	m_yuvType;

		ImageScroller*	m_pScroller;

#ifdef	HAVE_FAM
		wxFAM			m_fam;
#endif

        wxString        m_sYUVFilename;

        bool            m_bGrid;
        int             m_gridH;     // horizontal spacing
        int             m_gridV;    // vertical spacing
};

DECLARE_EVENT_TYPE(wxEVT_LOADIMAGE, -1)


#endif

