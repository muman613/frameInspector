#ifndef __YUV420IMAGEBUFFERY4M_H__
#define __YUV420IMAGEBUFFERY4M_H__

#include <wx/dynarray.h>
#include <yuv4mpeg.h>

#include "imageBuffer.h"
#include "misc_utils.h"

WX_DEFINE_ARRAY_SIZE_T(wxFileOffset, ArrayOfOffsets);

/**
  * Implement an imagebuffer which loads from composite YUV file.
  * Each component is stored in a consecutive block w X h.
  */

class YUV420ImageBufferY4M : public ImageBuffer {
	public:
		YUV420ImageBufferY4M();
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
		wxString			m_imageFilename;
		wxFile				m_file;

		y4m_stream_info_t	m_strInfo;
		y4m_frame_info_t	m_frmInfo;

		size_t				generateIndex();

		ArrayOfOffsets		m_frameOffsets;
};

#endif
