/**
 *  @file       crcVector.cpp
 *  @author     Michael A. Uman
 *  @date       February 5, 2013
 *  @brief      Classes representing CRC32 results/reference.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <wx/wx.h>
#include <wx/arrimpl.cpp>
#include <wx/textfile.h>
#include <wx/regex.h>
#include "crcVector.h"

WX_DEFINE_OBJARRAY(CRCENTRY_ARRAY);

crcEntry::crcEntry()
    :   m_luma(0),
        m_chroma(0)
{
    // ctor
}

crcEntry::crcEntry(wxUint32 luma, wxUint32 chroma)
    :   m_luma(luma),
        m_chroma(chroma)
{
    // ctor
}

crcEntry::crcEntry(const crcEntry& copy)
    :   m_luma(copy.m_luma),
        m_chroma(copy.m_chroma)
{
    // copy ctor
}

crcEntry::~crcEntry() {
    // dtor
}

crcEntry& crcEntry::operator =(const crcEntry& copy) {
    m_luma      = copy.m_luma;
    m_chroma    = copy.m_chroma;

    return *this;
}

bool crcEntry::operator ==(const crcEntry& compare) {
    bool bRes = false;

    if ((m_luma == compare.m_luma) && (m_chroma == compare.m_chroma))
        bRes = true;

    return bRes;
}

bool crcEntry::operator !=(const crcEntry& compare) {
    bool bRes = false;

    if ((m_luma != compare.m_luma) || (m_chroma != compare.m_chroma))
        bRes = true;

    return bRes;
}

/*----------------------------------------------------------------------------*/

crcVector::crcVector()
    :   m_bIsLoaded(false)
{
    //ctor
}

crcVector::~crcVector()
{
    //dtor
}

void crcVector::AddEntry(crcEntry& entry) {
    m_crcArray.Add( entry );
}

void crcVector::clear() {
    m_crcArray.Clear();
    m_bIsLoaded = false;
}

size_t crcVector::size() const {
    return m_crcArray.size();
}

/**
 *  Load CRC file into memory.
 */

bool crcVector::LoadFile(wxString sFilename) {
    wxTextFile      crcFile;
    bool            bRes = false;
    wxRegEx         regExp;

    wxLogDebug("Loading file %s", (const char*)sFilename.c_str());

    clear();

    if (crcFile.Open(sFilename)) {
        regExp.Compile( wxT("#[[:space:]]+([[:digit:]]+)[[:space:]]+luma[[:space:]]+([[:xdigit:]]+)[[:space:]]+chroma[[:space:]]+([[:xdigit:]]+)") );

        if (regExp.IsValid()) {
            //fprintf(stderr, "regexp compiled!\n");

            for (size_t i = 0 ; i < crcFile.GetLineCount() ; i++) {
                if (regExp.Matches( crcFile[i] )) {
                    wxString sFrameNo, sLumaCRC, sChromaCRC;

                    //fprintf(stderr, "%s\n", crcFile[i].c_str());

                    sFrameNo    = regExp.GetMatch( crcFile[i], 1);
                    sLumaCRC    = regExp.GetMatch( crcFile[i], 2);
                    sChromaCRC  = regExp.GetMatch( crcFile[i], 3);

                    if (i == (size_t)wxAtol( sFrameNo )) {
                        unsigned long luma, chroma;

                        if (sLumaCRC.ToULong( &luma, 16 ) && sChromaCRC.ToULong( &chroma, 16)) {
                            crcEntry entry(luma, chroma);

//#ifdef  _DEBUG
//                            fprintf(stderr, "%s\n", sFrameNo.c_str());
//                            fprintf(stderr, "%s %08lX\n", sLumaCRC.c_str(), luma);
//                            fprintf(stderr, "%s %08lX\n", sChromaCRC.c_str(), chroma);
//#endif

                            AddEntry( entry );
                        }
                    }
                }
            }

            m_bIsLoaded = bRes = true;
        }
    }

    //fprintf(stderr, "There are %ld frames in this crc vector!\n", m_crcArray.size());

    return bRes;
}

/**
 *  Return TRUE if both crcVectors are equal.
 */

bool crcVector::operator==(const crcVector& compare) {
//    bool bRes = true;

    if (size() == compare.size()) {
        for (size_t i = 0 ; i < size() ; i++) {
            if (m_crcArray[i] != compare.m_crcArray[i]) {
                return false;
            }
        }
    } else
        return false;

    return true;
}

bool crcVector::GetCRC(size_t index, wxUint32& luma, wxUint32& chroma) {
    bool bRes = false;
    if (index < m_crcArray.size()) {
        luma   = m_crcArray[index].m_luma;
        chroma = m_crcArray[index].m_chroma;
        bRes   = true;
    }

    return bRes;
}

bool crcVector::GetCRC(size_t index, crcEntry& entry) {
    bool bRes = false;

    if (index < m_crcArray.size()) {
        entry = m_crcArray[index];
        bRes   = true;
    }

    return bRes;
}

#ifdef  __WXDEBUG__

void crcVector::Dump(FILE* oFP) {
    for (size_t i = 0 ; i < m_crcArray.size() ; i++) {
        fprintf(oFP, "%-4ld : %10X %10X\n", i, m_crcArray[i].m_luma, m_crcArray[i].m_chroma);
    }
    return;
}

#endif

