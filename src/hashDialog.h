#ifndef __HASHDIALOG_H__
#define __HASHDIALOG_H__

#include "Dialogs.h"


class HashDialog : public HashDialogBase {
public:
    HashDialog(wxWindow* pParent);
    ~HashDialog();

    void SetChecksums(wxUint8* lumaSum, wxUint8* chromaSum);
};


#endif // __HASHDIALOG_H__

