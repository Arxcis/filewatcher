project(cube)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

# Set global variables
set(CMAKE_BUILD_TYPE Debug)

# Aliases
set(SRCDIR ${CMAKE_SOURCE_DIR}) 
set(BINDIR ${CMAKE_BINARY_DIR})

# Configure main executeable
add_executable(cube "${SRCDIR}/main.cpp")

set_target_properties(
    cube
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BINDIR}/bin"
)


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

