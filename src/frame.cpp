//==============================================================================
//	MODULE		:	frame.cpp
//	PROJECT		:	PlayVideoBrowser
//	PROGRAMMER	:	Michael A. Uman
//	DATE		:	February 14, 2006
//	COPYRIGHT	:	(C) 2006 Sigma Designs
//==============================================================================

//#define ALPHA_BLENDING		1
//#define _ENABLE_DEBUG		1
//#define	DEBUG_PAINT		1
//#define	DEBUG_HB		1
//#define   MODAL_CHECKSUM  1

//#ifdef HAVE_CONFIG_H
//	#include "config.h"
//#endif

#include <typeinfo>
#include <limits.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/wfstream.h>
#include <wx/numdlg.h>
#include <wx/dirdlg.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/artprov.h>
#include <wx/regex.h>
#include <wx/filedlg.h>
#include <wx/aboutdlg.h>
#include <wx/tglbtn.h>
#ifdef	ALPHA_BLENDING
#include <xmmintrin.h>
#endif
#include "viewApp.h"
#include "frame.h"
#include "imageBuffer.h"
#include "YUV420ImageBufferSplit.h"
#include "YUV420ImageBufferFile.h"
#include "DumpImageBuffer.h"
#include "GridSettingsDialog.h"
#include "cpupack.h"
#include "misc_utils.h"
#include "hashDialog.h"
#include "SizeDialog.h"
#include "crc32.h"

#include "../bitmaps/mag_small.xpm"
#include "../bitmaps/grid-small.xpm"

/* size of info box */
#define	INFO_WIDTH		128
#define	INFO_HEIGHT		40


DEFINE_EVENT_TYPE(wxEVT_LOADIMAGE)

BEGIN_EVENT_TABLE(Frame, wxFrame)
    /* menu events */
    EVT_MENU(Frame::ID_FILE_OPEN_SPLIT,		Frame::OnFileOpenSplit)
    EVT_MENU(Frame::ID_FILE_OPEN_FILE,		Frame::OnFileOpenFile)
    EVT_MENU(Frame::ID_FILE_OPEN_DUMP,      Frame::OnFileOpenDump)
    EVT_MENU(Frame::ID_FILE_REFRESH,        Frame::OnRefresh)
    EVT_MENU(Frame::ID_FILE_SETSIZE, 		Frame::OnFileSetSize)
#ifdef  OLD_SAVE
    EVT_MENU(Frame::ID_FILE_SAVE_AS,		Frame::OnFileSaveAs)
#else
	EVT_MENU(Frame::ID_FILE_SAVE_IMAGE,		Frame::OnFileSaveAs)
	EVT_MENU(Frame::ID_FILE_SAVE_YUV_SPLIT, Frame::OnFileSaveYUVSplit)
	EVT_MENU(Frame::ID_FILE_SAVE_YUV_COMP,  Frame::OnFileSaveYUVComp)

#endif
    EVT_MENU(Frame::ID_FILE_GRID_SETTINGS,  Frame::OnGridSettings)
    EVT_MENU(Frame::ID_FILE_EXIT, 			Frame::OnExit)

    EVT_MENU(Frame::ID_VIEW_NEXT, 			Frame::OnViewNext)
    EVT_MENU(Frame::ID_VIEW_PREV, 			Frame::OnViewPrevious)
    EVT_MENU(Frame::ID_VIEW_GOTO, 			Frame::OnGotoFrame)
    EVT_MENU(Frame::ID_VIEW_GO_HOME,		Frame::OnGotoHome)
    EVT_MENU(Frame::ID_VIEW_GO_LAST,        Frame::OnGotoLast)

    EVT_MENU_RANGE(Frame::ID_VIEW_SCALE_ACTUAL, Frame::ID_VIEW_SCALE_DOUBLE, Frame::OnViewScaleToggle)
    EVT_MENU(Frame::ID_VIEW_SCALE_CUSTOM,	Frame::OnViewScaleCustom)
    EVT_MENU(Frame::ID_VIEW_STEP,			Frame::OnViewAutoStep)
    EVT_MENU(Frame::ID_VIEW_GRID,           Frame::OnGridChange)
    EVT_MENU(Frame::ID_VIEW_420,			Frame::OnViewFormat)
    EVT_MENU(Frame::ID_VIEW_422,			Frame::OnViewFormat)
    EVT_MENU(Frame::ID_VIEW_444,            Frame::OnViewFormat)
    EVT_MENU(Frame::ID_VIEW_FULLSCREEN,     Frame::OnViewFullscreen)
    EVT_MENU(Frame::ID_VIEW_CHECKSUM,       Frame::OnViewChecksum)

    EVT_MENU(Frame::ID_HELP_ABOUT,			Frame::OnHelpAbout)

    /* slider control events */
    EVT_COMMAND_SCROLL(Frame::ID_FRAME_SLIDER, Frame::OnSlider)

    /* system events */
    EVT_TIMER(Frame::ID_TIMER, Frame::OnTimer)

    EVT_COMMAND( wxID_ANY, wxEVT_LOADIMAGE, Frame::OnLoadImage)

#ifdef ALPHA_BLENDING
    EVT_TIMER(Frame::ID_HEART, Frame::OnHeartbeat)
#endif

    EVT_SIZE(Frame::OnSize)
    EVT_CLOSE(Frame::OnClose)
//	EVT_PAINT(Frame::OnPaint)

#ifdef	HAVE_FAM
    EVT_FAM(Frame::OnFAMEvent)
#endif

    EVT_UPDATE_UI(Frame::ID_VIEW_GRID,      Frame::OnUpdateGridUI)
    EVT_UPDATE_UI(Frame::ID_VIEW_CHECKSUM, Frame::OnUpdateUI)
    EVT_UPDATE_UI(Frame::ID_FILE_SETSIZE,   Frame::OnUpdateUI)
    EVT_UPDATE_UI_RANGE(Frame::ID_FILE_SAVE_IMAGE, Frame::ID_FILE_SAVE_YUV_COMP, Frame::OnUpdateUI)
//	EVT_MENU(Frame::ID_FILE_SETPATH, 		Frame::OnFileSetPath)
//	EVT_MENU(Frame::ID_FILE_SETPREFIX,		Frame::OnFileSetPrefix)
END_EVENT_TABLE()

#if defined(_ENABLE_DEBUG) && defined(ALPHA_BLENDING)

static void show_results_m128(char* msg, __m128 result)
{
    float*	pData = (float *)&result;

    debug("%s = %f %f %f %f\n", msg, pData[0], pData[1], pData[2], pData[3]);

}

#endif

Frame::Frame()
    :	wxFrame(NULL, -1, wxT(APP_NAME), wxDefaultPosition, wxSize(640, 540)),
        m_imageID(0L),
        m_imagePath(wxT("")),
        m_prefix(wxT("out")),
        m_imageSize(wxSize(640,480)),
        m_pImage(0L),
        m_pScaledImage(0L),
        m_statBar(0L),
        m_nScale(IS_IMAGE_DEFAULT),
        m_bAutoStep(false),
        m_lastID(-1),
        m_alpha(1.0),
        m_type(YUV_SPLIT),
        m_buffer(0L),
#ifdef	HAVE_FAM
        m_yuvType(ImageBuffer::DATA_YUV420),
        m_fam(this),
#else
        m_yuvType(ImageBuffer::DATA_YUV420),
#endif
        m_bGrid(false),
        m_gridH(16),
        m_gridV(16),
    	m_gridColor("BLUE"),
        m_pHashDlg(0L)
{
    debug("Frame::Frame()\n");

#ifdef	HAVE_FAM
    m_fam.start();
#endif

#if 1
    crc32_be_init();
#endif

    wxBitmap*   pToolBM  = 0L;
    wxToolBar*	pToolBar = 0L;
    wxMenuBar*	pMenuBar = 0L;
    wxMenu*		pMenu    = 0L;
    wxMenu*		subMenu	 = 0L;

    /* create tool bar */
	pToolBar = CreateToolBar(wxTB_DEFAULT_STYLE);
//	pToolBar->AddTool(ID_FILE_SETPATH,	 wxT("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR), wxT("Set Path"));
	pToolBar->AddTool(ID_FILE_SAVE_IMAGE, wxT("Save"),
                      wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR),
                      wxT("Save image"));
    pToolBar->AddTool(ID_VIEW_GOTO, wxT("Goto Frame"),
                      wxArtProvider::GetBitmap(wxART_FIND, wxART_TOOLBAR),
                      wxT("Goto frame"));

    pToolBM = new wxBitmap( grid_small );
#if 1
    pToolBar->AddCheckTool(ID_VIEW_GRID, "Toggle Grid", *pToolBM , wxNullBitmap, "Enable/Disable Grid" );
#else
    wxBitmapToggleButton* pNewButton = new wxBitmapToggleButton(pToolBar, ID_VIEW_GRID, *pToolBM );
    pNewButton->Connect( ID_VIEW_GRID, wxEVT_MENU, wxCommandEventHandler(Frame::OnGridChange));
    pToolBar->AddControl(pNewButton, "toggle grid");
#endif
    delete pToolBM;
    pToolBar->AddSeparator();

#if wxCHECK_VERSION(2,9,5)
	pToolBar->AddTool(ID_VIEW_GO_HOME, 	wxT("Goto First Frame"),
                      wxArtProvider::GetBitmap(wxART_GOTO_FIRST, wxART_TOOLBAR),
                      wxT("Go to frame 0"));
#else
	pToolBar->AddTool(ID_VIEW_GO_HOME, 	wxT("Goto First Frame"),
                      wxArtProvider::GetBitmap(wxART_GO_HOME, wxART_TOOLBAR),
                      wxT("Go to frame 0"));
#endif
	pToolBar->AddTool(ID_VIEW_PREV, 	wxT("Prev"),
                      wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR),
                      wxT("Goto Previous"));

    /* create slider control */
    m_sliderCntrl = new wxSlider(pToolBar, ID_FRAME_SLIDER,
                                 0, 0, 100,
                                 wxDefaultPosition,
                                 wxSize(300,20),
                                 wxSL_HORIZONTAL|wxSL_AUTOTICKS);

    wxASSERT( m_sliderCntrl != 0L );

    m_sliderCntrl->SetToolTip( wxT("Frame Number") );

    pToolBar->AddControl(m_sliderCntrl);
    pToolBar->AddTool(ID_VIEW_NEXT, 	wxT("Next"), wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR), wxT("Goto Next"));
#if wxCHECK_VERSION(2,9,5)
	pToolBar->AddTool(ID_VIEW_GO_LAST,  wxT("Goto Last Frame"), wxArtProvider::GetBitmap(wxART_GOTO_LAST, wxART_TOOLBAR), wxT("Goto Last"));
#endif
    pToolBar->Realize();

    SetToolBar(pToolBar);

    /* create pull-down menus */
    pMenuBar = new wxMenuBar;

    /* File Menu */
    pMenu = new wxMenu;

    subMenu = new wxMenu;

    subMenu->Append(ID_FILE_OPEN_SPLIT, wxT("YUV Split...\tCtrl-S"), wxT("Open YUV split file"));
    subMenu->Append(ID_FILE_OPEN_FILE,	wxT("YUV File...\tCtrl-F"),  wxT("Open composite YUV file"));
    subMenu->Append(ID_FILE_OPEN_DUMP,  wxT("Dump Directory...\tCtrl-D"), wxT("Open rmfp dump directory"));

    pMenu->Append(ID_FILE_OPEN,			wxT("Open"), subMenu);
    pMenu->Append(ID_FILE_REFRESH,      wxT("&Refresh\tCtrl-R"), wxT("Reload current frame"));



#ifdef  OLD_SAVE
    pMenu->Append(ID_FILE_SAVE_AS,		wxT("Save as..."),			wxT("Save image to file"));
#else
    subMenu = new wxMenu;
    subMenu->Append(ID_FILE_SAVE_IMAGE,     wxT("Save frame as bitmap image..."));
    subMenu->Append(ID_FILE_SAVE_YUV_SPLIT, wxT("Save frame as YUV split..."));
    subMenu->Append(ID_FILE_SAVE_YUV_COMP,  wxT("Save frame as YUV composite..."));

    pMenu->Append(ID_FILE_SAVE,             wxT("Save"), subMenu);
#endif

	pMenu->AppendSeparator();
	pMenu->Append(ID_FILE_SETSIZE, 		wxT("Set &size...\tAlt-S"), 	wxT("Set image dimensions"));
    pMenu->Append(ID_FILE_GRID_SETTINGS, wxT("Grid Settings..."),    wxT("Adjust the grid spacing"));
    pMenu->AppendSeparator();
    pMenu->Append(ID_FILE_EXIT, 		wxT("E&xit Application\tCtrl-X"));

    pMenuBar->Append(pMenu, wxT("&File"));

    /* View Menu */
    pMenu = new wxMenu;

    pMenu->Append(ID_VIEW_NEXT, 			wxT("View &next image\tCtrl-N"));
    pMenu->Append(ID_VIEW_PREV, 			wxT("View &previous image\tCtrl-P"));
    pMenu->Append(ID_VIEW_GO_HOME,			wxT("Go to first frame\tCtrl-0"), wxT("Go to first frame"));
    pMenu->Append(ID_VIEW_GO_LAST,			wxT("Go to last frame\tCtrl-L"), wxT("Go to last frame"));
    pMenu->Append(ID_VIEW_GOTO, 			wxT("Goto frame...\tCtrl-G"), wxT("Go to specific frame #"));
    pMenu->AppendSeparator();

    subMenu = new wxMenu;

    subMenu->AppendCheckItem(ID_VIEW_SCALE_ACTUAL,	wxT("Actual size"));
    subMenu->AppendCheckItem(ID_VIEW_SCALE_SCALED,	wxT("Fit to window"));
    subMenu->AppendCheckItem(ID_VIEW_SCALE_HALF,	wxT("Half size"));
    subMenu->AppendCheckItem(ID_VIEW_SCALE_DOUBLE,	wxT("Double size"));
    subMenu->AppendSeparator();
    subMenu->AppendCheckItem(ID_VIEW_SCALE_CUSTOM,	wxT("Custom size..."));

    pMenu->Append(ID_VIEW_SCALE,			wxT("Scaling"), subMenu);

    pMenu->AppendCheckItem(ID_VIEW_GRID,    wxT("Enable Grid"));
    pMenu->AppendCheckItem(ID_VIEW_STEP,	wxT("Auto step\tCtrl-A"));
    pMenu->Append(ID_VIEW_FULLSCREEN,       wxT("Fullscreen\tF11"));

    pMenu->AppendSeparator();

    subMenu = new wxMenu;

    subMenu->AppendCheckItem(ID_VIEW_420,			wxT("4:2:0"));
    subMenu->AppendCheckItem(ID_VIEW_422,			wxT("4:2:2"));
    subMenu->AppendCheckItem(ID_VIEW_444,           wxT("4:4:4"));

    pMenu->Append(ID_VIEW_TYPE, wxT("YUV Format"), subMenu);
    pMenu->AppendSeparator();
    pMenu->AppendCheckItem(ID_VIEW_CHECKSUM,    wxT("Display Checksum"));

    pMenuBar->Append(pMenu, wxT("&View"));

    /* About Menu */
    pMenu = new wxMenu;

    pMenu->Append(ID_HELP_ABOUT,			wxT("About"));

    pMenuBar->Append(pMenu, wxT("&Help"));

    SetMenuBar(pMenuBar);

    m_statBar = CreateStatusBar(3);

    /* set parameters for image */
    LoadOptions();

    /* check scale */
    for (int x = 0 ; x <= IS_IMAGE_DOUBLE ; x++) {
        pMenuBar->FindItem(ID_VIEW_SCALE_ACTUAL + x)->Check((x == m_nScale)?true:false);
    }

    pMenuBar->FindItem(ID_VIEW_420)->Check(m_yuvType == ImageBuffer::DATA_YUV420?true:false);
    pMenuBar->FindItem(ID_VIEW_422)->Check(m_yuvType == ImageBuffer::DATA_YUV422?true:false);
    pMenuBar->FindItem(ID_VIEW_444)->Check(m_yuvType == ImageBuffer::DATA_YUV444?true:false);

    //printf("m_bGrid = %d\n", (int)m_bGrid);

    pMenuBar->FindItem(ID_VIEW_GRID)->Check(m_bGrid);

#if 0
    /* determine screen depth for color-keying in alpha-blending */
    m_depth = ::wxDisplayDepth();

    switch (m_depth) {
    case 8:
        m_maskMask = 0xf0;
        break;
    case 16:
        m_maskMask = 0xf8;
        break;
    case 24:
    default:
        m_maskMask = 0xff;
        break;
    }
#endif

    allocate_image_buffer();
    GetFrameRange();	// Get the range of frames

    /* clip image id between 0 and max */
    if (m_imageID < 0) {
        m_imageID = 0;
    }
    if (m_imageID > m_lastID) {
        m_imageID = m_lastID;
    }


    wxBoxSizer*		pSizer = 0L;

    pSizer = new wxBoxSizer( wxVERTICAL );
    assert(pSizer != 0L);

    m_pScroller = new ImageScroller(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    assert(m_pScroller != 0);

    pSizer->Add( m_pScroller, 1, wxEXPAND );
    SetSizer( pSizer );

    m_pHashDlg = new HashDialog(this);
    wxASSERT( m_pHashDlg != 0L );

    /* post event to cause 1st image to load */
    wxCommandEvent		event(wxEVT_LOADIMAGE, GetId());
    event.SetEventObject( this );

    GetEventHandler()->AddPendingEvent(event);

    /* start all timers */
    m_timer.SetOwner(this, ID_TIMER);
    m_heartbeat.SetOwner(this, ID_HEART);
    m_heartbeat.Start(30);

    /* Set the windows title */
    wxString sTmp = wxString::Format(wxT("%s V%d.%d"), APP_NAME, VERSION_MAJOR, VERSION_MINOR);
    SetTitle(sTmp);

    /* Set the windows icon */
    wxIcon* pWindowIcon = new wxIcon( mag_small ) ;//, 32, 32);
    SetIcon(*pWindowIcon);
    delete pWindowIcon;

    SetFocus();
//    printf("state = %d\n", m_pScroller->get_grid_state());

//    pMenuBar->Check(ID_VIEW_GRID, m_pScroller->get_grid_state());

}

/**
 *
 */

Frame::~Frame()
{
    debug("Frame::~Frame()\n");

#ifdef	HAVE_FAM
    m_fam.stop();
#endif

    if (m_pImage) {
        debug("-- deleting m_pImage!\n");
        delete m_pImage;
        m_pImage = 0L;
    }

    if (m_pScaledImage) {
        debug("-- deleting m_pScaledImage!\n");
        delete m_pScaledImage;
        m_pScaledImage = 0L;
    }

    free_image_buffer();
}

/**
 *
 */

void Frame::OnClose(wxCloseEvent& event)
{
    debug("Frame::OnClose()\n");

    SaveOptions();
    Destroy();

    return;
}

void Frame::OnExit(wxCommandEvent& event)
{
    Close();
}

/**
 *  Set the image width and height.
 */

void Frame::OnFileSetSize(wxCommandEvent& event) {
	debug("Frame::OnFileSetSize()\n");

    SizeDialog      dialog(this, m_imageSize.GetWidth(),  m_imageSize.GetHeight());

    if (dialog.ShowModal() == wxID_OK) {
        long w, h;

        dialog.GetFrameDimensions( w, h );

        if ((w > 0) && (h > 0)) {
            m_imageSize.Set(w,h);

            if (allocate_image_buffer()) {
                GetImage();
                Refresh();
            }
        }
    }

    return;
}

/** Display the next frame image. */
void Frame::OnViewNext(wxCommandEvent& event)
{
    debug("Frame::OnViewNext()\n");

    debug("-- imageID = %d lastID = %d\n", m_imageID, m_lastID);

    if ((m_imageID >= 0) && (m_imageID < m_lastID)) {
        m_imageID++;
        GetImage();
        m_alpha = 1.0;
        Refresh(false);
    } else {
        debug("WARNING: Attempting to move past last frame...\n");
    }
}

/** Display the previous frame image. */
void Frame::OnViewPrevious(wxCommandEvent& event)
{
    debug("Frame::OnViewPrevious()\n");

    if (m_imageID > 0) {
        m_imageID--;
        GetImage();
        m_alpha = 1.0;
        Refresh(false);
    } else {
        debug("WARNING: Attempting to move past first frame...\n");
    }
}

/** Prompt user for frame number and set image id. */
void Frame::OnGotoFrame(wxCommandEvent& event)
{
    debug("Frame::OnGotoFrame()\n");
    long n;

    n = ::wxGetNumberFromUser(wxT("Please select a frame"), wxT("Frame #"), wxT("Select frame"), m_imageID, 0, 999999);
    if (n != -1) {
        if ((n >= 0) && (n <= m_lastID)) {
            m_imageID = n;
            GetImage();
            m_alpha = 1.0;
            Refresh(false);
        } else {
        }
    }
}

//void Frame::OnPaint(wxPaintEvent& event) {
//#ifdef	DEBUG_PAINT
//	debug("Frame::OnPaint()\n");
//#endif
//
//	return;
//}

//------------------------------------------------------------------------------
//	FUNCTION	:	GetImage
//
//	PURPOSE		:	Load image into wxImage.
//------------------------------------------------------------------------------

bool Frame::GetImage()
{
    debug("GetImage()\n");

    bool				result = false;

    if (m_buffer == 0)
        return false;

    if (m_buffer->lastError() != ImageBuffer::ERROR_NO_ERROR) {
        debug("ERROR: m_buffer contains last error %d\n", m_buffer->lastError());
        return false;
    }

#ifdef	ENABLE_BUSY_CURSOR
    ::wxBeginBusyCursor();
#endif

    /* delete image if already loaded. */
    if (m_pImage) {
        delete m_pImage;
        m_pImage = 0L;
    }

    m_buffer->type(m_yuvType);

#ifdef  ENABLE_PROFILE_LOAD
    wxStopWatch swTimer;
#endif

    if (m_buffer->Load(m_imageID)) {
        wxString	sMsg;

#ifdef  ENABLE_PROFILE_LOAD
        swTimer.Pause();
        debug("PROFILE : load time %ld\n", swTimer.Time());
#endif

        /* create an empty image. */
        m_pImage = new wxImage(m_imageSize.GetWidth(), m_imageSize.GetHeight());
        assert(m_pImage != 0L);

        m_buffer->GetImage(m_pImage);

        m_pScroller->set_image( m_pImage, m_nScale );

        if (m_bGrid) {
            //printf("enabling grid!\n");
            m_pScroller->set_grid_dimensions( m_gridH, m_gridV, m_gridColor );
            m_pScroller->enable_grid();
        } else {
            //printf("disabling grid!\n");
            m_pScroller->disable_grid();
        }

        /* update UI elements to current image ID */
        UpdateStatusBar(m_imageID);
        m_sliderCntrl->SetValue(m_imageID);

#ifdef  MODAL_CHECKSUM
        if (m_buffer->CanChecksum()) {
            wxUint8 lumaSum[16], chromaSum[16];

            if (m_buffer->GetChecksum( m_imageID, lumaSum, chromaSum )) {
                m_pHashDlg->SetChecksums( lumaSum, chromaSum );
            }
        }
#endif  // MODAL_CHECKSUM

        result = true;
    } else {

#ifdef  ENABLE_PROFILE_LOAD
        swTimer.Pause();
        debug("PROFILE : load failed time %ld\n", swTimer.Time());
#endif

        wxMessageBox(wxT("Unable to load image!\nRead past end of file..."), wxT("frameInspector"));
        m_pScroller->set_image( (wxImage*)NULL );
    }

#ifdef	ENABLE_BUSY_CURSOR
    ::wxEndBusyCursor();
#endif

    return result;
}

void Frame::UpdateStatusBar(int id)
{
    wxString	sMsg;
    wxFileName  fname(m_imageFilename);

//    SetStatusText(wxT("shit"), 0);

    if (m_type == YUV_SPLIT) {
        sMsg = wxString::Format(wxT("Split : %s"), m_imagePath.c_str());
        SetStatusText(sMsg, 0);
    } else if (m_type == YUV_FILE) {
        sMsg = wxString::Format(wxT("File : %s"), fname.GetFullName().c_str() /* m_imageFilename.c_str() */);
        SetStatusText(sMsg, 0);
    } else if (m_type == YUV_DUMP) {
		sMsg = wxString::Format(wxT("Dump : %s"), m_imagePath.c_str() /* m_imageFilename.c_str() */ );
        SetStatusText(sMsg, 0);
    }
    sMsg = wxString::Format(wxT("Frame # %d of %d "), id, m_lastID);
    SetStatusText(sMsg, 1);

    if ((m_buffer != 0) && m_buffer->CanQueryFrameSize()) {
        int w, h;
        m_buffer->QueryFrameSize(w, h);
        m_imageSize.SetWidth(w);
        m_imageSize.SetHeight(h);
    }

    sMsg = wxString::Format(wxT("Frame Dimensions : %dx%d"),
                            m_imageSize.GetWidth(),
                            m_imageSize.GetHeight());
    /* update status bar */
    SetStatusText(sMsg,2);

    return;
}

/**
 *
 */

void Frame::OnViewScaleToggle(wxCommandEvent& event)
{
    debug("Frame::OnViewScaleToggle()\n");

    wxMenuBar*		menuBar = GetMenuBar();

    m_nScale = event.GetId() - ID_VIEW_SCALE_ACTUAL;

    for (int x = 0 ; x <= IS_IMAGE_DOUBLE ; x++) {
        menuBar->FindItem(ID_VIEW_SCALE_ACTUAL + x)->Check((x == m_nScale)?true:false);
    }

    GetImage();
    Refresh();
}

/**
 *  Enable/Disable automatic advance on timer.
 */

void Frame::OnViewAutoStep(wxCommandEvent& event)
{
    if (event.IsChecked()) {
        m_bAutoStep = true;
        m_timer.Start(100);
    } else {
        m_bAutoStep = false;
        m_timer.Stop();
    }
}

/** Advance frame id on timer event. */
void Frame::OnTimer(wxTimerEvent& event)
{
    debug("Frame::OnTimer()\n");

    if (m_bAutoStep) {
        if (m_imageID < m_lastID) {
            m_imageID++;
            GetImage();
            Refresh(false);
        } else {
            debug("No more frames - disabling timer...\n");
            m_bAutoStep = false;
            GetMenuBar()->FindItem(ID_VIEW_STEP)->Check(false);
            m_timer.Stop();
        }
    }

    return;
}

/**
  	Load options from config file. If commandline parameters were specified
	then over-ride configuration file.
*/
void Frame::LoadOptions()
{
    debug("Frame::LoadOptions()\n");

    wxCmdLineParser& parser = wxGetApp().m_cmdLine;
    wxConfig*	config = new wxConfig(wxT("frameInspector"));
    wxString	sTmp;
    long int	nTmp;
    long 		x, y,
                w, h;

    /* parse commandline arguments */
//	parser.Parse(false);

    if (config->Read(wxT("type"), &nTmp)) {
        m_type = (yuvType)nTmp;
    }

    /* get yuv buffer type */
    if (config->Read(wxT("yuvfmt"), &nTmp)) {
        m_yuvType = (ImageBuffer::dataType)nTmp;
    }

    /* get image width and height */
    if (!parser.Found(wxT("s"), &sTmp)) {
        config->Read(wxT("width"),  &w, m_imageSize.GetWidth());
        config->Read(wxT("height"), &h, m_imageSize.GetWidth());
    } else {
        sscanf(sTmp.ToAscii(), "%ldx%ld", &w, &h);
        debug("Width = %d Height = %d\n", w, h);
    }
    m_imageSize.Set(w,h);

    /* get YUV image path */
    if (!parser.Found(wxT("p"), &sTmp)) {
        config->Read(wxT("path"),	&m_imagePath);
		debug("Path = %s\n", (const char *)m_imagePath.c_str());
    } else {
        m_imagePath = sTmp;
        m_type = YUV_SPLIT;
		debug("Path = %s\n", (const char*)m_imagePath.c_str());
        GetSizeFromPath();
    }

    /* Get dump file path */
    if (parser.Found(wxT("d"), &sTmp)) {
        m_imagePath = sTmp;
        m_type = YUV_DUMP;
    }

//	if (!parser.Found(wxT("f"), &sTmp)) {
    if (!parser.Found(wxT("f"), &sTmp)) {
        config->Read(wxT("filename"), &m_imageFilename);
		debug("Filename = %s\n", (const char*)m_imageFilename.c_str());
    } else {
        wxFileName sFilename = sTmp;

        debug("--- FOUND FILENAME %s ---\n", (const char*)sTmp.c_str());

        if (sFilename.Normalize()) {
            m_imageFilename = sFilename.GetFullPath();
            debug("m_imageFilename = %s\n", (const char*)m_imageFilename.c_str());
        }

        m_type = YUV_FILE;
    }

    if (!parser.Found(wxT("r"), &sTmp)) {
        config->Read(wxT("prefix"), &m_prefix);
		debug("Prefix = %s\n", (const char*)m_prefix.c_str());
    } else {
        m_prefix = sTmp;
		debug("Prefix = %s\n", (const char*)m_prefix.c_str());
    }

    /* get image id to display */
    if (!parser.Found(wxT("c"), &m_imageID)) {
        config->Read(wxT("id"), &m_imageID, 0);
        debug("Image ID = %ld\n", m_imageID);
    }


    /* get scaling flag */
    config->Read(wxT("scaling"),	&m_nScale);

    /* get window position */
    if (config->Read(wxT("screenpos"), &sTmp)) {
        sscanf(sTmp.ToAscii(), "%ld,%ld,%ld,%ld", &x, &y, &w, &h);

        debug("Screen position @ %d,%d,%d,%d\n");

        wxRect screenPos(x,y,w,h);
        SetSize(screenPos);
    }



    /* get grid settings */
    if (config->Read(wxT("grid"), &nTmp, 0)) {
        m_bGrid = (nTmp == 1)?true:false;
    }

    if (config->Read(wxT("gridH"), &nTmp, m_gridH)) {
        m_gridH = nTmp;
    }

    if (config->Read(wxT("gridV"), &nTmp, m_gridV)) {
        m_gridV = nTmp;
    }

#ifdef  HAVE_FAM
    if (!parser.Found(wxT("m"), &nTmp)) {
        config->Read(wxT("fam"), &nTmp, 0);
        m_bEnableFAM = (nTmp == 1)?true:false;
    } else {
        m_bEnableFAM = (nTmp == 1)?true:false;
    }

    debug("FAM : %s\n", (m_bEnableFAM)?"enabled":"disabled");
#endif

    delete config;

    return;
}

/**
	Save options to config file. Save image path, id, width, height, scaling
	and screen position to configuration file.
*/

void Frame::SaveOptions()
{
    debug("Frame::SaveOptions()\n");
    wxConfig*	conf;
    wxRect		screenPos = GetRect();
    wxString	sTmp;

    conf = new wxConfig(wxT("frameInspector"));

    conf->Write(wxT("path"), 		m_imagePath);
    conf->Write(wxT("id"),		m_imageID);
    conf->Write(wxT("width"),		m_imageSize.GetWidth());
    conf->Write(wxT("height"),	m_imageSize.GetHeight());
    conf->Write(wxT("scaling"),	m_nScale);
    conf->Write(wxT("prefix"),	m_prefix);
    conf->Write(wxT("filename"),	m_imageFilename);
    conf->Write(wxT("type"),		(int)m_type);
    conf->Write(wxT("yuvfmt"),	(int)m_yuvType);
    conf->Write(wxT("grid"),    (int)m_bGrid);
    conf->Write(wxT("gridH"),   (int)m_gridH);
    conf->Write(wxT("gridV"),   (int)m_gridV);

#ifdef  HAVE_FAM
    conf->Write(wxT("fam"),		(int)m_bEnableFAM);
#endif

    sTmp = wxString::Format(wxT("%d,%d,%d,%d"),
                            screenPos.x, screenPos.y, screenPos.width, screenPos.height);
    conf->Write(wxT("screenpos"), sTmp);

    delete conf;
}

/** Find the last frame in media. */
void Frame::GetFrameRange()
{
    debug("Frame::GetFrameRange()\n");

    assert(m_buffer != 0L);

    m_lastID = m_buffer->GetFrameCount();
    debug("There are %d frames in media...\n", m_lastID);

    if (m_lastID != -1) {
        debug("-- setting range 0 - %d\n", m_lastID);

        m_sliderCntrl->Enable();
        m_sliderCntrl->SetRange(0, m_lastID);
    } else {
        debug("-- setting range 0 - 0\n");

        m_sliderCntrl->SetValue(0);
//		m_sliderCntrl->SetRange(0, 0);
        m_sliderCntrl->Disable();
    }

    return;
}

void Frame::OnGotoHome(wxCommandEvent& event)
{
    debug("Frame::OnGotoHome()\n");

    m_imageID 	= 0;
    m_alpha 	= 1.0;

    GetImage();
    Refresh(false);
}

void Frame::OnGotoLast(wxCommandEvent& event)
{
    debug("Frame::OnGotoHome()\n");

    m_imageID 	= m_lastID;
    m_alpha 	= 1.0;

    GetImage();
    Refresh(false);
}

void Frame::OnSlider(wxScrollEvent& event)
{
    wxEventType		evtType = event.GetEventType();

    debug("Event = %08x\n", (int)evtType);

    if ((evtType == wxEVT_SCROLL_THUMBRELEASE) ||
        (evtType == wxEVT_SCROLL_THUMBTRACK) ||
        (evtType == wxEVT_SCROLL_CHANGED)) {
        m_imageID = event.GetPosition();
        m_alpha = 1.0;
        GetImage();
        Refresh(false);
    }

    return;
}

void Frame::OnHelpAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo			dlgInfo;
    wxString                    sVersion;

    debug("Frame::OnHelpAbout()\n");

    dlgInfo.SetName(wxT("frameInspector"));
    dlgInfo.SetDescription(wxT("Display YUV frames"));
    dlgInfo.AddDeveloper(wxT("Michael A. Uman"));
    dlgInfo.SetCopyright(wxT("(C) 2009-2013 Sigma Designs"));
    dlgInfo.SetWebSite(wxT("ftp://revelation.soft.sdesigns.com/"));

    sVersion = wxString::Format(wxT("%d.%d"), VERSION_MAJOR, VERSION_MINOR);
    dlgInfo.SetVersion(sVersion);

    wxAboutBox( dlgInfo );
}

#if 0
void Frame::post_process(wxBitmap& bmp) {

    wxImage		imgA = bmp.ConvertToImage();

    /* only do alphablending on a CPU which supports SSE2 */
    if ((wxGetApp().get_cpu_info()->CPUSig_Flag_SSE2) && (m_alpha > 0.0)) {
        wxMemoryDC		memDC;
        wxBitmap		bm(INFO_WIDTH, INFO_HEIGHT);
        wxFont			font(18, wxFONTFAMILY_MODERN,wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxCoord			w, h;
        wxString		sMsg;
        wxColor			maskColor(255, 0, 255);
//		wxBrush			brush(*wxBLUE);
        wxBrush			brush(maskColor);
#ifdef  ALPHA_BLENDING
        float			alpha = m_alpha;
#endif

        if (bm.Ok()) {
#ifdef	ALPHA_BLENDING
            __m128		dataA, dataB, dataC;
            __m128		multS, multD;

            multS = _mm_set_ps1(alpha);
            multD = _mm_set_ps1(1 - alpha);
#endif

            /* draw the info block */
            memDC.SelectObject(bm);
            memDC.SetBrush(brush);
            memDC.SetFont(font);
            memDC.SetBackground(brush);
            memDC.SetBackgroundMode(wxSOLID);
            memDC.SetTextForeground(wxColour(255,255,0));
            memDC.SetTextBackground(maskColor);
            memDC.Clear();
            sMsg = wxString::Format(wxT("%02ld:%02ld:%02ld"), (m_imageID / 30 / 60), (m_imageID / 30) % 60, m_imageID % 30);
            memDC.GetTextExtent(sMsg, &w, &h);
            memDC.DrawText(sMsg, (INFO_WIDTH - w) / 2 , (INFO_HEIGHT - h) / 2);

            wxImage			imgB = bm.ConvertToImage();
            wxColor			mask(maskColor);

#ifndef	OLDWAY
            //	printf("MASK R%02x G%02x B%02x\n", mask.Red(), mask.Green(), mask.Blue());

            for (int y = 0 ; y < imgB.GetHeight(); y++) {
                unsigned char*	pDst = imgA.GetData() + (y * imgA.GetWidth() * 3) + (20 * 3);
                unsigned char*  pSrc = imgB.GetData() + (y * imgB.GetWidth() * 3);

                for (int x = 0 ; x < imgB.GetWidth() ; x++) {

                    /* check if this is color-key value */
                    if ((pSrc[0] != (mask.Red()   & m_maskMask)) ||
                        (pSrc[1] != (mask.Green() & m_maskMask)) ||
                        (pSrc[2] != (mask.Blue()  & m_maskMask))) {

#ifdef	ALPHA_BLENDING
                        dataA = _mm_setr_ps((float)pSrc[0], (float)pSrc[1], (float)pSrc[2], (float)0);
                        dataB = _mm_setr_ps((float)pDst[0], (float)pDst[1], (float)pDst[2], (float)0);

#if 0
#ifdef	_ENABLE_DEBUG
                        debug("A %02x%02x%02x\n", pSrc[0], pSrc[1], pSrc[2]);
                        debug("B %02x%02x%02x\n", pDst[0], pDst[1], pDst[2]);

                        show_results_m128("A", dataA);
                        show_results_m128("B", dataB);
#endif	// _DEBUG
#endif

                        dataA = _mm_mul_ps(dataA, multS);
                        dataB = _mm_mul_ps(dataB, multD);
                        dataC = _mm_add_ps(dataA, dataB);

                        float*	pData = (float*)&dataC;

                        *pDst++ = (char)*pData++;
                        *pDst++ = (char)*pData++;
                        *pDst++ = (char)*pData++;
                        pSrc += 3;
#else	// ALPHA_BLENDING
                        *pDst++ = *pSrc++;
                        *pDst++ = *pSrc++;
                        *pDst++ = *pSrc++;
#endif	// ALPHA_BLENDING

                    } else {
                        pSrc += 3;
                        pDst += 3;
                    }
                }
            }
#else

            //		alpha_blend(imgB, imgA, 0, 0, mask);

#endif

            bmp	=		wxBitmap(imgA);
        } else {
            fprintf(stderr, "ERROR: Unable to create bitmap!\n");
        }
    }
}
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

bool Frame::GetImageYSize(size_t* filesize)
{
    wxString 		imageFilename;
    wxFile			file;
    bool			result = false;

    /* create the image filename from the path and frame #. */
    imageFilename = wxString::Format(wxT("%s/%s_%d.Y"),
                                     m_imagePath.c_str(),
                                     m_prefix.c_str(),
                                     0);

    if (file.Open(imageFilename)) {
        file.SeekEnd();
        *filesize = file.Tell();
        result = true;
    } else {
		debug("ERROR: Unable to open file [%s]\n", (const char*)imageFilename.c_str());
    }

    return result;
}


#ifdef ALPHA_BLENDING

//------------------------------------------------------------------------------
//  Decrement alpha value on timer
//------------------------------------------------------------------------------

void Frame::OnHeartbeat(wxTimerEvent& event)
{
#ifdef	DEBUG_HB
    debug("Frame::OnHeartbeat()\n");
#endif

    if (m_alpha > 0) {
        m_alpha -= (float).05;
        Refresh();
    }

}

#endif

void Frame::GetSizeFromPath()
{
    /* attempt to read size from file 'size.txt' */
    wxString		sizeFile = m_imagePath + wxT("/size.txt");
    wxTextFile		file(sizeFile);

    if (file.Exists() && file.Open()) {
        wxString		line = file.GetLine(0);
        int				w, h;

        sscanf(line.ToAscii(), "%dx%d", &w, &h);
        m_imageSize.Set(w, h);
        file.Close();
    }

    return;
}

void Frame::OnFileOpenSplit(wxCommandEvent& event)
{
    wxString		prefix = ::wxGetTextFromUser(wxT("Image prefix"),
                             wxT("Set Image Prefix"),
                             m_prefix);

    if (!prefix.empty()) {
        m_prefix = prefix;
    } else {
        return;
    }

    const wxString& dir = ::wxDirSelector(wxT("Select output path"), m_imagePath);

    if (!dir.empty()) {
        m_imagePath = dir;
        m_imageFilename.Clear();
    } else {
        return;
    }

    GetSizeFromPath();

    m_imageID 	= 0;	/* reset id to zero */
    m_type 		= YUV_SPLIT;

    if (allocate_image_buffer()) {
        m_alpha = 1.0;
        GetFrameRange();
        GetImage();
        Refresh();
        ShowHideChecksumDlg();
    }

    return;
}

/**
 *  Handle user request to open a YUV file.
 */

void Frame::OnFileOpenFile(wxCommandEvent& event)
{
    wxString	newFile = ::wxFileSelector(wxT("Select YUV file"),
                                           m_imagePath, wxEmptyString, wxEmptyString,
                                           wxT("YUV Files (*.yuv)|*.yuv|All Files (*.*)|*.*"));

    if (!newFile.empty()) {
        wxFileName  fName(newFile);

        m_imageFilename = newFile;
        m_sYUVFilename  = fName.GetFullName();
        m_imagePath     = fName.GetPath();

        m_imageID 	= 0;	/* reset id to zero */
        m_type 		= YUV_FILE;

        if (allocate_image_buffer()) {
            m_alpha = 1.0;
            GetFrameRange();
            GetImage();
            Refresh();
            ShowHideChecksumDlg();
        } else {
            wxMessageBox(wxT("Unable to allocate image"));
        }
    }

    return;
}

/**
 *  Handle user request to open a 'dump' file...
 */

void Frame::OnFileOpenDump(wxCommandEvent& event)
{
	const wxString& dir = ::wxDirSelector(wxT("Select dump output path"), m_imagePath);

    debug("Frame::OnFileOpenDump()\n");

    if (!dir.empty()) {
        m_imagePath = dir;
        m_imageFilename.Clear();

		m_imageID 	= 0;	/* reset id to zero */
		m_type 		= YUV_DUMP;

		if (allocate_image_buffer()) {
		    m_alpha = 1.0;
		    GetFrameRange();
		    GetImage();
        	Refresh();
            ShowHideChecksumDlg();
        } else {
            wxMessageBox(wxT("Unable to allocate image"));
        }
    }

    return;
}

void Frame::ShowHideChecksumDlg() {
    if (m_pHashDlg->IsShown()) {
        m_pHashDlg->Show(m_buffer->CanChecksum());
    }
}

/**
 *  Refresh the display.
 */

void Frame::OnRefresh(wxCommandEvent& event)
{
    if (allocate_image_buffer()) {
        m_alpha = 1.0;
        GetFrameRange();
        GetImage();
        Refresh(false);
    }
    return;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

bool Frame::allocate_image_buffer()
{
//    int newWidth, newHeight;

    debug("Frame::allocate_image_buffer()\n");

    free_image_buffer();	/* delete any existing image buffer */

    if (m_type == YUV_SPLIT) {
        YUV420ImageBufferSplit*	newBuf = new YUV420ImageBufferSplit(m_imageSize.GetWidth(),
                m_imageSize.GetHeight());

        assert(newBuf != 0L);

        newBuf->setPrefix(m_prefix);
        newBuf->setPath(m_imagePath);

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);

#ifdef	HAVE_FAM
        if (m_bEnableFAM) {
            /* reset monitor and monitor directory. */
            m_fam.Reset();
            m_fam.MonitorDir(m_imagePath);
        }
#endif

    } else if (m_type == YUV_FILE) {
        YUV420ImageBufferFile* newBuf = new YUV420ImageBufferFile(m_imageSize.GetWidth(),
                m_imageSize.GetHeight());
        assert(newBuf != 0L);
        newBuf->SetFilename(m_imageFilename);

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);

        if (m_buffer->lastError() == ImageBuffer::ERROR_NO_ERROR) {


#ifdef	HAVE_FAM
            if (m_bEnableFAM) {
                /* reset monitor and monitor YUV file */
                m_fam.Reset();
                m_fam.MonitorFile(m_imageFilename);
            }
#endif
        } else {
            debug("ERROR: last error = %d\n", m_buffer->lastError());
            return false;
        }

    } else if (m_type == YUV_DUMP) {
        dumpImageBuffer* newBuf = new dumpImageBuffer(m_imageSize.GetWidth(), m_imageSize.GetHeight());
        assert(newBuf != 0L);

        newBuf->setPath(m_imagePath);

        m_buffer = dynamic_cast<ImageBuffer*>(newBuf);

#ifdef	HAVE_FAM
        if (m_bEnableFAM) {
            /* reset monitor and monitor YUV file */
            m_fam.Reset();
            m_fam.MonitorDir(m_imagePath);
        }
#endif
    }

    debug("m_buffer = 0x%08lx (%s)\n", m_buffer, typeid(m_buffer).name());

    GetFrameRange();

#if 0
    if (typeid(*m_buffer) == typeid(YUV420ImageBuffer)) {
        debug("-- m_buffer points to YUV420ImageBuffer object!\n");
    }
#endif

    return (m_buffer != 0);
}

/**
 *
 */

void Frame::free_image_buffer()
{
    debug("Frame::free_image_buffer()\n");

    if (m_buffer) {
        delete m_buffer;
        m_buffer = 0L;
    }

    return;
}

/**
 *
 */

void Frame::OnViewFormat(wxCommandEvent& event)
{
    debug("Frame::OnViewFormat()\n");
    wxMenuBar*	pMenu = GetMenuBar();

    switch (event.GetId()) {
    case ID_VIEW_420: {
        debug("-- setting 4:2:0\n");

        pMenu->FindItem(ID_VIEW_420)->Check(true);
        pMenu->FindItem(ID_VIEW_422)->Check(false);
        pMenu->FindItem(ID_VIEW_444)->Check(false);

        m_yuvType = ImageBuffer::DATA_YUV420;

        GetImage();
        Refresh();
    }
    break;

    case ID_VIEW_422: {
        debug("-- setting 4:2:2\n");

        pMenu->FindItem(ID_VIEW_420)->Check(false);
        pMenu->FindItem(ID_VIEW_422)->Check(true);
        pMenu->FindItem(ID_VIEW_444)->Check(false);

        m_yuvType = ImageBuffer::DATA_YUV422;
        GetImage();
        Refresh();
    }
    break;

    case ID_VIEW_444: {
        debug("-- setting 4:4:4\n");

        pMenu->FindItem(ID_VIEW_420)->Check(false);
        pMenu->FindItem(ID_VIEW_422)->Check(false);
        pMenu->FindItem(ID_VIEW_444)->Check(true);

        m_yuvType = ImageBuffer::DATA_YUV444;

        GetImage();
        Refresh();

    }
    break;

    default:
        debug("-- oops?!? invalid event!\n");
        break;
    }

}

/**
 *
 */

void Frame::OnFileSaveAs(wxCommandEvent& event)
{
    debug("Frame::OnFileSaveAs()\n");
    wxString	fileName;

    fileName = ::wxFileSelector(wxT("File to save"));

    if (!fileName.IsEmpty()) {
		debug("-- saving file %s...\n", (const char*)fileName.c_str());
        SaveBitmap(fileName);
    }

    return;
}

/**
 *
 */

void Frame::OnFileSaveYUVSplit(wxCommandEvent& event) {
    debug("Frame::OnFileSaveYUVSplit()\n");
    wxString        sFilename;

    sFilename = ::wxFileSelector(wxT("File to save"));

    if (!sFilename.IsEmpty()) {
        debug("-- saving YUV split file %s...\n", (const char*)sFilename.c_str());
        m_buffer->SaveYUV( sFilename, ImageBuffer::SAVE_YUV_SPLIT );
    }

    return;
}

/**
 *
 */

void Frame::OnFileSaveYUVComp(wxCommandEvent& event) {
    debug("Frame::OnFileSaveYUVComp()\n");
    wxString        sFilename;

    sFilename = ::wxFileSelector(wxT("File to save"), wxEmptyString, wxEmptyString,
                                 wxEmptyString, wxT("YUV Files (*.yuv)|*.yuv"),
                                 wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (!sFilename.IsEmpty()) {
        debug("-- saving YUV composite file %s...\n", (const char*)sFilename.c_str());
        m_buffer->SaveYUV( sFilename, ImageBuffer::SAVE_YUV_COMP );
    }

    return;
}

/**
 *
 */

void Frame::OnLoadImage(wxCommandEvent& event) {
    debug("Frame::OnLoadImage()\n");

    GetImage();
}

/**
 *
 */

void Frame::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    if (m_pImage) {
        m_pScroller->set_image( m_pImage, m_nScale );
    }
}

/**
 *
 */

void Frame::OnViewScaleCustom(wxCommandEvent& event)
{
    debug("Frame::OnViewScaleCustom()\n");
    long		defScale  = (long)(m_dScale * 100);
    long		nNewScale = ::wxGetNumberFromUser(wxT("Enter scale"),
                            wxT("Scale %"),
                            wxT("Scale dialog"),
                            defScale);
    if (nNewScale != -1) {
        m_dScale = nNewScale / 100;
        m_pScroller->set_image( m_pImage, m_dScale );
    }

    return;
}

/**
 *
 */

void Frame::OnGridChange(wxCommandEvent& event)
{
    debug("Frame::OnGridChange()\n");

    m_bGrid = !m_bGrid;

    if (m_bGrid) {
        m_pScroller->set_grid_dimensions( m_gridH, m_gridV, m_gridColor );
        m_pScroller->enable_grid();
    } else {
        m_pScroller->disable_grid();
    }

    return;
}

/**
 *
 */

void Frame::SaveBitmap(wxString sFilename)
{
    wxMenuBar*	pMenu = GetMenuBar();
    bool        bEnableGrid = pMenu->IsChecked(ID_VIEW_GRID);

    debug("Frame::SaveBitmap(%s)\n", (const char*)sFilename.c_str());

    if (m_pImage != 0) {

        if (bEnableGrid) {

            wxImage     imgDest;
            wxBitmap*   pBm = 0;
            wxMemoryDC  pDc;
            int         width, height;

            width = m_pImage->GetWidth();
            height = m_pImage->GetHeight();

            pBm = new wxBitmap(*m_pImage);
            pDc.SelectObject(*pBm);

            pDc.SetPen(*wxBLACK_PEN);

            for (int x = 0 ; x < width; x += 16) {
                pDc.DrawLine(x, 0, x, height);
            }
            for (int y = 0 ; y < height ; y += 16) {
                pDc.DrawLine(0, y, width, y);
            }

            imgDest = pBm->ConvertToImage();

            //        m_pImage->SaveFile(sFilename);
            imgDest.SaveFile(sFilename);
            delete pBm;
        } else {
            m_pImage->SaveFile(sFilename);
        }
    }
}

/**
    Toggle full-screen mode
 */

void Frame::OnViewFullscreen(wxCommandEvent& event)
{
    debug("Frame::OnViewFullscreen()\n");

    if (IsFullScreen()) {
        ShowFullScreen(false);
    } else {
        ShowFullScreen(true, wxFULLSCREEN_ALL);
    }

    return;
}

/**
    Allow user to adjust the grid settings...
 */

void Frame::OnGridSettings(wxCommandEvent& event)
{
    debug("Frame::OnGridSettings()\n");

    if (m_pScroller) {
        GridSettingsDialog  gridDlg(this) ;//, wxID_ANY);

        gridDlg.m_spinH->SetValue(m_gridH);
        gridDlg.m_spinV->SetValue(m_gridV);
        gridDlg.m_spinH->SetFocus();

        if (gridDlg.ShowModal() == wxID_OK) {
            m_gridH = gridDlg.m_spinH->GetValue();
            m_gridV = gridDlg.m_spinV->GetValue();
            m_pScroller->set_grid_dimensions( m_gridH, m_gridV, m_gridColor );
        }

    }
}

/**
 *
 */

void Frame::OnUpdateUI(wxUpdateUIEvent& event)
{
    //debug("OnUpdateUI!\n");
    int eventID = event.GetId();

    if (eventID == ID_VIEW_CHECKSUM) {
		if (m_buffer != 0L) {
		    event.Enable(m_buffer->CanChecksum());
		} else {
		    event.Enable(false);
		}
    } else if (eventID == ID_FILE_SETSIZE) {
        if (m_buffer != 0L) {
            if (m_buffer->CanQueryFrameSize()) {
                event.Enable(false);
            } else {
                event.Enable(true);
            }
        } else {
            event.Enable(true);
        }
    } else if ((eventID >= ID_FILE_SAVE_IMAGE) && (eventID <= ID_FILE_SAVE_YUV_COMP)) {
        if (m_buffer != 0L) {
            if (eventID == ID_FILE_SAVE_IMAGE) {
                event.Enable(true);
            } else {
                if (m_buffer->CanSave()) {
                    event.Enable(true);
                } else {
                    event.Enable(false);
                }
            }
        } else {
            event.Enable(false);
        }
    }
    return;
}

/**
 *
 */

void Frame::OnUpdateGridUI(wxUpdateUIEvent& event) {
    debug("Frame::OnUpdateGridUI()\n");

    event.Check(m_bGrid);

    return;
}


/**
    Display the checksum/hash dialog.
 */

void Frame::OnViewChecksum(wxCommandEvent& event) 
{
    debug("Frame::OnViewChecksum()\n");

    if (event.IsChecked()) {
        m_pHashDlg->Show(TRUE);
    } else {
        m_pHashDlg->Show(FALSE);
    }

}


#ifdef	HAVE_FAM

void Frame::OnFAMEvent(wxFAMEvent& event)
{
    debug("Frame::OnFAMEvent(%s - %d)\n", event.name().c_str(), event.famEvent());

    switch (event.famEvent()) {
//		case FAMChanged:
    case FAMDeleted:
    case FAMCreated:
        GetFrameRange();
        UpdateStatusBar(m_imageID);
//			GetImage();
//			Refresh();
        break;
    }

    debug("last Id = %d frame Id = %d\n", m_lastID, m_imageID);

    if (m_lastID == 0) {
        debug("-- loading image # 0\n");
        m_imageID = 0;
        GetImage();
        Refresh();
    }

}

#endif	// HAVE_FAM
