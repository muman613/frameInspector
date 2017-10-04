#ifndef __CRCVECTOR_H__
#define __CRCVECTOR_H__

#include <wx/wx.h>

class crcEntry;
class crcVector;

WX_DECLARE_OBJARRAY(crcEntry, CRCENTRY_ARRAY);

/**
 *
 */

class crcEntry {
    public:
        crcEntry();
        crcEntry(const crcEntry& copy);
        crcEntry(wxUint32 luma, wxUint32 chroma);
        virtual ~crcEntry();

        crcEntry& operator = (const crcEntry& copy);

        bool        operator ==(const crcEntry& compare);
        bool        operator !=(const crcEntry& compare);

    protected:
        friend class crcVector;

        wxUint32    m_luma;
        wxUint32    m_chroma;
};

/**
 *
 */

class crcVector
{
    public:
        crcVector();
        virtual ~crcVector();

        bool    IsLoaded() const {
            return m_bIsLoaded;
        }

        void    AddEntry(crcEntry& entry);
        bool    LoadFile(wxString sFilename);

        void    clear();
        size_t  size() const;

        bool    operator ==(const crcVector& compare);

        bool    GetCRC(size_t index, wxUint32& luma, wxUint32& chroma);
        bool    GetCRC(size_t index, crcEntry& entry);

#ifdef  __WXDEBUG__
        void    Dump(FILE* fOut = stderr);
#endif

    protected:
        bool            m_bIsLoaded;

    private:
        CRCENTRY_ARRAY  m_crcArray;
};


typedef crcVector       CRCVECTOR;

#endif // __CRCVECTOR_H__
