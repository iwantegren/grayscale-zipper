cmake_minimum_required(VERSION 3.16.3)

project(grayscale-zipper)

# main
add_executable(${PROJECT_NAME} src/main.cpp)

# add ZipperLibrary
add_subdirectory(src/ZipperLibrary)
target_link_libraries(${PROJECT_NAME} PRIVATE ZipperLibrary)
