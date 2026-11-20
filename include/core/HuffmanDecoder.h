#pragma once

#include "core/HuffmanTree.h"
#include <string>
#include <memory>

/**
 * @file HuffmanDecoder.h
 * @brief Handles decoding of bitstreams back into original text using the Huffman tree.
 * @author Member 5: Hermela Tadios (Decoding and File Management Developer)
 *
 * Tasks:
 *  1. Receive compressed bitstream.
 *  2. Traverse HuffmanTree according to each bit ('0' left, '1' right).
 *  3. Recover original characters.
 *  4. Handle single-character special case.
 *  5. Ensure recovered text matches original input.
 */
class HuffmanDecoder {
public:
    HuffmanDecoder() = default;
    ~HuffmanDecoder() = default;

    /**
     * @brief Decodes an encoded bit string using a HuffmanTree instance.
     */
    [[nodiscard]] std::string decode(const std::string& encodedBits, const HuffmanTree& tree);

    /**
     * @brief Decodes an encoded bit string starting from a tree root node.
     */
    [[nodiscard]] std::string decode(const std::string& encodedBits, const std::shared_ptr<HuffmanNode>& root);
};
