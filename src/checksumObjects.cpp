#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <wx/wx.h>
#include "checksumObjects.h"
#include "imageBuffer.h"

/**
 *
 */

checksumEntry::checksumEntry()
    :   m_algo(CHECKSUM_UNDEFINED),
        m_dataLen(0),
        m_chromaSum(0),
        m_lumaSum(0)
{
    wxLogDebug("checksumEntry::checksumEntry() default");
}

checksumEntry::checksumEntry(const checksumEntry& copy)
    :   m_algo(copy.m_algo),
        m_dataLen(copy.m_dataLen)
{
    wxLogDebug("checksumEntry::checksumEntry(copy)");

    m_chromaSum = new wxUint8[m_dataLen];
    m_lumaSum   = new wxUint8[m_dataLen];

    memcpy(m_chromaSum, copy.m_chromaSum, m_dataLen);
    memcpy(m_lumaSum, copy.m_lumaSum, m_dataLen);
}

/**
 *
 */

checksumEntry::checksumEntry(eChecksumAlgo algo, wxUint32 dataLen)
    :   m_algo(algo),
        m_dataLen(dataLen)
{
    // ctor
    wxLogDebug("checksumEntry::checksumEntry(%d, %d)", (int)algo, dataLen);

    m_chromaSum = new wxUint8[dataLen];
    m_lumaSum   = new wxUint8[dataLen];

    memset(m_chromaSum, 0, dataLen);
    memset(m_lumaSum,   0, dataLen);
}

/**
 *
 */

checksumEntry::~checksumEntry() {
    wxLogDebug("checksumEntry::~checksumEntry()");

    if (m_chromaSum) {
        delete [] m_chromaSum;
        m_chromaSum = nullptr;
    }
    if (m_lumaSum) {
        delete [] m_lumaSum;
        m_lumaSum = nullptr;
    }
}

/**
 *
 */

wxString checksumEntry::GetLumaSum(bool bUpper) {
    wxString    sLuma;

    if (m_algo != CHECKSUM_UNDEFINED) {
        for (size_t i = 0 ; i < m_dataLen ; i++) {
            if (bUpper)
                sLuma += wxString::Format("%02X", m_lumaSum[i]);
            else
                sLuma += wxString::Format("%02x", m_lumaSum[i]);
        }
    } else {
        sLuma = "UNDEFINED";
    }

    return sLuma;
}

/**
 *
 */

wxString checksumEntry::GetChromaSum(bool bUpper) {
    wxString    sChroma;

    if (m_algo != CHECKSUM_UNDEFINED) {
        for (size_t i = 0 ; i < m_dataLen ; i++) {
            if (bUpper)
                sChroma += wxString::Format("%02X", m_chromaSum[i]);
            else
                sChroma += wxString::Format("%02x", m_chromaSum[i]);
        }
    } else {
        sChroma = "UNDEFINED";
    }

    return sChroma;
}

/**
 *
 */

checksumEntry& checksumEntry::operator =(const checksumEntry& copy) {
    wxLogDebug("checksumEntry::operator =()");

    m_algo      = copy.m_algo;

    if (m_dataLen != copy.m_dataLen) {
        if (m_lumaSum) {
            delete [] m_lumaSum;
            m_lumaSum = nullptr;
        }
        if (m_chromaSum) {
            delete [] m_chromaSum;
            m_chromaSum = nullptr;
        }

        m_dataLen   = copy.m_dataLen;

        m_lumaSum   = new wxUint8[m_dataLen];
        m_chromaSum = new wxUint8[m_dataLen];
    }

    memcpy(m_lumaSum, copy.m_lumaSum, m_dataLen);
    memcpy(m_chromaSum, copy.m_chromaSum, m_dataLen);

    return *this;
}

/**
 *  Compare two checksumEntry objects.
 */

bool checksumEntry::operator ==(const checksumEntry& compare) {
    bool bRes = false;

    wxLogDebug("checksumEntry::operator ==(...)");

    if (m_algo == compare.m_algo) {

        wxASSERT(m_dataLen == compare.m_dataLen);

        if ((memcmp(m_chromaSum, compare.m_chromaSum, m_dataLen) == 0) &&
                (memcmp(m_lumaSum, compare.m_lumaSum, m_dataLen) == 0))
        {
            bRes = true;
        }
    }

    return bRes;
}

#ifdef __WXDEBUG__

void checksumEntry::Dump(FILE* fp) {
    fprintf(fp, "DUMP\n");
    return;
}

#endif  // __WXDEBUG__

/*----------------------------------------------------------------------------*/

checksumAlgoBase::checksumAlgoBase(eChecksumAlgo algo,
                                   wxUint32 dataLen,
                                   wxString name)
    :   m_name(name),
        m_algo(algo),
        m_dataLen(dataLen)
{
//ctor
}

checksumAlgoBase::~checksumAlgoBase() {
//dtor
}

wxString checksumAlgoBase::checksum_name() {
    return m_name;
}

eChecksumAlgo checksumAlgoBase::checksum_type() {
    return m_algo;
}

checksumEntry checksumAlgoBase::create_entry() {
    checksumEntry       newEntry(m_algo, m_dataLen);

    wxLogDebug("checksumEntry::create_entry()");

    return newEntry;
}

#ifdef  __WXDEBUG__

void checksumAlgoBase::Dump(FILE* fp) {
    fprintf(fp, "Dump of checksumAlgoBase:\n");
    fprintf(fp, "Name      : %s\n", (const char *)m_name.c_str());
    fprintf(fp, "Type      : %d\n", (int)m_algo);
    fprintf(fp, "Data Len  : %d\n", m_dataLen);
}

#endif  // __WXDEBUG__

/*----------------------------------------------------------------------------*/

checksumCRC::checksumCRC()
    :   checksumAlgoBase(CHECKSUM_CRC32, 4, "CRC32")
{
    // ctor
    wxLogDebug("checksumCRC::checksumCRC()");
}

checksumCRC::~checksumCRC()
{
    // dtor
    wxLogDebug("checksumCRC::~checksumCRC()");
}

checksumAlgoBase* checksumCRC::Create() {
    wxLogDebug("checksumCRC::Create()");
    return new checksumCRC;
}


/**
 *  Perform the crc32 calculation.
 */

bool checksumCRC::calculate(yuvBuffer* pBuffer, checksumEntry& entry) {
    wxUint32    crcValue = 0;
    wxUint8*    pCur = nullptr;

    wxLogDebug("checksumCRC::calculate(0x%p, ...)", pBuffer);

    entry = create_entry();

    crcValue = checksum.crc32_be( pBuffer->m_pY, pBuffer->m_lumaSize );

    pCur = (wxUint8*)&crcValue;
    for (size_t i = 0 ; i < sizeof(wxUint32) ; i++, pCur++) {
        entry.m_lumaSum[sizeof(wxUint32) - i - 1] = *pCur;
    }

    crcValue = checksum.crc32_be( pBuffer->m_pU, pBuffer->m_chromaSize,
                                  pBuffer->m_pV, pBuffer->m_chromaSize );

    pCur = (wxUint8*)&crcValue;
    for (size_t i = 0 ; i < sizeof(wxUint32) ; i++, pCur++) {
        entry.m_chromaSum[sizeof(wxUint32) - i - 1] = *pCur;
    }

    return true;
}


#ifdef HAVE_LIBGCRYPT
/*----------------------------------------------------------------------------*/

checksumMD5::checksumMD5()
    :   checksumAlgoBase(CHECKSUM_MD5, 16, "MD5")
{
    //ctor
    wxLogDebug("checksumMD5::checksumMD5()");
    gcry_md_open(&con, GCRY_MD_MD5, 0);
}

/**
 *
 */

checksumMD5::~checksumMD5()
{
    //dtor
    wxLogDebug("checksumMD5::~checksumMD5()");
    gcry_md_close(con);
}

/**
 *
 */

checksumAlgoBase* checksumMD5::Create() {
    wxLogDebug("checksumMD5::Create()");
    return new checksumMD5;
}

/**
 *  Perform the MD5 calculation.
 */

bool checksumMD5::calculate(yuvBuffer* pBuffer, checksumEntry& entry) {
    wxLogDebug("checksumMD5::calculate(0x%p, ...)", pBuffer);

    entry = create_entry();

    /*  Calculate the luma MD5sum */
    gcry_md_reset(con);
    gcry_md_write(con, pBuffer->m_pY, pBuffer->m_lumaSize);
    gcry_md_final(con);

    memcpy(entry.m_lumaSum, gcry_md_read(con, GCRY_MD_MD5), 16);

    /*  Calculate the chroma MD5sum */
    gcry_md_reset(con);
    for (wxUint32 i = 0 ; i < pBuffer->m_chromaSize ; i++) {
        gcry_md_putc(con, pBuffer->m_pU[i]);
        gcry_md_putc(con, pBuffer->m_pV[i]);
    }
    gcry_md_final(con);

    memcpy(entry.m_chromaSum, gcry_md_read(con, GCRY_MD_MD5), 16);

    return true;
}
/*----------------------------------------------------------------------------*/

checksumSHA1::checksumSHA1()
    :   checksumAlgoBase(CHECKSUM_SHA1, 20, "SHA1")
{
    //ctor
    wxLogDebug("checksumSHA1::checksumSHA1()");
    gcry_md_open(&con, GCRY_MD_SHA1, 0);
}

/**
 *
 */

checksumSHA1::~checksumSHA1()
{
    //dtor
    wxLogDebug("checksumSHA1::~checksumSHA1()");
    gcry_md_close(con);
}

/**
 *
 */

checksumAlgoBase* checksumSHA1::Create() {
    wxLogDebug("checksumSHA1::Create()");
    return new checksumSHA1;
}

/**
 *  Perform the MD5 calculation.
 */

bool checksumSHA1::calculate(yuvBuffer* pBuffer, checksumEntry& entry) {
    wxLogDebug("checksumSHA1::calculate(0x%p, ...)", pBuffer);

    entry = create_entry();

    /*  Calculate the luma MD5sum */
    gcry_md_reset(con);
    gcry_md_write(con, pBuffer->m_pY, pBuffer->m_lumaSize);
    gcry_md_final(con);

    memcpy(entry.m_lumaSum, gcry_md_read(con, GCRY_MD_SHA1), 20);

    /*  Calculate the chroma MD5sum */
    gcry_md_reset(con);
    for (wxUint32 i = 0 ; i < pBuffer->m_chromaSize ; i++) {
        gcry_md_putc(con, pBuffer->m_pU[i]);
        gcry_md_putc(con, pBuffer->m_pV[i]);
    }
    gcry_md_final(con);

    memcpy(entry.m_chromaSum, gcry_md_read(con, GCRY_MD_SHA1), 20);

    return true;
}
#endif // HAVE_LIBGCRYPT

/*----------------------------------------------------------------------------*/

//CHECKSUM_ALGO_VEC   checksumManager::m_algoVec;

checksumManager::checksumManager()
{
    // ctor
    wxLogDebug("checksumManager::checksumMananger()");

    AddAlgorithm( CHECKSUM_CRC32, "CRC32",  &checksumCRC::Create );

#ifdef HAVE_LIBGCRYPT
    AddAlgorithm( CHECKSUM_MD5,   "MD5SUM", &checksumMD5::Create );
    AddAlgorithm( CHECKSUM_SHA1,  "SHA1",   &checksumSHA1::Create );
#endif // HAVE_LIBGCRYPT

    wxLogDebug("There are %ld algorithms in vector...", m_algoVec.size());
}

checksumManager::~checksumManager()
{
    // ctor
    wxLogDebug("checksumManager::~checksumMananger()");
}

/**
 *  Add a checksum algorithm to the managed algoritm list.
 */

void checksumManager::AddAlgorithm(eChecksumAlgo algo,
                                   const wxString& sAlgoName,
                                   CHECKSUM_CREATE_FUNC* func)
{
    algoVecEntry        entry(algo, sAlgoName, func);

    wxLogDebug("checksumManager::AddAlgorithm(%d, %s, %p)", algo, sAlgoName, func);

    m_algoVec.push_back( entry );
}

/**
 *
 */

wxString checksumManager::GetAlgoName(eChecksumAlgo algo) {
    wxString    sResult = "Undefined";

    wxLogDebug("checksumManager::GetAlgoName(%d)", (int)algo);

    CHECKSUM_ALGO_VEC_ITER  aIter;

    for (aIter = m_algoVec.begin() ; aIter != m_algoVec.end() ; aIter++) {
        if ((*aIter).m_algoType == algo) {
            sResult = (*aIter).m_algoName;
            break;
        }
    }

    return sResult;
}

/**
 *  Given an algorithm name, return the algorithm type.
 */

eChecksumAlgo checksumManager::GetAlgoType(const wxString& name) {
    eChecksumAlgo   eResult = CHECKSUM_UNDEFINED;

    wxLogDebug("checksumManager::GetAlgoType(%s)", name);

    CHECKSUM_ALGO_VEC_ITER  aIter;

    for (aIter = m_algoVec.begin() ; aIter != m_algoVec.end() ; aIter++) {
        if ((*aIter).m_algoName.CmpNoCase(name) == 0) {
            eResult = (*aIter).m_algoType;
            break;
        }
    }

    return eResult;
}

/**
 *
 */

checksumAlgoBase* checksumManager::GetAlgoClass(eChecksumAlgo algo) {
    CHECKSUM_ALGO_VEC_ITER  aIter;
    checksumAlgoBase*       pAlgo = nullptr;
    CHECKSUM_CREATE_FUNC*   pFunc = nullptr;

    wxLogDebug("checksumManager::GetAlgoClass(%d)", (int)algo);

    for (aIter = m_algoVec.begin() ; aIter != m_algoVec.end() ; aIter++) {
        if ((*aIter).m_algoType == algo) {
            pFunc = (*aIter).m_algoCreate;
            break;
        }
    }

    if (pFunc != 0) {
        wxLogDebug("-- calling create function!");
        pAlgo = (*pFunc)();
    }

    return pAlgo;
}

CHECKSUM_ALGO_VEC_ITER checksumManager::begin() const {
    return m_algoVec.begin();
}

CHECKSUM_ALGO_VEC_ITER checksumManager::end() const {
    return m_algoVec.end();
}


#ifdef  __WXDEBUG__

void checksumManager::Dump(FILE* fp) {
    fprintf(fp, "Dump of checksumManager state:\n");
    fprintf(fp, "There are %ld algorithms in the table.\n", m_algoVec.size());

    for (CHECKSUM_ALGO_VEC_ITER iter = m_algoVec.begin() ; iter != m_algoVec.end() ; iter++) {
        fprintf(fp, "%-2d %-10s\n", (int)(*iter).m_algoType, (const char*)(*iter).m_algoName.c_str());
    }
}

#endif  // __WXDEBUG__
