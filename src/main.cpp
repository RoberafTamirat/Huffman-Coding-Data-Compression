#include <iostream>
#include <string>
#include "gui/HuffmanGui.h"

void printUsage(const std::string& programName) {
    std::cout << "=========================================================\n"
              << " Huffman Coding Data Compression & Decompression System\n"
              << "=========================================================\n"
              << "Usage:\n"
              << "  " << programName << "                       Launch Graphical Desktop Interface (GUI)\n"
              << "  " << programName << " --gui, -g             Launch Graphical Desktop Interface (GUI)\n"
              << "  " << programName << " --help, -h            Show this help manual\n"
              << "=========================================================\n"
              << "Note: Teammates will integrate their modules (FrequencyAnalyzer, \n"
              << "      HuffmanTree, HuffmanEncoder, HuffmanDecoder, FileHandler, \n"
              << "      and CompressionAnalyzer) into this coordinator entry point.\n";
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

    std::cerr << "Unknown argument: " << flag << "\n";
    printUsage(argv[0]);
    return 1;
}
