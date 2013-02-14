#ifndef	__WXFAM_H__
#define __WXFAM_H__

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <fam.h>
#include <vector>

#define WXWINDOWS24

typedef std::vector<FAMRequest>				FAMRequest_Vector;
typedef std::vector<FAMRequest>::iterator	FAMRequest_Iter;

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(EVT_FAMEVENT, 3000)
END_DECLARE_EVENT_TYPES()

/**
  * FAM event class.
  */

class wxFAMEvent : public wxEvent {

	DECLARE_DYNAMIC_CLASS(wxFAMEvent)

public:
	wxFAMEvent() {
		m_eventType = EVT_FAMEVENT;
	}

	wxFAMEvent(const char* filename, int famEvt)
	:	m_filename(filename),
		m_event(famEvt)
	{
		m_eventType = EVT_FAMEVENT;
	}
	wxFAMEvent(const wxFAMEvent& copy) {
		m_eventType = EVT_FAMEVENT;
		m_filename	= copy.m_filename;
		m_event		= copy.m_event;
	}

#ifdef	WXWINDOWS24
	virtual wxEvent* Clone() const { return new wxFAMEvent(*this); }
#endif

	const wxString& name() const {
		return m_filename;
	}
	int famEvent() const {
		return m_event;
	}

private:

	wxString		m_filename;
	int				m_event;
};

typedef void (wxEvtHandler::*wxFAMEventFunction)(wxFAMEvent&);

#define EVT_FAM(func)		\
	DECLARE_EVENT_TABLE_ENTRY( EVT_FAMEVENT, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxFAMEventFunction) & func, (wxObject *) NULL ),


/**
  *	The wxFAM class encapsulates the functionality of the File Access Monitor
  * API in a C++ class.
  */

class wxFAM : public wxThread {
	public:
		wxFAM(wxWindow* parent);
		virtual ~wxFAM();

		bool		MonitorFile(const wxString& fname);
		bool		MonitorDir(const wxString& dname);

		void		Reset();

/*	wxThread */

		virtual void* Entry();

//	protected:
		void				start();
		void				stop();

	private:

		FAMConnection		m_conn;
		FAMRequest_Vector	m_request;
		void				handle_fam_event();
		wxWindow*			m_parent;
};


#endif

