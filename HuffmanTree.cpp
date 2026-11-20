#include "HuffmanTree.h"

#include <queue>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <limits>
#include <memory>

using namespace std
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) const {
        return a->frequency > b->frequency;
    }
};

HuffmanTree::HuffmanTree() {
    root = nullptr;
}
void HuffmanTree::buildTree(
    const vector<pair<char, int>>& frequencies
) {
    nodes.clear();
    root = nullptr;
    unordered_set<char> characters;

    for (const auto& item : frequencies) {

        if (item.second <= 0) {
            throw invalid_argument(
                "Frequency must be greater than zero."
            );
        }

        if (!characters.insert(item.first).second) {
            throw invalid_argument(
                "Duplicate characters are not allowed."
            );
        }
    }
    if (frequencies.empty()) {
        return;
    }

    priority_queue<
        HuffmanNode*,
        vector<HuffmanNode*>,
        Compare
    > minHeap;
    nodes.reserve(2 * frequencies.size() - 1);

    for (const auto& item : frequencies) {

        nodes.push_back(
            make_unique<HuffmanNode>(
                item.first,
                item.second
            )
        );

        minHeap.push(nodes.back().get());
    }

    while (minHeap.size() > 1) {

        
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();
        if (left->frequency >
            numeric_limits<int>::max() - right->frequency) {

            throw overflow_error(
                "Combined frequency exceeds integer limit."
            );
        }
        int combinedFrequency =
            left->frequency + right->frequency;
        nodes.push_back(
            make_unique<HuffmanNode>(
                '\0',
                combinedFrequency
            )
        );

        HuffmanNode* parent = nodes.back().get();
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }
    root = minHeap.top();
    minHeap.pop();
}
HuffmanNode* HuffmanTree::getRoot() const {
    return root;
}


bool HuffmanTree::isEmpty() const {
    return root == nullptr;
}