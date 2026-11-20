# Huffman Coding for Data Compression

A high-performance C++ implementation of the **Huffman Coding algorithm** for lossless text data compression and decompression, featuring both a native **Desktop Graphical User Interface (GUI)** and a **Command-Line Interface (CLI)**.

---

## 👥 Team Members & Roles

| # | Member Name | Assigned Role & Subsystem Focus |
|---|---|---|
| **1** | **Roberaf Tamirat** | **GUI Development & Integration Coordinator** |
| 2 | Marcimoy Assefa | Frequency Analysis & Symbol Encoding |
| 3 | Mahlet Eyasu | Huffman Tree Construction & Prefix Code Generation |
| 4 | Bereket Yonatan | Huffman Tree Traversal & Decoding |
| 5 | Feven Asefa | File I/O Operations & Storage |
| 6 | Hermela Tadiwos | Compression Statistics & Efficiency Evaluation |

---

## 🚀 Key Features

- **Intuitive Desktop GUI**:
  - Multiline text input area with file loading capability.
  - One-click `[Compress Text]` and `[Decompress Text]` actions.
  - Interactive table displaying generated Huffman prefix codes, character frequencies, and bit lengths.
  - Real-time encoded bitstream visualization (`0`s and `1`s).
  - Decompressed original text display with automatic 100% lossless integrity verification.
  - Comprehensive compression statistics panel (original size, compressed size, compression ratio %, space savings %, Shannon entropy).
- **Core Algorithmic Engine**:
  - Min-heap priority queue construction for optimal prefix tree building.
  - Variable-length prefix code generation (prefix-free condition).
  - Fast bitstream traversal for exact symbol reconstruction.
- **Robust Input Validation & Error Handling**:
  - Visual error alerts for empty inputs, missing trees, or invalid bit sequences.
- **Cross-Platform CLI**:
  - Full CLI support (`--help`, `--demo`, `-c`, `-d`) for automated testing and benchmarking.

---

## 🛠️ Technologies
- **Language**: C++ (C++17 standard)
- **GUI Engine**: Native Win32 API & Common Controls (C++) / Tkinter Desktop Runner
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
│   │   ├── HuffmanTree.h         # Tree node structure & priority queue interface
│   │   ├── HuffmanEncoder.h      # Frequency analysis & encoding interface
│   │   ├── HuffmanDecoder.h      # Bitstream decoding & traversal interface
│   │   └── CompressionStats.h    # Member 6 compression statistics interface
│   ├── file/
│   │   └── FileHandler.h         # Text and binary file I/O operations
│   └── gui/
│       └── HuffmanGui.h          # Win32 desktop GUI header
├── src/
│   ├── main.cpp                  # Main application entry point (GUI / CLI)
│   ├── core/
│   │   ├── HuffmanTree.cpp       # Tree construction & code generation
│   │   ├── HuffmanEncoder.cpp    # Frequency mapping & bitstream encoding
│   │   ├── HuffmanDecoder.cpp    # Tree traversal & text reconstruction
│   │   └── CompressionStats.cpp  # Statistics calculation & formatting
│   ├── file/
│   │   └── FileHandler.cpp       # File read/write implementations
│   └── gui/
│       ├── HuffmanGui.cpp        # Win32 desktop GUI implementation
│       └── huffman_gui_app.py    # Desktop GUI runner application
└── tests/
    ├── unit/
    │   └── test_huffman.cpp      # Unit tests (Tree, Encoder, Decoder, Stats)
    └── integration/
        └── test_file_operations.cpp # Integration tests (File I/O)
```

---

## 💻 How to Run

### 1. Running the Desktop GUI
To launch the interactive Desktop GUI:
```bash
# Python instant desktop GUI:
python src/gui/huffman_gui_app.py

# Or run the compiled binary directly (defaults to GUI):
./build/HuffmanCoding
# Or explicitly:
./build/HuffmanCoding --gui
```

### 2. Running the Command-Line Interface (CLI)
```bash
# View command help
./build/HuffmanCoding --help

# Run in-memory end-to-end demonstration & test
./build/HuffmanCoding --demo

# Compress a text file
./build/HuffmanCoding -c examples/input.txt compressed_output.txt

# Decompress a file
./build/HuffmanCoding -d compressed_output.txt decompressed_output.txt
```

---

## 📖 Documentation
Detailed specifications for the GUI design, user workflow, and integration contracts are available in:
- [docs/gui_documentation.md](docs/gui_documentation.md)
- [docs/architecture.md](docs/architecture.md)
