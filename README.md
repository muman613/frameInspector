# frameInspector YUV Stream Viewer

The Versatile YUV file displayer

## Overview

frameInspector is a tool which reads a raw YUV file (in a variety of formats) and displays it in a window.

* Reads 'YUV Split' files with separate Y/U/V files.
* Reads 'YUV Composite' files containing multiple images.
* Supports many image resolutions from QCIF -> UHD.
* Supports YUV420p and YUV420p10 (big & little endian)
* Generates frame checksum or MD5 hash for chroma and luma components.
* Save individual frames as bitmap image or separate Y/U/V components.
* Uses optimized YUV conversion routine from `ffmpeg` and saves YUV4MPEG frames.

## Building project

### Building with autoconf/automake

*frameInspector* utilizes the standard 'autoconf' configuration system. Once the source-code is downloaded issue the following commands to build the project:

    $> ./autogen.sh        # This generates the 'configure' script.
    $> ./configure         # This generates the 'makefile'
    $> make -j N           # This builds the project with N processors

**NOTE:** To build using the `ffmpeg` associated library `libswscale` the configure command should include `--enable-libswscale`.

### Building with cmake

Now it is possible to build *frameInspector* using the cmake build system. This allows quick configuration and builds using the `cmake` tool.

    $> mkdir cmake-build-debug
    $> cd cmake-build-debug
    $> cmake -DCMAKE_BUILD_TYPE=Debug ..
    $> make -j 4

### Other build options

A Code::Blocks project is also provided for those using that popular IDE.

*frameInspector* should be able to build on Windows and Visual Studio projects are provided. Of course the wxWidgets library must be installed and the paths adjusted in the Visual Studio project files.

## Dependencies

frameInspector requires the following packages to be installed in order to build and run.

| Library   | Description   | Installation |
| -- | -- | -- |
| wxWidgets 3.0 | GUI Toolkit | sudo apt-get install libwxgtk3.0-gtk3-dev |
| libgcrypt | Used for hash functions | sudo apt-get install libgcrypt11-dev |
| libswscale4 | Part of ffmpeg used for YUV conversion | sudo apt-get install libswscale-dev |
| libmjpegtools | Used to save/load yuv4mpeg format | sudo-apt get install libmjpegtools-dev | 
 

## Screenshots
![Screenshot #1](media/frameInspector%20V2.32%20Build%20Nov%2019%202018_002.png)

