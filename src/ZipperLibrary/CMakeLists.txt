cmake_minimum_required(VERSION 3.16.3)

project(grayscalezipper VERSION 1.0.0)

set(SOURCES
    Decoder.cpp
    Encoder.cpp
    ImageHandler.cpp
)

set(HEADERS
    include/Decoder.h
    include/Encoder.h
    include/ImageHandler.h
    include/Misc.h
    include/ZipperException.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION /usr/lib
    ARCHIVE DESTINATION /usr/lib
    RUNTIME DESTINATION /usr/bin
)

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "/usr/lib")
