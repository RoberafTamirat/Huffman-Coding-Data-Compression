#include "core/CompressionAnalyzer.h"
#include <cmath>
#include <iomanip>
#include <sstream>

/**
 * @file CompressionAnalyzer.cpp
 * @author Member 6: Bereket Yonatan
 */

CompressionMetrics CompressionAnalyzer::evaluate(
    const std::string& originalText,
    const std::string& encodedBits,
    const std::unordered_map<unsigned char, size_t>& frequencies,
    const std::unordered_map<unsigned char, std::string>& codes) {

    (void)codes;
    CompressionMetrics m;
    m.originalSizeBytes = originalText.size();
    m.originalSizeBits = originalText.size() * 8;

    m.compressedPayloadBits = encodedBits.size();
    m.compressedPayloadBytes = (encodedBits.size() + 7) / 8;

    // Header overhead: store frequency table entries (1 byte char + 4 bytes freq each)
    m.uniqueCharacters = frequencies.size();
    m.headerOverheadBytes = m.uniqueCharacters * (sizeof(unsigned char) + sizeof(uint32_t)) + sizeof(uint32_t);
    m.totalCompressedBytes = m.compressedPayloadBytes + m.headerOverheadBytes;
    m.totalCompressedBits = m.totalCompressedBytes * 8;

    // Required formulas per project specification:
    // Compression ratio: Original size / Compressed size
    if (m.compressedPayloadBytes > 0) {
        m.compressionRatio = static_cast<double>(m.originalSizeBytes) /
                             static_cast<double>(m.compressedPayloadBytes);
    }

    // Space saved: ((Original size - Compressed size) / Original size) * 100
    if (m.originalSizeBytes > 0) {
        m.spaceSavedPercent = ((static_cast<double>(m.originalSizeBytes) -
                                static_cast<double>(m.compressedPayloadBytes)) /
                               static_cast<double>(m.originalSizeBytes)) * 100.0;

        m.averageCodeLength = static_cast<double>(m.compressedPayloadBits) /
                              static_cast<double>(m.originalSizeBytes);
    }

    // Shannon entropy: H(X) = -sum(P(x) * log2(P(x)))
    double totalChars = static_cast<double>(originalText.size());
    if (totalChars > 0.0) {
        double entropySum = 0.0;
        for (const auto& [ch, count] : frequencies) {
            double p = static_cast<double>(count) / totalChars;
            if (p > 0.0) {
                entropySum -= p * std::log2(p);
            }
        }
        m.entropy = entropySum;
    }

    return m;
}

std::string CompressionMetrics::formatReport() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "=== COMPRESSION EFFICIENCY EVALUATION (MEMBER 6 METRICS) ===\r\n"
        << "Original Size:          " << originalSizeBytes << " bytes (" << originalSizeBits << " bits)\r\n"
        << "Encoded Payload:        " << compressedPayloadBytes << " bytes (" << compressedPayloadBits << " bits)\r\n"
        << "Unique Symbols:         " << uniqueCharacters << "\r\n"
        << "Compression Ratio:      " << compressionRatio << " : 1 (Formula: Original / Compressed)\r\n"
        << "Space Saved:            " << spaceSavedPercent << " % (Formula: ((Orig - Comp) / Orig) * 100)\r\n"
        << "Average Code Length:    " << averageCodeLength << " bits/char\r\n"
        << "Shannon Entropy:        " << entropy << " bits/symbol\r\n"
        << "Header Overhead:        " << headerOverheadBytes << " bytes (if serialized with freq table)\r\n";
    return oss.str();
}
