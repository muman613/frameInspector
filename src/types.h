#ifndef __FRAMEINSPECTOR_TYPES_H__
#define __FRAMEINSPECTOR_TYPES_H__

typedef unsigned char UBYTE;

/// Structure containing RGB value of a pixel
typedef struct _pixel {
	UBYTE	r;	/*!< Red component */
	UBYTE	g;	/*!< Green component */
	UBYTE	b;	/*!< Blue component */
} PIXEL;

/// Image data representation
enum dataType {
    DATA_UNKNOWN,   //!< Data format unknown
    DATA_YUV420,    //!< Data in YUV420 format
    DATA_YUV422,    //!< Data in YUV422 format
    DATA_YUV444,    //!< Data in YUV444 format
    DATA_NV12,      //!< Data in NV12 format
    DATA_NV21,      //!< Data in NV21 format
    DATA_RGB32,     //!< Data in RGB32 format
};

enum yuvBufType {
    YUV_FILE_UNKNOWN,
    YUV_FILE_SPLIT,
    YUV_FILE_COMP,
    YUV_FILE_DUMP,
};

enum formatEndian {
    endian_little,
    endian_big,
    endian_undefined = -1,
};

#endif
