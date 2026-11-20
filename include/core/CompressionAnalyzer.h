#pragma once

#include <string>
#include <unordered_map>
#include <cstddef>

/**
 * @file CompressionAnalyzer.h
 * @brief Evaluates compression efficiency and performance metrics.
 * @author Member 6: Bereket Yonatan (Testing and Performance Evaluation Developer)
 *
 * Tasks:
 *  - Calculate original text size in bits.
 *  - Calculate actual compressed payload size in bits.
 *  - Account for compressed file frequency-table/header overhead.
 *  - Calculate compression ratio: Original size / Compressed size
 *  - Calculate space saved (%): ((Original size - Compressed size) / Original size) * 100
 */
struct CompressionMetrics {
    size_t originalSizeBytes{0};
    size_t originalSizeBits{0};

    size_t compressedPayloadBits{0};
    size_t compressedPayloadBytes{0};

    size_t headerOverheadBytes{0};
    size_t totalCompressedBytes{0};
    size_t totalCompressedBits{0};

    double compressionRatio{0.0};  // Formula: Original size / Compressed size
    double spaceSavedPercent{0.0}; // Formula: ((Original size - Compressed size) / Original size) * 100

    size_t uniqueCharacters{0};
    double averageCodeLength{0.0};
    double entropy{0.0};

    [[nodiscard]] std::string formatReport() const;
};

class CompressionAnalyzer {
public:
    CompressionAnalyzer() = default;
    ~CompressionAnalyzer() = default;

    /**
     * @brief Computes compression evaluation metrics according to agreed group formulas.
     */
    [[nodiscard]] static CompressionMetrics evaluate(
        const std::string& originalText,
        const std::string& encodedBits,
        const std::unordered_map<unsigned char, size_t>& frequencies,
        const std::unordered_map<unsigned char, std::string>& codes);
};
