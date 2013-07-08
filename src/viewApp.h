#ifndef	__APP_H__
#define __APP_H__

#include <wx/wx.h>
#include "cpupack.h"


class viewApp : public wxApp
{
public:

    virtual bool OnInit();

    /**
      * Return a pointer to CPUPACK.
      */
    const CPUPACK*	get_cpu_info() const {
        return (const CPUPACK*)&m_cpuPack;
    }

    wxCmdLineParser	    m_cmdLine;

private:

    bool	getCPUID();

    CPUPACK	            m_cpuPack;
};

DECLARE_APP(viewApp)

/* Application version information */

#define     APP_NAME            "frameInspector"
#define 	VERSION_MAJOR		1
#define		VERSION_MINOR		30
#define 	VERSION_RELEASE		"Jun 27, 2013"

#endif

