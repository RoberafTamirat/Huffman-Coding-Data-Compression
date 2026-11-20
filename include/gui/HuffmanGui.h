#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <commctrl.h>
#endif

#include <string>
#include <unordered_map>
#include <memory>

// Integrated modules from team members
#include "core/FrequencyAnalyzer.h"   // Member 2: Marcimoy Assefa
#include "core/HuffmanNode.h"          // Member 3: Feven Asefa
#include "core/HuffmanTree.h"          // Member 3: Feven Asefa
#include "core/HuffmanEncoder.h"       // Member 4: Mahlet Eyasu
#include "core/HuffmanDecoder.h"       // Member 5: Hermela Tadios
#include "file/FileHandler.h"          // Member 5: Hermela Tadios
#include "core/CompressionAnalyzer.h"  // Member 6: Bereket Yonatan

/**
 * @file HuffmanGui.h
 * @brief Graphical User Interface for the Huffman Coding Data Compression application.
 * @author Member 1: Roberaf Tamirat (GUI Developer & Integration Coordinator)
 *
 * Responsibilities:
 *  - Design simple, understandable desktop interface.
 *  - Text input area & file loading.
 *  - Compress button & Decompress button.
 *  - Display Huffman codes table.
 *  - Display encoded bitstream.
 *  - Display decompressed original text with 100% lossless verification.
 *  - Display compression statistics supplied by Member 6.
 *  - Input validation and error handling.
 *  - Module integration coordinator.
 */
class HuffmanGui {
public:
    HuffmanGui();
    ~HuffmanGui();

    HuffmanGui(const HuffmanGui&) = delete;
    HuffmanGui& operator=(const HuffmanGui&) = delete;

#ifdef _WIN32
    static int run(HINSTANCE hInstance = nullptr, int nCmdShow = SW_SHOWDEFAULT);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#else
    static int run();
#endif

private:
#ifdef _WIN32
    bool initialize(HINSTANCE hInstance, int nCmdShow);
    void createControls(HWND hwnd);
    void setupFonts();
    void resizeControls(int width, int height);

    // Event handlers
    void onCompress();
    void onDecompress();
    void onClear();
    void onLoadFile();

    // UI population helpers
    void populateCodesTable(const std::unordered_map<unsigned char, std::string>& codes,
                            const std::unordered_map<unsigned char, size_t>& freqs);
    void updateStatsDisplay(const CompressionMetrics& metrics);
    void setStatus(const std::string& message, bool isError = false);
    void showError(const std::string& message, const std::string& title = "Validation Notice");

    std::string getControlText(HWND controlHwnd) const;
    void setControlText(HWND controlHwnd, const std::string& text);

    // Window and Control Handles
    HWND hwndMain_{nullptr};
    HWND hwndInputText_{nullptr};
    HWND hwndBtnCompress_{nullptr};
    HWND hwndBtnDecompress_{nullptr};
    HWND hwndBtnClear_{nullptr};
    HWND hwndBtnLoadFile_{nullptr};
    HWND hwndListCodes_{nullptr};
    HWND hwndEncodedText_{nullptr};
    HWND hwndDecodedText_{nullptr};
    HWND hwndStatsText_{nullptr};
    HWND hwndStatusBar_{nullptr};

    // Label handles
    HWND lblInput_{nullptr};
    HWND lblCodes_{nullptr};
    HWND lblEncoded_{nullptr};
    HWND lblDecoded_{nullptr};
    HWND lblStats_{nullptr};

    HFONT hFontUi_{nullptr};
    HFONT hFontMono_{nullptr};
    HFONT hFontBold_{nullptr};
#endif

    // Team member modules integrated by Coordinator (Roberaf)
    FrequencyAnalyzer frequencyAnalyzer_; // Member 2
    HuffmanTree huffmanTree_;             // Member 3
    HuffmanEncoder encoder_;              // Member 4
    HuffmanDecoder decoder_;              // Member 5
    FileHandler fileHandler_;             // Member 5

    // State for encode/decode roundtrip
    std::string lastOriginalText_;
    std::string lastEncodedBits_;
    std::unordered_map<unsigned char, std::string> currentCodes_;
    std::unordered_map<unsigned char, size_t> currentFrequencies_;
    CompressionMetrics lastMetrics_;
    bool hasActiveEncoding_{false};
};
