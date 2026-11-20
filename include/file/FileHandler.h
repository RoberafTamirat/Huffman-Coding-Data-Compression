#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

/**
 * @brief Handles file I/O operations for reading plain text and reading/writing binary data.
 */
class FileHandler {
public:
    FileHandler() = default;
    ~FileHandler() = default;

    /**
     * @brief Reads entire plain text content from a file.
     * @param filePath Path to the input file.
     * @return String content of the file.
     */
    [[nodiscard]] std::string readTextFile(const std::string& filePath);

    /**
     * @brief Writes string content to a plain text file.
     * @param filePath Path to the output file.
     * @param content String data to write.
     * @return True if operation succeeded, false otherwise.
     */
    bool writeTextFile(const std::string& filePath, const std::string& content);

    /**
     * @brief Writes raw bytes to a binary file.
     * @param filePath Path to the destination file.
     * @param data Byte buffer to write.
     * @return True if operation succeeded, false otherwise.
     */
    bool writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);

    /**
     * @brief Reads raw bytes from a binary file.
     * @param filePath Path to the source file.
     * @return Vector containing all read bytes.
     */
    [[nodiscard]] std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    /**
     * @brief Retrieves size of a file in bytes.
     * @param filePath Target file path.
     * @return Byte count of the file or 0 if inaccessible.
     */
    [[nodiscard]] std::size_t getFileSize(const std::string& filePath);

    /**
     * @brief Checks if a given file path exists.
     * @param filePath Target file path.
     * @return True if the file exists, false otherwise.
     */
    [[nodiscard]] bool fileExists(const std::string& filePath) const;
};
