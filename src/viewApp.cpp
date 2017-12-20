//==============================================================================
//  MODULE      :   viewApp.cpp
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   October 4, 2013
//==============================================================================

#include <wx/wx.h>
#include <wx/gdicmn.h>
#include <wx/cmdline.h>
#include "viewApp.h"
#include "frame.h"
//#include "dbgutils.h"
#include "imageBuffer.h"

#ifdef  __WXMSW__
#include <intrin.h>
#endif

IMPLEMENT_APP(viewApp)

/**
 *  Commandline arguments for parser.
 */

static const wxCmdLineEntryDesc cmdLineDesc[] = {
    { wxCMD_LINE_SWITCH,    wxT_2("h"), wxT_2("help"),      wxT_2("Display help message"),  wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_OPTION,    wxT_2("f"), wxT_2("file"),      wxT_2("Set YUV File"),          wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,    wxT_2("p"), wxT_2("path"),      wxT_2("Set YUV path"),          wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,    wxT_2("d"), wxT_2("dump"),      wxT_2("Set dump path"),         wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,    wxT_2("s"), wxT_2("size"),      wxT_2("Set image size"),        wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,    wxT_2("c"), wxT_2("frame"),     wxT_2("Set frame #"),           wxCMD_LINE_VAL_NUMBER, },
    { wxCMD_LINE_OPTION,    wxT_2("r"), wxT_2("prefix"),    wxT_2("Set prefix"),            wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,    wxT_2("b"), wxT_2("bits"),      wxT_2("Bits Per Component"),    wxCMD_LINE_VAL_NUMBER, },

    { wxCMD_LINE_NONE },
};

/**
 *  Handle application initialization.
 */

bool viewApp::OnInit() {
#if defined(__WXDEBUG__) && defined(USE_WXLOGWINDOW)
//    m_pLogWindow = new wxLogWindow(NULL, "frameInspector Log Output", true, false);
//    wxLog::SetActiveTarget(m_pLogWindow);
#endif  // USE_WXLOGWINDOW

    wxLogDebug("viewApp::OnInit()");

    int depth = ::wxDisplayDepth();

    wxLogDebug("Screen depth = %d", depth);

    /* if running on a low color density screen, tell the user to prepare for the worst */

    if (depth < 16) {
        ::wxMessageBox(wxT_2("frameInspector should be run on hicolor or truecolor screen.\nDetected 256 color screen!"),
                       wxT_2("Message"), wxICON_EXCLAMATION);
    }

    getCPUID();	/* determine which CPU application is running on */

    ::wxInitAllImageHandlers();

    m_cmdLine.SetDesc(cmdLineDesc);
    m_cmdLine.SetCmdLine(wxGetApp().argc, wxGetApp().argv);
    wxString sLogo;
    sLogo = wxString::Format(wxT("%s v%d.%d %s"), APP_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE);
    m_cmdLine.SetLogo(sLogo);

//    wxLogDebug("arg1 = %s arg2 = %s arg3 = %s", wxGetApp().argv[1], "a", "b"); //wxGetApp().argv[2], wxGetApp().argv[3]);

    if (m_cmdLine.Parse() != 0) {
//       wxLogDebug("-- user help!\n");
        return false;
    }

    Frame*	myFrame = new Frame();

    myFrame->Show( true );
    SetTopWindow(myFrame);

    return true;
};

/**
 *  On exit destroy the debug log window.
 */

int viewApp::OnExit() {
#if defined(__WXDEBUG__) && defined(USE_WXLOGWINDOW)
    delete m_pLogWindow;
    m_pLogWindow = nullptr;
#endif

    return 0;
}

/**
 *  Get the host CPU identification.
 */

bool viewApp::getCPUID() {
    wxLogDebug("viewApp::getCPUID()");

    memset(&m_cpuPack, 0, sizeof(CPUPACK));

#ifdef	__GNUC__
#if (defined(__x86_64__) || defined(__i386__))

    /* Determine information about CPU */

    asm ( " movl $0, %%eax 				;	\
            cpuid 						;	\
            movl %%eax, (%0)			;	\
            movl %%ebx, 4(%0)			;	\
            movl %%ecx, 12(%0)			;	\
            movl %%edx, 8(%0)			;	\
                                        ;	\
            mov $1, %%eax				;	\
            cpuid						;	\
            movl %%eax, 16(%0)			;	\
            movl %%ebx, 20(%0)			;	\
            movl %%edx, 24(%0)			;	\
                                        ;	\
            /* test for 3DNOW */		;	\
            movl $0x80000000, %%eax		;	\
            cpuid						;	\
            cmpl $0x80000000, %%eax		;	\
            jl 0f						;	\
            movl $0x80000001, %%eax		;	\
            cpuid						;	\
            test $0x80000000, %%edx		;	\
            jz	0f                      ;	\
            movl $1, 28(%0)				;	\
            0:" 							\
          : /* No output */ 				\
          : "D" (&m_cpuPack)					\
          : "eax", "ebx", "ecx", "edx" );

#else   // Not an intel 32/64bit CPU...

    wxLogDebug("ERROR: Unable to obtain CPUID on non-Intel platforms!\n");
    return false;

#endif  // (defined(__x86_64__) || defined(__i386__))

#else

    /* Get CPUID from Windows intrinsic function */
    __cpuid((int *)&m_cpuPack, 0);
    __cpuid((int *)&m_cpuPack + 4, 1);

#endif

#ifdef	__WXDEBUG__
    wxString sTmp;

    wxLogDebug("maxcpu %d", m_cpuPack.maxCPUID);

    for (int x = 0 ; x < 12 ; x++) {
        sTmp += m_cpuPack.CPUTag[x];
    }

    wxLogDebug("CPUID : %s", sTmp);

    wxLogDebug("Stepping : %d", m_cpuPack.CPUSig_Mask);
    wxLogDebug("Model    : %d", m_cpuPack.CPUSig_Model);
    wxLogDebug("Family   : %d", m_cpuPack.CPUSig_Family);
    wxLogDebug("CPU Cnt  : %d", m_cpuPack.CPUSig_CPUCount);

    if (m_cpuPack.CPUSig_Flag_MMX) {
        wxLogDebug("HAS MMX!");
    }
    if (m_cpuPack.CPUSig_Flag_SSE2) {
        wxLogDebug("HAS SSE2!");
    }
    if (m_cpuPack.CPUSig_Flag_3DNOW) {
        wxLogDebug("HAS 3DNOW!");
    }
#endif	// __WXDEBUG__

    return true;
}

