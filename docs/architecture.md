# System Architecture: Huffman Coding for Data Compression

## 1. Overview
This project is a modular, high-performance C++ implementation of the **Huffman Coding algorithm** for lossless text compression and decompression. The architecture is explicitly organized to support collaborative development across six team members, keeping modules decoupled through strict interface contracts.

The application features a modern **Desktop Graphical User Interface (GUI)** for interactive operation alongside a command-line interface (CLI) for automated pipelines.

---

## 2. Component Diagram

```mermaid
graph TD
    User([User]) --> GUI[GUI / HuffmanGui Module]
    User --> CLI[CLI / main.cpp]

    GUI --> Encoder[HuffmanEncoder Module]
    GUI --> Decoder[HuffmanDecoder Module]
    GUI --> Stats[CompressionStats Module]
    GUI --> FileHandler[FileHandler Module]

    CLI --> FileHandler
    CLI --> Encoder
    CLI --> Decoder
    CLI --> Stats

    Encoder --> Tree[HuffmanTree & HuffmanNode]
    Decoder --> Tree
    Stats --> Encoder

    FileHandler --> FS[(File System)]
```

---

## 3. Directory Layout & Module Responsibilities

```text
Huffman-Coding-Data-Compression/
├── CMakeLists.txt              # Top-level build configuration (C++17, huffman_lib)
├── .gitignore                  # Git ignore rules for build artifacts & IDEs
├── README.md                   # Project overview, team roster & execution guide
├── docs/
│   ├── architecture.md         # System architecture blueprint & team guidelines
│   └── gui_documentation.md    # GUI design, workflow & module communication
├── examples/
│   └── input.txt               # Sample text for testing compression
├── include/
│   ├── core/
│   │   ├── HuffmanTree.h       # Tree node structure & tree building interface
│   │   ├── HuffmanEncoder.h    # Frequency analysis & encoding interface
│   │   ├── HuffmanDecoder.h    # Bitstream decoding interface
│   │   └── CompressionStats.h  # Compression metrics & evaluation interface
│   ├── file/
│   │   └── FileHandler.h       # Text & binary file I/O operations
│   └── gui/
│       └── HuffmanGui.h        # Win32 desktop GUI interface
├── src/
│   ├── main.cpp                # Application entry point (GUI / CLI dispatch)
│   ├── core/
│   │   ├── HuffmanTree.cpp     # Tree operations & prefix code generation
│   │   ├── HuffmanEncoder.cpp  # Frequency table & text encoding
│   │   ├── HuffmanDecoder.cpp  # Bitstream traversal & text reconstruction
│   │   └── CompressionStats.cpp# Metrics computation & summary formatting
│   ├── file/
│   │   └── FileHandler.cpp     # File I/O implementations
│   └── gui/
│       ├── HuffmanGui.cpp      # Win32 desktop GUI implementation
│       └── huffman_gui_app.py  # Interactive desktop GUI runner
└── tests/
    ├── unit/
    │   └── test_huffman.cpp    # Unit tests for core algorithms & statistics
    └── integration/
        └── test_file_operations.cpp # Integration tests for file read/write
```

---

## 4. Subsystem Details & Team Work Breakdown

| Subsystem | Source / Header Files | Primary Responsibilities | Team Focus |
| :--- | :--- | :--- | :--- |
| **GUI & Coordinator** | `include/gui/HuffmanGui.h`<br>`src/gui/HuffmanGui.cpp`<br>`src/gui/huffman_gui_app.py` | Desktop UI window, input controls, codes table, bitstream & decompressed text displays, input validation, module integration. | **Member 1 (Roberaf Tamirat)** |
| **Huffman Tree** | `include/core/HuffmanTree.h`<br>`src/core/HuffmanTree.cpp` | `HuffmanNode` priority queue min-heap construction, recursive tree traversal, prefix code assignment. | **Member 3 (Mahlet Eyasu)** |
| **Encoder** | `include/core/HuffmanEncoder.h`<br>`src/core/HuffmanEncoder.cpp` | Frequency table computation, encoding text into binary bitstreams. | **Member 2 (Marcimoy Assefa)** |
| **Decoder** | `include/core/HuffmanDecoder.h`<br>`src/core/HuffmanDecoder.cpp` | Tree traversal driven by bitstreams (`0` for left, `1` for right) to reconstruct original uncompressed text. | **Member 4 (Bereket Yonatan)** |
| **File I/O** | `include/file/FileHandler.h`<br>`src/file/FileHandler.cpp` | Text/binary file reading/writing, byte buffer conversions, file size checks. | **Member 5 (Feven Asefa)** |
| **Metrics & Stats** | `include/core/CompressionStats.h`<br>`src/core/CompressionStats.cpp` | Original vs. compressed metrics, compression ratio %, space savings %, entropy. | **Member 6 (Hermela Tadiwos)** |
| **Application Dispatch** | `src/main.cpp` | Command-line argument parsing (`--help`, `--demo`, `-c`, `-d`) & default GUI launch. | **Member 1 (Roberaf Tamirat)** |
| **Testing & QA** | `tests/unit/`<br>`tests/integration/` | Automated test suites ensuring regression-free changes across modules. | All Members |

---

## 5. Build & Execution Instructions

```bash
# 1. Run the Desktop GUI directly
python src/gui/huffman_gui_app.py

# 2. Build via CMake (MSVC / GCC)
cmake -B build -S .
cmake --build build

# 3. Run the compiled binary (launches Desktop GUI by default)
./build/HuffmanCoding

# 4. Run CLI demonstration & unit tests
./build/HuffmanCoding --demo
./build/tests/test_huffman
./build/tests/test_file_operations
```
