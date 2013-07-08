#ifndef __YUV420IMAGEBUFFERSPLIT_H__
#define __YUV420IMAGEBUFFERSPLIT_H__

#include "types.h"
#include "imageBuffer.h"

/**
  * Implement an imagebuffer which loads from Y, U, & V files.
  */

class YUV420ImageBufferSplit : public ImageBuffer
{
public:
    ///	YUV Image buffer constructor
    /**
      * @param width Width of image buffer.
      * @param height Height of image buffer.
      */
    YUV420ImageBufferSplit(int width, int height);
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
    void		setPath(const wxString& path) {
        m_imagePath = path;
    }
    void 		setPrefix(const wxString& prefix) {
        m_prefix = prefix;
    }

    ssize_t		GetFrameCount();


protected:

//		bool		m_ccir601;
    wxString	m_imagePath;
    wxString	m_prefix;
};

#endif
