#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "crc32.h"

#define CRCPOLY_BE 0x04c11db7
#define ADD_CRC32_BYTE(crc, c) (((crc) << 8) ^ crc_table[(((crc) >> 24) ^ (c)) & 0xFF])


#ifdef  USE_CLASS

crc32::crc32() {
    // ctor
    wxLogDebug("crc32::crc32()");

    crc32_be_init();
}

crc32::~crc32() {
    // dtor
    wxLogDebug("crc32::~crc32()");
}


void crc32::crc32_be_init(void) {
    wxUint32 crc, poly;
    wxUint32 i, j;

    wxLogDebug("crc32_be_init()");

    poly = CRCPOLY_BE;
    for (i = 0; i < 256; i++) {
        crc = (i << 24);
        for (j = 8; j > 0; j--)
            crc = (crc & 0x80000000) ? (crc << 1) ^ poly : crc << 1;
        crc_table[i] = crc;
    }

    return;
}

/**
 *
 */

wxUint32 crc32::crc32_be(wxUint8* buffer_start, wxUint32 buffer_length) {
    wxUint32            crc;
    //register wxUint8*   pData;

    wxLogDebug("crc32::crc32_be(0x%p, %d)", buffer_start, buffer_length);

    assert((buffer_start != nullptr) && (buffer_length != 0));

    crc = 0xFFFFFFFF;

    //pData = buffer_start;

    for (wxUint32 offset = 0 ; offset < buffer_length ; offset++) {
        crc = ADD_CRC32_BYTE(crc, *(buffer_start + offset));
    }

    return crc;
}

/**
 *  Overloaded function to calculate crc32 on UV buffers...
 */

wxUint32 crc32::crc32_be(wxUint8* buffer1_start, wxUint32 buffer1_length,
                         wxUint8* buffer2_start, wxUint32 buffer2_length)
{
    wxUint32   crc;
    wxUint32   buffer_length;

    wxLogDebug("crc32::crc32_be(0x%p, %d, 0x%p, %d)",
               buffer1_start, buffer1_length, buffer2_start, buffer2_length);

    assert(buffer1_length == buffer2_length);

    crc = 0xFFFFFFFF;

    buffer_length = buffer1_length;

    for (wxUint32 offset = 0 ; offset < buffer_length ; offset++) {
        crc = ADD_CRC32_BYTE(crc, *(buffer1_start + offset));
        crc = ADD_CRC32_BYTE(crc, *(buffer2_start + offset));
    }

    return crc;
}
#else   // USE_CLASS
static wxUint32 crc_table[256];

void crc32_be_init(void) {
    wxUint32 crc, poly;
    wxUint32 i, j;

    wxLogDebug("crc32_be_init()");

    poly = CRCPOLY_BE;
    for (i = 0; i < 256; i++) {
        crc = (i << 24);
        for (j = 8; j > 0; j--)
            crc = (crc & 0x80000000) ? (crc << 1) ^ poly : crc << 1;
        crc_table[i] = crc;
    }

    return;
}

/**
 *
 */

wxUint32 crc32_be(wxUint8* buffer_start, wxUint32 buffer_length) {
    wxUint32            crc;
    register wxUint8*   pData;

    wxLogDebug("crc32_be(0x%08lx, %ld)", buffer_start, buffer_length);

    assert((buffer_start != nullptr) && (buffer_length != 0));

    crc = 0xFFFFFFFF;

    pData = buffer_start;

    for (wxUint32 offset = 0 ; offset < buffer_length ; offset++) {
        crc = ADD_CRC32_BYTE(crc, *(buffer_start + offset));
    }

    return crc;
}

/**
 *  Overloaded function to calculate crc32 on UV buffers...
 */

wxUint32 crc32_be(wxUint8* buffer1_start, wxUint32 buffer1_length,
                  wxUint8* buffer2_start, wxUint32 buffer2_length)
{
    wxUint32   crc;
    wxUint32   buffer_length;

    wxLogDebug("crc32_be(0x%08lx, %ld, 0x%08lx, %ld)",
               buffer1_start, buffer1_length, buffer2_start, buffer2_length);

    assert(buffer1_length == buffer2_length);

    crc = 0xFFFFFFFF;

    buffer_length = buffer1_length;

    for (wxUint32 offset = 0 ; offset < buffer_length ; offset++) {
        crc = ADD_CRC32_BYTE(crc, *(buffer1_start + offset));
        crc = ADD_CRC32_BYTE(crc, *(buffer2_start + offset));
    }

    return crc;
}

#endif  // USE_CLASS
