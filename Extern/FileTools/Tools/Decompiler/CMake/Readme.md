# CMake Build Utilities

## TableCompiler

The TableCompiler.cmake file can be used to add a custom build targets.

### macro(ADD_TABLE TARGET ARGN)

```CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/TableCompiler.cmake)
include(TableCompiler)

set(ApiFiles
   StructDecl1.h
   StructDecl2.h
   StructDecl3.h
   StructDecl4.h
)

add_table(OutputVar ${ApiFiles})

set(Source
   Implementation.h
   Implementation.cpp
   ${OutputVar}
   ${ApiFiles}
)
add_library(LibName ${Source})
```

### macro(ADD_TABLE_VALIDATOR TARGET ARGN)

ADD_TABLE_VALIDATOR generates two output files

1. The same tables as above.
2. A validation program that asserts the computed structure sizes. If validation fails a compile error will be generated.

```CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/TableCompiler.cmake)
include(TableCompiler)

set(ApiFiles
   StructDecl1.h
   StructDecl2.h
   StructDecl3.h
   StructDecl4.h
)

add_table_validator(OutputVar ${ApiFiles})

set(Source
   Implementation.h
   Implementation.cpp 
   ${OutputVar}
   ${ApiFiles}
)
add_library(LibName ${Source})
```
