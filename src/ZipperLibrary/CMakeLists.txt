set(LIB_NAME "ZipperLibrary")

add_library(${LIB_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/Encoder.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Encoder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImageHandler.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImageHandler.cpp"
)

target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(${LIB_NAME} PRIVATE LIB_VERSION=1.1)