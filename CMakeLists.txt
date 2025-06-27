cmake_minimum_required(VERSION 3.13)

project(mu-unpackers VERSION 0.0.0 LANGUAGES CXX)

# for debug
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

# External dependencies
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")
find_package(ZLIB REQUIRED)
if(NOT TARGET nlohmann_json::nlohmann_json)
  find_package(nlohmann_json REQUIRED)
endif()

# Custom dependencies
# If the data_products target is already defined (e.g. by CPM), just use it
if(NOT TARGET DataProducts::data_products AND NOT TARGET data_products)
  find_package(DataProducts REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES "arm64")

# Create installation directories
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path prefix" FORCE)
endif()

# Set the path for where to look for the libraries of the installed target
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Add all the subdirectories
add_subdirectory(midasio)
add_subdirectory(src)

# Create the nalu executable
add_executable(unpack_nalu src/main_nalu.cc)

# Link libraries
target_link_libraries(unpack_nalu
    PUBLIC 
        unpackers 
)

# Create the wfd5 executable
add_executable(unpack_wfd5 src/main_wfd5.cc)

# Link libraries
target_link_libraries(unpack_wfd5
    PUBLIC 
        unpackers 
)

# Install the libraries
install(TARGETS 
        unpackers    
        LIBRARY DESTINATION lib
)

# Install the executables
install(TARGETS unpack_nalu
        DESTINATION bin
)

# Install the executables
install(TARGETS unpack_wfd5
        DESTINATION bin
)

# # Enable debugging symbols
# target_compile_options(unpacker PRIVATE -g -v)

# target_compile_options(unpacker PRIVATE -Wall -Wextra -Wno-undef)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
