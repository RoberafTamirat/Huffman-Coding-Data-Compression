#pragma once

#include <memory>
#include <string>
#include <unordered_map>

/**
 * @brief Represents a single node within the Huffman tree.
 */
struct HuffmanNode {
    char data;
    size_t frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char character, size_t freq)
        : data(character), frequency(freq), left(nullptr), right(nullptr) {}

    HuffmanNode(char character, size_t freq, std::shared_ptr<HuffmanNode> leftChild, std::shared_ptr<HuffmanNode> rightChild)
        : data(character), frequency(freq), left(std::move(leftChild)), right(std::move(rightChild)) {}

    [[nodiscard]] bool isLeaf() const noexcept {
        return !left && !right;
    }
};

/**
 * @brief Comparator functor for min-heap priority queue ordering by frequency.
 */
struct HuffmanNodeComparator {
    bool operator()(const std::shared_ptr<HuffmanNode>& lhs, const std::shared_ptr<HuffmanNode>& rhs) const {
        return lhs->frequency > rhs->frequency;
    }
};

/**
 * @brief Manages Huffman tree construction, traversal, and prefix-code generation.
 */
class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree() = default;

    // Disallow accidental expensive copies, allow moves
    HuffmanTree(const HuffmanTree&) = default;
    HuffmanTree& operator=(const HuffmanTree&) = default;
    HuffmanTree(HuffmanTree&&) noexcept = default;
    HuffmanTree& operator=(HuffmanTree&&) noexcept = default;

    /**
     * @brief Constructs the Huffman tree from character frequency mappings.
     * @param frequencies Map containing character counts.
     */
    void buildTree(const std::unordered_map<char, size_t>& frequencies);

    /**
     * @brief Traverses the tree to generate prefix binary codes for each leaf.
     * @return Map linking characters to their corresponding bit string representations.
     */
    [[nodiscard]] std::unordered_map<char, std::string> generateCodes() const;

    /**
     * @brief Returns the root of the tree.
     */
    [[nodiscard]] std::shared_ptr<HuffmanNode> getRoot() const noexcept;

    /**
     * @brief Sets or restores the root node (e.g. when deserializing).
     */
    void setRoot(std::shared_ptr<HuffmanNode> root) noexcept;

private:
    std::shared_ptr<HuffmanNode> root_;

    void generateCodesHelper(const std::shared_ptr<HuffmanNode>& node,
                             const std::string& currentCode,
                             std::unordered_map<char, std::string>& codes) const;
};
