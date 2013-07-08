#include <wx/wx.h>
#include <wx/gdicmn.h>
#include <wx/cmdline.h>
#include "viewApp.h"
#include "frame.h"
#include "dbgutils.h"
#include "imageBuffer.h"

#ifdef  __WXMSW__
#include <intrin.h>
#endif

IMPLEMENT_APP(viewApp)

static const wxCmdLineEntryDesc cmdLineDesc[] = {
    { wxCMD_LINE_SWITCH,    wxT("h"), wxT("help"),      wxT("Display help message"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_OPTION,	wxT("f"), wxT("file"),      wxT("Set YUV File"),	    wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,	wxT("p"), wxT("path"),      wxT("Set YUV path"),	    wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION, 	wxT("s"), wxT("size"),      wxT("Set image size"),      wxCMD_LINE_VAL_STRING, },
    { wxCMD_LINE_OPTION,	wxT("c"), wxT("frame"),     wxT("Set frame #"), 	    wxCMD_LINE_VAL_NUMBER, },
    { wxCMD_LINE_OPTION,	wxT("r"), wxT("prefix"),    wxT("Set prefix"),          wxCMD_LINE_VAL_STRING, },

#ifdef  HAVE_FAM
    { wxCMD_LINE_OPTION,	wxT("m"), wxT("monitor"),   wxT("Enable/Disable FAM"),  wxCMD_LINE_VAL_NUMBER, },
#endif

    { wxCMD_LINE_NONE },
};


bool viewApp::OnInit()
{
    debug("viewApp::OnInit()\n");

    int	depth = ::wxDisplayDepth();

    debug("Screen depth = %d\n", depth);

    /* if running on a low color density screen, tell the user to prepare for the worst */

    if (depth < 16) {
        ::wxMessageBox(_T("frameInspector should be run on hicolor or truecolor screen.\n" \
                          "Detected 256 color screen!"), _T("Message"), wxICON_EXCLAMATION);
    }

    getCPUID();	/* determine which CPU application is running on */

    ::wxInitAllImageHandlers();

    m_cmdLine.SetDesc(cmdLineDesc);
    m_cmdLine.SetCmdLine(wxGetApp().argc, wxGetApp().argv);
    wxString sLogo;
    sLogo = wxString::Format(wxT("%s v%d.%d %s"), APP_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE);
    m_cmdLine.SetLogo(sLogo);

    debug("arg1 = %s arg2 = %s arg3 = %s\n", wxGetApp().argv[1], "a", "b");

    if (m_cmdLine.Parse() != 0) {
//        debug("-- user help!\n");
        return false;
    }
    Frame*	myFrame = new Frame();

    myFrame->Show( true );
    SetTopWindow(myFrame);

    return true;
};

/**
 *  Get the host CPU identification.
 */

bool viewApp::getCPUID()
{
    debug("viewApp::getCPUID()\n");

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

    debug("ERROR: Unable to obtain CPUID on non-Intel platforms!\n");
    return false;

#endif  // (defined(__x86_64__) || defined(__i386__))

#else

    /* Get CPUID from Windows intrinsic function */
    __cpuid((int *)&m_cpuPack, 0);
    __cpuid((int *)&m_cpuPack + 4, 1);

#endif

#ifdef	_ENABLE_DEBUG
    debug("maxcpu %ld\n", m_cpuPack.maxCPUID);
    debug("CPUID : ");

    for (int x = 0 ; x < 12 ; x++) {
        debug("%c", m_cpuPack.CPUTag[x]);
    }
    debug("\n");

    debug("Stepping : %d\n"
          "Model    : %d\n"
          "Family   : %d\n",
          m_cpuPack.CPUSig_Mask,
          m_cpuPack.CPUSig_Model,
          m_cpuPack.CPUSig_Family);

    debug("CPU Cnt  : %d\n", m_cpuPack.CPUSig_CPUCount);

    if (m_cpuPack.CPUSig_Flag_MMX) {
        debug("HAS MMX!\n");
    }
    if (m_cpuPack.CPUSig_Flag_SSE2) {
        debug("HAS SSE2!\n");
    }
    if (m_cpuPack.CPUSig_Flag_3DNOW) {
        debug("HAS 3DNOW!\n");
    }
#endif	// _ENABLE_DEBUG


    return true;
}

