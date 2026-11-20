#pragma once

#include <memory>
#include <cstddef>

/**
 * @file HuffmanNode.h
 * @brief Represents a single node within the Huffman tree.
 * @author Member 3: Feven Asefa (Core Algorithm Developer - Huffman Tree & Priority Queue)
 *
 * Tasks:
 *  1. Store unsigned char character and size_t frequency.
 *  2. Implement left and right child pointers.
 *  3. Provide isLeaf() check.
 */
struct HuffmanNode {
    unsigned char data;
    size_t frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(unsigned char character, size_t freq)
        : data(character), frequency(freq), left(nullptr), right(nullptr) {}

    HuffmanNode(unsigned char character, size_t freq,
                std::shared_ptr<HuffmanNode> leftChild,
                std::shared_ptr<HuffmanNode> rightChild)
        : data(character), frequency(freq), left(std::move(leftChild)), right(std::move(rightChild)) {}

    [[nodiscard]] bool isLeaf() const noexcept {
        return !left && !right;
    }
};

/**
 * @brief Comparator functor for Min-Heap priority queue ordering by frequency.
 */
struct HuffmanNodeComparator {
    bool operator()(const std::shared_ptr<HuffmanNode>& lhs, const std::shared_ptr<HuffmanNode>& rhs) const {
        return lhs->frequency > rhs->frequency;
    }
};
