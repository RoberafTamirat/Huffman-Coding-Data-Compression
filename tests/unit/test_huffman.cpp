#include <iostream>
#include <cassert>
#include <string>
#include "core/FrequencyAnalyzer.h"
#include "core/HuffmanNode.h"
#include "core/HuffmanTree.h"
#include "core/HuffmanEncoder.h"
#include "core/HuffmanDecoder.h"
#include "core/CompressionAnalyzer.h"

void testFrequencyAnalysis() {
    std::cout << "[Test] Member 2: FrequencyAnalyzer... ";
    FrequencyAnalyzer analyzer;
    std::string text = "abracadabra";
    auto freqs = analyzer.analyze(text);

    assert(freqs['a'] == 5);
    assert(freqs['b'] == 2);
    assert(freqs['r'] == 2);
    assert(freqs['c'] == 1);
    assert(freqs['d'] == 1);

    std::cout << "PASSED\n";
}

void testHuffmanTreeConstruction() {
    std::cout << "[Test] Member 3: HuffmanTree Construction... ";
    std::unordered_map<unsigned char, size_t> freqs = {
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

void testHuffmanEncodingAndBitPacking() {
    std::cout << "[Test] Member 4: HuffmanEncoder & Bit Packing... ";
    std::unordered_map<unsigned char, std::string> codes = {
        {'a', "0"},
        {'b', "10"},
        {'c', "11"}
    };

    HuffmanEncoder encoder;
    std::string text = "abc";
    std::string bits = encoder.encode(text, codes);
    assert(bits == "01011");

    uint8_t validBits = 0;
    auto packed = encoder.packBits(bits, validBits);
    assert(!packed.empty());
    assert(validBits == 5);

    std::string unpacked = encoder.unpackBits(packed, validBits);
    assert(unpacked == bits);

    std::cout << "PASSED\n";
}

void testHuffmanDecoding() {
    std::cout << "[Test] Member 5: HuffmanDecoder Roundtrip... ";
    std::string original = "antigravity lossless compression with huffman coding";

    FrequencyAnalyzer freqAnalyzer;
    auto freqs = freqAnalyzer.analyze(original);

    HuffmanTree tree;
    tree.buildTree(freqs);
    auto codes = tree.generateCodes();

    HuffmanEncoder encoder;
    std::string encodedBits = encoder.encode(original, codes);

    HuffmanDecoder decoder;
    std::string decoded = decoder.decode(encodedBits, tree);
    assert(decoded == original);

    std::cout << "PASSED\n";
}

void testCompressionEvaluation() {
    std::cout << "[Test] Member 6: CompressionAnalyzer Evaluation... ";
    std::string text = "this is an example of huffman data compression for testing statistics";

    FrequencyAnalyzer freqAnalyzer;
    auto freqs = freqAnalyzer.analyze(text);

    HuffmanTree tree;
    tree.buildTree(freqs);
    auto codes = tree.generateCodes();

    HuffmanEncoder encoder;
    std::string encodedBits = encoder.encode(text, codes);

    CompressionMetrics metrics = CompressionAnalyzer::evaluate(text, encodedBits, freqs, codes);
    assert(metrics.originalSizeBytes == text.size());
    assert(metrics.compressedPayloadBits == encodedBits.size());
    assert(metrics.compressionRatio > 0.0);
    assert(metrics.spaceSavedPercent > 0.0);
    assert(!metrics.formatReport().empty());

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Running Unit Tests: Team Modules Verification\n";
    std::cout << "========================================\n";

    try {
        testFrequencyAnalysis();
        testHuffmanTreeConstruction();
        testHuffmanEncodingAndBitPacking();
        testHuffmanDecoding();
        testCompressionEvaluation();
        std::cout << "All team integration tests completed successfully.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Unit test failure: " << ex.what() << "\n";
        return 1;
    }
}
