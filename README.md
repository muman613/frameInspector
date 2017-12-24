# frameInspector YUV Stream Viewer

The Versatile YUV file displayer

## Overview
frameInspector is a tool which reads a raw YUV file (in a variety of formats) and displays it in a window.

* Reads 'YUV Split' files with seperate Y/U/V files.
* Reads 'YUV Composite' files containing multiple images.
* Supports many image resolutions from QCIF -> UHD.
* Supports YUV420p and YUV420p10 (big & little endian)

## Building project
*frameInspector* utilizes the standard 'autoconf' configuration system. Once the sourcecode is downloaded from the repsitory the project can be configured with the `'./configure'` command.

A Code::Blocks project is also provided for those using that popular IDE.

*frameInspector* should be able to build on Windows and Visual Studio projects are provided.

### Dependancies

* wxWidgets 3.0
* libgcrypt11-dev

