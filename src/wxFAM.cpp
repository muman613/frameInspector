#include <wx/wx.h>
#include <wx/thread.h>
#include "wxFAM.h"
#include "dbgutils.h"

//#define OLDWAY

IMPLEMENT_DYNAMIC_CLASS(wxFAMEvent, wxEvent)
DEFINE_EVENT_TYPE(EVT_FAMEVENT)

wxFAM::wxFAM(wxWindow* parent)
:	wxThread(wxTHREAD_JOINABLE),
	m_parent(parent)
{
	debug("wxFAM::wxFAM()\n");
}

wxFAM::~wxFAM()
{
	debug("wxFAM::~wxFAM()\n");

	return;
}

void wxFAM::start() {
	debug("wxFAM::start()\n");

	Create();
	Run();
}

void wxFAM::stop() {
	debug("wxFAM::stop()\n");

	Delete();
	Wait();
}

bool wxFAM::MonitorDir(const wxString& dname) {
	bool		result = false;
	int			status;
	FAMRequest	req;

	debug("wxFAM::MonitorDir(%s)\n", dname.c_str());

	status = FAMMonitorDirectory(&m_conn,
				dname.c_str(),
				&req,
				(void*)0L);

	m_request.push_back(req);

	result = (status == 0)?true:false;

	return result;
}

bool wxFAM::MonitorFile(const wxString& fname) {
	bool	result = false;
	int			status;
	FAMRequest	req;

	debug("wxFAM::MonitorFile(%s)\n", fname.c_str());

	status = FAMMonitorFile(&m_conn,
				fname.c_str(),
				&req,
				(void*)0L);

	m_request.push_back(req);

	result = (status == 0)?true:false;


	return result;
}

void* wxFAM::Entry() {
	debug("wxFAM::Entry()\n");

	bool	bDone = false;

	if (FAMOpen(&m_conn) != 0 ) {
		debug("-- Error opening FAM\n");
		goto exitThread;
	}

	debug("FAM opened fd = %ld\n", FAMCONNECTION_GETFD(&m_conn));

	while (!bDone) {
		struct timeval	val;
		int				num;
		FAMEvent		fam_evt;
		fd_set			read_fds;
		int				fam_fd;

//	--	Check if the main thread wants background thread to stop...

		if (TestDestroy()) {
			debug("-- TestDestroy() returned true!\n");
			bDone = true;
			continue;
		}

		FD_ZERO(&read_fds);
		fam_fd = FAMCONNECTION_GETFD(&m_conn);
		FD_SET(fam_fd, &read_fds);
//		FD_SET(STDIN, &read_fds);

		val.tv_sec	= 0;
		val.tv_usec	= 500;

//		debug("SELECT!\n");

		if ((num = select(fam_fd + 1, &read_fds, NULL, NULL, &val)) < 0) {
			debug("-- select failed!\n");
			return (void*)-1;
		}
		
//		debug("SELECT RETURN!\n");

//	--	Check if the select timed out...

		if (num == 0) {
			if (FAMPending(&m_conn)) {
				debug("-- handling FAM event!\n");
				handle_fam_event();
			}
			continue;
		}

		if (FD_ISSET(fam_fd, &read_fds)) {
			debug("-- handling FAM event!\n");
			handle_fam_event();
		}

	}

exitThread:

	FAMClose(&m_conn);

	debug("EXITING THREAD!\n");

	return (void*)0;
}

void wxFAM::handle_fam_event() {
	FAMEvent	famEvt;

	while (FAMPending(&m_conn) == 1) {
		if (FAMNextEvent(&m_conn, &famEvt) == 1) {
			debug("%d %s!\n", famEvt.code, famEvt.filename);

			wxFAMEvent	event(famEvt.filename, famEvt.code);
			m_parent->GetEventHandler()->AddPendingEvent(event);
		}
	}
}

void wxFAM::Reset() {
	FAMRequest_Iter	it;

	debug("wxFAM::Reset()\n");

	for (it = m_request.begin() ; it != m_request.end() ; it++) {
		FAMRequest	req = (*it);
		FAMCancelMonitor(&m_conn, &req);
	}
}

