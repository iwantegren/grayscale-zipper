cmake_minimum_required(VERSION 3.16.3)

project(grayscale-zipper)

find_library(LIBZIPPER libgrayscalezipper /out/lib/libgrayscalezipper)

include_directories(src/ZipperLibrary/include)

add_executable(${PROJECT_NAME} /src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBZIPPER})
