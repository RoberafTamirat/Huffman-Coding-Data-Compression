#pragma once

#include "core/HuffmanTree.h"
#include <string>
#include <unordered_map>
#include <cstddef>

/**
 * @brief Handles text analysis, frequency calculation, tree generation, and text encoding.
 */
class HuffmanEncoder {
public:
    HuffmanEncoder();
    ~HuffmanEncoder() = default;

    /**
     * @brief Computes frequency count for each distinct character in the text.
     * @param text Source input text.
     * @return Map of character to its frequency count.
     */
    [[nodiscard]] std::unordered_map<char, size_t> buildFrequencyTable(const std::string& text);

    /**
     * @brief Encodes the provided text into a bit string sequence (e.g. "0110...").
     * @param text Source input text.
     * @return Encoded bit representation as a string of '0's and '1's.
     */
    [[nodiscard]] std::string encode(const std::string& text);

    /**
     * @brief Returns the generated prefix codes mapping.
     */
    [[nodiscard]] const std::unordered_map<char, std::string>& getCodes() const noexcept;

    /**
     * @brief Returns the computed frequency table.
     */
    [[nodiscard]] const std::unordered_map<char, size_t>& getFrequencies() const noexcept;

    /**
     * @brief Returns a reference to the constructed Huffman tree.
     */
    [[nodiscard]] const HuffmanTree& getTree() const noexcept;

    /**
     * @brief Computes compression ratio: (compressed size / original size) * 100.
     * @param originalBytes Number of bytes in original uncompressed text.
     * @param compressedBytes Number of bytes in compressed output.
     * @return Compression ratio percentage.
     */
    [[nodiscard]] double calculateCompressionRatio(size_t originalBytes, size_t compressedBytes) const noexcept;

private:
    HuffmanTree tree_;
    std::unordered_map<char, size_t> frequencies_;
    std::unordered_map<char, std::string> codes_;
};
