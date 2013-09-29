#ifndef __CRC32_H__
#define __CRC32_H__

#include <wx/wx.h>
#include "types.h"

void crc32_be_init(void);
wxUint32 crc32_be(wxUint8* buffer_start, wxUint32 buffer_length);

/** Version used for chroma (UV) checksum */
wxUint32 crc32_be(wxUint8* buffer1_start, wxUint32 buffer1_length,
                  wxUint8* buffer2_start, wxUint32 buffer2_length);

#endif  // __CRC32_H__
