# CMakeLists.txt for frameInspector (November 19th, 2018)
# Michael Uman <muman613@gmail.com>

cmake_minimum_required(VERSION 3.12)
project(frameInspector)

set(CMAKE_CXX_STANDARD 14)

# The cmake build configuration requires all libraries, at this time there is
# no way to specify that some of these libraries (mjpegtools, libswscale) are
# optional as through the autoconf method of building the project.

include(cmake/Modules/FindGcryptConfig.cmake)
gcr_check(GCRYPT REQUIRED gcrypt)

find_package(PkgConfig REQUIRED)
pkg_search_module(POPT REQUIRED popt)
pkg_search_module(SWSCALE REQUIRED libswscale)
pkg_search_module(YUV4MPEG REQUIRED mjpegtools)

find_package(wxWidgets COMPONENTS core base adv)


include_directories(.)
include_directories(src)
include_directories(${POPT_INCLUDE_DIRS} ${SWSCALE_INCLUDE_DIRS} ${YUV4MPEG_INCLUDE_DIRS} )

include(${wxWidgets_USE_FILE})

add_definitions(-DHAVE_LIBSWSCALE=1 -DHAVE_LIBGCRYPT=1 -DHAVE_LIBMJPEGTOOLS=1)

add_executable(frameInspector
        src/bell.h
        src/ChecksumDialog.cpp
        src/ChecksumDialog.h
        src/checksumObjects.cpp
        src/checksumObjects.h
        src/component_buffer.cpp
        src/component_buffer.h
        src/controlParms.cpp
        src/controlParms.h
        src/cpupack.h
        src/crc32.cpp
        src/crc32.h
        src/crcVector.cpp
        src/crcVector.h
        src/Dialogs.cpp
        src/Dialogs.h
        src/DumpImageBuffer.cpp
        src/DumpImageBuffer.h
        src/dumpInternals.cpp
        src/dumpInternals.h
        src/formatDialog.cpp
        src/formatDialog.h
        src/frame.cpp
        src/frame.h
        src/GridSettingsDialog.cpp
        src/GridSettingsDialog.h
        src/hashDialog.cpp
        src/hashDialog.h
        src/imageBuffer.cpp
        src/imageBuffer.h
        src/imageSpec.h
        src/misc_utils.cpp
        src/misc_utils.h
        src/SizeDialog.cpp
        src/SizeDialog.h
        src/types.h
        src/viewApp.cpp
        src/viewApp.h
        src/YUV420ImageBufferFile.cpp
        src/YUV420ImageBufferFile.h
        src/YUV420ImageBufferSplit.cpp
        src/YUV420ImageBufferSplit.h
        src/YUV420ImageBufferY4M.cpp
        src/YUV420ImageBufferY4M.h
        src/YUVCompDialog.cpp
        src/YUVCompDialog.h
        src/yuvImageControl.cpp
        src/yuvImageControl.h
        src/YUVSplitDialog.cpp
        src/YUVSplitDialog.h
        config.h)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${POPT_LIBRARIES} ${SWSCALE_LIBRARIES} ${YUV4MPEG_LIBRARIES} ${GCRYPT_LIBRARIES})
