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

#include "core/HuffmanEncoder.h"
#include "core/HuffmanDecoder.h"
#include "core/CompressionStats.h"
#include "file/FileHandler.h"

/**
 * @brief Graphical User Interface for the Huffman Coding Data Compression application.
 * Developed by Roberaf Tamirat (Role 1: GUI Developer & Integration Coordinator).
 *
 * Provides full visual interaction:
 *  - Multiline text input area
 *  - Compress & Decompress buttons
 *  - Generated Huffman codes table (List-View)
 *  - Encoded binary bitstream display
 *  - Reconstructed decompressed text display
 *  - Compression statistics panel (Member 6 deliverables)
 *  - Input validation, file loading, and status messages
 */
class HuffmanGui {
public:
    HuffmanGui();
    ~HuffmanGui();

    // Prevent accidental copying
    HuffmanGui(const HuffmanGui&) = delete;
    HuffmanGui& operator=(const HuffmanGui&) = delete;

#ifdef _WIN32
    /**
     * @brief Launches the desktop application and runs the native Windows event message loop.
     */
    static int run(HINSTANCE hInstance = nullptr, int nCmdShow = SW_SHOWDEFAULT);

    /**
     * @brief Win32 Window Procedure dispatcher.
     */
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

    // Helper display updaters
    void populateCodesTable(const std::unordered_map<char, std::string>& codes,
                            const std::unordered_map<char, size_t>& freqs);
    void updateStatsDisplay(const CompressionStats& stats);
    void setStatus(const std::string& message, bool isError = false);
    void showError(const std::string& message, const std::string& title = "Validation Notice");

    // UI Utilities
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

    // Label handles for clean resizing
    HWND lblInput_{nullptr};
    HWND lblCodes_{nullptr};
    HWND lblEncoded_{nullptr};
    HWND lblDecoded_{nullptr};
    HWND lblStats_{nullptr};

    HFONT hFontUi_{nullptr};
    HFONT hFontMono_{nullptr};
    HFONT hFontBold_{nullptr};
#endif

    // Core algorithmic modules integrated by Coordinator
    HuffmanEncoder encoder_;
    HuffmanDecoder decoder_;
    FileHandler fileHandler_;

    std::string lastOriginalText_;
    std::string lastEncodedBits_;
    CompressionStats lastStats_;
    bool hasActiveEncoding_{false};
};
