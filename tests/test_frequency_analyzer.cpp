/**
 * @file test_frequency_analyzer.cpp
 * @brief Comprehensive automated unit tests for FrequencyAnalyzer module.
 * @author Member 2: Marsimoy Tesfaye (Role: Frequency Analysis Developer & Documentation Coordinator)
 * @group Group 7 - Design and Analysis of Algorithms (DAA)
 */

#include "core/FrequencyAnalyzer.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

// Self-contained test runner counters and assertion macros
static int g_testsRun = 0;
static int g_testsPassed = 0;
static int g_testsFailed = 0;

#define RUN_TEST(testFunc) \
    do { \
        ++g_testsRun; \
        std::cout << "[RUNNING] " << #testFunc << "... "; \
        try { \
            testFunc(); \
            ++g_testsPassed; \
            std::cout << "PASSED\n"; \
        } catch (const std::exception& e) { \
            ++g_testsFailed; \
            std::cout << "FAILED: " << e.what() << "\n"; \
        } catch (...) { \
            ++g_testsFailed; \
            std::cout << "FAILED (Unknown exception)\n"; \
        } \
    } while (0)

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            throw std::runtime_error("Assertion failed: " #cond " at line " + std::to_string(__LINE__)); \
        } \
    } while (0)

#define ASSERT_FALSE(cond) \
    do { \
        if (cond) { \
            throw std::runtime_error("Assertion failed: NOT(" #cond ") at line " + std::to_string(__LINE__)); \
        } \
    } while (0)

#define ASSERT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::ostringstream ss; \
            ss << "Expected " #actual " == " #expected " (" << (expected) \
               << ") but got (" << (actual) << ") at line " << __LINE__; \
            throw std::runtime_error(ss.str()); \
        } \
    } while (0)

#define ASSERT_NEAR(actual, expected, tol) \
    do { \
        if (std::fabs((actual) - (expected)) > (tol)) { \
            std::ostringstream ss; \
            ss << "Expected " #actual " near " #expected " within " << (tol) \
               << " but got diff " << std::fabs((actual) - (expected)) << " at line " << __LINE__; \
            throw std::runtime_error(ss.str()); \
        } \
    } while (0)

// =============================================================================
// Unit Test Cases
// =============================================================================

/**
 * @brief Test 1: Empty input string and default state.
 */
void testEmptyInput() {
    FrequencyAnalyzer analyzer;
    ASSERT_TRUE(analyzer.isEmpty());
    ASSERT_EQ(analyzer.getTotalCharacters(), 0);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 0);
    ASSERT_EQ(analyzer.getFrequency('a'), 0);
    ASSERT_TRUE(analyzer.getFrequencyVector().empty());
    ASSERT_TRUE(analyzer.getFrequencyMap().empty());
    ASSERT_TRUE(analyzer.getUnsignedFrequencyMap().empty());

    // Explicitly pass empty string
    analyzer.analyze("");
    ASSERT_TRUE(analyzer.isEmpty());
    ASSERT_EQ(analyzer.getTotalCharacters(), 0);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 0);
    ASSERT_TRUE(analyzer.getFrequencyVector().empty());
}

/**
 * @brief Test 2: Exactly one character.
 */
void testSingleCharacter() {
    FrequencyAnalyzer analyzer("Z");
    ASSERT_FALSE(analyzer.isEmpty());
    ASSERT_EQ(analyzer.getTotalCharacters(), 1);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 1);
    ASSERT_EQ(analyzer.getFrequency('Z'), 1);
    ASSERT_EQ(analyzer.getFrequency('z'), 0);
    ASSERT_NEAR(analyzer.getProbability('Z'), 1.0, 1e-6);

    const auto vec = analyzer.getFrequencyVector();
    ASSERT_EQ(vec.size(), 1);
    ASSERT_EQ(vec[0].first, 'Z');
    ASSERT_EQ(vec[0].second, 1);
}

/**
 * @brief Test 3: Repeated occurrences of a single character.
 */
void testRepeatedCharacters() {
    FrequencyAnalyzer analyzer("aaaaa");
    ASSERT_FALSE(analyzer.isEmpty());
    ASSERT_EQ(analyzer.getTotalCharacters(), 5);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 1);
    ASSERT_EQ(analyzer.getFrequency('a'), 5);
    ASSERT_EQ(analyzer.getFrequency('b'), 0);
    ASSERT_NEAR(analyzer.getProbability('a'), 1.0, 1e-6);

    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.lowercaseCount, 5);
    ASSERT_EQ(breakdown.uppercaseCount, 0);
    ASSERT_EQ(breakdown.digitCount, 0);
    ASSERT_EQ(breakdown.spaceCount, 0);
}

/**
 * @brief Test 4: Uppercase vs lowercase case sensitivity.
 */
void testCaseSensitivity() {
    FrequencyAnalyzer analyzer("AaAaA");
    ASSERT_EQ(analyzer.getTotalCharacters(), 5);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 2);
    ASSERT_EQ(analyzer.getFrequency('A'), 3);
    ASSERT_EQ(analyzer.getFrequency('a'), 2);

    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.uppercaseCount, 3);
    ASSERT_EQ(breakdown.lowercaseCount, 2);
}

/**
 * @brief Test 5: Digits and numeric characters.
 */
void testDigitsAndNumbers() {
    FrequencyAnalyzer analyzer("Year 2026! 100%");
    ASSERT_EQ(analyzer.getFrequency('2'), 2);
    ASSERT_EQ(analyzer.getFrequency('0'), 3);
    ASSERT_EQ(analyzer.getFrequency('6'), 1);
    ASSERT_EQ(analyzer.getFrequency('1'), 1);

    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.digitCount, 7); // '2', '0', '2', '6', '1', '0', '0'
}

/**
 * @brief Test 6: Whitespace characters (spaces, tabs, newlines, carriage returns).
 */
void testWhitespaceHandling() {
    std::string text = "Line 1\nLine 2\tTabbed\r\n";
    FrequencyAnalyzer analyzer(text);

    ASSERT_EQ(analyzer.getFrequency('\n'), 2);
    ASSERT_EQ(analyzer.getFrequency('\t'), 1);
    ASSERT_EQ(analyzer.getFrequency('\r'), 1);
    ASSERT_EQ(analyzer.getFrequency(' '), 2);

    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.spaceCount, 2);
    ASSERT_EQ(breakdown.otherWhitespace, 4); // 2 \n + 1 \t + 1 \r
}

/**
 * @brief Test 7: Printable punctuation and special symbols.
 */
void testPunctuationAndSymbols() {
    std::string symbols = "!@#$%^&*()_+-=[]{}|;':\",.<>?/~`";
    FrequencyAnalyzer analyzer(symbols);

    ASSERT_EQ(analyzer.getTotalCharacters(), symbols.size());
    ASSERT_EQ(analyzer.getUniqueCharacters(), symbols.size());

    for (char c : symbols) {
        ASSERT_EQ(analyzer.getFrequency(c), 1);
    }

    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.punctuationCount, symbols.size());
    ASSERT_EQ(breakdown.uppercaseCount, 0);
    ASSERT_EQ(breakdown.lowercaseCount, 0);
}

/**
 * @brief Test 8: Mixed text with letters, digits, whitespace, and punctuation.
 */
void testMixedTextAndProbabilities() {
    std::string text = "The quick brown fox jumps over the lazy dog. 123!";
    FrequencyAnalyzer analyzer(text);

    ASSERT_EQ(analyzer.getTotalCharacters(), text.size());
    ASSERT_EQ(analyzer.getFrequency(' '), 9);
    ASSERT_EQ(analyzer.getFrequency('.'), 1);
    ASSERT_EQ(analyzer.getFrequency('!'), 1);
    ASSERT_EQ(analyzer.getFrequency('1'), 1);
    ASSERT_EQ(analyzer.getFrequency('2'), 1);
    ASSERT_EQ(analyzer.getFrequency('3'), 1);

    // Verify all individual probabilities are in [0, 1] and sum to 1.0
    double probSum = 0.0;
    const auto vec = analyzer.getFrequencyVector();
    for (const auto& item : vec) {
        double p = analyzer.getProbability(item.first);
        ASSERT_TRUE(p > 0.0 && p <= 1.0);
        probSum += p;
    }
    ASSERT_NEAR(probSum, 1.0, 1e-6);
}

/**
 * @brief Test 9: Frequency vector ordering (descending frequency, ascending byte value).
 */
void testFrequencyVectorOrdering() {
    // "ABRACADABRA": A=5, B=2, R=2, C=1, D=1
    // Ties:
    //   B (ASCII 66) vs R (ASCII 82) -> B must precede R
    //   C (ASCII 67) vs D (ASCII 68) -> C must precede D
    FrequencyAnalyzer analyzer("ABRACADABRA");
    const auto vec = analyzer.getFrequencyVector();

    ASSERT_EQ(vec.size(), 5);

    // 1st: 'A' (5)
    ASSERT_EQ(vec[0].first, 'A');
    ASSERT_EQ(vec[0].second, 5);

    // 2nd and 3rd: 'B' and 'R' (frequency 2 each, 'B' < 'R' ascending byte value)
    ASSERT_EQ(vec[1].first, 'B');
    ASSERT_EQ(vec[1].second, 2);
    ASSERT_EQ(vec[2].first, 'R');
    ASSERT_EQ(vec[2].second, 2);

    // 4th and 5th: 'C' and 'D' (frequency 1 each, 'C' < 'D' ascending byte value)
    ASSERT_EQ(vec[3].first, 'C');
    ASSERT_EQ(vec[3].second, 1);
    ASSERT_EQ(vec[4].first, 'D');
    ASSERT_EQ(vec[4].second, 1);

    // Verify general non-increasing order of frequencies
    for (std::size_t i = 1; i < vec.size(); ++i) {
        ASSERT_TRUE(vec[i - 1].second >= vec[i].second);
    }
}

/**
 * @brief Test 10: Frequency map outputs for general lookups and GUI.
 */
void testFrequencyMapResults() {
    std::string sample = "Huffman Data Compression GUI 2026!";
    FrequencyAnalyzer analyzer(sample);

    const auto charMap = analyzer.getFrequencyMap();
    const auto unsignedMap = analyzer.getUnsignedFrequencyMap();

    ASSERT_EQ(charMap.size(), analyzer.getUniqueCharacters());
    ASSERT_EQ(unsignedMap.size(), analyzer.getUniqueCharacters());

    std::size_t totalFromMap = 0;
    for (const auto& [byteVal, count] : unsignedMap) {
        ASSERT_TRUE(count > 0);
        totalFromMap += count;
    }
    ASSERT_EQ(totalFromMap, sample.size());
}

/**
 * @brief Test 11: Stream-based analysis using std::istringstream.
 */
void testStreamInput() {
    std::istringstream stream("Stream test line 1\nStream test line 2\n");
    FrequencyAnalyzer analyzer;
    analyzer.analyze(stream);

    ASSERT_EQ(analyzer.getFrequency('\n'), 2);
    ASSERT_EQ(analyzer.getFrequency('S'), 2);
    ASSERT_EQ(analyzer.getFrequency('t'), 6); // 2 in "Stream", 4 in "test"
    ASSERT_EQ(analyzer.getFrequency('e'), 6); // 2 in "Stream", 2 in "test", 2 in "line"
    ASSERT_EQ(analyzer.getFrequency('1'), 1);
    ASSERT_EQ(analyzer.getFrequency('2'), 1);
}

/**
 * @brief Test 12: Clearing and reusing the analyzer.
 */
void testClearAndReuse() {
    FrequencyAnalyzer analyzer("First string 123");
    ASSERT_EQ(analyzer.getTotalCharacters(), 16);

    analyzer.clear();
    ASSERT_TRUE(analyzer.isEmpty());
    ASSERT_EQ(analyzer.getTotalCharacters(), 0);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 0);
    ASSERT_TRUE(analyzer.getFrequencyVector().empty());

    analyzer.analyze("Second");
    ASSERT_EQ(analyzer.getTotalCharacters(), 6);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 6);
    ASSERT_EQ(analyzer.getFrequency('S'), 1);
    ASSERT_EQ(analyzer.getFrequency('1'), 0); // Previous text counts must be completely wiped
}

/**
 * @brief Test 13: Extended byte values above 127 (128-255).
 * Validates that safe unsigned byte casting prevents negative index bugs.
 */
void testExtendedByteValuesAbove127() {
    // Construct bytes with values > 127: 0x80 (128), 0xC0 (192), 0xFF (255)
    std::string extendedBytes;
    extendedBytes += static_cast<char>(static_cast<unsigned char>(128));
    extendedBytes += static_cast<char>(static_cast<unsigned char>(128));
    extendedBytes += static_cast<char>(static_cast<unsigned char>(192));
    extendedBytes += static_cast<char>(static_cast<unsigned char>(255));

    FrequencyAnalyzer analyzer(extendedBytes);

    ASSERT_EQ(analyzer.getTotalCharacters(), 4);
    ASSERT_EQ(analyzer.getUniqueCharacters(), 3);
    ASSERT_EQ(analyzer.getFrequency(static_cast<unsigned char>(128)), 2);
    ASSERT_EQ(analyzer.getFrequency(static_cast<unsigned char>(192)), 1);
    ASSERT_EQ(analyzer.getFrequency(static_cast<unsigned char>(255)), 1);
    ASSERT_EQ(analyzer.getFrequency('A'), 0);

    const auto vec = analyzer.getFrequencyVector();
    ASSERT_EQ(vec.size(), 3);
    // Highest frequency is 128 (count 2)
    ASSERT_EQ(static_cast<unsigned char>(vec[0].first), 128);
    ASSERT_EQ(vec[0].second, 2);

    // Category breakdown should count extended bytes under otherCount
    const auto breakdown = analyzer.getCategoryBreakdown();
    ASSERT_EQ(breakdown.otherCount, 4);
}

// =============================================================================
// Main Test Runner Entry Point
// =============================================================================

int main() {
    std::cout << "=========================================================\n";
    std::cout << " Group 7 DAA: FrequencyAnalyzer Automated Unit Test Suite\n";
    std::cout << " Author: Member 2: Marsimoy Tesfaye\n";
    std::cout << " Role: Frequency Analysis Developer & Documentation Coord\n";
    std::cout << "=========================================================\n\n";

    RUN_TEST(testEmptyInput);
    RUN_TEST(testSingleCharacter);
    RUN_TEST(testRepeatedCharacters);
    RUN_TEST(testCaseSensitivity);
    RUN_TEST(testDigitsAndNumbers);
    RUN_TEST(testWhitespaceHandling);
    RUN_TEST(testPunctuationAndSymbols);
    RUN_TEST(testMixedTextAndProbabilities);
    RUN_TEST(testFrequencyVectorOrdering);
    RUN_TEST(testFrequencyMapResults);
    RUN_TEST(testStreamInput);
    RUN_TEST(testClearAndReuse);
    RUN_TEST(testExtendedByteValuesAbove127);

    std::cout << "\n=========================================================\n";
    std::cout << " TEST EXECUTION SUMMARY:\n";
    std::cout << "   Total Tests Run:    " << g_testsRun << "\n";
    std::cout << "   Tests Passed:       " << g_testsPassed << "\n";
    std::cout << "   Tests Failed:       " << g_testsFailed << "\n";
    std::cout << "=========================================================\n";

    if (g_testsFailed == 0) {
        std::cout << " >>> ALL 13 UNIT TESTS PASSED SUCCESSFULLY! <<<\n";
        return 0;
    } else {
        std::cerr << " >>> " << g_testsFailed << " TEST(S) FAILED! <<<\n";
        return 1;
    }
}
