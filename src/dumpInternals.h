#ifndef __DUMPINTERNALS_H__
#define __DUMPINTERNALS_H__

#include <wx/wx.h>
#include <wx/file.h>

#define RMCHIP_ID   1
#define RMCHIP_ID_SMP8634 2


namespace dumpInternals {

    struct box_struct {
        wxUint32 ID;
        wxUint32 Version;
        wxInt64  Size; //-1 is unknown
    };

    #define RMTILE_WIDTH_SHIFT  8
    #define RMTILE_HEIGHT_SHIFT 5
    #define RMTILE_SIZE_SHIFT   (RMTILE_WIDTH_SHIFT + RMTILE_HEIGHT_SHIFT)

    #ifndef CREATEMAGIC
    #define CREATEMAGIC(ch3, ch2, ch1, ch0) ( (wxInt32)(wxInt8)ch0 | ((wxInt32)(wxInt8) ch1 << 8) | ((wxInt32)(wxInt8)ch2 << 16) | ((wxInt32)(wxInt8)ch3 << 24))
    #endif


    // rmar = rmar(chive)
    #define RMAR_TAG     "rmar"
    #define RMAR_MAGIC   CREATEMAGIC('r','m','a','r')
    #define RMAR_VERSION (1)

    #define PICI_TAG     "pici"
    #define PICI_MAGIC   CREATEMAGIC('p','i','c','i')
    #define CHROMA_TAG   "chrm"
    #define CHROMA_MAGIC CREATEMAGIC('c','h','r','m')
    #define LUMA_TAG     "luma"
    #define LUMA_MAGIC   CREATEMAGIC('l','u','m','a')

    /* structures */

    enum EMhwlibPictureVersion {
        EMhwlibPictureVersionDemo = 1,
        EMhwlibPictureVersion2 = 2,
    };

    struct EMhwlibWindow {
        wxUint32 x;
        wxUint32 y;
        wxUint32 width;
        wxUint32 height;
    };

    struct EMhwlibPostProcessing {
        wxUint32 quantizer_addr;
        /* The address of the memory rectangle in which quantizer
           scale parameters are stored, two bytes per macro block
           (the post processing module needs 12-bit values but they
           are sent through r12 vbus channel on 16-bit values).
           When the frame is coded as two field pictures, the macro
           block quantizer scale data lines are interleaved: the first
           line corresponds to the first macro block row of the top
           field, the second line corresponds to the first macro block
           row of the bottom field, and so on.
        */
        wxUint32 quantizer_total_width;
        /* The total width of the buffer in tile units when
           quantizer_addr is not 0 (should equate to
           luma_total_width ).
        */
    };

    union EMhwlibPictureStereoscopicData {
        struct {
            /*
            Stereoscopic 3D format, see also 'EMhwlibStereoscopic...' types for element definitions
            [7:0]   Format (0=2D,etc.), as in "enum EMhwlibStereoscopicFormat":
                0 = 2D
                1 = Anaglyphic
                2 = Page Flip (Frame Sequential)
                3 = Line Interleave
                4 = Column Interleave
                5 = Checkerboard Interleave
                6 = Side by Side, Half
                7 = Over / Under, Half (Top and Bottom)
                8 = Line Alternative
                9 = Column Alternative
                10 = Checkerboard Alternative
                11 = Side by Side, Full
                12 = Over / Under, Full
                13 = Frame Packing
                14 = Field Alternative (Field Sequential)
                15 = L + Depth
                16 = L + Depth + Graphics + Graphics Depth
                17..255 = reserved
            */
            wxUint32 format:8;
            /*
            [9:8]   SubSampling Method, as in "enum EMhwlibStereoscopicSubSamplingMethod"
                0 = Horizontal
                1 = Quincunx
                2 = Vertical
                3 = reserved
            */
            wxUint32 subSamplingMethod:2;
            /*
            [11:10] SubSampling Position, as in "enum EMhwlibStereoscopicSubSamplingPosition"
                0 = Odd Left, Odd Right
                1 = Odd Left, Even Right
                2 = Even Left, Odd Right
                3 = Even Left, Even Right
            */
            wxUint32 subSamplingPosition:2;
            /*
            [12]    View Arrangement/Order
                If Format [7:0] is other than 2 (Page Flip):
                  0 = Left view is leftmost/topmost
                  1 = Right view is leftmost/topmost
                If Format [7:0] is 2 (Page flip):
                  1 = This is the first view
                  0 = This is a subsequent view
            */
            wxUint32 viewOrder:1;
            /*
            [14:13] Flip (This only applies to SbS or TnB formats)
                0 = No view is spatially flipped
                1 = reserved
                2 = Left view is spatially flipped, vertically for TnB, horizontally for SbS
                3 = Right view is spatially flipped, vertically for TnB, horizontally for SbS
            */
            wxUint32 flip:2;
            /*
            [15]    Force flag (Implementation specific, no general use)
                0 = Not forced / overridden / valid
                1 = Forced /overridden / valid
            */
            wxUint32 forceFlag:1;
            /*
            [23:16] View ID
                Index of the view stored in the current picture buffer.
                As a convention, when number of views is two, View ID = 0 is
                used for the left view and View ID = 1 is for the right view.
                When the number of views is greater than two, the application
                should provide separately to the display handler an association
                table mapping the View ID to a specific viewpoint.
                The View ID has to be less than the Number of Views, or zero.
            */
            wxUint32 viewID:8;
            /*
            [31:24] Number of Views
                The total number of views per access unit. If number of views is two or more,
                the Format [7:0] shall be 2, "Page Flip". The view parity shall be determined
                by the View ID in [23:16]. The view pairing shall be determined by the View
                Arrangement in bit [12], or by equal PTS values among pairs.
            */
            wxUint32 numViews:8;
        }bits;
        wxUint32 value;
    };
    /*  WARNING: This structure must be kept in sync with the definition of
     *  "decoder_picture_buffer_t dma struct" in /ucode_lib/ucode/video/global.h
     */
    struct EMhwlibPictureV1 {
        enum EMhwlibPictureVersion picture_version;

        wxUint32 luma_address;                          /* 0x00 */
        /* gbus address of the pixels if a single buffer is used to
           store the picture. It is the luma buffer in case of a non-
           interleaved (planar) picture
        */
        wxUint32 luma_total_width;                      /* 0x04 */
        /* Number of pixels of a line inside the storage buffer.
         */
        wxUint32 chroma_address;                        /* 0x08 */
        /* NULL if a single buffer is used to store the picture. It is the
           chroma buffer in case of a non-interleaved (planar)
           picture.
        */
        wxUint32 chroma_total_width;                    /* 0x0c */
        /* Number of samples in one line inside the storage buffer.
           Note: Each chroma sample is stored on two bytes.
        */
        struct EMhwlibWindow luma_position_in_buffer;   /* 0x10 */
        struct EMhwlibWindow chroma_position_in_buffer; /* 0x20 */
        /* Define the picture region containing valid pixels inside the
           display buffer. width and height are expressed in
           sample and line units. The height of the displayed picture
           is the height of the frame.
        */
        wxUint32 scaled_width;                          /* 0x30 */
        /* The display size of the buffer displayed in number of samples.
           The region inside the luma_position_in_buffer
           and chroma_position_in_buffer must be (virtually)
           scaled to a rectange of size scaled_width x scaled_height
           prior to any other operation.
        */
        wxUint32 scaled_height;                         /* 0x34 */
        /* The display size of the buffer displayed in number of lines.
           The region inside the luma_position_in_buffer
           and chroma_position_in_buffer must be (virtually)
           scaled to a rectange of size scaled_width x scaled_height
           prior to any other operation.
        */
        struct EMhwlibWindow panscan_1;                 /* 0x38 */
        struct EMhwlibWindow panscan_2;                 /* 0x48 */
        struct EMhwlibWindow panscan_3;                 /* 0x58 */
        struct EMhwlibWindow panscan_4;                 /* 0x68 */
        /* Describes the picture's displayable rectangular region in
           pan-scan mode in one-sixteenth (1/16) luma sample units
           relative to the luma_position_in_buffer rectangle.
           There are four values, because when repeat frame = 1 a
           picture is displayed as four fields and the position of the
           displayable area may change from one field to the other.
           Notes:
           - x and width are expressed in luma sample units;
             y and height are expressed in line units.
           - These rectangles are used only when chroma format is
             4:2:0 or 4:2:2 (and the pixel format is Y_UV).
             The chroma format is not signaled explicitly. The display
             handler should derive it from the luma_position_in_buffer
             and chroma_position_in_buffer's width and height values.
        */
        wxUint32 ar_x;                                  /* 0x78 */
        wxUint32 ar_y;                                  /* 0x7c */
        /* The pixel aspect ratio of the picture with size
           scaled_width * scaled_height
        */
        wxUint32 color_description;                     /* 0x80 */
        /* bits  Description
           0-7   Matrix coefficients:
                0 = RGB
                1 = ITU-R BT.709 / BT.1361 / IEC 61966-2-4 (xvYCC709)
                2 = Unspecified
                4 = FCC
                5 = ITU-R BT.470-2 System B,G / IEC 61966-2-4 (xvYCC601)
                6 = SMPTE 170 M
                7 = SMPTE 240M
                8 = YCoCg
           8-15  Transfer characteristics:
                1 = ITU-R BT.709 / BT.1361 conventional gamut
                2 = Unspecified
                4 = ITU-R BT.470-2 System M
                5 = ITU-R BT.470-2 System B,G
                6 = SMPTE 170M
                7 = SMPTE 240M
                8 = Linear
                9 = Logarithmic transfer characteristic (100:1 range)
                10 = Logarithmic transfer characteristic (316.22777:1 range)
                11 = IEC 61966-2-4 (xvYCC)
                12 = ITU-R BT.1361 extended gamut
           16-23 Color primaries:
                1 = ITU-R BT.709 / BT.1361 / IEC 61966-2-4 (xvYCC)
                2 = Unspecified
                4 = ITU-R BT.470-2 System M
                5 = ITU-R BT.470-2 System B,G
                6 = SMPTE 170M
                7 = SMPTE 240M
                8 = Generic Film (color filters using Illuminant C)
           24    Video range
           25-31 Reserved
        */
        wxUint32 first_pts_lo;                          /* 0x84 */
        /* Represents the least significant 32 bits (0 - 31) of the
           PTS. Associated with a progressive frame or with the first
           field of an interlaced frame.
        */
        wxUint32 first_pts_hi;                          /* 0x88 */
        /* Represents the most significant 32 bits (32 - 63) of the
           PTS. Associated with a progressive frame or with the first
           field of an interlaced frame.
        */
        wxUint32 delta_pts;                             /* 0x8c */
        /* Represents the offset of the PTS value associated with
           the second field with respect to the first one. It is present
           in the case of interlaced frames and it is used to derive
           the PTS value associated with the second field.
           In the case of progressive frames, it should be present for
           pan-scan modes. In this case, it is used to derive the
           value of the time stamps associated with the second, third
           and fourth field.
           Note: 0 means that the delta_pts value is unknown.
        */
        wxUint32 system_pts;                            /* 0x90 */
        /* bit description
           0   1 = PTS value from the system layer
               0 = interpolated PTS value
           12  1 = picture PTS already present
               0 = interpolate PTS value, if needed
               (system_pts[0]=0)
        */
        wxUint32 timecode;                              /* 0x94 */
        /*
         */
        wxUint32 time_increment_resolution;             /* 0x98 */
        /* Number of ticks per second. This represents the
           timescale for the PTS values.
        */
        wxUint32 native_time_increment_resolution;
        /* Value of number of ticks embedded in the
        elementary stream
        */

        wxUint32 picture_display_data;                  /* 0x9c */
        /* bits   Description
           0-1    Fields:
                0 = progressive frame
                1 = top field only (bottom field invalid)
                2 = bottom field only (top field invalid)
                3 = interlaced frame
           2      Top field first.
           3      Repeat first field.
           4-5    Number of repeated frames (0 - 3).
           6-7    Pixel format:
                0 = Y_UV (separate luma and chroma buffers)
                1 = Y (monochrome pictures)
                2 = YUYV (interleaved 422)
                3 = RGBA
           8-10   Bits per pixel:
                0 = 1 bpp
                1 = 2 bpp
                2 = 4 bpp
                3 = 8 bpp
                4 = 16 bpp
                5 = 24 bpp
                6 = 32 bpp
                7 = Reserved.
           11-13  Bit mapping:
                For Pixel Format = 3 (RGBA) and
                Bits/Pixel = 0 - 3 (1,2,4,8 bpp),
                    0 = LUT,
                    1 = gray level,
                    2 - 7 = reserved.
                For Pixel Format = 3 (RGBA) and
                Bits/Pixel = 4 (16 bpp),
                    0 = 565 RGB,
                    1 = 4444 ARGB,
                    2 = 1555 ARGB,
                    3 = 1 byte alpha + 1 byte LUT,
                    4-7 = Reserved
                For Pixel Format = 3 (RGBA) and
                Bits/Pixel = 5 (24 bpp),
                    0 = 888 RGB,
                    1 - 7 = reserved
                For Pixel Format = 3 (RGBA) and
                Bits/Pixel = 6 (32 bpp),
                    0 = 8888 RGBA,
                    1 - 7 = reserved
           14    Progressive sequence.
           15    Linear buffer:
                1 = Linear
                0 = Tiled
           16-19 Chroma-luma top field vertical phase offset.
             Specifies the chroma-luma phase offset in
             chroma pixel units.
           20-23 Chroma-luma top field horizontal phase offset.
             Specifies the chroma-luma phase offset in
             chroma pixel units.
           24-27 Chroma-luma bottom field vertical phase offset.
             Specifies the chroma-luma phase offset in
             chroma pixel units.
           28-31 Chroma-luma bottom field horizontal phase offset.
             Specifies the chroma-luma phase offset in
             chroma pixel units.
        */
    #if (RMCHIP_ID!=RMCHIP_ID_SMP8634)
        wxUint32 picture_data_extension;                /* 0xa0 */
        /* bits  Description
           0     Picture structure coding,
                0 = Frame,
                1 = Field.
           1-31  Reserved.
        */
    #endif
        union EMhwlibPictureStereoscopicData multi_view_parameters;
        /*
        Stereoscopic 3D format, see also 'EMhwlibStereoscopic...' types for element definitions
        [7:0]   Format (0=2D,etc.), as in "enum EMhwlibStereoscopicFormat":
            0 = 2D
            1 = Anaglyphic
            2 = Page Flip (Frame Sequential)
            3 = Line Interleave
            4 = Column Interleave
            5 = Checkerboard Interleave
            6 = Side by Side, Half
            7 = Over / Under, Half (Top and Bottom)
            8 = Line Alternative
            9 = Column Alternative
            10 = Checkerboard Alternative
            11 = Side by Side, Full
            12 = Over / Under, Full
            13 = Frame Packing
            14 = Field Alternative (Field Sequential)
            15 = L + Depth
            16 = L + Depth + Graphics + Graphics Depth
            17..255 = reserved
        [9:8]   SubSampling Method, as in "enum EMhwlibStereoscopicSubSamplingMethod"
            0 = Horizontal
            1 = Quincunx
            2 = Vertical
            3 = reserved
        [11:10] SubSampling Position, as in "enum EMhwlibStereoscopicSubSamplingPosition"
            0 = Odd Left, Odd Right
            1 = Odd Left, Even Right
            2 = Even Left, Odd Right
            3 = Even Left, Even Right
        [12]    View Arrangement/Order
            If Format [7:0] is other than 2 (Page Flip):
              0 = Left view is leftmost/topmost
              1 = Right view is leftmost/topmost
            If Format [7:0] is 2 (Page flip):
              1 = This is the first view
              0 = This is a subsequent view
        [14:13] Flip (This only applies to SbS or TnB formats)
            0 = No view is spatially flipped
            1 = reserved
            2 = Left view is spatially flipped, vertically for TnB, horizontally for SbS
            3 = Right view is spatially flipped, vertically for TnB, horizontally for SbS
        [15]    Force flag (Implementation specific, no general use)
            0 = Not forced / overridden / valid
            1 = Forced /overridden / valid
        [23:16] View ID
            Index of the view stored in the current picture buffer.
            As a convention, when number of views is two, View ID = 0 is
            used for the left view and View ID = 1 is for the right view.
            When the number of views is greater than two, the application
            should provide separately to the display handler an association
            table mapping the View ID to a specific viewpoint.
            The View ID has to be less than the Number of Views, or zero.
        [31:24] Number of Views
            The total number of views per access unit. If number of views is two or more,
            the Format [7:0] shall be 2, "Page Flip". The view parity shall be determined
            by the View ID in [23:16]. The view pairing shall be determined by the View
            Arrangement in bit [12], or by equal PTS values among pairs.
        */

        wxUint32 luma_scale;                            /* 0xa4 */
        /* The luma_scale is the multipliers the display should apply
           to the pixel's luma, with four fractional bits. 0x10 means
           no change.
        */
        wxUint32 chroma_scale;                          /* 0xa8 */
        /* The chroma_scale is the multiplier the display should
           apply to the pixel's chroma value, with four fractional bits.
           0x10 means no change.
        */
        wxUint32 picture_bytecount;                     /* 0xac */
        /* Byte count of the video elementary stream associated
           with the picture start code or header.
           0 = not used,
           1 = used for decoding.
        */
        wxUint32 user_metadata;                         /* 0xb0 */
        /* reserved
         */
        wxUint32 user_data_entry_status;	        /* 0xb4 */
        /* reserved
         */
        wxUint32 user_data_entry_size;                  /* 0xb8 */
        /* reserved
         */
        wxUint32 picture_decode_status;                 /* 0xbc */
        /* 0 = not used,
           1 = used for display,
           3 = on-screen.
        */
        wxUint32 picture_display_status;                /* 0xc0 */
        /* Return code attached to the picture. A non-zero value
         */
        wxUint32 error_status;                          /* 0xc4 */
        /* signals that errors occurred during the decoding.
         */
    #if (RMCHIP_ID!=RMCHIP_ID_SMP8634)
        struct EMhwlibPostProcessing post_processing;   /* 0xc8 */
    #endif
        wxUint32 palette_address;                       /* 0xd0 */
        /* gbus address of palette buffer.
         */
        wxUint32 palette_size;                          /* 0xd4 */
        /* Number of valid colors used.
         */
        wxUint32 palette_buffer_size;                   /* 0xd8 */
        /* Size of the palette buffer.
         */
        wxUint32 active_format;                         /* 0xdc */
        /* bits     Description
           0-3      AFD Tag. Contain values as defined in ETSI TS
                101 154 (DVB) Annex B resp. CEA 805.
           4-7      DVB AFD.
           8        AFD tag valid.
           9        Horizontal bar valid. 1 = Override AFD
           10       Vertical bar valid. 1 = Override AFD
           11       Scan info valid.
           12-13    Scan info:
                0 = Unknown (use format-inherent scan info)
                1 = Overscanned (picture can be cropped)
                2 = Underscanned (picture shall not be cropped).
           14-31    Reserved.
        */
        wxUint32 horizontal_bar;                        /* 0xe0 */
        /* bits     Description
           0-15     Bottom bar start.
           16-31    Top bar end (0 = no top bar).
        */
        wxUint32 vertical_bar;                          /* 0xe4 */
        /* bits     Description
           0-15     Right bar end.
           16-31    Left bar end (0 = no left bar).
        */
        wxUint32 MBData_ptr;
        /*
          pointer to the MBData structure in DRAM associated with the
          current decoder picture buffer structure.
        */

        wxUint32 frame_count;                           /* 0xe8 */
        /* Decoded frame counter
         */

        wxUint32 display_inband_cmd_tags;
        /*
        [7:0]   Tag:
          0 = END_OF_STREAM
          1 = DISCONTINUITY

        [8]     Valid_Tag_Flag:
          0: no tag is present
          1: a tag is present
        [10:9]   Non_Displayable_Pic_Flag:
          0 = the picture can be displayed
          1 = the picture should not be displayed
        */
    };

    struct EMhwlibPictureInfoV1 {
        struct EMhwlibPictureV1 Picture;
        wxUint32                PictureAddress;
    };


    /* functions */

    bool read_box(wxFile* fp, wxUint32& tag, wxUint32& version, wxInt64& size);

}

#endif  // __DUMPINTERNALS_H__

