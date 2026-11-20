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

# Core static library shared across CLI, GUI, and test suites
add_library(huffman_lib STATIC
    src/core/HuffmanTree.cpp
    src/core/HuffmanEncoder.cpp
    src/core/HuffmanDecoder.cpp
    src/core/CompressionStats.cpp
    src/file/FileHandler.cpp
    src/gui/HuffmanGui.cpp
)

target_include_directories(huffman_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Windows system libraries for native Win32 GUI & Common Controls
if (WIN32)
    target_link_libraries(huffman_lib PUBLIC
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
    huffman_lib
)

# Test targets
enable_testing()

add_executable(test_huffman
    tests/unit/test_huffman.cpp
)
target_link_libraries(test_huffman PRIVATE
    huffman_lib
)
add_test(NAME UnitTests COMMAND test_huffman)

add_executable(test_file_operations
    tests/integration/test_file_operations.cpp
)
target_link_libraries(test_file_operations PRIVATE
    huffman_lib
)
add_test(NAME IntegrationTests COMMAND test_file_operations)
