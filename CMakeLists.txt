cmake_minimum_required(VERSION 3.27)
project(usb_state_service)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)

add_executable(usb_state_service main.cpp)

target_include_directories(usb_state_service PRIVATE ${UDEV_INCLUDE_DIRS})
target_link_libraries(usb_state_service PRIVATE ${UDEV_LIBRARIES})
target_compile_options(usb_state_service PRIVATE ${UDEV_CFLAGS_OTHER})
