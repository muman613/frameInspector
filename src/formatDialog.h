#ifndef __formatDialog__
#define __formatDialog__

/**
@file
Subclass of formatDialogBase, which is generated by wxFormBuilder.
*/

#include "Dialogs.h"
#include "misc_utils.h"
#include "imageBuffer.h"
//// end generated include

/** Implementing formatDialogBase */
class formatDialog : public formatDialogBase
{
protected:
    // Handlers for formatDialogBase events.
    void OnUpdateUI( wxUpdateUIEvent& event );
public:
    /** Constructor */
    formatDialog( wxWindow* parent );
    //// end generated class members


    void SetDialogOptions(dataType format, int bits, formatEndian endian);
    void GetDialogOptions(dataType& format, int& bits, formatEndian& endian);

};

#endif // __formatDialog__
