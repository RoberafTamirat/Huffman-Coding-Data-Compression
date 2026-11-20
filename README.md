# Huffman Coding for Data Compression

A high-performance C++ implementation of the **Huffman Coding algorithm** for lossless text data compression and decompression, featuring a native **Desktop Graphical User Interface (GUI)** and a **Command-Line Interface (CLI)**.

---

## 👥 Team Members & Role Delegations

| # | Member Name | Assigned Role | Expected Files Owned |
|---|---|---|---|
| **1** | **Roberaf Tamirat** | **GUI Development & Integration Coordinator** | `HuffmanGui.h`, `HuffmanGui.cpp`, `main.cpp`, `gui_documentation.md` |
| 2 | Marcimoy Assefa | Frequency Analysis & Documentation Coordinator | `FrequencyAnalyzer.h`, `FrequencyAnalyzer.cpp`, Project Report |
| 3 | Feven Asefa | Huffman Tree & Priority Queue (Core Algorithm) | `HuffmanNode.h`, `HuffmanTree.h`, `HuffmanTree.cpp` |
| 4 | Mahlet Eyasu | Huffman Encoding & Bit-Packing | `HuffmanEncoder.h`, `HuffmanEncoder.cpp` |
| 5 | Hermela Tadios | Decoding & File Management | `HuffmanDecoder.h`, `HuffmanDecoder.cpp`, `FileHandler.h`, `FileHandler.cpp` |
| 6 | Bereket Yonatan | Testing & Performance Evaluation | `CompressionAnalyzer.h`, `CompressionAnalyzer.cpp`, Unit Tests |

---

## 🚀 Key Features

- **Intuitive Desktop GUI (Member 1 - Roberaf Tamirat)**:
  - Multiline text input area with file loading capability.
  - One-click `[Compress Text]` and `[Decompress Text]` actions.
  - Interactive table displaying generated Huffman prefix codes, character frequencies, and bit lengths.
  - Real-time encoded bitstream visualization (`0`s and `1`s).
  - Decompressed original text display with automatic 100% lossless integrity verification.
  - Comprehensive compression statistics panel (original size, compressed size, compression ratio, space savings %, Shannon entropy).
  - Basic input validation and error popups.
- **Core Algorithmic Engine**:
  - Min-heap priority queue construction for optimal prefix tree building.
  - Variable-length prefix code generation (prefix-free condition).
  - Fast bitstream traversal for exact symbol reconstruction.
- **Cross-Platform CLI**:
  - Full CLI support (`--help`, `--demo`, `-c`, `-d`) for automated testing and benchmarking.

---

## 🛠️ Technologies
- **Language**: C++ (C++17 standard)
- **GUI Engine**: Native Win32 API & Common Controls (C++)
- **Build System**: CMake (>= 3.15)
- **Data Structures**: Binary Trees, Min-Heap Priority Queues, Hash Tables

---

## 📁 Project Structure

```text
Huffman-Coding-Data-Compression/
├── CMakeLists.txt                # Build configuration (C++17, huffman_lib, targets)
├── README.md                     # Project overview, roles & instructions
├── docs/
│   ├── architecture.md           # System architecture & data flow pipelines
│   └── gui_documentation.md      # GUI design, workflow & integration documentation
├── examples/
│   └── input.txt                 # Sample text for testing compression
├── include/
│   ├── core/
│   │   ├── FrequencyAnalyzer.h   # Member 2 (Marcimoy): Frequency analysis interface
│   │   ├── HuffmanNode.h         # Member 3 (Feven): Node structure & heap comparator
│   │   ├── HuffmanTree.h         # Member 3 (Feven): Tree building & prefix codes
│   │   ├── HuffmanEncoder.h      # Member 4 (Mahlet): Bitstream encoding & bit packing
│   │   ├── HuffmanDecoder.h      # Member 5 (Hermela): Tree traversal & decoding
│   │   └── CompressionAnalyzer.h # Member 6 (Bereket): Compression evaluation metrics
│   ├── file/
│   │   └── FileHandler.h         # Member 5 (Hermela): File I/O operations
│   └── gui/
│       └── HuffmanGui.h          # Member 1 (Roberaf): Win32 desktop GUI header
├── src/
│   ├── main.cpp                  # Member 1 (Roberaf): Main application entry point
│   ├── core/
│   │   ├── FrequencyAnalyzer.cpp # Member 2 (Marcimoy): Frequency analysis implementation
│   │   ├── HuffmanTree.cpp       # Member 3 (Feven): Tree construction implementation
│   │   ├── HuffmanEncoder.cpp    # Member 4 (Mahlet): Encoding & bit packing implementation
│   │   ├── HuffmanDecoder.cpp    # Member 5 (Hermela): Decoding implementation
│   │   └── CompressionAnalyzer.cpp # Member 6 (Bereket): Efficiency metrics implementation
│   ├── file/
│   │   └── FileHandler.cpp       # Member 5 (Hermela): File I/O implementations
│   └── gui/
│       └── HuffmanGui.cpp        # Member 1 (Roberaf): Win32 desktop GUI implementation
└── tests/
    ├── unit/
    │   └── test_huffman.cpp      # Unit tests (Tree, Encoder, Decoder, Stats)
    └── integration/
        └── test_file_operations.cpp # Integration tests (File I/O)
```

---

## 💻 How to Build & Run

### 1. Build via CMake (MSVC / GCC)
```bash
cmake -B build -S .
cmake --build build
```

### 2. Run the Application
```bash
# Launch the Desktop GUI (default):
./build/HuffmanCoding

# Launch via explicit flag:
./build/HuffmanCoding --gui

# Run in-memory demonstration & test:
./build/HuffmanCoding --demo

# View CLI manual:
./build/HuffmanCoding --help
```

---

## 📖 Documentation
Detailed specifications for the GUI design, user workflow, and integration contracts are available in:
- [docs/gui_documentation.md](docs/gui_documentation.md)
- [docs/architecture.md](docs/architecture.md)
