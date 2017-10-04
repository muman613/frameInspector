#include <wx/wx.h>
#include "dumpInternals.h"
//#include "dbgutils.h"

namespace dumpInternals {
    bool read_box(wxFile* fp, wxUint32& tag, wxUint32& version, wxInt64& size) {
        struct box_struct bs = {0};

        wxLogDebug("read_box()");

        wxASSERT(fp != 0L);

        if (fp->Read(&bs, sizeof(bs)) != sizeof(bs)) {
            wxLogDebug("ERROR: Unable to read box!");
            return false;
        }

        /* must swap these values */
        tag     = wxUINT32_SWAP_ALWAYS(bs.ID);
        version = wxUINT32_SWAP_ALWAYS(bs.Version);
        size    = wxINT64_SWAP_ALWAYS(bs.Size);

//#ifdef  __WXDEBUG__
//        char *sTag = (char *)&tag;
//
//        wxLogDebug("tag %04x [%c%c%c%c] version %04x %lld", tag, sTag[3], sTag[2], sTag[1], sTag[0], version, size);
//#endif

        return true;
    }

}
