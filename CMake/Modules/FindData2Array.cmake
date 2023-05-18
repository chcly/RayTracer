find_file(Data2Array_EXE NAMES Data2Array  Data2Array.exe  PATHS "$ENV{PATH}")


if (USING_EMSCRIPTEN AND NOT Data2Array_EXE)

    # Work around for: https://github.com/emscripten-core/emscripten/issues/10078

    unset(CMAKE_FIND_ROOT_PATH)
    set(CMAKE_FIND_ROOT_PATH /)

    find_file(Data2Array_EXE NAMES Data2Array  Data2Array.exe  PATHS "$ENV{PATH}")

endif()

if (Data2Array_EXE)
    set(Data2Array_FOUND TRUE)
else()
    set(Data2Array_FOUND FALSE)
endif()