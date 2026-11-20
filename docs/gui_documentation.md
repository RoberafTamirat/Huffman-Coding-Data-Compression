# GUI Architecture, Workflow & Integration Documentation
**Author:** Roberaf Tamirat  
**Role:** GUI Developer and Integration Coordinator (Member 1)  
**Project:** Huffman Coding for Data Compression  

---

## 1. Overview & Responsibility
As the **GUI Developer and Integration Coordinator**, Member 1 (Roberaf Tamirat) is responsible for:
1. Designing and building an intuitive, responsive desktop Graphical User Interface (GUI) in pure C++ using the native Windows Win32 API and Common Controls.
2. Integrating the individual modules developed by the other five team members through agreed interface contracts:
   - **Member 2 (Marcimoy Assefa):** `FrequencyAnalyzer` (`FrequencyAnalyzer.h` / `FrequencyAnalyzer.cpp`)
   - **Member 3 (Feven Asefa):** `HuffmanNode` and `HuffmanTree` (`HuffmanNode.h`, `HuffmanTree.h` / `HuffmanTree.cpp`)
   - **Member 4 (Mahlet Eyasu):** `HuffmanEncoder` (`HuffmanEncoder.h` / `HuffmanEncoder.cpp`)
   - **Member 5 (Hermela Tadios):** `HuffmanDecoder` and `FileHandler` (`HuffmanDecoder.h`, `FileHandler.h`)
   - **Member 6 (Bereket Yonatan):** `CompressionAnalyzer` (`CompressionAnalyzer.h` / `CompressionAnalyzer.cpp`)
3. Providing input validation, error alerts, and status notifications.
4. Orchestrating the application entry point (`main.cpp`) and the build configuration (`CMakeLists.txt`).

---

## 2. Desktop Interface Design & Wireframe

The GUI layout is organized into 5 clearly demarcated functional zones designed for clarity and ease of use:

```text
+------------------------------------------------------------------------------------------------------+
|  [Header Banner] Huffman Coding Data Compression System — Roberaf Tamirat (Role 1)                   |
+------------------------------------------------------------------------------------------------------+
|  1. Source Text Input:                                       [📂 Load Text File...]  [ Clear All ]  |
|  +------------------------------------------------------------------------------------------------+  |
|  | Enter or paste uncompressed source text here...                                                |  |
|  +------------------------------------------------------------------------------------------------+  |
|                                                                                                      |
|  [ 🗜️ Compress Text ]     [ 🔓 Decompress Text ]                                                      |
|                                                                                                      |
|  +--------------------------------------------+  +------------------------------------------------+  |
|  | 2. Generated Huffman Codes Table:          |  | 3. Encoded Bitstream Output (0s & 1s):         |  |
|  | +-------+-------+------+--------------+----+  | +--------------------------------------------+ |  |
|  | |Symbol | ASCII | Freq | Huffman Code |Bits|  | | 0110100111000101011010001...                 | |  |
|  | +-------+-------+------+--------------+----+  | +--------------------------------------------+ |  |
|  | | 'e'   |  101  |  16  | 00           |  2 |  |                                                |  |
|  | | ' '   |   32  |   4  | 1101         |  4 |  | 4. Reconstructed Decompressed Text:           |  |
|  | | 'h'   |  104  |   1  | 11110        |  5 |  | +--------------------------------------------+ |  |
|  | +-------+-------+------+--------------+----+  | | huffman coding for data compression        | |  |
|  |                                            |  | +--------------------------------------------+ |  |
|  +--------------------------------------------+  +------------------------------------------------+  |
|                                                                                                      |
|  5. Compression Statistics & Metrics (Member 6 Deliverables):                                        |
|  +------------------------------------------------------------------------------------------------+  |
|  | Original: 35 bytes (280 bits) | Compressed: 18 bytes (141 bits) | Space Saved: 48.6%           |  |
|  | Compression Ratio: 1.94 : 1  | Avg Code Length: 4.03 bits/char | Shannon Entropy: 3.82 bits     |  |
|  +------------------------------------------------------------------------------------------------+  |
|  [Status Bar] Ready | Lossless verification: 100% Match                                              |
+------------------------------------------------------------------------------------------------------+
```

---

## 3. Interface Components Breakdown

| Component | Control Type | Description |
|---|---|---|
| **Source Text Input** | Multiline Edit Control (`IDC_EDIT_INPUT`) | Accepts direct user keyboard input, pasted clipboard text, or populated from disk via the file loader. |
| **Load Text File** | Push Button (`IDC_BTN_LOAD_FILE`) | Launches a native Open File Dialog to import `.txt` files directly into the input area. |
| **Clear All** | Push Button (`IDC_BTN_CLEAR`) | Resets all input boxes, code tables, output fields, and metrics to their default clean state. |
| **Compress Text** | Action Button (`IDC_BTN_COMPRESS`) | Triggers frequency counting, tree building, prefix code generation, bitstream encoding, and stats computation. |
| **Decompress Text** | Action Button (`IDC_BTN_DECOMPRESS`) | Takes the active encoded bitstream and traverses the Huffman tree to reconstruct the exact original text. |
| **Huffman Codes Table** | List-View (`IDC_LIST_CODES`) | Displays a multi-column table showing Symbol, ASCII value, Occurrence Frequency, Prefix Binary Code, and Bit Length. |
| **Encoded Bitstream** | Read-Only Edit Control (`IDC_EDIT_ENCODED`) | Displays the sequence of `'0'`s and `'1'`s generated by encoding. Allows selection and copying. |
| **Decompressed Text** | Read-Only Edit Control (`IDC_EDIT_DECODED`) | Displays the decoded string for visual side-by-side verification against the original input. |
| **Compression Statistics** | Metrics Panel (`IDC_EDIT_STATS`) | Formats metrics computed by Member 6: Original Size, Compressed Size, Ratio, Space Savings (%), and Entropy. |
| **Status Bar** | Common Control (`IDC_STATUS_BAR`) | Displays real-time progress, character/bit counts, error alerts, and lossless verification status. |

---

## 4. End-to-End User Workflow

```mermaid
flowchart TD
    A([User launches application]) --> B[Source text entered or file loaded]
    B --> C{User clicks [Compress Text]}
    C -->|Empty Input| D[Validation Alert: Text is empty]
    D --> B
    C -->|Valid Text| E[FrequencyAnalyzer counts character frequencies]
    E --> F[HuffmanTree builds Min-Heap & generates prefix codes]
    F --> G[HuffmanEncoder transforms text to bitstream]
    G --> H[CompressionAnalyzer computes size, ratios & entropy]
    H --> I[UI displays codes table, bitstream & statistics]
    I --> J{User clicks [Decompress Text]}
    J -->|No active tree/bits| K[Validation Alert: Compress first]
    K --> I
    J -->|Valid bitstream| L[HuffmanDecoder traverses tree]
    L --> M[Decompressed text rendered on screen]
    M --> N{Integrity Check: Decoded == Original?}
    N -->|Match| O[Status Bar: Lossless Verification 100% PASSED]
    N -->|Mismatch| P[Status Bar: Warning Mismatch]
```

---

## 5. How the GUI Communicates with Compression Modules

The GUI acts as a high-level **Coordinator & Controller**, decoupling user actions from low-level data structures:

```mermaid
sequenceDiagram
    autonumber
    actor User
    participant GUI as HuffmanGui (Role 1: Roberaf)
    participant Freq as FrequencyAnalyzer (Role 2: Marcimoy)
    participant Tree as HuffmanTree (Role 3: Feven)
    participant Enc as HuffmanEncoder (Role 4: Mahlet)
    participant Dec as HuffmanDecoder (Role 5: Hermela)
    participant File as FileHandler (Role 5: Hermela)
    participant Stats as CompressionAnalyzer (Role 6: Bereket)

    User->>GUI: Clicks [Load Text File]
    GUI->>File: readTextFile(filePath)
    File-->>GUI: Returns raw string content
    GUI->>GUI: Displays text in Input Area

    User->>GUI: Clicks [Compress Text]
    GUI->>GUI: Validate text not empty
    GUI->>Freq: analyze(rawText)
    Freq-->>GUI: Returns frequencies map
    GUI->>Tree: buildTree(frequencies)
    GUI->>Tree: generateCodes()
    Tree-->>GUI: Returns prefix codes map
    GUI->>Enc: encode(rawText, codes)
    Enc-->>GUI: Returns encodedBits string
    GUI->>Stats: evaluate(rawText, encodedBits, freqs, codes)
    Stats-->>GUI: Returns CompressionMetrics struct
    GUI->>GUI: Populates Codes Table, Bitstream & Stats Display

    User->>GUI: Clicks [Decompress Text]
    GUI->>Dec: decode(encodedBits, huffmanTree)
    Dec-->>GUI: Returns reconstructed text
    GUI->>GUI: Compare decoded == original (100% verification)
    GUI->>User: Displays decompressed text & success alert
```

---

## 6. Input Validation & Error Handling

To ensure robust operation and prevent crashes:
1. **Empty Input Check**: If the user presses `[Compress Text]` with an empty input area, an alert is triggered:
   `"Input text area is empty! Please type text into the input area or click 'Load Text File...'."`
2. **Missing Tree/Bits Check**: If `[Decompress Text]` is clicked before any compression has taken place, the system alerts:
   `"No compressed bitstream available to decompress. Please enter text and click [Compress Text] first."`
3. **File I/O Verification**: If a user selects an inaccessible or non-existent file, `FileHandler` safely catches errors and notifies the user via the status bar.
4. **Non-Printable Character Formatting**: Non-printable characters (like newlines `\n`, carriage returns `\r`, tabs `\t`, and spaces) are formatted with readable labels like `[SPACE]`, `[LF \n]`, `[TAB]` or hex representations in the codes table so they do not break UI formatting.
