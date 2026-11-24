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

# Core algorithmic modules library (Group 7 Core Modules)
# Member 2 (Marsimoy Tesfaye): FrequencyAnalyzer
add_library(huffman_core_lib STATIC
    src/core/FrequencyAnalyzer.cpp
)

target_include_directories(huffman_core_lib PUBLIC
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
    huffman_core_lib
)

# ==============================================================================
# Testing Targets (Group 7 DAA Verification)
# ==============================================================================
enable_testing()

# Automated test executable for FrequencyAnalyzer (Member 2 Deliverable)
add_executable(test_frequency_analyzer
    tests/test_frequency_analyzer.cpp
)

target_link_libraries(test_frequency_analyzer PRIVATE
    huffman_core_lib
)

add_test(NAME FrequencyAnalyzerTest COMMAND test_frequency_analyzer)
