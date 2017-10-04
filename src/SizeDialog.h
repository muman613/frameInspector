#ifndef __SizeDialog__
#define __SizeDialog__

/**
@file
Subclass of SizeDialogBase, which is generated by wxFormBuilder.
*/

#include "Dialogs.h"
#include "misc_utils.h"

/** Implementing SizeDialogBase */
class SizeDialog : public SizeDialogBase
{
public:
	/** Constructor */
	SizeDialog( wxWindow* parent, int width, int height );

    void    GetFrameDimensions(long& width, long& height);

protected:
    void    OnChangeStandard( wxCommandEvent& event );
    void    SetStandardToDimensions(int width, int height);
};

#endif // __SizeDialog__
