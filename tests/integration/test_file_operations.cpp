#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <filesystem>
#include "file/FileHandler.h"

void testTextFileReadWrite() {
    std::cout << "[Test] FileHandler Text Read/Write... ";
    FileHandler handler;
    std::string testPath = "test_temp_output.txt";
    std::string expectedContent = "Huffman Data Compression File I/O Verification\nLine 2";

    bool writeSuccess = handler.writeTextFile(testPath, expectedContent);
    assert(writeSuccess);
    assert(handler.fileExists(testPath));

    std::string readContent = handler.readTextFile(testPath);
    assert(readContent == expectedContent);

    // Clean up temporary file
    std::error_code ec;
    std::filesystem::remove(testPath, ec);

    std::cout << "PASSED\n";
}

void testBinaryFileReadWrite() {
    std::cout << "[Test] FileHandler Binary Read/Write... ";
    FileHandler handler;
    std::string testPath = "test_temp_binary.bin";
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0xFF, 0x42};

    bool writeSuccess = handler.writeBinaryFile(testPath, data);
    assert(writeSuccess);
    assert(handler.fileExists(testPath));
    assert(handler.getFileSize(testPath) == data.size());

    std::vector<uint8_t> readData = handler.readBinaryFile(testPath);
    assert(readData.size() == data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        assert(readData[i] == data[i]);
    }

    // Clean up temporary file
    std::error_code ec;
    std::filesystem::remove(testPath, ec);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Running Integration Tests: File Operations\n";
    std::cout << "========================================\n";

    try {
        testTextFileReadWrite();
        testBinaryFileReadWrite();
        std::cout << "All integration tests completed successfully.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Integration test failure: " << ex.what() << "\n";
        return 1;
    }
}
