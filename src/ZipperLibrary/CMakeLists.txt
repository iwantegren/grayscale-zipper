# set(LIB_NAME "ZipperLibrary")

# add_library(${LIB_NAME}
#     "${CMAKE_CURRENT_SOURCE_DIR}/Encoder.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/Encoder.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/Decoder.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/Decoder.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/ImageHandler.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/ImageHandler.cpp"
# )

# target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_compile_definitions(${LIB_NAME} PRIVATE LIB_VERSION=1.1)

cmake_minimum_required(VERSION 3.16.3)

project(grayscalezipper VERSION 1.0.0)

set(SOURCES
    Decoder.cpp
    Encoder.cpp
    ImageHandler.cpp
)

set(HEADERS
    Decoder.h
    Encoder.h
    ImageHandler.h
    Misc.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(FILES ${HEADERS} DESTINATION include)