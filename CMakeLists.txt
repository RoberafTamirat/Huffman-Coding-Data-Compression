cmake_minimum_required(VERSION 3.15)
project(HuffmanCoding VERSION 1.0.0 LANGUAGES CXX)

# Standard C++17 configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings for code quality
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Core GUI library owned by Member 1 (Roberaf Tamirat)
add_library(huffman_gui_lib STATIC
    src/gui/HuffmanGui.cpp
)

target_include_directories(huffman_gui_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Windows system libraries for native Win32 GUI & Common Controls
if (WIN32)
    target_link_libraries(huffman_gui_lib PUBLIC
        user32
        gdi32
        comctl32
        comdlg32
    )
endif()

# Main executable (defaults to GUI, accepts CLI flags)
add_executable(HuffmanCoding
    src/main.cpp
)

target_link_libraries(HuffmanCoding PRIVATE
    huffman_gui_lib
)
