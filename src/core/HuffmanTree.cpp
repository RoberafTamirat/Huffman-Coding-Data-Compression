#include "core/HuffmanTree.h"
#include <queue>
#include <vector>

HuffmanTree::HuffmanTree() : root_(nullptr) {}

void HuffmanTree::buildTree(const std::unordered_map<char, size_t>& frequencies) {
    if (frequencies.empty()) {
        root_ = nullptr;
        return;
    }

    // Min-heap priority queue to store nodes ordered by frequency
    std::priority_queue<std::shared_ptr<HuffmanNode>,
                        std::vector<std::shared_ptr<HuffmanNode>>,
                        HuffmanNodeComparator> minHeap;

    for (const auto& [character, freq] : frequencies) {
        minHeap.push(std::make_shared<HuffmanNode>(character, freq));
    }

    // Build the tree by combining two lowest-frequency nodes repeatedly
    while (minHeap.size() > 1) {
        auto left = minHeap.top();
        minHeap.pop();

        auto right = minHeap.top();
        minHeap.pop();

        size_t combinedFreq = left->frequency + right->frequency;
        auto parent = std::make_shared<HuffmanNode>('\0', combinedFreq, left, right);
        minHeap.push(parent);
    }

    root_ = minHeap.empty() ? nullptr : minHeap.top();
}

std::unordered_map<char, std::string> HuffmanTree::generateCodes() const {
    std::unordered_map<char, std::string> codes;
    if (!root_) {
        return codes;
    }

    // Special case: Single distinct character in input
    if (root_->isLeaf()) {
        codes[root_->data] = "0";
        return codes;
    }

    generateCodesHelper(root_, "", codes);
    return codes;
}

void HuffmanTree::generateCodesHelper(const std::shared_ptr<HuffmanNode>& node,
                                      const std::string& currentCode,
                                      std::unordered_map<char, std::string>& codes) const {
    if (!node) {
        return;
    }

    if (node->isLeaf()) {
        codes[node->data] = currentCode;
        return;
    }

    generateCodesHelper(node->left, currentCode + "0", codes);
    generateCodesHelper(node->right, currentCode + "1", codes);
}

std::shared_ptr<HuffmanNode> HuffmanTree::getRoot() const noexcept {
    return root_;
}

void HuffmanTree::setRoot(std::shared_ptr<HuffmanNode> root) noexcept {
    root_ = std::move(root);
}
