# Available command-line arguments:
#  -DCMAKE_C_COMPILER   - path to your c compiler
#  -DCMAKE_CXX_COMPILER - path to your c++ compieler
#  -DCMAKE_CXX_STDLIB   - path to your c++ standard library
#  -DCMAKE_BUILD_TYPE   - Debug, RelWithDebInfo or Release

project(cube)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

# Aliases
set(SRCDIR ${CMAKE_SOURCE_DIR}) 
set(BINDIR ${CMAKE_BINARY_DIR})

#
# @note1
# If compiler is clang++-6.0 on Apple installed using brew
#  we have to make sure that the 
#             /usr/local/Cellar/llvm/6.0.0/lib 
#  standard library is used, together with the
#             /usr/local/Cellar/llvm/6.0.0/bin/clang++ compiler
# 
# @note2
#  for link_directories() to work it 
#       has to happen before add_executable() - JSolsvik
# 
link_directories(${CMAKE_CXX_STDLIB})


# Configure main executeable
add_executable(cube "${SRCDIR}/main.cpp")


set_target_properties(
    cube
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BINDIR}/lib"
)


#
# if whaterver compiler - https://stackoverflow.com/a/10055571/9636402 - 2018-05-19
# 
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(
        cube
        PRIVATE c++experimental
    )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(
        cube
        PRIVATE stdc++fs
    )
endif()


#
# CONFIGURE DIRECTORIES (configure_file())
#
function(configure_directory src dest)

    file(GLOB files ${src}/*)
    foreach(absolutepath ${files})
        get_filename_component(filename ${absolutepath} NAME)
        message("Conf file: " ${src}/${filename} " -> " ${dest}/${filename})
        configure_file(${src}/${filename} ${dest}/${filename} COPYONLY)
    endforeach(absolutepath)

endfunction()

configure_directory(${SRCDIR}/assets/scenes ${BINDIR}/assets/scenes)
configure_directory(${SRCDIR}/assets/materials ${BINDIR}/assets/materials)
configure_directory(${SRCDIR}/assets/shaders ${BINDIR}/assets/shaders)
configure_directory(${SRCDIR}/assets/models ${BINDIR}/assets/models)
configure_directory(${SRCDIR}/assets/textures ${BINDIR}/assets/textures)

