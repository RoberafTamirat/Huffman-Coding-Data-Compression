#pragma once

#include "core/HuffmanTree.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cstddef>

/**
 * @file HuffmanEncoder.h
 * @brief Handles prefix code generation, text encoding, and bit-packing into bytes.
 * @author Member 4: Mahlet Eyasu (Encoding Algorithm Developer)
 *
 * Tasks:
 *  1. Receive HuffmanTree and generate codes mapping.
 *  2. Encode text into binary bit sequence ('0's and '1's).
 *  3. Pack encoded bits into bytes (uint8_t) for actual storage.
 *  4. Track number of valid bits in the final byte.
 *  5. Handle single unique character input.
 */
class HuffmanEncoder {
public:
    HuffmanEncoder() = default;
    ~HuffmanEncoder() = default;

    /**
     * @brief Encodes source text using the provided codes mapping.
     */
    [[nodiscard]] std::string encode(const std::string& text,
                                     const std::unordered_map<unsigned char, std::string>& codes);

    /**
     * @brief Packs a string of '0's and '1's into a vector of bytes.
     * @param bitString String containing only '0' and '1'.
     * @param validBitsInLastByte Output parameter recording valid bit count (1-8) in the final byte.
     * @return Vector of packed bytes.
     */
    [[nodiscard]] std::vector<uint8_t> packBits(const std::string& bitString, uint8_t& validBitsInLastByte);

    /**
     * @brief Unpacks a byte buffer back into a bit string sequence.
     */
    [[nodiscard]] std::string unpackBits(const std::vector<uint8_t>& bytes, uint8_t validBitsInLastByte);
};
