#ifndef COMPRESSION_ANALYZER_H
#define COMPRESSION_ANALYZER_H

#include <string>
#include <cstdint>

struct CompressionStats {
    uint64_t originalBytes = 0;
    uint64_t originalBits = 0;
    uint64_t compressedPayloadBits = 0;
    uint64_t compressedPayloadBytes = 0;
    uint64_t headerBytes = 0;
    uint64_t totalCompressedBytes = 0;
    double compressionRatio = 0;
    double spaceSavedPercentage = 0;
};

class CompressionAnalyzer {
public:
    CompressionStats analyze(const std::string& text) const;
    void displayResults(const CompressionStats& stats) const;
};

#endif
