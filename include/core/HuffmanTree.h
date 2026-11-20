#pragma once

#include "core/HuffmanNode.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <cstddef>

/**
 * @file HuffmanTree.h
 * @brief Manages Huffman tree construction, traversal, and prefix code generation.
 * @author Member 3: Feven Asefa (Core Algorithm Developer - Huffman Tree & Priority Queue)
 *
 * Tasks:
 *  - Implement Min Priority Queue using std::priority_queue.
 *  - Extract two lowest-frequency nodes, combine into parent, push back.
 *  - Handle empty input and single-character input.
 */
class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree() = default;

    HuffmanTree(const HuffmanTree&) = default;
    HuffmanTree& operator=(const HuffmanTree&) = default;
    HuffmanTree(HuffmanTree&&) noexcept = default;
    HuffmanTree& operator=(HuffmanTree&&) noexcept = default;

    /**
     * @brief Constructs the Huffman tree from character frequency mappings.
     * @param frequencies Map containing unsigned char to frequency count.
     */
    void buildTree(const std::unordered_map<unsigned char, size_t>& frequencies);

    /**
     * @brief Traverses the tree to generate prefix binary codes for each leaf.
     * @return Map linking characters to their corresponding bit string representations.
     */
    [[nodiscard]] std::unordered_map<unsigned char, std::string> generateCodes() const;

    /**
     * @brief Returns the root of the tree.
     */
    [[nodiscard]] std::shared_ptr<HuffmanNode> getRoot() const noexcept;

    /**
     * @brief Sets or restores the root node.
     */
    void setRoot(std::shared_ptr<HuffmanNode> root) noexcept;

private:
    std::shared_ptr<HuffmanNode> root_;

    void generateCodesHelper(const std::shared_ptr<HuffmanNode>& node,
                             const std::string& currentCode,
                             std::unordered_map<unsigned char, std::string>& codes) const;
};
