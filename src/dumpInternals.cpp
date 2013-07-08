#include <wx/wx.h>
#include "dumpInternals.h"
#include "dbgutils.h"

namespace dumpInternals
{
bool read_box(wxFile* fp, wxUint32& tag, wxUint32& version, wxInt64& size)
{
    struct box_struct bs = {0};

    debug("read_box()\n");

    wxASSERT(fp != 0L);

    if (fp->Read(&bs, sizeof(bs)) != sizeof(bs)) {
        debug("ERROR: Unable to read box!\n");
        return false;
    }

    /* must swap these values */
    tag     = wxUINT32_SWAP_ALWAYS(bs.ID);
    version = wxUINT32_SWAP_ALWAYS(bs.Version);
    size    = wxINT64_SWAP_ALWAYS(bs.Size);

    return true;
}

}
