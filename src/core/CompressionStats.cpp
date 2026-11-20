#include "core/CompressionStats.h"
#include <cmath>
#include <iomanip>
#include <sstream>

CompressionStats CompressionStats::calculate(const std::string& originalText,
                                             const std::string& encodedBits,
                                             const std::unordered_map<char, size_t>& frequencies,
                                             const std::unordered_map<char, std::string>& codes) {
    CompressionStats stats;
    stats.originalCharacterCount = originalText.size();
    stats.originalSizeBytes = originalText.size();
    stats.originalSizeBits = originalText.size() * 8;

    stats.compressedBitsCount = encodedBits.size();
    // Round up to nearest byte for serialized storage
    stats.compressedSizeBytes = (encodedBits.size() + 7) / 8;
    stats.uniqueCharacterCount = frequencies.size();

    if (stats.originalSizeBytes > 0) {
        stats.compressionRatioPercent = (static_cast<double>(stats.compressedSizeBytes) /
                                         static_cast<double>(stats.originalSizeBytes)) * 100.0;
        stats.spaceSavingPercent = 100.0 - stats.compressionRatioPercent;
        stats.averageCodeLength = static_cast<double>(stats.compressedBitsCount) /
                                  static_cast<double>(stats.originalCharacterCount);
    }

    // Shannon entropy calculation: H(X) = -sum(P(x) * log2(P(x)))
    double totalChars = static_cast<double>(originalText.size());
    if (totalChars > 0.0) {
        double entropySum = 0.0;
        for (const auto& [ch, count] : frequencies) {
            double p = static_cast<double>(count) / totalChars;
            if (p > 0.0) {
                entropySum -= p * (std::log2(p));
            }
        }
        stats.entropy = entropySum;
    }

    return stats;
}

std::string CompressionStats::formatSummary() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "=== COMPRESSION METRICS & STATISTICS ===\r\n"
        << "Original Size:          " << originalSizeBytes << " bytes (" << originalSizeBits << " bits)\r\n"
        << "Compressed Size:        " << compressedSizeBytes << " bytes (" << compressedBitsCount << " bits)\r\n"
        << "Unique Characters:      " << uniqueCharacterCount << "\r\n"
        << "Average Code Length:    " << averageCodeLength << " bits/char\r\n"
        << "Theoretical Entropy:    " << entropy << " bits/char\r\n"
        << "Compression Ratio:      " << compressionRatioPercent << " %\r\n"
        << "Space Savings:          " << spaceSavingPercent << " %\r\n";
    if (spaceSavingPercent > 0.0) {
        oss << "Efficiency:             Reduced by " << spaceSavingPercent << "% compared to original.\r\n";
    } else {
        oss << "Efficiency:             Small input overhead; ratio >= 100%.\r\n";
    }
    return oss.str();
}
