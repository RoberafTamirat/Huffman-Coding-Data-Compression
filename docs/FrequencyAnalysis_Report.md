# Frequency Analysis Subsystem Documentation
**Author:** Marsimoy Tesfaye (Member 2)
**Role:** Frequency Analysis Developer & Documentation Coordinator
**Module:** `FrequencyAnalyzer` (`include/core/FrequencyAnalyzer.h`, `src/core/FrequencyAnalyzer.cpp`)
**Project:** Huffman Coding for Lossless Data Compression (Group 7)

---

## 1. Module Overview

The `FrequencyAnalyzer` class serves as the foundational preprocessing engine of the Group 7 Huffman Coding pipeline. Its primary algorithmic responsibility is to scan input text or streams, calculate exact occurrence frequencies across all 256 possible 8-bit byte values ($0$ to $255$), categorize character types, and supply formatted frequency data directly to downstream modules:

1. **Member 3 (`HuffmanTree` by Feven Asefa)**: Supplies a descending-ordered vector of positive frequency pairs (`std::vector<std::pair<char, int>>`) required to build the priority queue min-heap.
2. **Member 1 (`HuffmanGui` by Roberaf Tamirat)**: Supplies an unsigned byte map (`std::unordered_map<unsigned char, std::size_t>`) used to populate the desktop GUI symbol table and compute Shannon entropy.
3. **General Dictionary Lookups**: Supplies a standard character map (`std::unordered_map<char, int>`).

---

## 2. API Reference & Data Structures

### 2.1. CharacterCategories Structure
The `CharacterCategories` structure summarizes the demographic composition of the analyzed byte stream:

```cpp
struct CharacterCategories {
    std::size_t uppercaseCount{0};   // Count of 'A'-'Z' (ASCII 65-90)
    std::size_t lowercaseCount{0};   // Count of 'a'-'z' (ASCII 97-122)
    std::size_t digitCount{0};       // Count of '0'-'9' (ASCII 48-57)
    std::size_t spaceCount{0};       // Count of ASCII 32 (' ')
    std::size_t otherWhitespace{0};  // Count of '\t', '\n', '\r', '\v', '\f'
    std::size_t punctuationCount{0}; // Count of printable punctuation & symbols (std::ispunct)
    std::size_t otherCount{0};       // Count of control codes (< 32 except whitespace) and extended bytes (> 127)
};
```

### 2.2. Class Constructors
```cpp
FrequencyAnalyzer();
explicit FrequencyAnalyzer(const std::string& text);
```
- **`FrequencyAnalyzer()`**: Initializes an empty frequency table with all 256 byte counts and counters set to zero.
- **`explicit FrequencyAnalyzer(const std::string& text)`**: Constructs the object and immediately analyzes the provided string in a single pass.

### 2.3. Text & Stream Ingestion Methods
```cpp
void analyze(const std::string& text);
void analyze(std::istream& stream);
void clear() noexcept;
```
- **`analyze(const std::string& text)`**: Resets state and tallies byte occurrences across the input string in $O(N)$ time.
- **`analyze(std::istream& stream)`**: Resets state and streams data in 8 KB (8,192 byte) chunks to process arbitrarily large files with minimal memory overhead.
- **`clear() noexcept`**: Resets all 256 array slots to 0 and clears `totalCharacters_` and `uniqueCharacters_`.

### 2.4. Inspection & Statistical Queries
```cpp
[[nodiscard]] std::size_t getFrequency(char ch) const noexcept;
[[nodiscard]] std::size_t getFrequency(unsigned char ch) const noexcept;
[[nodiscard]] bool isEmpty() const noexcept;
[[nodiscard]] std::size_t getTotalCharacters() const noexcept;
[[nodiscard]] std::size_t getUniqueCharacters() const noexcept;
[[nodiscard]] double getProbability(char ch) const noexcept;
[[nodiscard]] CharacterCategories getCategoryBreakdown() const noexcept;
```
- **`getFrequency(...)`**: Returns the occurrence count of a given byte in $O(1)$ time. Overloaded for both signed `char` and `unsigned char`.
- **`isEmpty()`**: Returns `true` if no characters have been processed (`totalCharacters_ == 0`).
- **`getTotalCharacters()`**: Returns the total number of bytes processed ($N$).
- **`getUniqueCharacters()`**: Returns the number of distinct byte symbols ($K$) where count $> 0$ ($0 \le K \le 256$).
- **`getProbability(char ch)`**: Computes empirical probability $P(x_i) = \frac{\text{freq}(x_i)}{N}$. Returns $0.0$ if the analyzer is empty.
- **`getCategoryBreakdown()`**: Evaluates the 256-element table and returns populated `CharacterCategories`.

---

## 3. Purpose of Integration Export Formats

To eliminate adapter boilerplate across modules, `FrequencyAnalyzer` provides three dedicated export representations:

```
                          FrequencyAnalyzer Internal State
                          std::array<std::size_t, 256> frequencies_
                                      │
            ┌─────────────────────────┼─────────────────────────┐
            ▼                         ▼                         ▼
   getFrequencyVector()     getUnsignedFrequencyMap()    getFrequencyMap()
   std::vector<pair<char,int>> std::unordered_map<uchar,size_t> std::unordered_map<char,int>
            │                         │                         │
            ▼                         ▼                         ▼
     Member 3 (Tree)           Member 1 (GUI)             General Lookup
     HuffmanTree::buildTree()  GUI Table & Entropy        Symbol Queries
```

### 3.1. `std::vector<std::pair<char, int>> getFrequencyVector() const`
- **Target Consumer:** Member 3's `HuffmanTree::buildTree()`.
- **Precondition Guarantees:**
  1. **Strictly Positive Counts:** Only characters with $\text{freq} > 0$ are included (filters out all zero-frequency alphabet symbols).
  2. **No Duplicate Characters:** Each unique character appears exactly once.
  3. **Deterministic Ordering:** Sorted descending by occurrence count. In case of identical frequencies, symbols are ordered strictly by ascending unsigned byte value ($0$–$255$).
  4. **Overflow Safety:** Throws `std::overflow_error` if any frequency exceeds `std::numeric_limits<int>::max()`.

### 3.2. `std::unordered_map<unsigned char, std::size_t> getUnsignedFrequencyMap() const`
- **Target Consumer:** Member 1's `HuffmanGui` desktop interface.
- **Precondition Guarantees:**
  - Keys are explicitly typed as `unsigned char` and values as `std::size_t`, matching the GUI's native table population and Shannon entropy calculations without requiring type conversions.

### 3.3. `std::unordered_map<char, int> getFrequencyMap() const`
- **Target Consumer:** Standard dictionary operations and unit test assertions.
- **Precondition Guarantees:**
  - Fast $O(1)$ average symbol lookups with signed `char` keys. Checks for integer overflow before casting.

### 3.4. `std::string getSummaryString() const`
- Formats analysis results into an aligned, human-readable summary table displaying total bytes, unique symbols, category counts, byte values ($0$–$255$), occurrence counts, and percentage probabilities.

---

## 4. Algorithmic Complexity Analysis

| Method | Time Complexity | Auxiliary Space | Operational Details |
| :--- | :---: | :---: | :--- |
| `analyze(const std::string&)` | $\Theta(N)$ | $\Theta(1)$ | Single pass over $N$ characters. Array indexing is strictly $O(1)$. No dynamic memory allocated. |
| `analyze(std::istream&)` | $\Theta(N)$ | $\Theta(1)$ | Reads data in fixed 8 KB stack buffers. Auxiliary space is bounded and independent of file size. |
| `clear()` | $\Theta(256) = O(1)$ | $O(1)$ | Resets fixed 256 array slots using `frequencies_.fill(0)`. |
| `getFrequency(ch)` | $O(1)$ | $O(1)$ | Direct array access via `frequencies_[static_cast<unsigned char>(ch)]`. |
| `getProbability(ch)` | $O(1)$ | $O(1)$ | Single array lookup and floating-point division. |
| `getCategoryBreakdown()` | $\Theta(256) = O(1)$ | $O(1)$ | Iterates over 256 fixed elements and evaluates classification predicates. |
| `getFrequencyVector()` | $O(K \log K)$ | $O(K)$ | Collects $K$ non-zero entries ($K \le 256$) and sorts them. Since $K \le 256$, comparisons $\le 256 \log_2(256) \approx 2048$ operations. |
| `getUnsignedFrequencyMap()` | $O(K)$ average | $O(K)$ | Reserves buckets for $K$ unique elements and populates the hash table. |
| `getFrequencyMap()` | $O(K)$ average | $O(K)$ | Reserves buckets for $K$ unique elements and populates the hash table. |
| `getSummaryString()` | $O(K)$ | $O(K)$ | Formats $K$ entries into an output string stream. |

---

## 5. Invariants, Safety Guarantees & Verification

1. **Memory Safety via Safe Unsigned Byte Casting:**
   - In C++, standard `char` is signed on x86/Windows, taking values in $[-128, 127]$. Accessing an array with negative indices triggers out-of-bounds memory corruption.
   - The implementation safely converts all input characters:
     ```cpp
     const auto byteIndex = static_cast<unsigned char>(c);
     ++frequencies_[byteIndex];
     ```
   - This ensures safe mapping to indices $0$ through $255$.
2. **Verified Deterministic Tie-Breaking:**
   - The vector sorting comparator implemented in `src/core/FrequencyAnalyzer.cpp`:
     ```cpp
     std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
         if (a.second != b.second) {
             return a.second > b.second; // descending frequency
         }
         return static_cast<unsigned char>(a.first) < static_cast<unsigned char>(b.first); // ascending unsigned byte value
     });
     ```
   - **Verification:** When two distinct symbols share identical frequencies (e.g. `'B'` [ASCII 66] and `'R'` [ASCII 82] each appearing 2 times in `"ABRACADABRA"`), the tie-breaker guarantees that `'B'` strictly precedes `'R'` on every run and platform.
3. **Empty Input Safety:**
   - An empty string `""` leaves counters at zero and returns empty containers without null dereferences or division-by-zero errors.

---

## 6. Byte-Level Processing vs. Multi-Byte Unicode (UTF-8)

A critical architectural characteristic of `FrequencyAnalyzer` is that **it operates strictly at the byte level ($0$–$255$), not on Unicode code points or grapheme clusters**:

- **8-Bit Octet Processing:** The analyzer treats each input file as an unencoded sequence of raw 8-bit octets.
- **UTF-8 Multi-Byte Characters:** In UTF-8 encoding, non-ASCII characters occupy 2, 3, or 4 contiguous bytes:
  - For example, Ethiopian/Amharic Fidel characters occupy **3 bytes** each in UTF-8:
    - `'ሀ'` (U+1200) is encoded as the 3-byte sequence: `0xE1 0x88 0x80`
    - `'ለ'` (U+1208) is encoded as the 3-byte sequence: `0xE1 0x88 0x88`
  - In this analyzer, analyzing `"ሀለ"` increments the counts of bytes `0xE1` (count 2), `0x88` (count 2), `0x80` (count 1), and `0x88` (further incremented).
- **Correctness in Huffman Data Compression:** Operating at the byte level is standard and correct for general-purpose text compression. Huffman coding assigns prefix bit codes to individual 8-bit bytes. When decompressed, the exact same byte sequence is recovered losslessly, completely preserving the original UTF-8 text and Unicode characters upon decoding.

---

## 7. Subsystem Limitations & Boundary Considerations

1. **Integer Representation Boundary:**
   - While `frequencies_` internally uses 64-bit `std::size_t`, Member 3's `HuffmanNode` uses signed 32-bit `int` (`char character; int frequency;`).
   - If a single symbol occurs more than $2,147,483,647$ times ($\text{INT\_MAX}$), `getFrequencyVector()` and `getFrequencyMap()` safely throw `std::overflow_error` rather than silently wrapping to negative numbers.
2. **Zero-Order Statistical Model:**
   - Character frequencies are tallied independently of context. The analyzer does not track bigrams, trigrams, or Markov transition probabilities (higher-order compression models like PPM or LZ77).
3. **Vector Return Semantics:**
   - `getFrequencyVector()`, `getFrequencyMap()`, and `getUnsignedFrequencyMap()` construct and return containers by value. In C++17, compiler Return Value Optimization (RVO) and move semantics ensure zero unnecessary copying.
