#include <wx/wx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "misc_utils.h"

namespace utils {

    void digest_to_string(wxUint8* pDigest, std::string& sOut) {
        std::stringstream  sBuf;

        sBuf << std::hex << std::setfill('0') << std::uppercase;

        for (int i = 0 ; i < 16 ; i++) {
            sBuf << std::setw(2) << (unsigned int)pDigest[i];
        }

        sOut = sBuf.str();

        return;
    }

    void display_digest(wxUint8* digest) {
        for (int i = 0 ; i < 16 ; i++) {
            printf("%02x",digest[i]);
        }
        printf("\n");
    }

};

