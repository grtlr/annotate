cmake_minimum_required (VERSION 2.8.11)

project(annotate)

find_package(OpenCV 3.0.0 REQUIRED core imgproc ximgproc)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

qt5_wrap_cpp(
    MOC_OUTFILES
    include/window.h
    include/imagewidget.h
    include/image_label.h
    include/partitioning.h
    include/loadfiles.h
)

add_executable(
    annotate
    ${MOC_OUTFILES}
    main.cpp
)

target_link_libraries(
    annotate
    Qt5::Widgets
    ${OpenCV_LIBS}
)
