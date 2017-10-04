#ifndef __MISC_UTILS__
#define __MISC_UTILS__

#include <wx/wx.h>
#include <wx/colour.h>
#include <vector>
#include "types.h"
//#include "imageBuffer.h"
//#include "YUV420ImageBufferSplit.h"
#include "imageSpec.h"

namespace utils {

    using FRAME_FILEBASE_VEC    =   std::vector<wxString>;

    enum videoStd {
        STD_CUSTOM,
        STD_SQCIF,
        STD_QCIF,
        STD_SCIF,
        STD_SIF,
        STD_CIF,
        STD_VGA,
        STD_4SIF,
        STD_4CIF,
        STD_DVD_N,
        STD_DVD_P,
        STD_SVGA,
        STD_720p,
        STD_SXGA,
        STD_1080p,
        STD_4k2k,
    };

    struct vidstd_entry {
        videoStd        standard;
        wxString        name;
        int             width;
        int             height;
    };

    struct yuvfmt_entry {
        dataType        format;
        wxString        name;
    };

    typedef struct imageParms {
        videoStd        standard;
        int             width;
        int             height;
        int             depth;
        formatEndian    endianness;
        dataType        yuvFmt;
    } imageParms;

    void        digest_to_string(wxUint8* pDigest, std::string& sOut);
    void        display_digest(wxUint8* digest);

#if !wxCHECK_VERSION(2,9,5)
    /* color functions */
    wxColor     string_to_color(wxString sColor);
    wxString    color_to_string(wxColor cColor);
#endif

    /* Video standard related functions */
    videoStd        GetStandardFromSize(const wxSize& size);
    videoStd        GetStandardFromSize(int width, int height);
    wxSize          GetSizeFromStandard(videoStd standard);
    size_t          GetStandardCount();
    videoStd        GetStandardFromName(wxString& name);

    bool            GetStandardFromIndex(size_t index, videoStd& standard,
                                         wxString& name, int& w, int& h);

    vidstd_entry*   GetStandardFromIndex(size_t index);
    void            GetStandardArray(wxArrayString& array);

    void            PopulateStandardDropDown(wxChoice* stdChoice);

    dataType        GetFormatFromName(wxString sName);
    void            GetFormatArray(wxArrayString& array);

    void            Beep();

    bool            ParseFilePath(wxString& sPath, imageParms& parms);

    wxString        getFrameFilename(wxString sPath,
                                     wxString sFileSpec,
                                     size_t frame);

    bool            GetSplitBufferFrameVec(const YUV420ImageSpec& spec,
                                           FRAME_FILEBASE_VEC& frameVec);
};

#endif // __MISC_UTILS__

