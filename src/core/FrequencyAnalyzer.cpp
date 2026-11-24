/**
 * @file FrequencyAnalyzer.cpp
 * @brief Implementation of character frequency analysis module.
 * @author Member 2: Marsimoy Tesfaye (Role: Frequency Analysis Developer & Documentation Coordinator)
 * @group Group 7 - Design and Analysis of Algorithms (DAA)
 */

#include "core/FrequencyAnalyzer.h"

#include <cctype>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include <stdexcept>

FrequencyAnalyzer::FrequencyAnalyzer() {
    clear();
}

FrequencyAnalyzer::FrequencyAnalyzer(const std::string& text) {
    analyze(text);
}

void FrequencyAnalyzer::clear() noexcept {
    frequencies_.fill(0);
    totalCharacters_ = 0;
    uniqueCharacters_ = 0;
}

void FrequencyAnalyzer::analyze(const std::string& text) {
    clear();
    if (text.empty()) {
        return;
    }

    // Single-pass O(N) frequency tallying with safe unsigned byte casting
    for (char c : text) {
        const auto byteIndex = static_cast<unsigned char>(c);
        if (frequencies_[byteIndex] == 0) {
            ++uniqueCharacters_;
        }
        ++frequencies_[byteIndex];
    }
    totalCharacters_ = text.size();
}

void FrequencyAnalyzer::analyze(std::istream& stream) {
    clear();
    constexpr std::size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    while (stream.read(buffer, sizeof(buffer)) || stream.gcount() > 0) {
        const std::streamsize bytesRead = stream.gcount();
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            const auto byteIndex = static_cast<unsigned char>(buffer[i]);
            if (frequencies_[byteIndex] == 0) {
                ++uniqueCharacters_;
            }
            ++frequencies_[byteIndex];
            ++totalCharacters_;
        }
    }
}

std::size_t FrequencyAnalyzer::getFrequency(char ch) const noexcept {
    return getFrequency(static_cast<unsigned char>(ch));
}

std::size_t FrequencyAnalyzer::getFrequency(unsigned char ch) const noexcept {
    return frequencies_[ch];
}

bool FrequencyAnalyzer::isEmpty() const noexcept {
    return totalCharacters_ == 0;
}

std::size_t FrequencyAnalyzer::getTotalCharacters() const noexcept {
    return totalCharacters_;
}

std::size_t FrequencyAnalyzer::getUniqueCharacters() const noexcept {
    return uniqueCharacters_;
}

double FrequencyAnalyzer::getProbability(char ch) const noexcept {
    if (totalCharacters_ == 0) {
        return 0.0;
    }
    const auto count = getFrequency(ch);
    return static_cast<double>(count) / static_cast<double>(totalCharacters_);
}

CharacterCategories FrequencyAnalyzer::getCategoryBreakdown() const noexcept {
    CharacterCategories breakdown;

    for (std::size_t i = 0; i < frequencies_.size(); ++i) {
        const std::size_t count = frequencies_[i];
        if (count == 0) continue;

        const auto uc = static_cast<unsigned char>(i);

        if (std::isupper(uc)) {
            breakdown.uppercaseCount += count;
        } else if (std::islower(uc)) {
            breakdown.lowercaseCount += count;
        } else if (std::isdigit(uc)) {
            breakdown.digitCount += count;
        } else if (uc == ' ') {
            breakdown.spaceCount += count;
        } else if (std::isspace(uc)) {
            // Newlines, tabs, carriage returns, vertical tabs, form feeds
            breakdown.otherWhitespace += count;
        } else if (std::ispunct(uc)) {
            breakdown.punctuationCount += count;
        } else {
            breakdown.otherCount += count;
        }
    }

    return breakdown;
}

std::vector<std::pair<char, int>> FrequencyAnalyzer::getFrequencyVector() const {
    std::vector<std::pair<char, int>> result;
    if (uniqueCharacters_ == 0) {
        return result;
    }

    result.reserve(uniqueCharacters_);

    for (std::size_t i = 0; i < frequencies_.size(); ++i) {
        if (frequencies_[i] > 0) {
            // Check for integer overflow before converting std::size_t to int
            if (frequencies_[i] > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
                throw std::overflow_error("Frequency count exceeds maximum integer limit for Huffman tree node.");
            }

            // Explicitly cast to char for Member 3's HuffmanTree interface
            const auto ch = static_cast<char>(static_cast<unsigned char>(i));
            const auto freq = static_cast<int>(frequencies_[i]);
            result.emplace_back(ch, freq);
        }
    }

    // Deterministic ordering: descending frequency, then ascending byte value
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second; // higher frequency first
        }
        return static_cast<unsigned char>(a.first) < static_cast<unsigned char>(b.first); // ascending byte value
    });

    return result;
}

std::unordered_map<char, int> FrequencyAnalyzer::getFrequencyMap() const {
    std::unordered_map<char, int> result;
    result.reserve(uniqueCharacters_);

    for (std::size_t i = 0; i < frequencies_.size(); ++i) {
        if (frequencies_[i] > 0) {
            // Check for integer overflow before converting std::size_t to int
            if (frequencies_[i] > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
                throw std::overflow_error("Frequency count exceeds maximum integer limit for map value.");
            }

            result.emplace(static_cast<char>(static_cast<unsigned char>(i)),
                           static_cast<int>(frequencies_[i]));
        }
    }

    return result;
}

std::unordered_map<unsigned char, std::size_t> FrequencyAnalyzer::getUnsignedFrequencyMap() const {
    std::unordered_map<unsigned char, std::size_t> result;
    result.reserve(uniqueCharacters_);

    for (std::size_t i = 0; i < frequencies_.size(); ++i) {
        if (frequencies_[i] > 0) {
            result.emplace(static_cast<unsigned char>(i), frequencies_[i]);
        }
    }

    return result;
}

std::string FrequencyAnalyzer::getSummaryString() const {
    std::ostringstream oss;
    oss << "=== Character Frequency Analysis Summary ===\n";
    oss << "Total characters:   " << totalCharacters_ << "\n";
    oss << "Unique characters:  " << uniqueCharacters_ << "\n";

    const auto cat = getCategoryBreakdown();
    oss << "Category Breakdown:\n";
    oss << "  - Uppercase (A-Z):       " << cat.uppercaseCount << "\n";
    oss << "  - Lowercase (a-z):       " << cat.lowercaseCount << "\n";
    oss << "  - Digits (0-9):          " << cat.digitCount << "\n";
    oss << "  - Spaces (' '):          " << cat.spaceCount << "\n";
    oss << "  - Other Whitespace (\\n,\\t): " << cat.otherWhitespace << "\n";
    oss << "  - Punctuation & Symbols: " << cat.punctuationCount << "\n";
    if (cat.otherCount > 0) {
        oss << "  - Other/Extended Bytes:  " << cat.otherCount << "\n";
    }

    oss << "\nDetailed Symbol Frequencies:\n";
    const auto vec = getFrequencyVector();
    for (const auto& [ch, count] : vec) {
        const auto uc = static_cast<unsigned char>(ch);
        std::string displaySymbol;
        if (uc == ' ') displaySymbol = "[SPACE]";
        else if (uc == '\n') displaySymbol = "[LF \\n]";
        else if (uc == '\r') displaySymbol = "[CR \\r]";
        else if (uc == '\t') displaySymbol = "[TAB]";
        else if (std::isprint(uc)) displaySymbol = std::string(1, ch);
        else {
            std::ostringstream hexOss;
            hexOss << "0x" << std::hex << std::uppercase << static_cast<int>(uc);
            displaySymbol = hexOss.str();
        }

        const double prob = getProbability(ch) * 100.0;
        oss << "  " << std::left << std::setw(10) << displaySymbol
            << " (byte value " << std::right << std::setw(3) << static_cast<int>(uc) << "): "
            << std::setw(6) << count << " occurrences ("
            << std::fixed << std::setprecision(2) << prob << "%)\n";
    }

    return oss.str();
}
