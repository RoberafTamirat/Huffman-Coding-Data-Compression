#pragma once

#include <string>
#include <unordered_map>
#include <cstddef>

/**
 * @file FrequencyAnalyzer.h
 * @brief Character frequency analysis module.
 * @author Member 2: Marcimoy Assefa (Frequency Analysis & Documentation Coordinator)
 *
 * Tasks:
 *  1. Read input text.
 *  2. Count how frequently each character occurs (letters, numbers, spaces, punctuation).
 *  3. Return frequency mapping using unsigned char for byte-based text processing.
 */
class FrequencyAnalyzer {
public:
    FrequencyAnalyzer() = default;
    ~FrequencyAnalyzer() = default;

    /**
     * @brief Computes frequency count for each distinct character in the text.
     * @param text Source input text.
     * @return Map of character (unsigned char) to its occurrence count.
     */
    [[nodiscard]] std::unordered_map<unsigned char, size_t> analyze(const std::string& text);

    /**
     * @brief Returns the most recently computed frequency map.
     */
    [[nodiscard]] const std::unordered_map<unsigned char, size_t>& getFrequencies() const noexcept;

private:
    std::unordered_map<unsigned char, size_t> frequencies_;
};
