#include "core/HuffmanDecoder.h"

std::string HuffmanDecoder::decode(const std::string& encodedBits, const HuffmanTree& tree) {
    return decode(encodedBits, tree.getRoot());
}

std::string HuffmanDecoder::decode(const std::string& encodedBits, const std::shared_ptr<HuffmanNode>& root) {
    if (!root || encodedBits.empty()) {
        return "";
    }

    std::string decodedText;
    auto currentNode = root;

    // Special case: Tree has only one distinct character (single leaf node)
    if (currentNode->isLeaf()) {
        decodedText.append(encodedBits.length(), currentNode->data);
        return decodedText;
    }

    for (char bit : encodedBits) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else if (bit == '1') {
            currentNode = currentNode->right;
        }

        if (!currentNode) {
            // Malformed bit sequence or unexpected traversal path
            break;
        }

        if (currentNode->isLeaf()) {
            decodedText += currentNode->data;
            currentNode = root;
        }
    }

    return decodedText;
}
