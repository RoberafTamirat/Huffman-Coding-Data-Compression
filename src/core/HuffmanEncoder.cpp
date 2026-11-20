#include "core/HuffmanEncoder.h"

/**
 * @file HuffmanEncoder.cpp
 * @author Member 4: Mahlet Eyasu
 */

std::string HuffmanEncoder::encode(const std::string& text,
                                   const std::unordered_map<unsigned char, std::string>& codes) {
    if (text.empty() || codes.empty()) {
        return "";
    }

    std::string encodedBits;
    encodedBits.reserve(text.size() * 4);

    for (unsigned char ch : text) {
        auto it = codes.find(ch);
        if (it != codes.end()) {
            encodedBits += it->second;
        }
    }

    return encodedBits;
}

std::vector<uint8_t> HuffmanEncoder::packBits(const std::string& bitString, uint8_t& validBitsInLastByte) {
    if (bitString.empty()) {
        validBitsInLastByte = 0;
        return {};
    }

    std::vector<uint8_t> packed;
    packed.reserve((bitString.size() + 7) / 8);

    uint8_t currentByte = 0;
    int bitCount = 0;

    for (char bit : bitString) {
        currentByte = static_cast<uint8_t>((currentByte << 1) | (bit == '1' ? 1 : 0));
        ++bitCount;

        if (bitCount == 8) {
            packed.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }

    if (bitCount > 0) {
        currentByte = static_cast<uint8_t>(currentByte << (8 - bitCount));
        packed.push_back(currentByte);
        validBitsInLastByte = static_cast<uint8_t>(bitCount);
    } else {
        validBitsInLastByte = 8;
    }

    return packed;
}

std::string HuffmanEncoder::unpackBits(const std::vector<uint8_t>& bytes, uint8_t validBitsInLastByte) {
    if (bytes.empty()) {
        return "";
    }

    std::string bitString;
    bitString.reserve(bytes.size() * 8);

    for (size_t i = 0; i < bytes.size(); ++i) {
        uint8_t b = bytes[i];
        int bitsToRead = (i == bytes.size() - 1) ? validBitsInLastByte : 8;
        for (int bit = 7; bit >= 8 - bitsToRead; --bit) {
            bitString.push_back(((b >> bit) & 1) ? '1' : '0');
        }
    }

    return bitString;
}
