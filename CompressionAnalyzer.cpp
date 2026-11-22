#include "CompressionAnalyzer.h"

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iomanip>

using namespace std;

struct Node {
    unsigned char character;
    uint64_t frequency;
    Node* left;
    Node* right;

    Node(unsigned char ch, uint64_t freq)
        : character(ch), frequency(freq),
          left(nullptr), right(nullptr) {}

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) const {
        return a->frequency > b->frequency;
    }
};

void calculateCodeLengths(
    Node* root,
    int depth,
    unordered_map<unsigned char, int>& lengths
) {
    if (root == nullptr) {
        return;
    }

    if (root->isLeaf()) {
        lengths[root->character] = (depth == 0) ? 1 : depth;
        return;
    }

    calculateCodeLengths(root->left, depth + 1, lengths);
    calculateCodeLengths(root->right, depth + 1, lengths);
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

CompressionStats CompressionAnalyzer::analyze(
    const string& text
) const {
    CompressionStats stats{};

    stats.originalBytes = static_cast<uint64_t>(text.size());
    stats.originalBits = stats.originalBytes * 8;

    if (text.empty()) {
        return stats;
    }

    unordered_map<unsigned char, uint64_t> frequency;
    for (unsigned char ch : text) {
        frequency[ch]++;
    }

    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (const auto& item : frequency) {
        minHeap.push(new Node(item.first, item.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();

        Node* right = minHeap.top();
        minHeap.pop();

        Node* parent = new Node(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }

    Node* root = minHeap.top();
    minHeap.pop();

    unordered_map<unsigned char, int> lengths;
    calculateCodeLengths(root, 0, lengths);

    for (const auto& item : frequency) {
        stats.compressedPayloadBits +=
            item.second * static_cast<uint64_t>(lengths[item.first]);
    }

    stats.compressedPayloadBytes =
        (stats.compressedPayloadBits + 7) / 8;

    // 256 frequency-table entries, estimated as 4 bytes per entry.
    // This is an estimate; use the real header format for file-size results.
    stats.headerBytes = 256 * 4;

    stats.totalCompressedBytes =
        stats.compressedPayloadBytes + stats.headerBytes;

    if (stats.totalCompressedBytes > 0) {
        stats.compressionRatio =
            static_cast<double>(stats.originalBytes) /
            static_cast<double>(stats.totalCompressedBytes);
    }

    if (stats.originalBytes > 0) {
        stats.spaceSavedPercentage =
            (1.0 -
             static_cast<double>(stats.totalCompressedBytes) /
             static_cast<double>(stats.originalBytes)) * 100.0;
    }

    deleteTree(root);
    return stats;
}

void CompressionAnalyzer::displayResults(
    const CompressionStats& stats
) const {
    cout << "\n===== COMPRESSION ANALYSIS =====\n";

    cout << "Original size: "
         << stats.originalBytes << " bytes\n";

    cout << "Original bits: "
         << stats.originalBits << " bits\n";

    cout << "Compressed payload: "
         << stats.compressedPayloadBits << " bits\n";

    cout << "Compressed payload bytes: "
         << stats.compressedPayloadBytes << " bytes\n";

    cout << "Estimated header: "
         << stats.headerBytes << " bytes\n";

    cout << "Estimated total compressed size: "
         << stats.totalCompressedBytes << " bytes\n";

    cout << fixed << setprecision(2);

    cout << "Payload compression ratio: "
         << stats.compressionRatio << ":1\n";

    cout << "Estimated space saved: "
         << stats.spaceSavedPercentage << "%\n";

    cout << "================================\n";
}

