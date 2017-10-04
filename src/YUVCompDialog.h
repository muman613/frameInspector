#ifndef __YUVCompDialog__
#define __YUVCompDialog__

/**
@file
Subclass of YUVCompDialogBase, which is generated by wxFormBuilder.
*/

#include "Dialogs.h"
#include "misc_utils.h"
#include "imageBuffer.h"

/** Implementing YUVCompDialogBase */
class YUVCompDialog : public YUVCompDialogBase
{
    public:

    /** Constructor */
    YUVCompDialog( wxWindow* parent );
//	YUVCompDialog( wxWindow* parent, int width, int height, wxString sPath = wxEmptyString );
//	YUVCompDialog( wxWindow* parent, const wxSize& imageSize, const wxString& sPath = wxEmptyString );

    void    SetDialogOptions(const wxSize        frameSize,
                             const wxString      sPath,
                             const wxString      sLastFile,
                             const wxUint16      bits      = 8,
                             const formatEndian fmt = endian_little,
                             const dataType      yuvFmt    = DATA_YUV420);

    void    GetFrameDimensions(wxSize& frameSize);
    void    GetFrameDimensions(long& width, long& height);
    void    GetFrameBits(wxUint16& bits);
    void    GetFrameEndianness(formatEndian& endian);
    void    GetFrameYUVFmt(dataType& yuvFmt);
    void    GetFilePath(wxString& sFilePath);

protected:
	// Handlers for YUVCompDialogBase events.
	void    OnChangeStandard( wxCommandEvent& event );
	void    OnFileSelected( wxFileDirPickerEvent& event );
    void    SetStandardToDimensions(int width, int height);

    virtual void OnUpdateUI(wxUpdateUIEvent& event);

    //bool    ParseFilePath(wxString& sPath, imageParms& parms);
};

#endif // __YUVCompDialog__
