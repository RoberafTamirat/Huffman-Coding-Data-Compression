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
#include <vector>
#include <memory>
#include <cstdint>

/**
 * @brief Compression evaluation metrics structure for GUI display.
 * Designed to receive metrics supplied by Member 6 (Bereket Yonatan).
 */
struct GuiMetrics {
    size_t originalSizeBytes{0};
    size_t originalSizeBits{0};
    size_t compressedPayloadBytes{0};
    size_t compressedPayloadBits{0};
    size_t uniqueCharacters{0};
    double compressionRatio{0.0};
    double spaceSavedPercent{0.0};
    double averageCodeLength{0.0};
    double entropy{0.0};

    [[nodiscard]] std::string format() const;
};

/**
 * @file HuffmanGui.h
 * @brief Graphical User Interface for the Huffman Coding Data Compression application.
 * @author Member 1: Roberaf Tamirat (Role 1: GUI Developer & Integration Coordinator)
 *
 * Responsibilities:
 *  - Design simple and understandable desktop interface (Task 1).
 *  - Text input area (Task 2).
 *  - Compress button (Task 3).
 *  - Decompress button (Task 4).
 *  - Display generated Huffman codes table (Task 5).
 *  - Display encoded bitstream output (Task 6).
 *  - Display decompressed original text (Task 7).
 *  - Display compression statistics supplied by Member 6 (Task 8).
 *  - Basic input validation and error dialogs (Task 9).
 *  - Integration coordinator providing hooks for team members' modules (Task 10).
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
    void updateStatsDisplay(const GuiMetrics& metrics);
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

    // Internal coordinator engine for standalone GUI execution prior to team merge
    void internalAnalyze(const std::string& text);
    std::string internalEncode(const std::string& text);
    std::string internalDecode(const std::string& bitString);

    // State
    std::string lastOriginalText_;
    std::string lastEncodedBits_;
    std::unordered_map<unsigned char, std::string> currentCodes_;
    std::unordered_map<unsigned char, size_t> currentFrequencies_;
    GuiMetrics lastMetrics_;
    bool hasActiveEncoding_{false};
};
