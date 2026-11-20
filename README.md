# Huffman Coding for Data Compression

A modular, high-performance C++ implementation of the **Huffman Coding algorithm** for lossless text data compression, featuring a native **Desktop Graphical User Interface (GUI)** and integration coordinator.

---

## 🎯 Project Objectives
- Calculate character frequencies (letters, numbers, spaces, punctuation).
- Construct an optimal Huffman tree using a priority queue (min-heap).
- Encode text into compact binary prefix bitstreams.
- Decode compressed data losslessly back to original text.
- Evaluate compression efficiency (original vs. compressed size, compression ratio, space saved, Shannon entropy).

---

## 👥 Team Members & Role Delegations

| # | Member Name | Assigned Role | Module Files (Each Member Implements Separately) | Status |
|---|---|---|---|:---:|
| **1** | **Roberaf Tamirat** | **GUI Development & Integration Coordinator** | `include/gui/HuffmanGui.h`<br>`src/gui/HuffmanGui.cpp`<br>`src/gui/huffman_gui_app.py`<br>`src/main.cpp`<br>`docs/gui_documentation.md` | **Completed** |
| 2 | Marsimoy Tesfaye | Frequency Analysis & Documentation Coordinator | `include/core/FrequencyAnalyzer.h`<br>`src/core/FrequencyAnalyzer.cpp`<br>Project Report | *Pending Member 2* |
| 3 | Feven Asefa | Huffman Tree & Priority Queue (Core Algorithm) | `include/core/HuffmanNode.h`<br>`include/core/HuffmanTree.h`<br>`src/core/HuffmanTree.cpp` | *Pending Member 3* |
| 4 | Mahlet Eyasu | Huffman Encoding & Bit-Packing | `include/core/HuffmanEncoder.h`<br>`src/core/HuffmanEncoder.cpp` | *Pending Member 4* |
| 5 | Hermela Tadiwos | Decoding & File Management | `include/core/HuffmanDecoder.h`<br>`src/core/HuffmanDecoder.cpp`<br>`include/file/FileHandler.h`<br>`src/file/FileHandler.cpp` | *Pending Member 5* |
| 6 | Bereket Yonatan | Testing & Performance Evaluation | `include/core/CompressionAnalyzer.h`<br>`src/core/CompressionAnalyzer.cpp`<br>`tests/` | *Pending Member 6* |

---

## 🚀 Key Features (Role 1: Roberaf Tamirat)

- **Desktop Graphical Interface**:
  - Multiline text input area with file loading capability (`.txt`).
  - One-click `[Compress Text]` and `[Decompress Text]` actions.
  - Interactive table displaying generated Huffman prefix codes, character frequencies, and bit lengths.
  - Real-time encoded bitstream visualization (`0`s and `1`s).
  - Decompressed original text display with automatic 100% lossless integrity verification.
  - Comprehensive compression statistics panel (original size, compressed size, compression ratio, space savings %, Shannon entropy).
  - Robust input validation and error popup dialogs.
- **Integration Architecture**:
  - Standardized interfaces in `include/` designed for seamless linking with other members' modules upon pull request merge.

---

## 📁 Repository Structure (Member 1 Initial Push)

```text
Huffman-Coding-Data-Compression/
├── CMakeLists.txt                # Build configuration (C++17, huffman_gui_lib)
├── README.md                     # Project overview, roles & instructions
├── .gitignore                    # Git ignore rules for build artifacts & IDEs
├── docs/
│   ├── architecture.md           # System architecture & integration contracts
│   └── gui_documentation.md      # GUI design, workflow & integration documentation
├── examples/
│   └── input.txt                 # Sample text for testing compression
├── include/
│   └── gui/
│       └── HuffmanGui.h          # Member 1 (Roberaf): Win32 desktop GUI header
└── src/
    ├── main.cpp                  # Member 1 (Roberaf): Main application entry point
    └── gui/
        ├── HuffmanGui.cpp        # Member 1 (Roberaf): Win32 desktop GUI implementation
        └── huffman_gui_app.py    # Member 1 (Roberaf): Desktop GUI interactive runner
```

---

## 💻 How to Run the Desktop GUI

Run the interactive desktop interface directly:
```bash
python src/gui/huffman_gui_app.py
```

Or build with CMake (MSVC / GCC):
```bash
cmake -B build -S .
cmake --build build
./build/HuffmanCoding
```

---

## 📌 Instructions for Team Members 2, 3, 4, 5, and 6

Each member must implement their assigned module on their own branch and submit a Pull Request:
1. Clone the repository:
   ```bash
   git clone https://github.com/RoberafTamirat/Huffman-Coding-Data-Compression.git
   ```
2. Create your own feature branch:
   - Member 2: `git checkout -b feature/frequency-analyzer`
   - Member 3: `git checkout -b feature/huffman-tree`
   - Member 4: `git checkout -b feature/huffman-encoder`
   - Member 5: `git checkout -b feature/decoder-file-handling`
   - Member 6: `git checkout -b feature/testing-performance`
3. Add your assigned files into `include/` and `src/`.
4. Commit your work under your own name and push to GitHub!
