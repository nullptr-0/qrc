cmake_minimum_required(VERSION 3.10)
project(qrc)

set(CMAKE_CXX_STANDARD 17)

# zlib
add_subdirectory(lib/zlib)
include_directories(lib/zlib "${CMAKE_BINARY_DIR}/lib/zlib")
link_directories("${CMAKE_BINARY_DIR}/lib/zlib/${CMAKE_BUILD_TYPE}")
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ZLIB_FNAME "zlibstatic${CMAKE_DEBUG_POSTFIX}")
else()
    set(ZLIB_FNAME "zlibstatic")
endif()
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "Disable Zlib examples")

add_executable(qrcdec qrcdec.cpp des.cpp)
target_link_libraries(qrcdec ${ZLIB_FNAME})

add_executable(qrc2lrc qrc2lrc.cpp)

add_custom_target(qrc ALL DEPENDS zlibstatic qrcdec qrc2lrc)