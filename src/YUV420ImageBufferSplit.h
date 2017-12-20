//==============================================================================
//  MODULE      :   YUV420ImageBufferSplit.h
//  PROJECT     :   frameInspector
//  PROGRAMMER  :   Michael A. Uman
//  DATE        :   October 8, 2013
//==============================================================================

#ifndef __YUV420IMAGEBUFFERSPLIT_H__
#define __YUV420IMAGEBUFFERSPLIT_H__

#include "types.h"
#include "imageBuffer.h"
//#include "misc_utils.h"
#include "imageSpec.h"

//class YUV420ImageSpec {
//public:
//    YUV420ImageSpec() { };
//    YUV420ImageSpec(wxString sPath, wxString sPrefix, int first, int last)
//    :   m_imagePath(sPath), m_prefix(sPrefix),
//        m_first(first), m_last(last)
//    {
//
//    }
//
//    wxString        m_imagePath;
//    wxString        m_prefix;
//    int             m_first         =   0;
//    int             m_last          =   -1;
//};

/**
  * Implement an imagebuffer which loads from Y, U, & V files.
  */

class YUV420ImageBufferSplit : public ImageBuffer {
public:
    ///	YUV Image buffer constructor
    /**
      * @param width Width of image buffer.
      * @param height Height of image buffer.
      */
    YUV420ImageBufferSplit(int width, int height, int bits = 8,
                           formatEndian endianness = endian_little);
    virtual ~YUV420ImageBufferSplit();

    bool		Load(size_t frame);

    /// Load image from file by name.
    /**
      * This function attempts to load the frame by
      * looking for files named {filename}.Y and {filename}.U
      * and {filename}.V. If these files exist they are read
      * into the image and converted to RGB.
      * @param filename Name of file to load
      * @return true on success
      */
    bool		Load(const char* filename);
    /// Store image data into wxImage
    /**
      * Store image data into wxImage
      */
    void		GetImage(wxImage* pImage);

    /// Return the PIXEL at location x,y.
    /**
      *	@param x X coordinate
      * @param y Y coordinate
      * @return Pointer to PIXEL structure.
      */
    PIXEL*		getPixel(int x, int y);

    /// Set the path to load from.
    /**
      * @param path wxString containing path
      */
//		void		setPath(const wxString& path) {
//			m_imagePath = path;
//		}
//		void 		setPrefix(const wxString& prefix) {
//			m_prefix = prefix;
//		}
    void        setImageSpec(const YUV420ImageSpec& spec) {
        m_spec = spec;
    }

    ssize_t		GetFrameCount();
    wxString    GetFileBase(size_t frame);
    void        getImageVec();

protected:
//		wxString                        m_imagePath;
//		wxString                        m_prefix;
    YUV420ImageSpec                 m_spec;

    utils::FRAME_FILEBASE_VEC       m_fileVec;
};

#endif
