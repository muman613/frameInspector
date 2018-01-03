//==============================================================================
//  MODULE      :   frame.h
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   September 30, 2013
//==============================================================================

#ifndef __FRAME_H__
#define __FRAME_H__

#include <wx/wx.h>
#include <wx/timer.h>

#include "imageBuffer.h"
#include "crcVector.h"
#include "yuvImageControl.h"
#include "Dialogs.h"
#include "misc_utils.h"

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif


class ChecksumDialog;
class controlParms;

/**
 *
 */

class Frame : public wxFrame {
public:
    Frame();
    ~Frame();

    // NOTE: Menu id's moved into new wxMenuBar class...
    enum {
        ID_FRAME_SLIDER = 2000,
        ID_TIMER,
        ID_HEART,
    };

    void        OnClose(wxCloseEvent& event);
    void        OnExit(wxCommandEvent& event);
    void        OnFormatSetSize(wxCommandEvent& event);
    void        OnFileSaveAs(wxCommandEvent& event);
    void        OnFileSaveYUVSplit(wxCommandEvent& event);
    void        OnFileSaveYUVComp(wxCommandEvent& event);
    void        OnFileOpenSplit(wxCommandEvent& event);
    void        OnFileOpenFile(wxCommandEvent& event);
    void        OnFileOpenDump(wxCommandEvent& event);
    void        OnFileOpenY4M(wxCommandEvent& event);
    void        OnFileClose(wxCommandEvent& event);
    void        OnViewNext(wxCommandEvent& event);
    void        OnViewPrevious(wxCommandEvent& event);
    void        OnGotoFrame(wxCommandEvent& event);
    void        OnGotoHome(wxCommandEvent& event);
    void        OnGotoLast(wxCommandEvent& event);
    void        OnViewScaleToggle(wxCommandEvent& event);
    void        OnViewScaleCustom(wxCommandEvent& event);
    void        OnViewAutoStep(wxCommandEvent& event);
    void        OnViewFullscreen(wxCommandEvent& event);
    void        OnViewChecksum(wxCommandEvent& event);
    void        OnHelpAbout(wxCommandEvent& event);
    void        OnTimer(wxTimerEvent& event);
    void        OnHeartbeat(wxTimerEvent& event);
    void        OnPaint(wxPaintEvent& event);
    void        OnSlider(wxScrollEvent& event);
    void        OnGridChange(wxCommandEvent& event);
    void        OnGridSettings(wxCommandEvent& event);
    void        OnUpdateUI(wxUpdateUIEvent& event);
    void        OnUpdateGridUI(wxUpdateUIEvent& event);
    void        OnUpdateNavUI(wxUpdateUIEvent& event);
    void        OnUpdateScaleUI(wxUpdateUIEvent& event);
    void        OnFormatYUV(wxCommandEvent& event);
    void        OnYUVMaskChange(wxCommandEvent& event);

    /// Load options from config file.
    void        LoadOptions();
    /// Save options to config file.
    void        SaveOptions();

    void        OnLoadImage(wxCommandEvent& event);

    void        SaveBitmap(wxString sFilename);

    void        OnRefresh(wxCommandEvent& event);

    bool        LoadReferenceCRC(wxString sCRCName);


    bool        GetFormatBits(wxUint16& bits);

#ifdef  ENABLE_CONVERT
    void        OnConvertTo(wxCommandEvent& event);
#endif

    /// wxWidgets command dispatch table.
    DECLARE_EVENT_TABLE();

private:
    bool        GetImageYSize(size_t* size);
    void        UpdateStatusBar();
    void        UpdateSlider();
    void        UpdateChecksum();
    bool        RefreshImage();
    void        UpdateMenuState();
    bool        ParseCmdLine(wxCmdLineParser& parser, controlParms& parms);
    void        updateFormatMask(wxUpdateUIEvent& event);

    wxMenuBar*  CreateMenuBar();                ///< Create the menubar

    yuvImageControl*        m_imageControl;
    wxString                m_imagePath;        ///< Current image path
    wxString                m_imageFile;        ///< For YUV Composite the file name
    wxString                m_prefix;           ///< Name prefix {i.e. out}
    wxSize                  m_imageSize;        ///< Size of image
    wxUint16                m_bits;             ///< 8 or 10 bits per component
    formatEndian            m_endianness;       ///< Endianness of 10bit components
    int                     m_nFirst        = 0;
    int                     m_nLast         = 1000;
    dataType                m_yuvFmt;
    wxStatusBar*            m_statBar;          ///< Window status bar
    bool                    m_bAutoStep;        ///< Enable automatic timer advance
    wxTimer                 m_timer;            ///< Window timer
    wxSlider*               m_sliderCntrl;

    ChecksumDialog*         m_pChksumDlg;       ///< Pointer to checksum dialog
    wxMenuBar*              m_pMenuBar;         ///< Menu bar

    CRCVECTOR               m_refCRC;           ///< Reference checksum

    checksumAlgoBase*       m_chksumAlgo;       ///< Current checksum algoritm
    checksumManager         m_chksumMgr;        ///< Checksum manager class
};

DECLARE_EVENT_TYPE(wxEVT_LOADIMAGE, -1)


#endif

