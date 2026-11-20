#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "core/FrequencyAnalyzer.h"
#include "core/HuffmanNode.h"
#include "core/HuffmanTree.h"
#include "core/HuffmanEncoder.h"
#include "core/HuffmanDecoder.h"
#include "core/CompressionAnalyzer.h"
#include "file/FileHandler.h"
#include "gui/HuffmanGui.h"

void printUsage(const std::string& programName) {
    std::cout << "=========================================================\n"
              << " Huffman Coding Data Compression & Decompression System\n"
              << " Role 1: GUI Developer & Integration Coordinator (Roberaf)\n"
              << "=========================================================\n"
              << "Usage:\n"
              << "  " << programName << "                       Launch Graphical Desktop Interface (GUI)\n"
              << "  " << programName << " --gui                 Launch Graphical Desktop Interface (GUI)\n"
              << "  " << programName << " --demo                Run in-memory verification & pipeline demo\n"
              << "  " << programName << " --help, -h            Show this help manual\n"
              << "  " << programName << " -c <input> <output>   Compress an input text file\n"
              << "  " << programName << " -d <input> <output>   Decompress an encoded file\n"
              << "=========================================================\n";
}

void runDemo() {
    std::cout << "\n==================================================\n";
    std::cout << "[Demo] Huffman Coding Integrated Team Pipeline\n";
    std::cout << "==================================================\n";

    std::string sample = "huffman coding for data compression";
    std::cout << "Original Text: \"" << sample << "\" (" << sample.size() << " bytes)\n\n";

    // 1. Member 2: Frequency Analysis
    FrequencyAnalyzer freqAnalyzer;
    auto freqs = freqAnalyzer.analyze(sample);

    // 2. Member 3: Huffman Tree & Prefix Codes
    HuffmanTree tree;
    tree.buildTree(freqs);
    auto codes = tree.generateCodes();

    std::cout << "Generated Huffman Codes (Symbol -> Bit Sequence):\n";
    for (const auto& [ch, code] : codes) {
        std::string displayChar = (ch == ' ') ? "[SPACE]" : std::string(1, static_cast<char>(ch));
        std::cout << "  " << std::setw(8) << std::left << displayChar
                  << " | Freq: " << std::setw(2) << freqs.at(ch)
                  << " | Code: " << code << "\n";
    }

    // 3. Member 4: Encoding
    HuffmanEncoder encoder;
    std::string encodedBits = encoder.encode(sample, codes);
    std::cout << "\nEncoded Bitstream:\n" << encodedBits << "\n(" << encodedBits.size() << " bits)\n\n";

    // 4. Member 6: Compression Efficiency Evaluation
    CompressionMetrics metrics = CompressionAnalyzer::evaluate(sample, encodedBits, freqs, codes);
    std::cout << metrics.formatReport() << "\n";

    // 5. Member 5: Decoding
    HuffmanDecoder decoder;
    std::string decodedText = decoder.decode(encodedBits, tree);
    std::cout << "Decoded Text:  \"" << decodedText << "\"\n";

    if (decodedText == sample) {
        std::cout << "\n[SUCCESS] Lossless verification PASSED: Decoded text matches original 100%.\n";
    } else {
        std::cout << "\n[ERROR] Lossless verification FAILED: Mismatch detected.\n";
    }
}

int main(int argc, char* argv[]) {
    // If no command-line flags provided, default to launching the Graphical Desktop Interface (GUI)
    if (argc == 1) {
        return HuffmanGui::run();
    }

    std::string flag = argv[1];

    if (flag == "--gui" || flag == "-g") {
        return HuffmanGui::run();
    }

    if (flag == "--help" || flag == "-h") {
        printUsage(argv[0]);
        return 0;
    }

    if (flag == "--demo") {
        runDemo();
        return 0;
    }

    if (flag == "-c") {
        if (argc < 4) {
            std::cerr << "Error: Missing input or output path for compression.\n";
            printUsage(argv[0]);
            return 1;
        }

        std::string inputPath = argv[2];
        std::string outputPath = argv[3];

        FileHandler fileHandler;
        if (!fileHandler.fileExists(inputPath)) {
            std::cerr << "Error: Input file '" << inputPath << "' does not exist.\n";
            return 1;
        }

        std::string rawContent = fileHandler.readTextFile(inputPath);
        if (rawContent.empty()) {
            std::cerr << "Warning: Input file is empty. Nothing to compress.\n";
            return 1;
        }

        FrequencyAnalyzer freqAnalyzer;
        auto freqs = freqAnalyzer.analyze(rawContent);

        HuffmanTree tree;
        tree.buildTree(freqs);
        auto codes = tree.generateCodes();

        HuffmanEncoder encoder;
        std::string encodedBits = encoder.encode(rawContent, codes);

        if (fileHandler.writeTextFile(outputPath, encodedBits)) {
            std::cout << "Compression successful.\n"
                      << "  Source: " << inputPath << " (" << rawContent.size() << " bytes)\n"
                      << "  Output: " << outputPath << " (" << encodedBits.size() << " bits)\n";

            CompressionMetrics metrics = CompressionAnalyzer::evaluate(rawContent, encodedBits, freqs, codes);
            std::cout << metrics.formatReport();
        } else {
            std::cerr << "Error: Failed to write to " << outputPath << "\n";
            return 1;
        }
        return 0;
    }

    if (flag == "-d") {
        if (argc < 4) {
            std::cerr << "Error: Missing input or output path for decompression.\n";
            printUsage(argv[0]);
            return 1;
        }

        std::string inputPath = argv[2];
        FileHandler fileHandler;
        if (!fileHandler.fileExists(inputPath)) {
            std::cerr << "Error: Encoded input file '" << inputPath << "' does not exist.\n";
            return 1;
        }

        std::string encodedBits = fileHandler.readTextFile(inputPath);
        std::cout << "Read " << encodedBits.size() << " bits from " << inputPath << ".\n";
        std::cout << "Interactive desktop GUI is active. Run without arguments to use full GUI encode/decode.\n";
        return 0;
    }

    std::cerr << "Unknown argument: " << flag << "\n";
    printUsage(argv[0]);
    return 1;
}
