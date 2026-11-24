# Design and Analysis of Algorithms (DAA) Mini-Project Report

## Lossless Text Data Compression Using Huffman Coding in C++

---

**Project Title:** Huffman Coding for Data Compression
**Course:** Design and Analysis of Algorithms (DAA)
**Group:** Group 7
**Academic Year:** 2026
**Module Author & Documentation Coordinator:** Marsimoy Tesfaye (Member 2)
**Assigned Roles:** Frequency Analysis Developer and Documentation Coordinator

---

### Team Roster and Module Delegations

| Member # | Member Name      | Assigned Role                                  | Primary Module Artifacts                                                                                                                                                        |                Integration Status                 |
| :------: | :--------------- | :--------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | :-----------------------------------------------: |
|    1     | Roberaf Tamirat  | GUI Development & Integration Coordinator      | `include/gui/HuffmanGui.h`<br>`src/gui/HuffmanGui.cpp`<br>`src/gui/huffman_gui_app.py`<br>`src/main.cpp`<br>`docs/gui_documentation.md`                                         |                    Integrated                     |
|    2     | Marsimoy Tesfaye | Frequency Analysis & Documentation Coordinator | `include/core/FrequencyAnalyzer.h`<br>`src/core/FrequencyAnalyzer.cpp`<br>`tests/test_frequency_analyzer.cpp`<br>`docs/project_report.md`<br>`docs/FrequencyAnalysis_Report.md` | Implementation Completed — Test Execution Pending |
|    3     | Feven Asefa      | Huffman Tree & Priority Queue (Min-Heap)       | `include/core/HuffmanNode.h`<br>`include/core/HuffmanTree.h`<br>`src/core/HuffmanTree.cpp`                                                                                      |              Feature Branch Prepared              |
|    4     | Mahlet Eyasu     | Huffman Encoding & Bit-Packing Engine          | `include/core/HuffmanEncoder.h`<br>`src/core/HuffmanEncoder.cpp`                                                                                                                |                    In Progress                    |
|    5     | Hermela Tadiwos  | Decoding & File Management Module              | `include/core/HuffmanDecoder.h`<br>`src/core/HuffmanDecoder.cpp`<br>`include/file/FileHandler.h`<br>`src/file/FileHandler.cpp`                                                  |                    In Progress                    |
|    6     | Bereket Yonatan  | Testing & Performance Evaluation               | `include/core/CompressionAnalyzer.h`<br>`src/core/CompressionAnalyzer.cpp`<br>`tests/`                                                                                          |                    In Progress                    |

---

## 1. Introduction

Data compression is a fundamental area of theoretical computer science and algorithm design. It addresses the practical challenge of representing information in a more compact binary representation without sacrificing any original data. In modern digital infrastructure, network transmission bandwidth and physical storage capacity are constrained, costly resources. Lossless compression algorithms reduce physical storage footprints, minimize transmission latencies, and lower energy expenditures across distributed systems.

This project implements an extensible, modular, and high-performance software suite in modern ISO C++ (C++17) implementing the Huffman Coding algorithm for lossless data compression. The software architecture is explicitly partitioned across six distinct functional modules, ranging from byte-level frequency analysis to binary min-heap tree synthesis, prefix code generation, bit-packing, tree-driven decoding, and an interactive native desktop Graphical User Interface (GUI).

As the Frequency Analysis Developer and Documentation Coordinator (Member 2), this report provides a comprehensive academic treatise covering the algorithmic foundations, mathematical formulations, data structure engineering, complexity analysis, integration contracts, and test specifications of the Frequency Analysis Subsystem, while documenting the overall Group 7 system architecture.

---

## 2. Background and Motivation

Textual data in computational systems is traditionally represented using fixed-length encoding standards:

- **Standard ASCII**: Uses 7 bits per character (representing 128 distinct symbols), typically stored in an 8-bit byte.
- **Extended ASCII / ISO 8859**: Uses 8 bits per character (representing $2^8 = 256$ distinct symbols).
- **Fixed Unicode (UTF-32)**: Allocates 32 bits (4 bytes) uniformly per code point.

Under fixed-length character encoding schemes, every symbol consumes the exact same number of bits regardless of its statistical occurrence frequency within the text. In natural language prose, character distributions are highly non-uniform. For example, in typical English text, characters such as `'e'`, the space character `' '`, and `'t'` appear with significantly higher frequency than rare characters like `'z'`, `'q'`, or `'x'`. Devoting 8 full bits to frequent characters introduces substantial statistical redundancy.

Variable-length prefix encoding solves this fundamental inefficiency by assigning shorter binary prefix codes to frequently occurring symbols and longer codes to rarer symbols.

---

## 3. Problem Statement

Standard uncompressed text representation suffers from substantial information-theoretic redundancy:

1. Uniform 8-bit allocation causes unnecessary storage inflation for natural text where character frequencies vary widely.
2. Transmitting uncompressed data across resource-constrained networks increases latency, packet transmission overhead, and cloud ingress/egress costs.
3. Simple character-counting algorithms frequently fail to safely handle byte values above 127, leading to negative index array accesses and memory corruption when using signed character types.

To resolve these challenges, Group 7 was tasked with designing, implementing, and validating a robust, modular, and lossless data compression pipeline in C++ that:

- Accurately tallies byte occurrences across all 256 possible 8-bit values ($0$ to $255$) without signedness bugs or memory corruption.
- Constructs an optimal prefix tree using a priority queue min-heap in $O(K \log K)$ time.
- Compresses and decompresses text with 100% lossless fidelity.
- Provides a native desktop Graphical User Interface (GUI) and CLI dispatching for real-time visualization and efficiency metrics.

---

## 4. Project Objectives

### 4.1. General Objective

To design, implement, integrate, and evaluate a modular, robust, and high-performance Lossless Data Compression system in modern C++ (C++17) using the Huffman Coding algorithm, supported by a native desktop GUI and comprehensive automated unit testing.

### 4.2. Specific Objectives

1. **Character Frequency Analysis (Member 2 Deliverable):**
   - Implement an $O(N)$ single-pass byte-frequency analyzer capable of processing strings and chunked streams.
   - Accurately count uppercase letters, lowercase letters, numbers, spaces, tabs, newlines, punctuation, and extended byte values ($0$–$255$).
   - Supply formatted frequency data directly compatible with Member 3's `HuffmanTree` (`std::vector<std::pair<char, int>>`) and Member 1's `HuffmanGui` (`std::unordered_map<unsigned char, std::size_t>`).
2. **Optimal Huffman Tree Construction (Member 3):**
   - Implement binary min-heap priority queue structures to iteratively merge least-frequent nodes into a binary prefix tree.
3. **Prefix Bitstream Encoding & Bit-Packing (Member 4):**
   - Translate text into binary code strings and pack bits into physical 8-bit bytes (`uint8_t`).
4. **Lossless Decompression & File I/O (Member 5):**
   - Traverse the Huffman tree bitstream-by-bitstream to reconstruct original source text without data loss, managing binary file storage.
5. **Empirical Evaluation & Testing (Member 6 & Team):**
   - Calculate compression ratios, space savings percentages, average code lengths, and Shannon entropy.
6. **Graphical User Interface & Coordination (Member 1):**
   - Provide interactive visualization using native Win32 Desktop GUI and CLI execution dispatching.

---

## 5. Scope and Limitations

### 5.1. Scope

- **Domain:** Lossless text and byte stream data compression.
- **Alphabet Domain:** Full 8-bit byte space ($0$ to $255$, spanning all 256 values of the extended ASCII and binary byte range).
- **Programming Language & Standard:** ISO C++17, compiling under strict warning flags (`/W4` on MSVC, `-Wall -Wextra -Wpedantic` on GCC/Clang).
- **Execution Modes:** Native Win32 desktop application, Python desktop runner fallback, command-line arguments (`--help`, `--gui`), and standalone automated unit test suites.

### 5.2. Limitations

- **Byte-Level vs. Semantic Unicode:** The analyzer operates strictly on 8-bit octets ($0$–$255$), not on multi-byte Unicode code points or grapheme clusters. For multi-byte UTF-8 text (such as Ethiopian/Amharic script), each character occupies 3 bytes (e.g., `'áˆ€'` = `0xE1 0x88 0x80`), which are counted as three separate byte events. Decompression remains 100% lossless at the byte level, but semantic character frequencies are not modeled.
- **Zero-Order Statistical Model:** The algorithm computes symbol frequencies independently of context. It does not exploit inter-symbol correlation, repeated multi-character words, or dictionary patterns (unlike Lempel-Ziv algorithms such as LZ77, LZW, or Deflate).
- **Small Input Overhead:** For very small input texts (e.g., $< 30$ bytes), transmitting or serializing the prefix code table or tree header can result in negative net space savings.

---

## 6. Theoretical Foundations

### 6.1. Claude Shannon's Source Coding Theorem and Entropy

In his seminal 1948 paper _"A Mathematical Theory of Communication"_, Claude E. Shannon established the theoretical lower bound on lossless data compression, known as **Information Entropy** $H(X)$:

$$H(X) = - \sum_{i=1}^{K} P(x_i) \log_2 P(x_i)$$

Where:

- $K$ is the number of distinct unique symbols present in the text ($1 \le K \le 256$).
- $P(x_i)$ is the empirical probability of occurrence of symbol $x_i$, defined as:

$$P(x_i) = \frac{\text{Frequency}(x_i)}{\text{Total Processed Bytes } N}, \quad \text{such that } \sum_{i=1}^{K} P(x_i) = 1.0$$

#### Mathematical Bounds on Entropy

- **Lower Bound:** $H(X) = 0$ bits/symbol, occurring when the input text consists entirely of a single repeated character ($K = 1, P(x_1) = 1.0$).
- **Upper Bound:** $H(X) \le \log_2 K \le \log_2(256) = 8.0$ bits/symbol, occurring when all 256 symbols appear with perfectly uniform probability ($P(x_i) = \frac{1}{256}$).
- Shannon's Source Coding Theorem proves that no lossless symbol-by-symbol code can represent the source text with an average bit length smaller than $H(X)$.

### 6.2. David Huffman's Optimal Prefix-Free Codes

In 1952, David A. Huffman published _"A Method for the Construction of Minimum-Redundancy Codes"_, introducing an optimal greedy algorithm for constructing variable-length prefix codes:

1. **Prefix-Free Property:** No codeword is a prefix of any other codeword. For example, if `'e'` is assigned `01`, no other character can begin with `01`. This enables instantaneous, unambiguous decoding without delimiter markers.
2. **Minimum Redundancy Optimality:** The algorithm minimizes the expected average codeword length $L$:

$$L = \sum_{i=1}^{K} P(x_i) \cdot \text{length}(\text{code}(x_i)) \ge H(X)$$

### 6.3. The Algorithmic Role of Frequency Analysis

Frequency analysis represents the indispensable first phase of Huffman compression:

1. The source input must be ingested to determine the exact frequency distribution $\text{freq}(x_i)$ for all $K$ unique symbols.
2. Characters with $\text{freq}(x_i) = 0$ must be pruned so they do not populate leaf nodes in the binary tree.
3. The resulting probabilities dictate the priority queue ordering and the depth of each leaf in the final Huffman tree.

---

## 7. Data Structures & Architectural Design

The `FrequencyAnalyzer` subsystem was engineered with rigorous algorithmic considerations:

```text
+---------------------------------------------------------------------------------+
|                                FrequencyAnalyzer                                |
+---------------------------------------------------------------------------------+
| - frequencies_: std::array<std::size_t, 256>   // Fixed direct-address array    |
| - totalCharacters_: std::size_t                // Sum of all byte occurrences   |
| - uniqueCharacters_: std::size_t               // Distinct non-zero symbols (K) |
+---------------------------------------------------------------------------------+
| + analyze(const std::string& text): void                                        |
| + analyze(std::istream& stream): void                                           |
| + clear() noexcept: void                                                        |
| + getFrequency(char ch) const noexcept: std::size_t                             |
| + getFrequency(unsigned char ch) const noexcept: std::size_t                    |
| + getProbability(char ch) const noexcept: double                                |
| + getCategoryBreakdown() const noexcept: CharacterCategories                    |
| + getFrequencyVector() const: std::vector<std::pair<char, int>>                 |
| + getUnsignedFrequencyMap() const: std::unordered_map<unsigned char, size_t>    |
| + getFrequencyMap() const: std::unordered_map<char, int>                        |
+---------------------------------------------------------------------------------+
```

### 7.1. Direct-Address Fixed Array (`std::array<std::size_t, 256>`)

Rather than using a dynamic hash map during initial text scanning, `FrequencyAnalyzer` uses a fixed-size direct-address array:

- **Memory Allocation Details:** The array size is fixed at compile time: $256 \times \text{sizeof}(\text{std::size\_t}) = 256 \times 8 = 2,048$ bytes (2 KB) on 64-bit architectures. It is allocated as part of the object's contiguous memory footprint (on the stack frame when declared locally), avoiding dynamic heap allocation during frequency counting.
- **Cache Locality:** Contiguous memory layout maximizes CPU L1/L2 cache line hits.
- **Strict Constant Time:** Each increment `++frequencies_[byteIndex]` is guaranteed strictly $O(1)$.

### 7.2. Safe Unsigned Byte Conversion

In standard C++, primitive `char` is signed on x86/Windows, taking values in $[-128, 127]$. Accessing an array with negative indices triggers out-of-bounds memory corruption. To ensure complete memory safety:

```cpp
const auto byteIndex = static_cast<unsigned char>(c);
++frequencies_[byteIndex];
```

### 7.3. Deterministic Sorting with Strict Tie-Breaking

Member 3's `HuffmanTree` requires deterministic input to ensure that tree construction is identical across compilers. In `src/core/FrequencyAnalyzer.cpp`, the vector comparator enforces:

```cpp
std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) {
        return a.second > b.second; // Primary: Descending occurrence frequency
    }
    return static_cast<unsigned char>(a.first) < static_cast<unsigned char>(b.first); // Secondary: Ascending unsigned byte value
});
```

This guarantees that symbols with identical frequencies (such as `'B'` [ASCII 66] and `'R'` [ASCII 82] in `"ABRACADABRA"`) are ordered deterministically.

---

## 8. Algorithm Explanation & Flow

### 8.1. Single-Pass Frequency Tallying Algorithm

```text
Algorithm: AnalyzeFrequencies(Text T)
Input: String T of length N bytes
Output: Populated frequency table frequencies_[0..255]

1. Initialize frequencies_[0..255] with 0
2. Set totalCharacters_ = 0
3. Set uniqueCharacters_ = 0
4. For each character c in T do:
     byteIndex = (unsigned char) c
     If frequencies_[byteIndex] == 0 then:
         uniqueCharacters_ = uniqueCharacters_ + 1
     End If
     frequencies_[byteIndex] = frequencies_[byteIndex] + 1
     totalCharacters_ = totalCharacters_ + 1
   End For
5. Return
```

### 8.2. Character Classification Algorithm

The `getCategoryBreakdown()` method evaluates the 256-element table and populates the `CharacterCategories` structure:

- **`uppercaseCount`**: Tally of bytes where `std::isupper(uc)` is true (`'A'`–`'Z'`).
- **`lowercaseCount`**: Tally of bytes where `std::islower(uc)` is true (`'a'`–`'z'`).
- **`digitCount`**: Tally of bytes where `std::isdigit(uc)` is true (`'0'`–`'9'`).
- **`spaceCount`**: Tally of ASCII space characters (`' '`, value 32).
- **`otherWhitespace`**: Tally of `'\t'`, `'\n'`, `'\r'`, `'\v'`, `'\f'`.
- **`punctuationCount`**: Tally of printable punctuation/symbols (`std::ispunct`).
- **`otherCount`**: Tally of control characters and extended bytes ($> 127$).

---

## 9. Time and Space Complexity Analysis

It is vital to distinguish the complexity of **Frequency Analysis** from the complexity of subsequent **Huffman Tree Construction** and **Encoding**:

### 9.1. Frequency Analysis Complexity (Member 2 Subsystem)

- **Time Complexity:**
  - **Text Ingestion (`analyze(text)`):** Iterating through a string of length $N$ takes $\Theta(N)$ operations. Each array lookup and increment is $O(1)$. Total: $\Theta(N)$.
  - **Stream Ingestion (`analyze(stream)`):** Reads $N$ bytes in 8 KB chunked buffers. Total: $\Theta(N)$.
  - **Vector Extraction & Sorting (`getFrequencyVector()`):** Scanning the 256-element table takes $O(256) = O(1)$ operations. Sorting $K$ unique symbols takes $O(K \log K)$ comparisons. Because $K \le 256$, $K \log K \le 256 \log_2(256) = 2,048$ operations, which represents an upper-bounded constant time overhead.
  - **Overall Frequency Analysis Time:** $\Theta(N + K \log K)$, which scales linearly as $O(N)$.
- **Space Complexity:**
  - **Internal Working Storage:** Direct-address array of 2,048 bytes and scalar counters consume $O(1)$ constant auxiliary memory, independent of $N$.
  - **Output Storage:** Returning the frequency vector consumes $O(K)$ memory, where $1 \le K \le 256$.

### 9.2. End-to-End Pipeline Complexity Comparison

| Pipeline Stage         | Module Owner        | Primary Operation                       |  Time Complexity  |        Auxiliary Space         |
| :--------------------- | :------------------ | :-------------------------------------- | :---------------: | :----------------------------: |
| Frequency Analysis     | Member 2 (Marsimoy) | Single-pass byte counting & vector sort | $O(N + K \log K)$ | $O(1)$ internal, $O(K)$ export |
| Tree Construction      | Member 3 (Feven)    | Priority Queue (Min-Heap) build         |   $O(K \log K)$   |          $O(K)$ nodes          |
| Prefix Code Generation | Member 3 (Feven)    | Tree DFS traversal                      |      $O(K)$       |     $O(K)$ recursion & map     |
| Bitstream Encoding     | Member 4 (Mahlet)   | Text translation & bit-packing          |      $O(N)$       |          $O(N)$ bits           |
| Bitstream Decoding     | Member 5 (Hermela)  | Bit-by-bit tree traversal               |      $O(N)$       |      $O(N)$ output string      |
| Total Pipeline         | Group 7 Integration | Full Compress & Decompress Cycle        | $O(N + K \log K)$ |             $O(N)$             |

---

## 10. Module Integration Contracts

### 10.1. Integration with Member 3 (Feven Asefa — Huffman Tree)

Member 3's `HuffmanTree.h` requires:

```cpp
void buildTree(const std::vector<std::pair<char, int>>& frequencies);
```

Preconditions enforced by Member 3:

1. `frequency > 0`: Throws `std::invalid_argument` if any frequency is $\le 0$.
2. No duplicates: Throws `std::invalid_argument` if a symbol appears twice.
3. Overflow guard: Throws `std::overflow_error` if combined frequency exceeds integer limits.

**Compliance:** `FrequencyAnalyzer::getFrequencyVector()` guarantees:

- Filters out all zero counts (only positive frequencies returned).
- Collects each unique character exactly once (no duplicates).
- Safely validates that counts fit within `std::numeric_limits<int>::max()` before casting, throwing `std::overflow_error` upon violation.

### 10.2. Integration with Member 1 (Roberaf Tamirat — GUI Coordinator)

Member 1's `HuffmanGui.h` manages desktop UI tables and entropy calculations using:

```cpp
std::unordered_map<unsigned char, std::size_t>
```

**Compliance:** `FrequencyAnalyzer::getUnsignedFrequencyMap()` returns native `unsigned char` keys and `std::size_t` counts, enabling direct integration without type casting.

---

## 11. Testing Methodology and Test Suite Specifications

A comprehensive automated unit test suite was implemented in [`tests/test_frequency_analyzer.cpp`](file:///c:/Users/LEO/OneDrive/Documents/Group7-DAA/Huffman-Coding-Data-Compression/tests/test_frequency_analyzer.cpp), consisting of **13 distinct test scenarios**:

|  #  | Test Function Name               | Tested Input Scenario                                 | Target Invariants Checked                                                                 |         Verification Status          |
| :-: | :------------------------------- | :---------------------------------------------------- | :---------------------------------------------------------------------------------------- | :----------------------------------: |
|  1  | `testEmptyInput`                 | `""` and default constructor                          | `isEmpty()==true`, `totalCharacters==0`, `uniqueCharacters==0`, all vectors & maps empty  | Implemented — Test Execution Pending |
|  2  | `testSingleCharacter`            | Exactly one character `"Z"`                           | `total==1`, `unique==1`, `getFrequency('Z')==1`, `prob('Z')==1.0`, vector size 1          | Implemented — Test Execution Pending |
|  3  | `testRepeatedCharacters`         | `"aaaaa"`                                             | `total==5`, `unique==1`, `getFrequency('a')==5`, `lowercaseCount==5`                      | Implemented — Test Execution Pending |
|  4  | `testCaseSensitivity`            | `"AaAaA"`                                             | Case distinction: `'A': 3`, `'a': 2`; `uppercaseCount==3`, `lowercaseCount==2`            | Implemented — Test Execution Pending |
|  5  | `testDigitsAndNumbers`           | `"Year 2026! 100%"`                                   | Counts '0': 3, '1': 1, '2': 1, '6': 1, verifying digitCount == 6                          | Implemented — Test Execution Pending |
|  6  | `testWhitespaceHandling`         | `"Line 1\nLine 2\tTabbed\r\n"`                        | Preserves `\n` (2), `\t` (1), `\r` (1), spaces (2); `otherWhitespace==4`                  | Implemented — Test Execution Pending |
|  7  | `testPunctuationAndSymbols`      | 31 punctuation symbols                                | All 31 symbols counted with frequency 1; `punctuationCount==31`                           | Implemented — Test Execution Pending |
|  8  | `testMixedTextAndProbabilities`  | 49-character mixed pangram                            | All probabilities $0 < P(x) \le 1.0$ and sum strictly to $1.000000$                       | Implemented — Test Execution Pending |
|  9  | `testFrequencyVectorOrdering`    | `"ABRACADABRA"`                                       | Descending frequency (`'A': 5`), ties broken by ascending byte value (`'B'` before `'R'`) | Implemented — Test Execution Pending |
| 10  | `testFrequencyMapResults`        | `"Huffman Data Compression GUI 2026!"`                | Map size equals unique count; sum of map frequencies equals input length                  | Implemented — Test Execution Pending |
| 11  | `testStreamInput`                | Multiline `std::istringstream` (39 bytes)             | Multi-line stream ingestion matches direct string counting without truncation             | Implemented — Test Execution Pending |
| 12  | `testClearAndReuse`              | `"First string 123"` $\to$ `clear()` $\to$ `"Second"` | Complete reset of internal state; previous string counts fully purged                     | Implemented — Test Execution Pending |
| 13  | `testExtendedByteValuesAbove127` | Byte values `128`, `128`, `192`, `255`                | Safe unsigned byte indexing prevents signed char underflow or memory corruption           | Implemented — Test Execution Pending |

_Status Note on Native Testing:_ All 13 unit tests are fully implemented in [`tests/test_frequency_analyzer.cpp`](file:///c:/Users/LEO/OneDrive/Documents/Group7-DAA/Huffman-Coding-Data-Compression/tests/test_frequency_analyzer.cpp) and integrated into the CMake build system. However, because a native C++ compiler (MSVC / GCC) is not configured in the local development environment PATH, the test binary has not been compiled or executed natively. In accordance with strict academic integrity, this report does not claim that the native C++ test binary has passed execution; native verification remains pending configuration of the build toolchain.

---

## 12. Build System Configuration and Pull Request Workflow

### 12.1. CMake Build Integration

In [`CMakeLists.txt`](file:///c:/Users/LEO/OneDrive/Documents/Group7-DAA/Huffman-Coding-Data-Compression/CMakeLists.txt), the project is configured cleanly into modular static libraries:

- `huffman_gui_lib`: Houses Member 1's native Win32 GUI implementation (`src/gui/HuffmanGui.cpp`).
- `huffman_core_lib`: Houses Member 2's core frequency analysis implementation (`src/core/FrequencyAnalyzer.cpp`).
- `test_frequency_analyzer`: Standalone test executable linking against `huffman_core_lib`.
- `HuffmanCoding`: Main executable linking both `huffman_gui_lib` and `huffman_core_lib`.

### 12.2. Git Collaboration Workflow

In accordance with professional software engineering practices:

1. All Frequency Analysis development was executed on the isolated feature branch: `feature/frequency-analyzer`.
2. No direct commits or pushes were made to the `main` branch.
3. Development was partitioned into progressive, reviewable stages:
   - Stage 1: Interface and Header (`include/core/FrequencyAnalyzer.h`).
   - Stage 2: Implementation (`src/core/FrequencyAnalyzer.cpp`).
   - Stage 3: Automated Unit Testing (`tests/test_frequency_analyzer.cpp`).
   - Stage 4: CMake Integration (`CMakeLists.txt`).
   - Stage 5: Subsystem Documentation (`docs/FrequencyAnalysis_Report.md`).
   - Stage 6: Academic Project Report (`docs/project_report.md`).
4. Upon final review, the feature branch is merged into `main` via a formal GitHub Pull Request (PR).

---

## 13. References

1. Huffman, David A. (1952). _"A Method for the Construction of Minimum-Redundancy Codes"_. Proceedings of the IRE, 40(9), pp. 1098–1101.
2. Shannon, Claude E. (1948). _"A Mathematical Theory of Communication"_. Bell System Technical Journal, 27(3), pp. 379–423.
3. Cormen, Thomas H., Leiserson, Charles E., Rivest, Ronald L., and Stein, Clifford (2009). _"Introduction to Algorithms"_ (3rd ed.). MIT Press, Chapter 16.3: "Huffman codes", pp. 428–437.
4. Knuth, Donald E. (1998). _"The Art of Computer Programming, Volume 3: Sorting and Searching"_ (2nd ed.). Addison-Wesley.
5. Stroustrup, Bjarne (2018). _"The C++ Programming Language"_ (4th ed.). Addison-Wesley.
