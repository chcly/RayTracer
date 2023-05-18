# CMakeModules

This module provides a shared space to include CMake macros in other projects.

It should be included somewhere in the source tree as a sub-module and the CMake module path should be modified to find the subdirectory.

```CMake

list(APPEND CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/Path/To/CMakeModules)

```