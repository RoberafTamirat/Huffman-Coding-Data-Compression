#include "file/FileHandler.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string FileHandler::readTextFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileHandler::writeTextFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file << content;
    return file.good();
}

bool FileHandler::writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    if (!data.empty()) {
        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    }
    return file.good();
}

std::vector<uint8_t> FileHandler::readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return {};
    }

    auto fileSize = file.tellg();
    if (fileSize <= 0) {
        return {};
    }

    std::vector<uint8_t> buffer(static_cast<size_t>(fileSize));
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    return buffer;
}

std::size_t FileHandler::getFileSize(const std::string& filePath) {
    std::error_code ec;
    auto size = std::filesystem::file_size(filePath, ec);
    if (ec) {
        return 0;
    }
    return static_cast<std::size_t>(size);
}

bool FileHandler::fileExists(const std::string& filePath) const {
    std::error_code ec;
    return std::filesystem::exists(filePath, ec);
}
