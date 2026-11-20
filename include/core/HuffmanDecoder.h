#pragma once

#include "core/HuffmanTree.h"
#include <string>
#include <memory>

/**
 * @brief Handles decoding of encoded bit strings back into original text using the Huffman tree.
 */
class HuffmanDecoder {
public:
    HuffmanDecoder() = default;
    ~HuffmanDecoder() = default;

    /**
     * @brief Decodes an encoded bit string using a HuffmanTree instance.
     * @param encodedBits String of '0's and '1's representing compressed data.
     * @param tree Huffman tree containing prefix codes.
     * @return Decoded original text string.
     */
    [[nodiscard]] std::string decode(const std::string& encodedBits, const HuffmanTree& tree);

    /**
     * @brief Decodes an encoded bit string starting from a tree root node.
     * @param encodedBits String of '0's and '1's representing compressed data.
     * @param root Root node of the Huffman tree.
     * @return Decoded original text string.
     */
    [[nodiscard]] std::string decode(const std::string& encodedBits, const std::shared_ptr<HuffmanNode>& root);
};
