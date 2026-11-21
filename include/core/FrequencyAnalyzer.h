/**
 * @file FrequencyAnalyzer.h
 * @brief Character frequency analysis module for Huffman Coding data compression.
 * @author Member 2: Marsimoy Tesfaye (Role: Frequency Analysis Developer & Documentation Coordinator)
 * @group Group 7 - Design and Analysis of Algorithms (DAA)
 *
 * Responsibilities:
 *  - Accurately count occurrences of every distinct character in an input text.
 *  - Correctly categorize and handle uppercase letters, lowercase letters, digits,
 *    whitespace (spaces, tabs, newlines, carriage returns), punctuation, and symbols.
 *  - Safely handle edge cases including empty strings, single-character inputs,
 *    and non-ASCII/extended byte values.
 *  - Provide output formatted for direct, seamless compatibility with:
 *      1. Huffman Tree Module (Member 3 - Feven Asefa): std::vector<std::pair<char, int>>
 *      2. GUI & Coordinator Module (Member 1 - Roberaf Tamirat): std::unordered_map<unsigned char, std::size_t>
 *  - Deliver statistical helper metrics such as total character count, unique character count,
 *    and empirical probability distributions for entropy evaluation.
 */

#ifndef FREQUENCY_ANALYZER_H
#define FREQUENCY_ANALYZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>

/**
 * @struct CharacterCategories
 * @brief Summary breakdown of character classifications within analyzed text.
 */
struct CharacterCategories {
    std::size_t uppercaseCount{0};   // 'A'-'Z'
    std::size_t lowercaseCount{0};   // 'a'-'z'
    std::size_t digitCount{0};       // '0'-'9'
    std::size_t spaceCount{0};       // ' '
    std::size_t otherWhitespace{0};  // '\t', '\n', '\r', etc.
    std::size_t punctuationCount{0}; // Printable punctuation and symbols
    std::size_t otherCount{0};       // Control characters or extended bytes
};

/**
 * @class FrequencyAnalyzer
 * @brief Analyzes input text to compute character occurrence frequencies.
 */
class FrequencyAnalyzer {
public:
    /**
     * @brief Default constructor. Initializes an empty frequency table.
     */
    FrequencyAnalyzer();

    /**
     * @brief Construct and immediately analyze an input string.
     * @param text The input source string.
     */
    explicit FrequencyAnalyzer(const std::string& text);

    /**
     * @brief Reset all internal state and frequency counts.
     */
    void clear() noexcept;

    /**
     * @brief Analyze character occurrences in the provided string.
     * Replaces any existing analysis state.
     * @param text The input string to analyze.
     */
    void analyze(const std::string& text);

    /**
     * @brief Analyze character occurrences from an input stream (e.g. file stream).
     * Replaces any existing analysis state.
     * @param stream Input stream containing text data.
     */
    void analyze(std::istream& stream);

    /**
     * @brief Get the occurrence count for a specific character.
     * @param ch The target character.
     * @return Occurrence frequency (0 if not present).
     */
    [[nodiscard]] std::size_t getFrequency(char ch) const noexcept;

    /**
     * @brief Get the occurrence count for a specific unsigned byte.
     * @param ch The target byte.
     * @return Occurrence frequency (0 if not present).
     */
    [[nodiscard]] std::size_t getFrequency(unsigned char ch) const noexcept;

    /**
     * @brief Check whether the analyzer contains any analyzed data.
     * @return true if totalCharacters == 0, false otherwise.
     */
    [[nodiscard]] bool isEmpty() const noexcept;

    /**
     * @brief Total number of characters processed (sum of all frequencies).
     * @return Total character count.
     */
    [[nodiscard]] std::size_t getTotalCharacters() const noexcept;

    /**
     * @brief Number of distinct characters with frequency > 0.
     * @return Unique character count.
     */
    [[nodiscard]] std::size_t getUniqueCharacters() const noexcept;

    /**
     * @brief Empirical probability P(ch) = freq(ch) / totalCharacters.
     * @param ch The target character.
     * @return Probability between 0.0 and 1.0.
     */
    [[nodiscard]] double getProbability(char ch) const noexcept;

    /**
     * @brief Detailed breakdown of character categories (letters, digits, whitespace, etc.).
     * @return CharacterCategories structure with count totals.
     */
    [[nodiscard]] CharacterCategories getCategoryBreakdown() const noexcept;

    // =========================================================================
    // Integration Interfaces with Other Team Modules
    // =========================================================================

    /**
     * @brief Formats frequencies for Member 3's HuffmanTree module (Feven Asefa).
     *
     * In accordance with HuffmanTree::buildTree(const std::vector<std::pair<char, int>>&):
     *  - Returns ONLY characters with frequency > 0 (filters out unused alphabet symbols).
     *  - Guarantees zero duplicate character entries.
     *  - Sorted deterministically by frequency descending, then ASCII code ascending.
     *
     * @return Vector of (character, frequency) pairs.
     */
    [[nodiscard]] std::vector<std::pair<char, int>> getFrequencyVector() const;

    /**
     * @brief Formats frequencies as an unordered map with standard char keys.
     * Useful for general lookups and dictionary operations.
     * @return Map of char -> int frequency.
     */
    [[nodiscard]] std::unordered_map<char, int> getFrequencyMap() const;

    /**
     * @brief Formats frequencies for Member 1's HuffmanGui module (Roberaf Tamirat).
     *
     * Directly matches HuffmanGui's internal signature:
     * std::unordered_map<unsigned char, std::size_t>
     *
     * @return Map of unsigned char -> std::size_t frequency.
     */
    [[nodiscard]] std::unordered_map<unsigned char, std::size_t> getUnsignedFrequencyMap() const;

    /**
     * @brief Formatted textual summary of analysis results for logging and report generation.
     * @return Human-readable summary string.
     */
    [[nodiscard]] std::string getSummaryString() const;

private:
    /// Fast direct-mapped fixed array for all 256 possible byte values (0-255).
    /// Provides O(1) random-access tallying with zero heap allocations during scanning.
    std::array<std::size_t, 256> frequencies_{};

    std::size_t totalCharacters_{0};
    std::size_t uniqueCharacters_{0};
};

#endif // FREQUENCY_ANALYZER_H
