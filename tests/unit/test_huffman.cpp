#include <iostream>
#include <cassert>
#include <string>
#include "core/HuffmanTree.h"
#include "core/HuffmanEncoder.h"
#include "core/HuffmanDecoder.h"
#include "core/CompressionStats.h"

void testHuffmanTreeConstruction() {
    std::cout << "[Test] HuffmanTree Construction... ";
    std::unordered_map<char, size_t> freqs = {
        {'a', 5},
        {'b', 9},
        {'c', 12},
        {'d', 13},
        {'e', 16},
        {'f', 45}
    };

    HuffmanTree tree;
    tree.buildTree(freqs);
    assert(tree.getRoot() != nullptr);
    assert(tree.getRoot()->frequency == 100);

    auto codes = tree.generateCodes();
    assert(codes.size() == 6);
    assert(codes.find('f') != codes.end());

    std::cout << "PASSED\n";
}

void testHuffmanEncoderFrequencyTable() {
    std::cout << "[Test] HuffmanEncoder Frequency Table... ";
    HuffmanEncoder encoder;
    std::string text = "abracadabra";
    auto freqs = encoder.buildFrequencyTable(text);

    assert(freqs['a'] == 5);
    assert(freqs['b'] == 2);
    assert(freqs['r'] == 2);
    assert(freqs['c'] == 1);
    assert(freqs['d'] == 1);

    std::cout << "PASSED\n";
}

void testHuffmanEncodeDecodeRoundTrip() {
    std::cout << "[Test] Encode / Decode Roundtrip... ";
    std::string original = "antigravity lossless compression with huffman coding";

    HuffmanEncoder encoder;
    std::string encodedBits = encoder.encode(original);
    assert(!encodedBits.empty());

    HuffmanDecoder decoder;
    std::string decoded = decoder.decode(encodedBits, encoder.getTree());
    assert(decoded == original);

    std::cout << "PASSED\n";
}

void testCompressionStatsCalculation() {
    std::cout << "[Test] CompressionStats Calculation... ";
    std::string text = "this is an example of huffman data compression for testing statistics";
    HuffmanEncoder encoder;
    std::string encodedBits = encoder.encode(text);

    CompressionStats stats = CompressionStats::calculate(text, encodedBits,
                                                         encoder.getFrequencies(),
                                                         encoder.getCodes());

    assert(stats.originalCharacterCount == text.size());
    assert(stats.originalSizeBytes == text.size());
    assert(stats.compressedBitsCount == encodedBits.size());
    assert(stats.compressedSizeBytes == (encodedBits.size() + 7) / 8);
    assert(stats.uniqueCharacterCount == encoder.getFrequencies().size());
    assert(stats.averageCodeLength > 0.0);
    assert(stats.entropy > 0.0);
    assert(!stats.formatSummary().empty());

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Running Unit Tests: Huffman Core Modules\n";
    std::cout << "========================================\n";

    try {
        testHuffmanTreeConstruction();
        testHuffmanEncoderFrequencyTable();
        testHuffmanEncodeDecodeRoundTrip();
        testCompressionStatsCalculation();
        std::cout << "All unit tests completed successfully.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Unit test failure: " << ex.what() << "\n";
        return 1;
    }
}
