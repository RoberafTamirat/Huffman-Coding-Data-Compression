#include "core/FrequencyAnalyzer.h"

/**
 * @file FrequencyAnalyzer.cpp
 * @author Member 2: Marcimoy Assefa
 */

std::unordered_map<unsigned char, size_t> FrequencyAnalyzer::analyze(const std::string& text) {
    frequencies_.clear();
    for (unsigned char ch : text) {
        ++frequencies_[ch];
    }
    return frequencies_;
}

const std::unordered_map<unsigned char, size_t>& FrequencyAnalyzer::getFrequencies() const noexcept {
    return frequencies_;
}
