#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <wx/wx.h>
#include <wx/regex.h>
#include <wx/sound.h>
#include <wx/filename.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "YUV420ImageBufferSplit.h"

#include "misc_utils.h"

#ifdef  ENABLE_SOUND
#   include "bell.h"
#endif

#define BEEP_PATH           "/usr/share/sounds/KDE-Im-Message-In.ogg"

namespace utils {

static struct vidstd_entry vidStd[] = {
    {   STD_CUSTOM, "Custom",    -1,   -1, },
    {   STD_SQCIF,  "SQCIF",    128,   96, },
    {   STD_QCIF,   "QCIF",     176,  144, },
    {   STD_SCIF,   "SCIF",     256,  192, },
    {   STD_SIF,    "SIF",      352,  240, },
    {   STD_CIF,    "CIF",      352,  288, },
    {   STD_VGA,    "VGA",      640,  480, },
    {   STD_4SIF,   "4SIF",     704,  480, },
    {   STD_4CIF,   "4CIF",     704,  576, },
    {   STD_DVD_N,  "DVD NTSC", 720,  480, },
    {   STD_DVD_P,  "DVD PAL",  720,  576, },
    {   STD_SVGA,   "SVGA",     800,  600, },
    {   STD_720p,   "720p",    1280,  720, },
    {   STD_SXGA,   "SXGA",    1280, 1024, },
    {   STD_1080p,  "1080p",   1920, 1080, },
    {   STD_4k2k,   "4k2k",    3840, 2160, },
};

static size_t stdCount = sizeof(vidStd)/sizeof(vidstd_entry);

static struct yuvfmt_entry fmtTable[] = {
    { DATA_YUV420,        "yuv420", },
    { DATA_YUV422,        "yuv422", },
    { DATA_YUV444,        "yuv444", },
    { DATA_NV12,          "nv12",   },
    { DATA_NV21,          "nv21",   },
};

static size_t fmtCount = sizeof(fmtTable)/sizeof(yuvfmt_entry);

#ifdef  ENABLE_SOUND
static wxSound         sound( _bell_wav_len, _bell_wav );
#endif

/**
 *
 */

void digest_to_string(wxUint8* pDigest, std::string& sOut) {
    std::stringstream  sBuf;

    sBuf << std::hex << std::setfill('0') << std::uppercase;

    for (int i = 0 ; i < 16 ; i++) {
        sBuf << std::setw(2) << (unsigned int)pDigest[i];
    }

    sOut = sBuf.str();

    return;
}

/**
 *
 */

void display_digest(wxUint8* digest) {
    for (int i = 0 ; i < 16 ; i++) {
        printf("%02x",digest[i]);
    }
    printf("\n");
}


#if !wxCHECK_VERSION(2,9,5)

/**
 *
 */

wxColor string_to_color(wxString sColor) {
    wxColor     cColor;
    if (!cColor.Set(sColor)) {
        cColor.Set(wxT("black"));
    }
    return cColor;
}

/**
 *
 */

wxString color_to_string(wxColor cColor) {
    wxString sColor;
    sColor = cColor.GetAsString(wxC2S_CSS_SYNTAX);
    return sColor;
}
#endif

/**
 *  Overloaded helper function...
 */

videoStd    GetStandardFromSize(const wxSize& size) {
    return GetStandardFromSize( size.GetWidth(), size.GetHeight() );
}

/**
 *
 */

videoStd    GetStandardFromSize(int width, int height) {
    videoStd    standard = STD_CUSTOM;

    wxLogDebug("utils::GetStandardFromSize(%d, %d)", width, height);

    for (size_t i = 0 ; i < stdCount ; i++) {
        if ((vidStd[i].width == width) && (vidStd[i].height == height)) {
            standard = vidStd[i].standard;
            break;
        }
    }

    return standard;
}

/**
 *
 */

wxSize GetSizeFromStandard(videoStd standard) {
    wxSize  stdSize = wxSize(-1, -1);

    wxLogDebug("utils::GetSizeFromStandard(%d)", (int)standard);

#ifdef  __WXDEBUG__
    if (standard == STD_CUSTOM) {
        wxLogDebug("ERROR: Called GetSizeFromStandard with value of STD_CUSTOM!");
    }
#endif

    for (size_t i = 0 ; i < stdCount ; i++) {
        if (vidStd[i].standard == standard) {
            stdSize = wxSize(vidStd[i].width, vidStd[i].height);
            break;
        }
    }

    return stdSize;
}

/**
 *  Return # of standards in array.
 */

size_t GetStandardCount() {
    return stdCount;
}

bool GetStandardFromIndex(size_t index, videoStd& standard,
                          wxString& name, int& w, int& h)
{
    bool bRes = false;

    wxLogDebug("utils::GetStandardFromIndex(%d, ...)", index);

    if (index <= stdCount) {
        standard    = vidStd[index].standard;
        name        = vidStd[index].name;
        w           = vidStd[index].width;
        h           = vidStd[index].height;
        bRes        = true;
    }

    return bRes;
}

/**
 *
 */

vidstd_entry* GetStandardFromIndex(size_t index) {
    vidstd_entry*   pEntry = nullptr;

    wxLogDebug("utils::GetStandardFromIndex(%d)", index);

    if (index <= stdCount) {
        pEntry = &vidStd[index];
    }

    return pEntry;
}

/**
 *  Populate a wxChoice control with all video standards.
 */

void PopulateStandardDropDown(wxChoice* stdChoice) {
    wxLogDebug("utils::PopulateStandardDropDown(%p)", stdChoice);

    for (size_t index = 0 ; index < stdCount ; index++) {
        wxString                stdLabel;
        utils::vidstd_entry*    pEntry = nullptr;

        pEntry = &vidStd[index];

        if ((pEntry->width == -1) && (pEntry->height == -1)) {
            stdLabel = wxString::Format("%s", pEntry->name);
        } else {
            stdLabel = wxString::Format("%s (%d x %d)", pEntry->name,
                                        pEntry->width, pEntry->height);
        }

        stdChoice->Append( stdLabel, pEntry );
    }
}


#ifdef  ENABLE_SOUND

/**
 *  Make a beep sound.
 */

void Beep() {
    wxLogDebug("utils::Beep()");

    if (sound.IsOk()) {
        wxLogDebug("-- playing!");
        sound.Play(wxSOUND_SYNC);
    }
}

#endif  // ENABLE_SOUND

void GetStandardArray(wxArrayString& array) {
    array.Clear();

    for (size_t i = 0 ; i < stdCount ; i++) {
        array.push_back(vidStd[i].name);
    }

    return;
}

videoStd GetStandardFromName(wxString& name) {
    for (size_t index = 0 ; index < stdCount ; index++) {
        if (vidStd[index].name.CmpNoCase(name) == 0) {
            return vidStd[index].standard;
        }
    }
    return STD_CUSTOM;
}

/**
 *  Extract information about file from filename.
 */

bool ParseFilePath(wxString& sPath, imageParms& parms) {
    wxArrayString       stdArray; //, fmtArray;
    wxRegEx             regex;
    wxString            sExpression;
    bool                bRes = false;

    wxLogDebug("ParseFilePath(%s, ...)", sPath);

    /* Initialize parms to sane values */
    parms.standard   = utils::STD_CUSTOM;
    parms.width      = parms.height = 0;
    parms.depth      = -1;
    parms.endianness = endian_undefined;
    parms.yuvFmt     = DATA_UNKNOWN;

    utils::GetStandardArray(stdArray);

    sExpression = "(?i)[_-]("; // caseless comparison

    // skip 'CUSTOM" string in array...
    for (size_t x = 1 ; x < stdArray.Count() ; x++) {
        sExpression += stdArray[x];// + "|";
        if (x < (stdArray.Count() - 1)) {
            sExpression += "|";
        }
    }
    sExpression += ")[_-]";
    wxLogDebug("sExpression = %s", sExpression);

    regex.Compile(sExpression, wxRE_ADVANCED);

    if (regex.Matches(sPath)) {
        wxString sMatched = regex.GetMatch(sPath, 1);
        wxLogDebug("Matched %s!", sMatched);

        parms.standard = utils::GetStandardFromName( sMatched );

        if (parms.standard != utils::STD_CUSTOM) {
            wxSize stdSize = utils::GetSizeFromStandard( parms.standard );

            parms.width  = stdSize.GetWidth();
            parms.height = stdSize.GetHeight();

            wxLogDebug("Width = %d Height = %d", parms.width, parms.height);

            bRes = true;
        } else {
            wxLogDebug("-- What are we doing here?");
        }
    }

    sExpression = "[_-](\\d+)x(\\d+)[_\\-\\.]";

    regex.Compile(sExpression, wxRE_ADVANCED);

    if (regex.Matches(sPath)) {
        wxString sWidth = regex.GetMatch(sPath, 1),
                 sHeight = regex.GetMatch(sPath, 2);
        wxLogDebug("Matched %s & %s", sWidth, sHeight);

        parms.width = atoi(sWidth);
        parms.height = atoi(sHeight);
        bRes = true;
    }

    sExpression = "[_-](\\d+)bit";

    regex.Compile(sExpression, wxRE_ADVANCED);

    if (regex.Matches(sPath)) {
        long value;
        wxString sBits = regex.GetMatch(sPath, 1);

        sBits.ToLong(&value);

        parms.depth = value;
        bRes = true;
    }

    sExpression = "[_-](le|be)[_-]*";

    regex.Compile(sExpression, wxRE_ADVANCED);

    if (regex.Matches(sPath)) {
        wxString sEndian = regex.GetMatch(sPath, 1);

        if (sEndian == "le") {
            parms.endianness = endian_little;
        } else if (sEndian == "be") {
            parms.endianness = endian_big;
        }
        bRes = true;
    }

    sExpression = "[_-](yuv420|yuv422|yuv444|nv12|nv21)[-_\\.]";

    regex.Compile(sExpression, wxRE_ADVANCED);


    if (regex.Matches(sPath)) {
        wxString yuvFmt = regex.GetMatch(sPath, 1);

        parms.yuvFmt = utils::GetFormatFromName( yuvFmt );
        bRes = true;
    }

    return bRes;
}


dataType        GetFormatFromName(wxString sName) {
    for (size_t i = 0 ; i < fmtCount ; i++) {
        if (fmtTable[i].name == sName) {
            return fmtTable[i].format;
        }
    }
    return DATA_UNKNOWN;
}

void GetFormatArray(wxArrayString& array) {
    for (size_t i = 0 ; i < fmtCount ; i++) {
        array.push_back(fmtTable[i].name);
    }

    return;
}

wxString& remove_trailing_backslashes(wxString& path)
{
    auto inb = path.find_last_not_of(L'/');

    if(inb != wxString::npos)
        path.erase(inb + 1); //inb + 1 <= size(), valid for erase()
    else //empty string or only backslashes
        path.clear();

    return path; //to allow chaining
}

wxString    getFrameFilename(wxString sPath,
                             wxString sFileSpec,
                             size_t frame)
{
    wxString    imageFilePath;
    wxRegEx     frameRE("\\%[0-9]*d", wxRE_ADVANCED);

    if (frameRE.Matches(sFileSpec)) {
        imageFilePath = remove_trailing_backslashes(sPath) + "/" + wxString::Format(sFileSpec, (int)frame);
    } else {
        imageFilePath = sPath + "/" + sFileSpec;
    }

    return imageFilePath;
}

/**
 *  Find all .Y/.U/.V files in directory in range and return
 *  a std::vector of strings containing the base filenames.
 */

bool            GetSplitBufferFrameVec(const YUV420ImageSpec& spec,
                                       FRAME_FILEBASE_VEC& frameVec)
{
    size_t	 		frameID = 0L;
    wxFileName		framePath;
    wxString        sFrameFilebase;
//      bool			bDone = false;
    wxRegEx         frameRE("\\%[0-9]*d", wxRE_ADVANCED);
    wxString	    name;
    int			    count           = 0;

    wxLogDebug("GetSplitBufferFrameVec(%s, %s, %d, %d)",
               spec.m_imagePath, spec.m_prefix,
               spec.m_first, spec.m_last);

    frameVec.clear();

    /* If fileSpec has '%d' spec then count all frames. */

    if (frameRE.Matches(spec.m_prefix)) {
        wxLogDebug("-- string contains format spec!");

        frameID = spec.m_first;

        while (frameID < (size_t)spec.m_last) {
            sFrameFilebase  = getFrameFilename( spec.m_imagePath,
                                                spec.m_prefix,
                                                frameID );

            count = 0;

            name = sFrameFilebase + ".Y";
            framePath.Assign(name);
            if (framePath.FileExists()) {
                count++;
            }

            name = sFrameFilebase + ".U";
            framePath.Assign(name);
            if (framePath.FileExists()) {
                count++;
            }

            name = sFrameFilebase + ".V";
            framePath.Assign(name);
            if (framePath.FileExists()) {
                count++;
            }

            //		wxLogDebug("%d %d", frameID, count);
            if (count == 3) {
                wxLogDebug("-- Found %s", sFrameFilebase);
                frameVec.push_back( sFrameFilebase );
            }

            frameID++;
        }
    } else {
        sFrameFilebase = spec.m_imagePath + "/" + spec.m_prefix;

        count = 0;

        name = sFrameFilebase + ".Y";
        framePath.Assign(name);
        if (framePath.FileExists()) {
            count++;
        }

        name = sFrameFilebase + ".U";
        framePath.Assign(name);
        if (framePath.FileExists()) {
            count++;
        }

        name = sFrameFilebase + ".V";
        framePath.Assign(name);
        if (framePath.FileExists()) {
            count++;
        }

        //		wxLogDebug("%d %d", frameID, count);

        if (count == 3) {
            frameID++;
            frameVec.push_back( sFrameFilebase );
        }
    }

    return (frameVec.size() > 0)?true:false;
}

};


