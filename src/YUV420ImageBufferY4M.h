#ifndef __YUV420IMAGEBUFFERY4M_H__
#define __YUV420IMAGEBUFFERY4M_H__

#include "imageBuffer.h"
#include "misc_utils.h"

/**
  * Implement an imagebuffer which loads from composite YUV file.
  * Each component is stored in a consecutive block w X h.
  */

class YUV420ImageBufferY4M : public ImageBuffer {
	public:
		YUV420ImageBufferY4M(int width, int height, int bits = 8,
                        formatEndian endianness = endian_little);
		virtual ~YUV420ImageBufferY4M();

		bool		Load(size_t frame);
		void		GetImage(wxImage* pImage);
		ssize_t		GetFrameCount();
		PIXEL*		getPixel(int x, int y);

		bool		SetFilename(const wxString& filename);

        bool        CanChecksum() const {
            return true;
        }
        //bool        GetChecksum(size_t frame, wxUint8* lumaSum, wxUint8* chromaSum);

	protected:

//		bool		m_ccir601;
		wxString	m_imageFilename;
		wxFile		m_file;
};

#endif
