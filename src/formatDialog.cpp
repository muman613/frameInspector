#include "formatDialog.h"

formatDialog::formatDialog( wxWindow* parent )
:
formatDialogBase( parent )
{

}

/**
 *
 */

void formatDialog::OnUpdateUI( wxUpdateUIEvent& event )
{
    switch (event.GetId()) {
    case wxID_FORMAT_ENDIAN:
        {
            int nChoice = m_bitsChoice->GetSelection();

            event.Enable( ((nChoice == 1) || (nChoice == 2))?true:false );
        }
        break;
    default:
        break;
    }
}

/**
 *
 */

void formatDialog::SetDialogOptions(dataType format,
                                    int bits,
                                    formatEndian endian)
{

    if ((format >= DATA_UNKNOWN) && (format <= DATA_NV21)) {
        int selection = (int)format - (int)DATA_YUV420;

        m_fmtChoice->SetSelection( selection );
    }

    switch (bits) {
        case 8:
            m_bitsChoice->SetSelection(0);
            break;
        case 10:
            m_bitsChoice->SetSelection(1);
            break;
        case 16:
            m_bitsChoice->SetSelection(2);
            break;
    }

    switch (endian) {
        case endian_little:
            m_bitsEndian->SetSelection(0);
            break;
        case endian_big:
            m_bitsEndian->SetSelection(1);
            break;
        default:
            break;
    }

    return;
}

/**
 *
 */

void formatDialog::GetDialogOptions(dataType& format, int& bits, formatEndian& endian)
{
    int fmtSelection = m_fmtChoice->GetSelection();
    int bitSelection = m_bitsChoice->GetSelection();
    int endSelection = m_bitsEndian->GetSelection();

    format = (dataType)((int)DATA_YUV420 + fmtSelection);
    switch (bitSelection) {
    case 0:
        bits = 8;
        break;
    case 1:
        bits = 10;
        break;
    case 2:
        bits = 16;
        break;
    }
//    bits   = (bitSelection == 0)?8:10;
    endian = (formatEndian)((int)endian_little + endSelection);

    return;
}
