//==============================================================================
//  MODULE      :   frame.cpp
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   April 9, 2014
//==============================================================================

//#define     DEBUG_UPDATE_UI     1
#define     USE_YUVCOMPDIALOG
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
#include "viewApp.h"
#include "frame.h"
#include "imageBuffer.h"
#include "YUV420ImageBufferSplit.h"
#include "YUV420ImageBufferFile.h"
#include "DumpImageBuffer.h"
#include "GridSettingsDialog.h"
#include "cpupack.h"
#include "misc_utils.h"
//#include "hashDialog.h"
#include "ChecksumDialog.h"
#include "SizeDialog.h"
#include "YUVCompDialog.h"
#include "YUVSplitDialog.h"
#include "controlParms.h"
#include "crc32.h"
#include "formatDialog.h"

/* Include XPM bitmaps */
#include "../bitmaps/mag_small.xpm"
#include "../bitmaps/grid-small.xpm"

using namespace utils;

DEFINE_EVENT_TYPE(wxEVT_LOADIMAGE)

BEGIN_EVENT_TABLE(Frame, wxFrame)
    /* menu events */
    EVT_MENU(ID_FILE_OPEN_SPLIT,     Frame::OnFileOpenSplit)
    EVT_MENU(ID_FILE_OPEN_FILE,      Frame::OnFileOpenFile)
    EVT_MENU(ID_FILE_OPEN_DUMP,      Frame::OnFileOpenDump)
    EVT_MENU(ID_FILE_CLOSE,          Frame::OnFileClose)
    EVT_MENU(ID_FILE_REFRESH,        Frame::OnRefresh)

    EVT_MENU(ID_FILE_SAVE_IMAGE,     Frame::OnFileSaveAs)
    EVT_MENU(ID_FILE_SAVE_YUV_SPLIT, Frame::OnFileSaveYUVSplit)
    EVT_MENU(ID_FILE_SAVE_YUV_COMP,  Frame::OnFileSaveYUVComp)

#ifdef  ENABLE_CONVERT
    EVT_MENU(ID_FILE_CONV_TO_SPLIT,  Frame::OnConvertTo)
    EVT_MENU(ID_FILE_CONV_TO_COMPOSITE, Frame::OnConvertTo)
#endif  // ENABLE_CONVERT

//    EVT_MENU(ID_FILE_GRID_SETTINGS,  Frame::OnGridSettings)
    EVT_MENU(ID_FILE_EXIT,           Frame::OnExit)

    EVT_MENU(ID_FORMAT_SETSIZE,      Frame::OnFormatSetSize)
    EVT_MENU(ID_FORMAT_YUV,          Frame::OnFormatYUV)

    EVT_MENU(ID_VIEW_NEXT,           Frame::OnViewNext)
    EVT_MENU(ID_VIEW_PREV,           Frame::OnViewPrevious)
    EVT_MENU(ID_VIEW_GOTO,           Frame::OnGotoFrame)
    EVT_MENU(ID_VIEW_GO_HOME,        Frame::OnGotoHome)
    EVT_MENU(ID_VIEW_GO_LAST,        Frame::OnGotoLast)
    EVT_MENU(ID_VIEW_STEP,           Frame::OnViewAutoStep)
    EVT_MENU(ID_VIEW_GRID,           Frame::OnGridChange)
    EVT_MENU(ID_VIEW_FULLSCREEN,     Frame::OnViewFullscreen)

    EVT_MENU_RANGE(ID_VIEW_SCALE_ACTUAL, ID_VIEW_SCALE_DOUBLE, Frame::OnViewScaleToggle)
    EVT_MENU_RANGE(ID_CHECKSUM_NONE,     ID_CHECKSUM_SHA1,     Frame::OnViewChecksum)

    EVT_MENU_RANGE(ID_FORMAT_MASK_Y, ID_FORMAT_MASK_V, Frame::OnYUVMaskChange)

    EVT_MENU(ID_HELP_ABOUT,          Frame::OnHelpAbout)

    EVT_UPDATE_UI(ID_VIEW_GRID,      Frame::OnUpdateGridUI)
    EVT_UPDATE_UI(ID_VIEW_STEP,      Frame::OnUpdateUI)
    EVT_UPDATE_UI(Frame::ID_FRAME_SLIDER, Frame::OnUpdateNavUI)

    EVT_UPDATE_UI_RANGE(ID_FILE_OPEN_SPLIT,   ID_FILE_EXIT,           Frame::OnUpdateUI)
    EVT_UPDATE_UI_RANGE(ID_VIEW_NEXT,         ID_VIEW_GOTO,           Frame::OnUpdateNavUI)
    EVT_UPDATE_UI_RANGE(ID_VIEW_SCALE_ACTUAL, ID_VIEW_SCALE_DOUBLE,   Frame::OnUpdateScaleUI)
    EVT_UPDATE_UI_RANGE(ID_CHECKSUM_NONE,     ID_CHECKSUM_SHA1,       Frame::OnUpdateUI)

    EVT_UPDATE_UI_RANGE(ID_FORMAT_SETSIZE,    ID_FORMAT_PROPERTIES,   Frame::OnUpdateUI)
#if 0

    EVT_MENU_RANGE(Frame::ID_VIEW_420, Frame::ID_VIEW_NV21, Frame::OnViewFormat)




//    EVT_UPDATE_UI_RANGE(Frame::ID_FILE_CLOSE,       Frame::ID_FILE_GRID_SETTINGS,   Frame::OnUpdateUI)
    EVT_UPDATE_UI_RANGE(Frame::ID_VIEW_NEXT,        Frame::ID_VIEW_GO_LAST,         Frame::OnUpdateNavUI)
    EVT_UPDATE_UI_RANGE(Frame::ID_FILE_SAVE_IMAGE,  Frame::ID_FILE_SAVE_YUV_COMP,   Frame::OnUpdateUI)
#endif  // 0

    /* slider control events */
    EVT_COMMAND_SCROLL(Frame::ID_FRAME_SLIDER, Frame::OnSlider)

    /* system events */
    EVT_TIMER(Frame::ID_TIMER, Frame::OnTimer)

    EVT_COMMAND( wxID_ANY, wxEVT_LOADIMAGE, Frame::OnLoadImage)

    EVT_CLOSE(Frame::OnClose)



END_EVENT_TABLE()

/**
 *  Frame constructor...
 */

Frame::Frame()
    :   wxFrame(NULL, -1, wxT(APP_NAME), wxDefaultPosition, wxSize(640, 540)),
        m_imagePath(wxT(".")),
        m_prefix(wxT("pic")),
        m_imageSize(wxSize(640,480)),
        m_bits(8),
        m_endianness(endian_little),
        m_nFirst(0),
        m_nLast(1000),
        m_yuvFmt(DATA_YUV420),
        m_statBar(0L),
        m_bAutoStep(false),
        m_pChksumDlg(0L),
        m_chksumAlgo(0L)
{
    wxToolBar*  pToolBar = 0L;
//    wxMenuBar*  pMenuBar = 0L;
    wxMenu*     pMenu    = 0L;
//    wxMenu*     subMenu	 = 0L;

    wxLogDebug("Frame::Frame()");

//  crc32_be_init();

    /* create tool bar */
    pToolBar = CreateToolBar( /* wxTB_DEFAULT_STYLE */ );
    wxASSERT( pToolBar != 0L );

#if wxCHECK_VERSION(2,9,5)
    pToolBar->AddTool(ID_FILE_SAVE_IMAGE, wxT("Save Bitmap Image"),
                      wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR),
                      wxT("Save Bitmap Image"), wxITEM_DROPDOWN);

    pMenu = new wxMenu;

    pMenu->Append( ID_FILE_SAVE_YUV_COMP,   wxT("Save YUV Composite"),  wxT("Save YUV Composite") );
    pMenu->Append( ID_FILE_SAVE_YUV_SPLIT,  wxT("Save YUV Split"),      wxT("Save YUV Split") );
    pMenu->Append( ID_FILE_SAVE_IMAGE,      wxT("Save Bitmap Image"),   wxT("Save Bitmap Image") );

    pToolBar->SetDropdownMenu( ID_FILE_SAVE_IMAGE, pMenu );
#else
    pToolBar->AddTool(ID_FILE_SAVE_IMAGE, wxT("Save Bitmap Image"),
                      wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR),
                      wxT("Save Bitmap image") );
#endif

    pToolBar->AddTool(ID_VIEW_GOTO, wxT("Goto Frame"),
                      wxArtProvider::GetBitmap(wxART_FIND, wxART_TOOLBAR),
                      wxT("Goto frame"));

    pToolBar->AddCheckTool(ID_VIEW_GRID, "Toggle Grid", wxBitmap( grid_small ) , wxNullBitmap, "Enable/Disable Grid" );

#ifdef  __WXGTK__
    pToolBar->AddCheckTool(ID_VIEW_STEP, wxT("Auto-Step"),
                           wxArtProvider::GetBitmap(wxT("media-playback-start"), wxART_TOOLBAR), wxNullBitmap,
                           wxT("Auto-Step"));
#endif

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
    pToolBar->AddTool(ID_VIEW_NEXT,     wxT("Next"), wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR), wxT("Goto Next"));
#if wxCHECK_VERSION(2,9,5)
    pToolBar->AddTool(ID_VIEW_GO_LAST,  wxT("Goto Last Frame"), wxArtProvider::GetBitmap(wxART_GOTO_LAST, wxART_TOOLBAR), wxT("Goto Last"));
#endif
    pToolBar->Realize();

    SetToolBar(pToolBar);

    m_pMenuBar = CreateMenuBar();
    SetMenuBar(m_pMenuBar);

    /* Create status bar */
    m_statBar = CreateStatusBar(3);
    wxASSERT( m_statBar != 0L );

#if wxCHECK_VERSION(2,9,5)
    int sbStyles[3] = {
        wxSB_SUNKEN,
        wxSB_SUNKEN,
        wxSB_SUNKEN,
    };
    m_statBar->SetStatusStyles(3, sbStyles);
#endif

    /* Create the checksum dialog */
    m_pChksumDlg = new ChecksumDialog(this);
    wxASSERT(m_pChksumDlg != 0L);
    m_pChksumDlg->Center();

    wxBoxSizer*     pSizer = 0L;

    pSizer = new wxBoxSizer( wxVERTICAL );
    assert(pSizer != 0L);

    /* Add the image control to the frame */
    m_imageControl = new yuvImageControl(this, wxID_ANY);
    wxASSERT( m_imageControl != 0L );
    pSizer->Add( m_imageControl, 1, wxEXPAND );

    m_imageControl->SetImageSize( m_imageSize );

    SetSizer( pSizer );

    LoadOptions();

    /* post event to cause 1st image to load */
    wxCommandEvent          event(wxEVT_LOADIMAGE, GetId());
    event.SetEventObject( this );

    GetEventHandler()->AddPendingEvent(event);

    /* start all timers */
    m_timer.SetOwner(this, ID_TIMER);

    /* Set the windows title */
    wxString sTmp = wxString::Format(wxT("%s V%d.%d Build %s"),
                                     APP_NAME,
                                     VERSION_MAJOR,
                                     VERSION_MINOR,
                                     __DATE__);
    SetTitle(sTmp);

    /* Set the windows icon */
    wxIcon* pWindowIcon = new wxIcon( mag_small) ;//, 32, 32);
    SetIcon(*pWindowIcon);
    delete pWindowIcon;

    SetFocus();
}

/**
 *
 */

Frame::~Frame() {
    wxLogDebug("Frame::~Frame()");

    if (m_chksumAlgo != 0L) {
        delete m_chksumAlgo;
        m_chksumAlgo = 0L;
    }
}

/**
 *
 */

void Frame::OnClose(wxCloseEvent& event) {
    wxLogDebug("Frame::OnClose()");

    SaveOptions();
    Destroy();

    return;
}

/**
 *
 */

void Frame::OnExit(wxCommandEvent& event) {
    Close();
}

/**
 *  Set the image width and height.
 */

void Frame::OnFormatSetSize(wxCommandEvent& event) {
    SizeDialog      dialog(this, m_imageSize.GetWidth(),
                           m_imageSize.GetHeight());

    wxLogDebug("Frame::OnFormatSetSize()");

    if (dialog.ShowModal() == wxID_OK) {
        long w, h;

        dialog.GetFrameDimensions( w, h );

        if ((w > 0) && (h > 0)) {
            m_imageSize.Set(w,h);

            if (m_imageControl->IsValid()) {
                m_imageControl->GotoFrame(0);
                m_imageControl->SetImageSize( m_imageSize );
                m_imageControl->RefreshImage();
            }

            UpdateStatusBar();
            UpdateSlider();
            UpdateChecksum();
        }
    }

    return;
}

/** Display the next frame image. */
void Frame::OnViewNext(wxCommandEvent& event) {
    wxLogDebug("Frame::OnViewNext()");

    if (m_imageControl->NextFrame()) {
        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    } else {
        wxLogDebug("WARNING: Attempting to move past last frame...");
#ifdef  ENABLE_SOUND
        utils::Beep();
#endif
    }
}

/** Display the previous frame image. */
void Frame::OnViewPrevious(wxCommandEvent& event) {
    wxLogDebug("Frame::OnViewPrevious()");

    if (m_imageControl->PrevFrame()) {
        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    } else {
        wxLogDebug("WARNING: Attempting to move past first frame...");
#ifdef  ENABLE_SOUND
        utils::Beep();
#endif
    }
}

/** Prompt user for frame number and set image id. */
void Frame::OnGotoFrame(wxCommandEvent& event) {
    size_t lastFrame, curFrame;
    long n;

    wxLogDebug("Frame::OnGotoFrame()");

    if (m_imageControl->IsValid()) {
        lastFrame = m_imageControl->GetImageCount();
        curFrame  = m_imageControl->GetCurrentFrame();

        n = ::wxGetNumberFromUser(wxT("Please select a frame"), wxT("Frame #"),
                                  wxT("Select frame"), (long)curFrame, 1, (long)lastFrame);

        if (n != -1) {
            if (m_imageControl->GotoFrame( n - 1 )) {
                UpdateStatusBar();
                UpdateSlider();
                UpdateChecksum();
            }
        }
    } else {
        wxLogDebug("WARNING: Attempting to open 'Goto Frame' dialog without a valid image control!");
    }
}

/**
 *
 */

void Frame::UpdateStatusBar() {
    wxString    sMsg;
    wxSize      imageSize;

    wxLogDebug("Frame::UpdateStatusBar()");

    if (m_imageControl->IsValid()) {
        /* Get buffer identifier */
        sMsg = m_imageControl->GetIdentifier();
        SetStatusText(sMsg, 0);

        sMsg = wxString::Format(wxT("Frame # %d of %d "),
                                (int)m_imageControl->GetCurrentFrame() + 1,
                                (int)m_imageControl->GetImageCount() + 1);
        SetStatusText(sMsg, 1);

        imageSize = m_imageControl->GetImageSize();

        if (m_imageControl->GetImageBits() > 8) {
            sMsg = wxString::Format(wxT("Frame Dimensions : %dx%d - %d bit (%s) %s"),
                                    imageSize.GetWidth(),
                                    imageSize.GetHeight(),
                                    m_imageControl->GetImageBits(),
                                    (m_imageControl->GetEndianness() == endian_little)?"LE":"BE",
                                    m_imageControl->GetYUVFormatString());
        } else {
            sMsg = wxString::Format(wxT("Frame Dimensions : %dx%d - %d bit %s"),
                                    imageSize.GetWidth(),
                                    imageSize.GetHeight(),
                                    m_imageControl->GetImageBits(),
                                    m_imageControl->GetYUVFormatString());
        }
        SetStatusText(sMsg,2);
    } else {
        SetStatusText("Please open a file", 0);
        SetStatusText("", 1);
        SetStatusText("", 2);
    }

    return;
}

/**
 *
 */

void Frame::OnViewScaleToggle(wxCommandEvent& event) {
    scaleVal       scale; //, i;

    wxLogDebug("Frame::OnViewScaleToggle()");

    scale = (scaleVal)(event.GetId() - ID_VIEW_SCALE_ACTUAL);

    m_imageControl->SetImageScale( scale );
}

/**
 *  Enable/Disable automatic advance on timer.
 */

void Frame::OnViewAutoStep(wxCommandEvent& event) {
    if (event.IsChecked()) {
        m_bAutoStep = true;
        m_timer.Start(200);
    } else {
        m_bAutoStep = false;
        m_timer.Stop();
    }
}

/**
 *  Advance frame id on timer event.
 */

void Frame::OnTimer(wxTimerEvent& event) {
    wxLogDebug("Frame::OnTimer()");

    if (m_bAutoStep) {
        if (m_imageControl->NextFrame()) {
            UpdateStatusBar();
            UpdateSlider();
            UpdateChecksum();
        } else {
            wxLogDebug("No more frames - disabling timer...");
            m_bAutoStep = false;
            GetMenuBar()->FindItem(ID_VIEW_STEP)->Check(false);
            m_timer.Stop();
        }
    }

    return;
}

/**
 *  Load options from config file. If commandline parameters were specified
 *  then over-ride configuration file.
*/

void Frame::LoadOptions() {
    wxCmdLineParser&        parser = wxGetApp().m_cmdLine;
    wxConfig*               config = new wxConfig(wxT("frameInspector"));
    wxString                sTmp;
    long                    x, y,
                            w, h;
    controlParms            parms;

    wxLogDebug("Frame::LoadOptions()");

    /* get window position */
    if (config->Read(wxT("screenpos"), &sTmp)) {
        sscanf(sTmp.ToAscii(), "%ld,%ld,%ld,%ld", &x, &y, &w, &h);

        wxLogDebug("Screen position @ %ld,%ld,%ld,%ld", x, y, w, h);

        wxRect screenPos(x,y,w,h);
        SetSize(screenPos);
    }

    /* get checksum dialog position */
    if (config->Read("crcpos", &sTmp)) {
        sscanf(sTmp.ToAscii(), "%ld,%ld", &x, &y);

        wxLogDebug("crc position @ %ld,%ld", x, y);
        m_pChksumDlg->SetPosition(wxPoint(x,y));
    }

    if (parms.LoadParmsFromConfig( config, "control1" )) {
        wxLogDebug("Parameters loaded from config!");
    }

    if (ParseCmdLine(parser, parms)) {
        wxLogDebug("Parsed commandline options!");
    }

    if (m_imageControl->SetState( parms )) {
        m_imageFile     = m_imageControl->GetImageName();
        m_imagePath     = m_imageControl->GetImagePath();
        m_imageSize     = m_imageControl->GetImageSize();
        m_bits          = m_imageControl->GetImageBits();
        m_endianness    = m_imageControl->GetEndianness();
        m_prefix        = m_imageControl->GetImagePrefix();
        m_nFirst        = parms.m_spec.m_first;
        m_nLast         = parms.m_spec.m_last;

        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    } else {
        wxLogDebug("Unable to set state of control!");
    }

    return;
}

/**
 *  Save options to config file. Save image path, id, width, height, scaling
 *  and screen position to configuration file.
*/

void Frame::SaveOptions() {
    wxConfig*   conf;
    wxRect      screenPos = GetRect();
    wxString    sTmp;
    int         x,y;

    wxLogDebug("Frame::SaveOptions()");

    conf = new wxConfig(wxT("frameInspector"));
    wxASSERT( conf != 0L );

    sTmp = wxString::Format(wxT("%d,%d,%d,%d"),
                            screenPos.x, screenPos.y, screenPos.width, screenPos.height);
    conf->Write(wxT("screenpos"),   sTmp);
//    conf->Write(wxT("path"),        m_imagePath);

    m_pChksumDlg->GetPosition(&x, &y);
    sTmp = wxString::Format(wxT("%d,%d"), x, y);
    conf->Write(wxT("crcpos"),      sTmp);

    if (m_imageControl->SaveState( conf, "control1" )) {

    } else {
        wxLogDebug("WARNING: SaveState returned false!");
    }

    delete conf;
}

/**
 *
 */

void Frame::OnGotoHome(wxCommandEvent& event) {
    wxLogDebug("Frame::OnGotoHome()");

    if (m_imageControl->IsValid()) {
        m_imageControl->GotoFrame( 0 );
        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    }
}

/**
 *
 */

void Frame::OnGotoLast(wxCommandEvent& event) {
    wxLogDebug("Frame::OnGotoLast()");

    if (m_imageControl->IsValid()) {
        m_imageControl->GotoFrame( m_imageControl->GetImageCount() );
        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    }
}

/**
 *
 */

void Frame::OnSlider(wxScrollEvent& event) {
    wxEventType     evtType = event.GetEventType();

    wxLogDebug("Frame::OnSlider() [Event = %08x]", (int)evtType);

    if ((evtType == wxEVT_SCROLL_THUMBRELEASE) ||
            (evtType == wxEVT_SCROLL_THUMBTRACK) ||
            (evtType == wxEVT_SCROLL_CHANGED))
    {
        size_t frame = event.GetPosition();

        if (m_imageControl->GotoFrame( frame )) {
            UpdateStatusBar();
            UpdateChecksum();
        } else {
            wxLogDebug("WARNING: GotoFrame returned false!");
        }
    }

    return;
}

/**
 *  Display the 'About' box...
 */

void Frame::OnHelpAbout(wxCommandEvent& event) {
    wxAboutDialogInfo           dlgInfo;
    wxString                    sVersion;
    wxString                    sDescription;

    wxLogDebug("Frame::OnHelpAbout()");

    sDescription = wxString::Format(wxT("Versatile YUV image viewer.\nBuilt with %s."),
                                    wxVERSION_STRING);

    dlgInfo.SetName(wxT("frameInspector"));
    dlgInfo.SetDescription(sDescription);

    dlgInfo.AddDeveloper(wxT("Michael A. Uman"));
    dlgInfo.SetCopyright(wxT("(C) 2016-2017 Uman Software Designs"));
    dlgInfo.SetWebSite(wxT("ftp://laserquad.ddns.net"));

    sVersion = wxString::Format(wxT("%d.%d"), VERSION_MAJOR, VERSION_MINOR);
    dlgInfo.SetVersion(sVersion);

    wxAboutBox( dlgInfo );
}

/**
 *  Open a YUV split directory.
 */

void Frame::OnFileOpenSplit(wxCommandEvent& event) {
    YUVSplitDialog  openDlg(this);

    wxLogDebug("Frame::OnFileOpenSplit()");

    YUV420ImageSpec     spec( m_imagePath, m_prefix, m_nFirst, m_nLast);

//    openDlg.SetDialogOptions( m_imageSize, m_imagePath, m_prefix, m_bits,
//                              m_endianness, m_yuvFmt );
    openDlg.SetDialogOptions( m_imageSize, spec, m_bits,
                              m_endianness, m_yuvFmt );

    if (openDlg.ShowModal() == wxID_OK) {
        YUV420ImageSpec spec;

        openDlg.GetFileInfo( m_imagePath, m_prefix );
        openDlg.GetFrameDimensions( m_imageSize );
        openDlg.GetFrameBits( m_bits );
        openDlg.GetFrameEndianness( m_endianness );
        openDlg.GetFrameYUVFmt( m_yuvFmt );
        openDlg.GetImageSpec( spec );

        m_nFirst = spec.m_first;
        m_nLast  = spec.m_last;

        if (m_imageControl->OpenYUVSplit( spec, m_imageSize,
                                          m_bits, m_yuvFmt, m_endianness ))
        {
            UpdateStatusBar();
            UpdateSlider();
            UpdateChecksum();
        }
    }

    return;
}

/**
 *  Handle user request to open a YUV file.
 */

void Frame::OnFileOpenFile(wxCommandEvent& event) {
    wxString    newFile;

    wxLogDebug("Frame::OnFileOpenFile()");

    /** NOTE: This code does not work due to a bug in the wxFilePickerCntl class */

    YUVCompDialog           openDlg(this);

    openDlg.SetDialogOptions( m_imageSize, m_imagePath, m_imageFile, m_bits, m_endianness, m_yuvFmt );

    if (openDlg.ShowModal() == wxID_OK) {

        openDlg.GetFilePath( newFile );
        openDlg.GetFrameDimensions( m_imageSize );
        openDlg.GetFrameBits( m_bits );
        openDlg.GetFrameEndianness( m_endianness );
        openDlg.GetFrameYUVFmt( m_yuvFmt );

        if (m_imageControl->OpenYUVComposite( newFile, m_imageSize, m_bits, m_yuvFmt, m_endianness )) {
            wxFileName  fName(newFile);

            UpdateStatusBar();
            UpdateSlider();
            UpdateChecksum();

            m_imagePath     = fName.GetPath();
            m_imageFile     = newFile;
        }
    }

    return;
}

/**
 *  Handle user request to open a 'dump' file...
 */

void Frame::OnFileOpenDump(wxCommandEvent& event) {
    wxString dir;

    wxLogDebug("Frame::OnFileOpenDump()");

    dir  = ::wxDirSelector(wxT("Select dump output path"), m_imagePath);

    if (!dir.IsEmpty()) {
        if (m_imageControl->OpenYUVDump( dir )) {
            m_imageSize = m_imageControl->GetImageSize();
            m_imagePath = dir;

            UpdateStatusBar();
            UpdateSlider();
        }
    }

    return;
}

/**
 *
 */

void Frame::OnFileClose(wxCommandEvent& event) {
    wxLogDebug("Frame::OnFileClose()");

    if (m_imageControl->IsValid()) {
        m_imageControl->CloseImage();
        m_pChksumDlg->Show(FALSE);
        if (m_chksumAlgo != 0L) {
            delete m_chksumAlgo;
            m_chksumAlgo = 0L;
        }
        UpdateStatusBar();
        UpdateSlider();
    }
}

/**
 *
 */

/**
 *  Refresh the display.
 */

void Frame::OnRefresh(wxCommandEvent& event) {
    wxLogDebug("Frame::OnRefresh()");

    RefreshImage();
//    if (m_imageControl->IsValid()) {
//        m_imageControl->RefreshImage();
//        UpdateStatusBar();
//        UpdateSlider();
//    }

    return;
}


/**
 *  Save image as a bitmap file.
 */

void Frame::OnFileSaveAs(wxCommandEvent& event) {
    wxString	fileName;

    wxLogDebug("Frame::OnFileSaveAs()");

    fileName = ::wxFileSelector(wxT("File to save (Bitmap Image)"), wxEmptyString, wxEmptyString,
                                wxEmptyString, wxFileSelectorDefaultWildcardStr,
                                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (!fileName.IsEmpty()) {
        wxLogDebug("-- saving file %s...", (const char*)fileName.c_str());
        SaveBitmap(fileName);
    }

    return;
}

/**
 *  Save frame as a YUV split file.
 */

void Frame::OnFileSaveYUVSplit(wxCommandEvent& event) {
    wxString        sFilename;

    wxLogDebug("Frame::OnFileSaveYUVSplit()");

    sFilename = ::wxFileSelector(wxT("File to save (YUV Split)"), wxEmptyString, wxEmptyString,
                                 wxT("*.yuv"), wxT("YUV Files (*.yuv)|*.yuv"),
                                 wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (!sFilename.IsEmpty()) {
        wxLogDebug("-- saving YUV split file %s...", (const char*)sFilename.c_str());
        m_imageControl->SaveYUVData( sFilename, ImageBuffer::SAVE_YUV_SPLIT );
    }

    return;
}

/**
 *  Save frame as a composite YUV file.
 */

void Frame::OnFileSaveYUVComp(wxCommandEvent& event) {
    wxString        sFilename;

    wxLogDebug("Frame::OnFileSaveYUVComp()");

    sFilename = ::wxFileSelector(wxT("File to save (YUV Composite)"), wxEmptyString, wxEmptyString,
                                 wxEmptyString, wxT("YUV Files (*.yuv)|*.yuv"),
                                 wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (!sFilename.IsEmpty()) {
        wxLogDebug("-- saving YUV composite file %s...", (const char*)sFilename.c_str());
        m_imageControl->SaveYUVData( sFilename, ImageBuffer::SAVE_YUV_COMP );
    }

    return;
}

/**
 *  Handle wxEVT_LOADIMAGE message.
 */

void Frame::OnLoadImage(wxCommandEvent& event) {
    wxLogDebug("Frame::OnLoadImage()");

//    GetImage();
}

/**
 *
 */

void Frame::OnGridChange(wxCommandEvent& event) {
    bool bGrid = m_imageControl->get_grid_state();

    wxLogDebug("Frame::OnGridChange()");

    if (bGrid) {
        m_imageControl->disable_grid();
    } else {
        m_imageControl->enable_grid();
    }

    return;
}

/**
 *  Save the current bitmap image to a file.
 */

void Frame::SaveBitmap(wxString sFilename) {
    wxLogDebug("Frame::SaveBitmap(%s)", (const char*)sFilename.c_str());

    if (m_imageControl->IsValid()) {
        if (m_imageControl->SaveBitmap( sFilename )) {

        } else {
            wxLogDebug("WARNING: Unable to save bitmap file!");
        }
    } else {
        wxLogDebug("WARNING: Trying to save bitmap from invalid control!");
    }
}

/**
 *  Toggle full-screen mode
 */

void Frame::OnViewFullscreen(wxCommandEvent& event) {
    wxLogDebug("Frame::OnViewFullscreen()");

    if (IsFullScreen()) {
        ShowFullScreen(false);
    } else {
        ShowFullScreen(true, wxFULLSCREEN_NOBORDER|wxFULLSCREEN_NOCAPTION);
    }

    return;
}

/**
 *  Allow user to adjust the grid settings...
 */

void Frame::OnGridSettings(wxCommandEvent& event) {
    wxLogDebug("Frame::OnGridSettings()");

    int gridH, gridV;
    wxColor gridColor;
    bool gridState;

    GridSettingsDialog  gridDlg(this) ;//, wxID_ANY);

    m_imageControl->get_grid_settings( gridH, gridV, gridColor, gridState );

    gridDlg.m_spinH->SetValue(gridH);
    gridDlg.m_spinV->SetValue(gridV);
    gridDlg.m_gridColorControl->SetColour( gridColor );
    gridDlg.m_spinH->SetFocus();

    if (gridDlg.ShowModal() == wxID_OK) {
        gridH = gridDlg.m_spinH->GetValue();
        gridV = gridDlg.m_spinV->GetValue();
        gridColor = gridDlg.m_gridColorControl->GetColour();

        m_imageControl->set_grid_dimensions( gridH, gridV, gridColor );
    }
}

/**
 * Handle UpdateUI event...
 */

void Frame::OnUpdateUI(wxUpdateUIEvent& event) {
    int eventID = event.GetId();

    //wxLogDebug("OnUpdateUI id %d!", eventID);

    switch (eventID) {
    case ID_FILE_CLOSE:
    case ID_FILE_REFRESH:
    case ID_FILE_SAVE_IMAGE:
    case ID_FILE_SAVE_YUV_SPLIT:
    case ID_FILE_SAVE_YUV_COMP:
        event.Enable(m_imageControl->IsValid());
        break;

#ifdef  ENABLE_CONVERT
    case ID_FILE_CONVERT:
        event.Enable(m_imageControl->IsValid());
        break;

    case ID_FILE_CONV_TO_COMPOSITE:
        event.Enable(m_imageControl->GetBufferType() != YUV_FILE_COMP);
        break;

    case ID_FILE_CONV_TO_SPLIT:
        event.Enable(m_imageControl->GetBufferType() != YUV_FILE_SPLIT);
        break;
#endif  // ENABLE_CONVERT

    case ID_FORMAT_SETSIZE:
        event.Enable(m_imageControl->CanSetSize());
        break;

    case ID_FORMAT_YUV:
        event.Enable(m_imageControl->IsValid());
        break;

    case ID_FORMAT_MASK_Y:
    case ID_FORMAT_MASK_U:
    case ID_FORMAT_MASK_V:
        updateFormatMask(event);
        break;

    case ID_VIEW_STEP:
        if (m_imageControl->IsValid()) {
            event.Enable(true);
            event.Check(m_bAutoStep);
        } else {
            event.Enable(false);
        }
        break;

    default:
        if ((eventID >= ID_CHECKSUM_NONE) &&
                (eventID <= ID_CHECKSUM_SHA1))
        {
            wxLogDebug("OnUpdateUI checksum ID %d", eventID);

            if (m_imageControl->IsValid()) {
                eChecksumAlgo eAlgo = CHECKSUM_UNDEFINED;

                eAlgo = (eChecksumAlgo)((int)eAlgo + (eventID - ID_CHECKSUM_NONE));

                if (m_chksumAlgo) {
                    eChecksumAlgo curAlgo = m_chksumAlgo->checksum_type();

                    event.Check(eAlgo == curAlgo);

                } else {
                    event.Check(eventID == ID_CHECKSUM_NONE);
                }
                event.Enable(true);
            } else {
                event.Enable(false);
            }
        }
        break;
    }

    return;
}

/**
 *  Enable/Disable grid UI elements.
 */

void Frame::OnUpdateGridUI(wxUpdateUIEvent& event) {
#ifdef  DEBUG_UPDATE_UI
    wxLogDebug("Frame::OnUpdateGridUI()");
#endif

    if (m_imageControl->IsValid()) {
        event.Enable(true);
        event.Check(m_imageControl->get_grid_state());
    } else {
        event.Enable(false);
    }

    return;
}

/**
 *  Display the checksum/hash dialog.
 */

void Frame::OnViewChecksum(wxCommandEvent& event) {
    int eventID         = event.GetId();
    eChecksumAlgo eAlgo = CHECKSUM_UNDEFINED;

    wxLogDebug("Frame::OnViewChecksum() id %d", eventID);

    eAlgo = (eChecksumAlgo)((int)eAlgo + (eventID - ID_CHECKSUM_NONE));

    wxLogDebug("eAlgo = %d", (int)eAlgo);

    if (eAlgo != CHECKSUM_UNDEFINED) {
        if (m_chksumAlgo != 0L) {
            delete m_chksumAlgo;
            m_chksumAlgo = 0L;
        }
        m_chksumAlgo = m_chksumMgr.GetAlgoClass( eAlgo );

        wxASSERT( m_chksumAlgo != 0L );

        m_pChksumDlg->SetAlgoName( m_chksumAlgo->checksum_name() );
        m_pChksumDlg->Show(TRUE);

        UpdateChecksum();
    } else {
        m_pChksumDlg->Show(FALSE);

        if (m_chksumAlgo != 0L) {
            delete m_chksumAlgo;
            m_chksumAlgo = 0L;
        }
    }

//    if (event.IsChecked()) {
//        m_pHashDlg->Show(TRUE);
//    } else {
//        m_pHashDlg->Show(FALSE);
//    }
}

/**
 *
 */

bool Frame::LoadReferenceCRC(wxString sCRCName) {
    bool bRes = false;

    wxLogDebug("Frame::LoadReferenceCRC(%s)", (const char*)sCRCName.c_str());

    bRes = m_refCRC.LoadFile( sCRCName );

#ifdef  __WXDEBUG__
    wxLogDebug("result %d", (int)bRes);

    if (bRes) {
        wxLogDebug("-- found %ld frames!", m_refCRC.size());
        m_refCRC.Dump(stderr);
    }
#endif

    return bRes;
}

/**
 *
 */

void Frame::OnUpdateNavUI(wxUpdateUIEvent& event) {
#ifdef  DEBUG_UPDATE_UI
    wxLogDebug("Frame::OnUpdateNavUI()");
#endif

    event.Enable(m_imageControl->IsValid());

    return;
}

/**
 *
 */

void Frame::OnUpdateScaleUI(wxUpdateUIEvent& event) {
    int id = event.GetId();
    int scale = (int)m_imageControl->GetImageScale();

#ifdef  DEBUG_UPDATE_UI
    wxLogDebug("Frame::OnUpdateScaleUI()");
#endif


    if (m_imageControl->IsValid()) {
        event.Enable(true);

        if ((id - ID_VIEW_SCALE_ACTUAL) == scale) {
            event.Check(true);
        } else {
            event.Check(false);
        }
    } else {
        event.Enable(false);
    }
}

/**
 *
 */

void Frame::UpdateSlider() {
    wxLogDebug("Frame::UpdateSlider()");

    if (m_imageControl->IsValid()) {
        size_t count = m_imageControl->GetImageCount();
        if (count > 1) {
            m_sliderCntrl->Enable(true);
            m_sliderCntrl->SetRange( 0, m_imageControl->GetImageCount() );
            m_sliderCntrl->SetValue( m_imageControl->GetCurrentFrame() );
        } else {
            m_sliderCntrl->Enable(false);
            m_sliderCntrl->SetRange( 0, 0 );
            m_sliderCntrl->SetValue( 0 );
        }
    } else {
        m_sliderCntrl->Enable(false);
        m_sliderCntrl->SetRange( 0, 0 );
        m_sliderCntrl->SetValue( 0 );
    }
}

/**
 *
 */

void Frame::UpdateChecksum() {
    checksumEntry       entry;

    wxLogDebug("Frame::UpdateChecksum()");

    if (m_imageControl->IsValid() && (m_chksumAlgo != 0)) {
        if (m_imageControl->GetChecksum( m_chksumAlgo, entry )) {
            wxString sLuma, sChroma;

            wxLogDebug("Checksum OK");

            sLuma   = entry.GetLumaSum(false);
            sChroma = entry.GetChromaSum(false);

            m_pChksumDlg->SetChecksumValues( sLuma, sChroma );
        }
    }

    return;
}

/**
 *
 */

bool Frame::RefreshImage() {
    bool bRes = false;

    wxLogDebug("Frame::RefreshImage()");

    if (m_imageControl->IsValid()) {
        m_imageControl->RefreshImage();
        UpdateStatusBar();
        UpdateSlider();
        bRes = true;
    }

    return bRes;
}

/**
 *
 */

bool Frame::GetFormatBits(wxUint16& bits) {
    bool        bRes = false;
    wxString    choices[] = { "8-bit", "10-bit" };

    wxSingleChoiceDialog        dlg(this, "Bit Depth", "Bits Per Component", 2, choices);

    switch (m_bits) {
    case 8:
        dlg.SetSelection(0);
        break;
    case 10:
        dlg.SetSelection(1);
        break;
    }

    if (dlg.ShowModal() == wxID_OK) {
        switch (dlg.GetSelection()) {
        case 0:
            bits = 8;
            break;
        case 1:
            bits = 10;
            break;
        }

        bRes = true;
    }

    return bRes;
}

/**
 *
 */

bool Frame::ParseCmdLine(wxCmdLineParser& parser, controlParms& parms) {
    bool bRes = false;
    int cnt = 0;
    wxString sInputFile, sInputPath, sDumpPath, sPrefix, sTmp;
    long w,h;
    long nTmp = 0;

    wxLogDebug("Frame::ParseInputFile(...)");

    cnt += (parser.Found("f")?1:0) +
           (parser.Found("p")?1:0) +
           (parser.Found("d")?1:0);

    if (cnt == 1) {
        if (parser.Found("f", &sInputFile)) {
            imageParms  image_parms;

            parms.m_bufType       = YUV_FILE_COMP;
            parms.m_imageFilename = sInputFile;
            parms.m_yuvType       = DATA_YUV420;    // assume YUV 4:2:0

            if (utils::ParseFilePath( sInputFile, image_parms )) {
                parms.m_imageSize.Set( image_parms.width, image_parms.height );

                if (image_parms.depth != -1)
                    parms.m_bits = image_parms.depth;
            }
            bRes = true;
        } else if (parser.Found("p", &sInputPath)) {
            parms.m_bufType   = YUV_FILE_SPLIT;
            parms.m_imagePath = sInputPath;
            parms.m_yuvType   = DATA_YUV420;    // assume YUV 4:2:0

            if (parser.Found("r", &sPrefix)) {
                parms.m_prefix = sPrefix;
            }
            bRes = true;
        } else if (parser.Found("d", &sDumpPath)) {
            parms.m_bufType   = YUV_FILE_DUMP;
            parms.m_imagePath = sDumpPath;
            parms.m_yuvType   = DATA_YUV420;    // assume YUV 4:2:0

            bRes = true;
        }
    }

    /* get image width and height */
    if (parser.Found("s", &sTmp)) {
        sscanf(sTmp.ToAscii(), "%ldx%ld", &w, &h);
        parms.m_imageSize.Set(w,h);
        bRes = true;
    }
    /* get current frame */
    if (parser.Found("c", &nTmp)) {
        parms.m_curFrame = (nTmp - 1);
        bRes = true;
    }
    /* get bits per component */
    if (parser.Found("b", &nTmp)) {
        parms.m_bits = nTmp;
        bRes = true;
    }

    return bRes;
}

#ifdef  ENABLE_CONVERT
/**
 *  Handle conversion.
 */

void Frame::OnConvertTo(wxCommandEvent& event) {
    int id = event.GetId();

    wxLogDebug("Frame::OnConvertTo()");

    switch (id) {
    case ID_FILE_CONV_TO_SPLIT:
    case ID_FILE_CONV_TO_COMPOSITE:
        break;
    }

    return;
}
#endif  // ENABLE_CONVERT

/**
 *  Create an instance of the frameInspectorMenu object...
 */

wxMenuBar* Frame::CreateMenuBar() {
    wxMenuBar   *pMenuBar   = 0L;

    pMenuBar = new frameInspectorMenu;
    wxASSERT( pMenuBar != 0L );

    return pMenuBar;
}

/**
 *
 */

void Frame::OnFormatYUV(wxCommandEvent& event) {
    formatDialog        dlg(this);

    wxLogDebug("Frame::OnFormatYUV()");

    dlg.SetDialogOptions( m_imageControl->GetYUVFormat(),
                          m_imageControl->GetImageBits(),
                          m_imageControl->GetEndianness() );

    if (dlg.ShowModal() == wxID_OK) {
        dataType            yuvFmt;
        int                 bits;
        formatEndian endian;

        dlg.GetDialogOptions(yuvFmt, bits, endian);

        m_bits          = bits;
        m_endianness    = endian;

        m_imageControl->SetYUVFormat(yuvFmt, bits, endian);
        RefreshImage();

        UpdateStatusBar();
        UpdateSlider();
        UpdateChecksum();
    }

    return;
}

/**
 *  Change the YUV mask.
 */

void Frame::updateFormatMask(wxUpdateUIEvent& event)
{

    if (m_imageControl != 0L) {
        yuvMask mask;
        int eventID = event.GetId();
        bool bChecked = false;

        if (m_imageControl->getYUVMask( mask )) {
            switch (eventID) {
            case ID_FORMAT_MASK_Y:
                bChecked = mask.bMaskY;
                break;
            case ID_FORMAT_MASK_U:
                bChecked = mask.bMaskU;
                break;
            case ID_FORMAT_MASK_V:
                bChecked = mask.bMaskV;
                break;
            }

            event.Check( bChecked );
        } else {

        }
    } else {

    }
}

/**
 *
 */

void Frame::OnYUVMaskChange(wxCommandEvent& event) {
    int         id = event.GetId();
    yuvMask     mask;

    wxASSERT(m_imageControl != 0L);

    if ((m_imageControl != 0L) && (m_imageControl->getYUVMask( mask ))) {
        switch (id) {
        case ID_FORMAT_MASK_Y:
            mask.bMaskY = !mask.bMaskY;
            break;
        case ID_FORMAT_MASK_U:
            mask.bMaskU = !mask.bMaskU;
            break;
        case ID_FORMAT_MASK_V:
            mask.bMaskV = !mask.bMaskV;
            break;
        }
        m_imageControl->setYUVMask( mask );

        RefreshImage();
    }

    return;
}
