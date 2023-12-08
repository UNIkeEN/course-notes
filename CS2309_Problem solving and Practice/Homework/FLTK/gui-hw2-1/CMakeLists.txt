CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
SET(CMAKE_CXX_STANDARD 11)
SET(my_app_name gui-hw2-1)

PROJECT(${my_app_name})

INCLUDE_DIRECTORIES("../fltk-1.3.8-dist/include")

FILE(GLOB_RECURSE all_source_files "src/*.cpp" "src/*.c")
FILE(GLOB_RECURSE all_head_files "src/*.hpp" "src/*.h")

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    # Windows
    MESSAGE(STATUS "current platform: Windows")
    LINK_DIRECTORIES("../fltk-1.3.8-dist/lib/Windows-release")
    ADD_EXECUTABLE(
            ${my_app_name}
            ${all_head_files}
            ${all_source_files}
    )
    TARGET_LINK_LIBRARIES(
            ${my_app_name}
            PRIVATE
            fltk
            fltk_jpeg
            fltk_images
    )
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    # Linux 
    MESSAGE(STATUS "current platform: Linux ")

    LINK_DIRECTORIES("../fltk-1.3.8-dist/lib/Linux-release")
    ADD_EXECUTABLE(
            ${my_app_name}
            ${all_head_files}
            ${all_source_files}
    )
    TARGET_LINK_LIBRARIES(
            ${my_app_name}
            PRIVATE
            fltk
            x11
            dl
            fltk_jpeg
            fltk_images
    )
ELSE ()
    # 其他操作系统
    MESSAGE(STATUS "unsupported platform: ${CMAKE_SYSTEM_NAME}")
ENDIF ()