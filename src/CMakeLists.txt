# Glob source files
file(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wfd5/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/fc7/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/nalu/*.cc
)

# Set unpacker sources
set(UNPACKER_SOURCES
    ${SOURCE_FILES}
)

# Make an object file
add_library(unpackers SHARED
  ${MIDASIO_SOURCES}  
  ${UNPACKER_SOURCES}
)

# Make an alias for the unpackers library
add_library(Unpackers::unpackers ALIAS unpackers)

# Include directories
target_include_directories(unpackers
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/midasio>
        $<INSTALL_INTERFACE:include>
        ${ROOT_INCLUDE_DIRS}
)

# Libraries
target_link_libraries(unpackers
    PUBLIC
        nlohmann_json::nlohmann_json  
        ${ZLIB_LIBRARIES}  
        ${ROOT_LIBRARIES}
        DataProducts::data_products
)

#These are the unpackers for which we make root dictionary
set(Unpackers 
    ${PROJECT_SOURCE_DIR}/include/unpackers/common/*.hh
    ${PROJECT_SOURCE_DIR}/include/unpackers/wfd5/*.hh
    ${PROJECT_SOURCE_DIR}/fc7/*.hh
    ${PROJECT_SOURCE_DIR}/include/unpackers/nalu/*.hh
)

# Make the ROOT dictionary
root_generate_dictionary(UnpackerDictionary
    ${Unpackers} 
    MODULE unpackers
    LINKDEF ${PROJECT_SOURCE_DIR}/include/unpackers/LinkDef.h
)

# Install the ROOT dictionary
install(FILES
            ${PROJECT_BINARY_DIR}/src/libunpackers_rdict.pcm
            ${PROJECT_BINARY_DIR}/src/libunpackers.rootmap
        DESTINATION lib
)

# Install the library
install(TARGETS unpackers
        EXPORT UnpackersTargets
        LIBRARY DESTINATION lib
)

# Install the headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include
)

# Install the midasio headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/midasio/
        DESTINATION include
        FILES_MATCHING PATTERN "*h*"
)

# Export
install(EXPORT UnpackersTargets
  FILE UnpackersTargets.cmake
  NAMESPACE Unpackers::
  DESTINATION lib/cmake/Unpackers
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/UnpackersConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/UnpackersConfig.cmake"
  INSTALL_DESTINATION lib/cmake/Unpackers
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/UnpackersConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/UnpackersConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/UnpackersConfigVersion.cmake"
  DESTINATION lib/cmake/Unpackers
)