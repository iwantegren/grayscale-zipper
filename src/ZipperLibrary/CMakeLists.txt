set(LIB_NAME "ZipperLibrary")

add_library(${LIB_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/Zipper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Zipper.cpp"
)

target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(${LIB_NAME} PRIVATE LIB_VERSION=1.1)