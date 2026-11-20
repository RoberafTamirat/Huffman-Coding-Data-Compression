#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

/**
 * @file FileHandler.h
 * @brief Handles file I/O operations for reading plain text and reading/writing binary data.
 * @author Member 5: Hermela Tadios (Decoding and File Management Developer)
 *
 * Tasks:
 *  - Read text input from ".txt" files.
 *  - Save compressed output to a binary file.
 *  - Store the information required to reconstruct the Huffman Tree.
 *  - Load previously saved compressed files.
 *  - Save recovered text into a new text file.
 */
class FileHandler {
public:
    FileHandler() = default;
    ~FileHandler() = default;

    /**
     * @brief Reads entire plain text content from a file.
     */
    [[nodiscard]] std::string readTextFile(const std::string& filePath);

    /**
     * @brief Writes string content to a plain text file.
     */
    bool writeTextFile(const std::string& filePath, const std::string& content);

    /**
     * @brief Writes raw bytes to a binary file.
     */
    bool writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);

    /**
     * @brief Reads raw bytes from a binary file.
     */
    [[nodiscard]] std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    /**
     * @brief Retrieves size of a file in bytes.
     */
    [[nodiscard]] std::size_t getFileSize(const std::string& filePath);

    /**
     * @brief Checks if a given file path exists.
     */
    [[nodiscard]] bool fileExists(const std::string& filePath) const;
};
