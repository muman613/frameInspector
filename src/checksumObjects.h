#ifndef __CHECKSUMOBJECTS_H__
#define __CHECKSUMOBJECTS_H__

#include <wx/wx.h>
#include <wx/stdstream.h>
#include <gcrypt.h>
#include "crc32.h"

class yuvBuffer;

enum eChecksumAlgo {
    CHECKSUM_UNDEFINED,
    CHECKSUM_CRC32,
    CHECKSUM_MD5,
    CHECKSUM_SHA1,
};

/**
 *
 */

class checksumEntry {
public:
    checksumEntry();
    checksumEntry(eChecksumAlgo algo, wxUint32 dataLen);
    checksumEntry(const checksumEntry& copy);
    virtual ~checksumEntry();

    checksumEntry& operator = (const checksumEntry& copy);

    bool operator == (const checksumEntry& compare);

    wxString        GetLumaSum(bool bUpper = true);
    wxString        GetChromaSum(bool bUpper = true);

#ifdef  __WXDEBUG__
    void            Dump(FILE* fp);
#endif

    /* Data elements */
    eChecksumAlgo   m_algo;
    wxUint32        m_dataLen;
    wxUint8*        m_chromaSum;
    wxUint8*        m_lumaSum;
};

/**
 *
 */

class checksumAlgoBase {
public:
    checksumAlgoBase(eChecksumAlgo algo, wxUint32 dataLen, wxString name);
    virtual ~checksumAlgoBase();

    virtual bool            calculate(yuvBuffer* pBuffer,
                                      checksumEntry& checksum) = 0;
    checksumEntry           create_entry();

    wxString                checksum_name();
    eChecksumAlgo           checksum_type();

#ifdef  __WXDEBUG__
    virtual void            Dump(FILE* fp);
#endif

protected:
    wxString                m_name;
    eChecksumAlgo           m_algo;
    wxUint32                m_dataLen;
};

typedef checksumAlgoBase* (CHECKSUM_CREATE_FUNC)(void);

/**
 *
 */

class checksumCRC : public checksumAlgoBase {
public:
    checksumCRC();
    virtual ~checksumCRC();

    static checksumAlgoBase* Create();

    bool calculate(yuvBuffer* pBuffer, checksumEntry& checksum);

protected:
    crc32   checksum;
};

/**
 *
 */

class checksumMD5 : public checksumAlgoBase {
public:
    checksumMD5();
    virtual ~checksumMD5();

    static checksumAlgoBase* Create();

    bool calculate(yuvBuffer* pBuffer, checksumEntry& checksum);

protected:
    gcry_md_hd_t    con;
};

/**
 *
 */

class checksumSHA1 : public checksumAlgoBase {
public:
    checksumSHA1();
    virtual ~checksumSHA1();

    static checksumAlgoBase* Create();

    bool calculate(yuvBuffer* pBuffer, checksumEntry& checksum);

protected:
    gcry_md_hd_t    con;
};


/**
 *
 */

class algoVecEntry {
public:
    algoVecEntry(eChecksumAlgo algo,
                 const wxString& name,
                 CHECKSUM_CREATE_FUNC* func)
        :  m_algoType(algo),
           m_algoName(name),
           m_algoCreate(func)
    {
        // ctor
    }

    eChecksumAlgo           m_algoType;
    wxString                m_algoName;
    CHECKSUM_CREATE_FUNC*   m_algoCreate;
};

typedef wxVector<algoVecEntry>                  CHECKSUM_ALGO_VEC;
typedef wxVector<algoVecEntry>::const_iterator  CHECKSUM_ALGO_VEC_ITER;

/**
 *
 */

class checksumManager {
public:
    checksumManager();
    ~checksumManager();

    wxString                GetAlgoName(eChecksumAlgo algo);
    eChecksumAlgo           GetAlgoType(const wxString& name);
    checksumAlgoBase*       GetAlgoClass(eChecksumAlgo algo);

    CHECKSUM_ALGO_VEC_ITER  begin() const;
    CHECKSUM_ALGO_VEC_ITER  end() const;

#ifdef  __WXDEBUG__
    void                    Dump(FILE* fp);
#endif  // __WXDEBUG__

protected:

    void                    AddAlgorithm(eChecksumAlgo algo,
                                         const wxString& sAlgoName,
                                         CHECKSUM_CREATE_FUNC* func);

    CHECKSUM_ALGO_VEC       m_algoVec;
};
#endif // __CHECKSUMOBJECTS_H__
