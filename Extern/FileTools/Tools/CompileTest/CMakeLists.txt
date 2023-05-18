
list(APPEND CMAKE_MODULE_PATH  
    ${FileTools_SOURCE_DIR}/Tools/Compiler/CMake 
    ${FileTools_SOURCE_DIR}/Tools/Decompiler/CMake 
    )
include(TableDecompiler)
include(TableCompiler)

# Build a list of files to convert back to .h files 

file(GLOB SourceBlend *.blend)
file(GLOB SourceBullet *.bullet)

set(DecompileFiles  ${SourceBlend} ${SourceBullet})
decompile_files(ResultFiles "-b" ${DecompileFiles})


# From the generated headers recompile it to a table
# and add the validation target to check the result size 
set(ResultTables )

foreach(File ${ResultFiles})
    add_table_validator(Table ${File})

    get_filename_component(Base ${Table} NAME_WE)
    set(VTarget ${Base}Validator)
    
    set_target_properties(${VTarget} PROPERTIES FOLDER "ValidationTargets")

    list(APPEND ResultTables ${Table})

endforeach()

add_executable(CompileTest Main.cpp ${ResultFiles} ${ResultTables})
set_target_properties(CompileTest PROPERTIES FOLDER "Units/Compile")
