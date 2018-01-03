bin_PROGRAMS=frameInspector

frameInspector_SOURCES=ChecksumDialog.cpp Dialogs.cpp DumpImageBuffer.cpp GridSettingsDialog.cpp SizeDialog.cpp \
					YUV420ImageBufferFile.cpp YUV420ImageBufferSplit.cpp YUVCompDialog.cpp YUVSplitDialog.cpp   \
					checksumObjects.cpp component_buffer.cpp controlParms.cpp crc32.cpp crcVector.cpp 			\
					dumpInternals.cpp formatDialog.cpp frame.cpp imageBuffer.cpp misc_utils.cpp	viewApp.cpp 	\
					yuvImageControl.cpp

noinst_HEADERS=ChecksumDialog.h Dialogs.h DumpImageBuffer.h GridSettingsDialog.h SizeDialog.h                   \
			   YUV420ImageBufferFile.h YUV420ImageBufferSplit.h YUVCompDialog.h checksumObjects.h cpupack.h     \
			   crc32.h crcVector.h dumpInternals.h frame.h imageBuffer.h misc_utils.h types.h viewApp.h         \
			   yuvImageControl.h

if INCLUDE_LIBMJPEGTOOLS
frameInspector_SOURCES += YUV420ImageBufferY4M.cpp
CXXFLAGS += $(LIBMJPEGTOOLS_CFLAGS)
LIBS += $(LIBMJPEGTOOLS_LIBS)
endif
