//==============================================================================
//  MODULE      :   viewApp.h
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   October 4, 2013
//==============================================================================

#ifndef __APP_H__
#define __APP_H__

#include <wx/wx.h>
#include "cpupack.h"


class viewApp : public wxApp {
public:

    virtual bool OnInit();
    virtual int  OnExit();

    /**
      * Return a pointer to CPUPACK.
      */
    const CPUPACK*  get_cpu_info() const {
        return (const CPUPACK*)&m_cpuPack;
    }

    wxCmdLineParser	    m_cmdLine;

private:

    bool                getCPUID();
    CPUPACK             m_cpuPack;

#if defined(__WXDEBUG__) && defined(USE_WXLOGWINDOW)
    wxLogWindow*        m_pLogWindow;
#endif
};

DECLARE_APP(viewApp)

/* Application version information */

#define     APP_NAME            "frameInspector"
#define     VERSION_MAJOR       2
#define     VERSION_MINOR       32
#define     VERSION_RELEASE     "October 4, 2017"

#endif  // __APP_H__
