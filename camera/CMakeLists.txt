cmake_minimum_required(VERSION 3.16)
project(CameraDetector)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" ${CMAKE_PREFIX_PATH})

find_package(OpenCV REQUIRED)

add_executable(CameraDetector
    main.cpp
    cv.cpp
)

target_include_directories(CameraDetector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(CameraDetector PRIVATE
    ${OpenCV_LIBS}
)
