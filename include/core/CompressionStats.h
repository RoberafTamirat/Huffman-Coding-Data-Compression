#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

/**
 * @brief Represents compression statistics computed during encoding.
 * Supplied as part of Member 6's deliverables and integrated into the GUI by Member 1.
 */
struct CompressionStats {
    size_t originalCharacterCount{0};
    size_t originalSizeBytes{0};
    size_t originalSizeBits{0};

    size_t compressedBitsCount{0};
    size_t compressedSizeBytes{0};

    double compressionRatioPercent{0.0}; // (compressed / original) * 100
    double spaceSavingPercent{0.0};      // (1 - (compressed / original)) * 100

    size_t uniqueCharacterCount{0};
    double averageCodeLength{0.0};       // Total compressed bits / total original characters
    double entropy{0.0};                 // Theoretical Shannon entropy (bits/symbol)

    /**
     * @brief Computes statistics based on source text, character frequencies, and generated prefix codes.
     */
    static CompressionStats calculate(const std::string& originalText,
                                      const std::string& encodedBits,
                                      const std::unordered_map<char, size_t>& frequencies,
                                      const std::unordered_map<char, std::string>& codes);

    /**
     * @brief Generates a clean, human-readable summary suitable for display in the GUI and reports.
     */
    [[nodiscard]] std::string formatSummary() const;
};
