#include "core/HuffmanEncoder.h"

HuffmanEncoder::HuffmanEncoder() : tree_() {}

std::unordered_map<char, size_t> HuffmanEncoder::buildFrequencyTable(const std::string& text) {
    frequencies_.clear();
    for (char ch : text) {
        ++frequencies_[ch];
    }
    return frequencies_;
}

std::string HuffmanEncoder::encode(const std::string& text) {
    if (text.empty()) {
        return "";
    }

    buildFrequencyTable(text);
    tree_.buildTree(frequencies_);
    codes_ = tree_.generateCodes();

    std::string encodedBits;
    encodedBits.reserve(text.size() * 4);

    for (char ch : text) {
        auto it = codes_.find(ch);
        if (it != codes_.end()) {
            encodedBits += it->second;
        }
    }

    return encodedBits;
}

const std::unordered_map<char, std::string>& HuffmanEncoder::getCodes() const noexcept {
    return codes_;
}

const std::unordered_map<char, size_t>& HuffmanEncoder::getFrequencies() const noexcept {
    return frequencies_;
}

const HuffmanTree& HuffmanEncoder::getTree() const noexcept {
    return tree_;
}

double HuffmanEncoder::calculateCompressionRatio(size_t originalBytes, size_t compressedBytes) const noexcept {
    if (originalBytes == 0) {
        return 0.0;
    }
    return (static_cast<double>(compressedBytes) / static_cast<double>(originalBytes)) * 100.0;
}
