cmake_minimum_required(VERSION 3.16.3)

project(grayscale-zipper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

set(PROJECT_SOURCES
        src/main.cpp
        src/FileTableModel.h
        src/FileTableModel.cpp
        src/ZipperWrapper.h
        src/ZipperWrapper.cpp
        src/qml/qml.qrc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(src/ZipperLibrary/include)
link_directories(out/lib)

target_link_libraries(${PROJECT_NAME}
  PRIVATE 
  Qt${QT_VERSION_MAJOR}::Core 
  Qt${QT_VERSION_MAJOR}::Quick
  grayscalezipper)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()