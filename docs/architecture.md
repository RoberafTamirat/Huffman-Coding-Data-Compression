# System Architecture: Huffman Coding for Data Compression

## 1. Overview
This project is a modular, high-performance C++ implementation of the **Huffman Coding algorithm** for lossless text data compression and decompression. The architecture is explicitly organized to support collaborative development across six team members, keeping modules decoupled through strict interface contracts.

This branch represents **Member 1 (Roberaf Tamirat — GUI Developer & Integration Coordinator)**. It provides the central Desktop Graphical User Interface (GUI), main entry point, and coordinator engine, ready to integrate each member's module as they complete their assignments.

---

## 2. Component Diagram

```mermaid
graph TD
    User([User]) --> GUI[GUI / HuffmanGui Module - Member 1]
    User --> CLI[CLI / main.cpp - Member 1]

    GUI --> Freq[FrequencyAnalyzer - Member 2]
    GUI --> Tree[HuffmanTree - Member 3]
    GUI --> Encoder[HuffmanEncoder - Member 4]
    GUI --> Decoder[HuffmanDecoder - Member 5]
    GUI --> FileHandler[FileHandler - Member 5]
    GUI --> Analyzer[CompressionAnalyzer - Member 6]

    FileHandler --> FS[(File System)]
```

---

## 3. Directory Layout & Module Responsibilities

```text
Huffman-Coding-Data-Compression/
├── CMakeLists.txt              # Top-level build configuration (C++17, huffman_gui_lib)
├── .gitignore                  # Git ignore rules for build artifacts & IDEs
├── README.md                   # Project overview, team roster & execution guide
├── docs/
│   ├── architecture.md         # System architecture blueprint & team guidelines
│   └── gui_documentation.md    # GUI design, workflow & module communication
├── examples/
│   └── input.txt               # Sample text for testing compression
├── include/
│   └── gui/
│       └── HuffmanGui.h        # Member 1 (Roberaf): Win32 desktop GUI interface
└── src/
    ├── main.cpp                # Member 1 (Roberaf): Entry point (GUI / CLI dispatch)
    └── gui/
        ├── HuffmanGui.cpp      # Member 1 (Roberaf): Win32 desktop GUI implementation
        └── huffman_gui_app.py  # Member 1 (Roberaf): Interactive desktop runner
```

---

## 4. Subsystem Details & Team Work Breakdown

| Subsystem | Source / Header Files | Primary Responsibilities | Team Focus |
| :--- | :--- | :--- | :--- |
| **GUI & Coordinator** | `include/gui/HuffmanGui.h`<br>`src/gui/HuffmanGui.cpp`<br>`src/gui/huffman_gui_app.py` | Desktop UI window, input controls, codes table, bitstream & decompressed text displays, input validation, module integration. | **Member 1 (Roberaf Tamirat)** |
| **Frequency Analysis** | `include/core/FrequencyAnalyzer.h`<br>`src/core/FrequencyAnalyzer.cpp` | Frequency table computation, handling letters/numbers/spaces/punctuation, unsigned char mapping. | **Member 2 (Marcimoy Assefa)** |
| **Huffman Tree** | `include/core/HuffmanNode.h`<br>`include/core/HuffmanTree.h`<br>`src/core/HuffmanTree.cpp` | `HuffmanNode` priority queue min-heap construction, recursive tree traversal, prefix code assignment. | **Member 3 (Feven Asefa)** |
| **Encoder** | `include/core/HuffmanEncoder.h`<br>`src/core/HuffmanEncoder.cpp` | Text-to-bitstream encoding, packing bits into `uint8_t` bytes, tracking valid bits in final byte. | **Member 4 (Mahlet Eyasu)** |
| **Decoder & File I/O** | `include/core/HuffmanDecoder.h`<br>`src/core/HuffmanDecoder.cpp`<br>`include/file/FileHandler.h`<br>`src/file/FileHandler.cpp` | Tree traversal driven by bitstreams, text and binary file reading/writing, serialized compressed file storage. | **Member 5 (Hermela Tadios)** |
| **Testing & Evaluation** | `include/core/CompressionAnalyzer.h`<br>`src/core/CompressionAnalyzer.cpp`<br>`tests/` | Automated test suites, edge cases, compression ratio formula (`Original / Compressed`), space savings % formula. | **Member 6 (Bereket Yonatan)** |
| **Application Dispatch** | `src/main.cpp` | Command-line argument parsing (`--help`, `--demo`, `-c`, `-d`) & default GUI launch. | **Member 1 (Roberaf Tamirat)** |

---

## 5. Build & Execution Instructions

```bash
# Run the Desktop GUI directly:
python src/gui/huffman_gui_app.py

# Or build via CMake:
cmake -B build -S .
cmake --build build
./build/HuffmanCoding
```
