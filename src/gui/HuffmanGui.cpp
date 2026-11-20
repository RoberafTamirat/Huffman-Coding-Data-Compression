#include "gui/HuffmanGui.h"

#ifdef _WIN32
#include <commdlg.h>
#include <sstream>
#include <vector>
#include <iomanip>

#pragma comment(lib, "comctl32.lib")

// Control Identifiers
constexpr int IDC_EDIT_INPUT     = 1001;
constexpr int IDC_BTN_COMPRESS   = 1002;
constexpr int IDC_BTN_DECOMPRESS = 1003;
constexpr int IDC_BTN_CLEAR      = 1004;
constexpr int IDC_BTN_LOAD_FILE  = 1005;
constexpr int IDC_LIST_CODES     = 1006;
constexpr int IDC_EDIT_ENCODED   = 1007;
constexpr int IDC_EDIT_DECODED   = 1008;
constexpr int IDC_EDIT_STATS     = 1009;
constexpr int IDC_STATUS_BAR     = 1010;

// Static pointer for Window Procedure routing
static HuffmanGui* g_guiInstance = nullptr;

HuffmanGui::HuffmanGui() = default;

HuffmanGui::~HuffmanGui() {
    if (hFontUi_)   DeleteObject(hFontUi_);
    if (hFontMono_) DeleteObject(hFontMono_);
    if (hFontBold_) DeleteObject(hFontBold_);
}

int HuffmanGui::run(HINSTANCE hInstance, int nCmdShow) {
    if (!hInstance) {
        hInstance = GetModuleHandle(nullptr);
    }

    // Initialize Windows Common Controls (ListView, StatusBar)
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    HuffmanGui app;
    g_guiInstance = &app;

    if (!app.initialize(hInstance, nCmdShow)) {
        MessageBoxA(nullptr, "Failed to initialize Huffman GUI window.", "Startup Error", MB_ICONERROR);
        return 1;
    }

    // Main Win32 Event Message Loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_guiInstance = nullptr;
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK HuffmanGui::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!g_guiInstance) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
        case WM_CREATE: {
            g_guiInstance->hwndMain_ = hwnd;
            g_guiInstance->setupFonts();
            g_guiInstance->createControls(hwnd);
            return 0;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            g_guiInstance->resizeControls(width, height);
            return 0;
        }

        case WM_COMMAND: {
            int controlId = LOWORD(wParam);
            int notification = HIWORD(wParam);

            if (notification == BN_CLICKED) {
                switch (controlId) {
                    case IDC_BTN_COMPRESS:
                        g_guiInstance->onCompress();
                        break;
                    case IDC_BTN_DECOMPRESS:
                        g_guiInstance->onDecompress();
                        break;
                    case IDC_BTN_CLEAR:
                        g_guiInstance->onClear();
                        break;
                    case IDC_BTN_LOAD_FILE:
                        g_guiInstance->onLoadFile();
                        break;
                    default:
                        break;
                }
            }
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

bool HuffmanGui::initialize(HINSTANCE hInstance, int nCmdShow) {
    const char CLASS_NAME[] = "HuffmanCodingMainWindowClass";

    WNDCLASSEXA wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXA);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = HuffmanGui::WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExA(&wc)) {
        return false;
    }

    hwndMain_ = CreateWindowExA(
        0,
        CLASS_NAME,
        "Huffman Coding Data Compression System - GUI Integration (Roberaf Tamirat)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1080, 760,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwndMain_) {
        return false;
    }

    ShowWindow(hwndMain_, nCmdShow);
    UpdateWindow(hwndMain_);
    return true;
}

void HuffmanGui::setupFonts() {
    hFontUi_ = CreateFontA(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    hFontBold_ = CreateFontA(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    hFontMono_ = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
}

void HuffmanGui::createControls(HWND hwnd) {
    HINSTANCE hInst = GetModuleHandle(nullptr);

    // Section 1: Input text area and buttons
    lblInput_ = CreateWindowExA(0, "STATIC", "1. Source Text Input (Enter text to compress or load from file):",
                                WS_CHILD | WS_VISIBLE,
                                20, 10, 500, 20, hwnd, nullptr, hInst, nullptr);

    hwndBtnLoadFile_ = CreateWindowExA(0, "BUTTON", "Load Text File...",
                                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                       760, 10, 130, 26, hwnd, (HMENU)IDC_BTN_LOAD_FILE, hInst, nullptr);

    hwndBtnClear_ = CreateWindowExA(0, "BUTTON", "Clear All",
                                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                    900, 10, 120, 26, hwnd, (HMENU)IDC_BTN_CLEAR, hInst, nullptr);

    hwndInputText_ = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                                     WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                                     20, 40, 1020, 100, hwnd, (HMENU)IDC_EDIT_INPUT, hInst, nullptr);

    // Action Control Bar
    hwndBtnCompress_ = CreateWindowExA(0, "BUTTON", "  [Compress Text]  ",
                                       WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                                       20, 150, 180, 36, hwnd, (HMENU)IDC_BTN_COMPRESS, hInst, nullptr);

    hwndBtnDecompress_ = CreateWindowExA(0, "BUTTON", "  [Decompress Text]  ",
                                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                         210, 150, 180, 36, hwnd, (HMENU)IDC_BTN_DECOMPRESS, hInst, nullptr);

    // Section 2: Split columns (Left: Huffman Codes Table, Right: Bitstream & Decoded Text)
    lblCodes_ = CreateWindowExA(0, "STATIC", "2. Generated Prefix Codes Table (Huffman Tree Leaves):",
                                WS_CHILD | WS_VISIBLE,
                                20, 195, 420, 20, hwnd, nullptr, hInst, nullptr);

    hwndListCodes_ = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, "",
                                     WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
                                     20, 220, 420, 310, hwnd, (HMENU)IDC_LIST_CODES, hInst, nullptr);
    ListView_SetExtendedListViewStyle(hwndListCodes_, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Add Columns to ListView
    LVCOLUMNA col;
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    col.pszText = const_cast<char*>("Symbol");
    col.cx = 80;
    ListView_InsertColumn(hwndListCodes_, 0, &col);

    col.pszText = const_cast<char*>("ASCII");
    col.cx = 60;
    ListView_InsertColumn(hwndListCodes_, 1, &col);

    col.pszText = const_cast<char*>("Freq");
    col.cx = 65;
    ListView_InsertColumn(hwndListCodes_, 2, &col);

    col.pszText = const_cast<char*>("Huffman Code");
    col.cx = 125;
    ListView_InsertColumn(hwndListCodes_, 3, &col);

    col.pszText = const_cast<char*>("Bits");
    col.cx = 55;
    ListView_InsertColumn(hwndListCodes_, 4, &col);

    // Encoded Bitstream Panel
    lblEncoded_ = CreateWindowExA(0, "STATIC", "3. Encoded Bitstream Output (0s and 1s):",
                                  WS_CHILD | WS_VISIBLE,
                                  460, 195, 400, 20, hwnd, nullptr, hInst, nullptr);

    hwndEncodedText_ = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                                       WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                                       460, 220, 580, 140, hwnd, (HMENU)IDC_EDIT_ENCODED, hInst, nullptr);

    // Decompressed Text Panel
    lblDecoded_ = CreateWindowExA(0, "STATIC", "4. Reconstructed Decompressed Text:",
                                  WS_CHILD | WS_VISIBLE,
                                  460, 370, 400, 20, hwnd, nullptr, hInst, nullptr);

    hwndDecodedText_ = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                                       WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                                       460, 395, 580, 135, hwnd, (HMENU)IDC_EDIT_DECODED, hInst, nullptr);

    // Section 3: Compression Statistics Panel (Member 6 deliverables)
    lblStats_ = CreateWindowExA(0, "STATIC", "5. Compression Metrics & Statistics (Supplied by Member 6):",
                                WS_CHILD | WS_VISIBLE,
                                20, 540, 600, 20, hwnd, nullptr, hInst, nullptr);

    hwndStatsText_ = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT",
                                     "Ready. Enter or load text above and click [Compress Text] to generate statistics.",
                                     WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
                                     20, 565, 1020, 110, hwnd, (HMENU)IDC_EDIT_STATS, hInst, nullptr);

    // Status Bar
    hwndStatusBar_ = CreateWindowExA(0, STATUSCLASSNAMEA, "Ready",
                                     WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                                     0, 0, 0, 0, hwnd, (HMENU)IDC_STATUS_BAR, hInst, nullptr);

    // Apply Fonts
    HWND uiControls[] = {
        lblInput_, hwndBtnLoadFile_, hwndBtnClear_, hwndBtnCompress_, hwndBtnDecompress_,
        lblCodes_, lblEncoded_, lblDecoded_, lblStats_
    };
    for (HWND c : uiControls) {
        SendMessage(c, WM_SETFONT, (WPARAM)hFontBold_, TRUE);
    }

    SendMessage(hwndInputText_,   WM_SETFONT, (WPARAM)hFontUi_, TRUE);
    SendMessage(hwndListCodes_,   WM_SETFONT, (WPARAM)hFontMono_, TRUE);
    SendMessage(hwndEncodedText_, WM_SETFONT, (WPARAM)hFontMono_, TRUE);
    SendMessage(hwndDecodedText_, WM_SETFONT, (WPARAM)hFontUi_, TRUE);
    SendMessage(hwndStatsText_,   WM_SETFONT, (WPARAM)hFontMono_, TRUE);
    SendMessage(hwndStatusBar_,   WM_SETFONT, (WPARAM)hFontUi_, TRUE);

    // Provide default initial sample text for instant testing
    setControlText(hwndInputText_, "huffman coding for data compression");
}

void HuffmanGui::resizeControls(int width, int height) {
    if (width <= 0 || height <= 0 || !hwndInputText_) return;

    int pad = 20;
    int topHeight = 100;

    // Source input text
    SetWindowPos(hwndBtnLoadFile_, nullptr, width - pad - 260, 10, 125, 26, SWP_NOZORDER);
    SetWindowPos(hwndBtnClear_,    nullptr, width - pad - 125, 10, 125, 26, SWP_NOZORDER);
    SetWindowPos(hwndInputText_,   nullptr, pad, 40, width - (pad * 2), topHeight, SWP_NOZORDER);

    // Buttons
    SetWindowPos(hwndBtnCompress_,   nullptr, pad, 150, 180, 36, SWP_NOZORDER);
    SetWindowPos(hwndBtnDecompress_, nullptr, pad + 190, 150, 180, 36, SWP_NOZORDER);

    // Middle area split
    int middleTop = 195;
    int bottomStatsHeight = 120;
    int statusHeight = 22;
    int middleHeight = height - middleTop - bottomStatsHeight - statusHeight - (pad * 2);

    if (middleHeight < 180) middleHeight = 180;

    int leftColWidth = (width * 42) / 100;
    int rightColLeft = pad + leftColWidth + 15;
    int rightColWidth = width - rightColLeft - pad;

    SetWindowPos(lblCodes_,      nullptr, pad, middleTop, leftColWidth, 20, SWP_NOZORDER);
    SetWindowPos(hwndListCodes_, nullptr, pad, middleTop + 25, leftColWidth, middleHeight - 25, SWP_NOZORDER);

    int halfRight = (middleHeight - 55) / 2;
    SetWindowPos(lblEncoded_,      nullptr, rightColLeft, middleTop, rightColWidth, 20, SWP_NOZORDER);
    SetWindowPos(hwndEncodedText_, nullptr, rightColLeft, middleTop + 25, rightColWidth, halfRight, SWP_NOZORDER);

    int decodedTop = middleTop + 25 + halfRight + 10;
    SetWindowPos(lblDecoded_,      nullptr, rightColLeft, decodedTop, rightColWidth, 20, SWP_NOZORDER);
    SetWindowPos(hwndDecodedText_, nullptr, rightColLeft, decodedTop + 25, rightColWidth, halfRight, SWP_NOZORDER);

    // Bottom Stats
    int statsTop = middleTop + middleHeight + 10;
    SetWindowPos(lblStats_,      nullptr, pad, statsTop, width - (pad * 2), 20, SWP_NOZORDER);
    SetWindowPos(hwndStatsText_, nullptr, pad, statsTop + 25, width - (pad * 2), bottomStatsHeight - 30, SWP_NOZORDER);

    // Status bar auto-sizes via WM_SIZE
    SendMessage(hwndStatusBar_, WM_SIZE, 0, 0);
}

void HuffmanGui::onCompress() {
    std::string text = getControlText(hwndInputText_);

    // Task 9: Input validation
    if (text.empty()) {
        showError("Input text area is empty!\nPlease type text into the input area or click 'Load Text File...'.",
                  "Input Validation Error");
        setStatus("Error: Input text is empty. Compression aborted.", true);
        return;
    }

    lastOriginalText_ = text;

    // Task 3 & 10: Run compression pipeline
    lastEncodedBits_ = encoder_.encode(text);
    const auto& codes = encoder_.getCodes();
    const auto& freqs = encoder_.getFrequencies();

    // Task 5: Display Huffman codes in table
    populateCodesTable(codes, freqs);

    // Task 6: Display encoded bitstream
    setControlText(hwndEncodedText_, lastEncodedBits_);

    // Task 8: Compute & display compression statistics
    lastStats_ = CompressionStats::calculate(text, lastEncodedBits_, freqs, codes);
    updateStatsDisplay(lastStats_);

    // Clear previous decompressed text to highlight roundtrip step
    setControlText(hwndDecodedText_, "");
    hasActiveEncoding_ = true;

    std::ostringstream msg;
    msg << "Compression completed successfully! Text of " << text.size()
        << " bytes encoded into " << lastEncodedBits_.size()
        << " bits. Space saved: " << std::fixed << std::setprecision(1)
        << lastStats_.spaceSavingPercent << "%.";
    setStatus(msg.str());
}

void HuffmanGui::onDecompress() {
    // Task 9: Input validation
    if (!hasActiveEncoding_ || lastEncodedBits_.empty()) {
        showError("No compressed bitstream available to decompress.\nPlease enter text and click [Compress Text] first.",
                  "Decompression Error");
        setStatus("Error: Decompression attempted with no encoded data.", true);
        return;
    }

    std::string encodedBits = getControlText(hwndEncodedText_);
    if (encodedBits.empty()) {
        showError("Encoded bitstream field is empty.", "Validation Notice");
        return;
    }

    // Task 4, 7 & 10: Run decompression pipeline
    std::string decoded = decoder_.decode(encodedBits, encoder_.getTree());
    setControlText(hwndDecodedText_, decoded);

    if (decoded == lastOriginalText_) {
        std::ostringstream msg;
        msg << "Decompression verified: Reconstructed text (" << decoded.size()
            << " bytes) matches original text 100% losslessly!";
        setStatus(msg.str());
    } else {
        setStatus("Warning: Decompressed text does not match original text.", true);
    }
}

void HuffmanGui::onClear() {
    setControlText(hwndInputText_, "");
    setControlText(hwndEncodedText_, "");
    setControlText(hwndDecodedText_, "");
    setControlText(hwndStatsText_, "Ready. Enter text and click [Compress Text].");
    ListView_DeleteAllItems(hwndListCodes_);
    lastOriginalText_.clear();
    lastEncodedBits_.clear();
    hasActiveEncoding_ = false;
    setStatus("Cleared all input, outputs, and tables.");
}

void HuffmanGui::onLoadFile() {
    char filename[MAX_PATH] = {0};

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hwndMain_;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        std::string content = fileHandler_.readTextFile(filename);
        if (content.empty() && !fileHandler_.fileExists(filename)) {
            showError("Could not open selected file.", "File I/O Error");
            return;
        }
        setControlText(hwndInputText_, content);
        std::ostringstream msg;
        msg << "Loaded " << content.size() << " bytes from " << filename;
        setStatus(msg.str());
    }
}

void HuffmanGui::populateCodesTable(const std::unordered_map<char, std::string>& codes,
                                    const std::unordered_map<char, size_t>& freqs) {
    ListView_DeleteAllItems(hwndListCodes_);

    int index = 0;
    for (const auto& [ch, code] : codes) {
        LVITEMA item = {};
        item.mask = LVIF_TEXT;
        item.iItem = index;
        item.iSubItem = 0;

        // Symbol display
        std::string symbolDisplay;
        if (ch == ' ')       symbolDisplay = "[SPACE]";
        else if (ch == '\n') symbolDisplay = "[LF \\n]";
        else if (ch == '\r') symbolDisplay = "[CR \\r]";
        else if (ch == '\t') symbolDisplay = "[TAB]";
        else if (std::isprint(static_cast<unsigned char>(ch))) {
            symbolDisplay = std::string(1, ch);
        } else {
            std::ostringstream hexOss;
            hexOss << "0x" << std::hex << std::uppercase << (static_cast<int>(ch) & 0xFF);
            symbolDisplay = hexOss.str();
        }

        item.pszText = const_cast<char*>(symbolDisplay.c_str());
        ListView_InsertItem(hwndListCodes_, &item);

        // Subitem 1: ASCII Code
        std::string asciiVal = std::to_string(static_cast<int>(static_cast<unsigned char>(ch)));
        ListView_SetItemText(hwndListCodes_, index, 1, const_cast<char*>(asciiVal.c_str()));

        // Subitem 2: Frequency
        size_t freq = 0;
        auto fit = freqs.find(ch);
        if (fit != freqs.end()) {
            freq = fit->second;
        }
        std::string freqStr = std::to_string(freq);
        ListView_SetItemText(hwndListCodes_, index, 2, const_cast<char*>(freqStr.c_str()));

        // Subitem 3: Huffman Code
        ListView_SetItemText(hwndListCodes_, index, 3, const_cast<char*>(code.c_str()));

        // Subitem 4: Bit length
        std::string lenStr = std::to_string(code.size());
        ListView_SetItemText(hwndListCodes_, index, 4, const_cast<char*>(lenStr.c_str()));

        ++index;
    }
}

void HuffmanGui::updateStatsDisplay(const CompressionStats& stats) {
    setControlText(hwndStatsText_, stats.formatSummary());
}

void HuffmanGui::setStatus(const std::string& message, bool isError) {
    (void)isError;
    if (hwndStatusBar_) {
        SendMessageA(hwndStatusBar_, SB_SETTEXTA, 0, (LPARAM)message.c_str());
    }
}

void HuffmanGui::showError(const std::string& message, const std::string& title) {
    MessageBoxA(hwndMain_, message.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
}

std::string HuffmanGui::getControlText(HWND controlHwnd) const {
    if (!controlHwnd) return "";
    int length = GetWindowTextLengthA(controlHwnd);
    if (length <= 0) return "";

    std::vector<char> buffer(length + 1);
    GetWindowTextA(controlHwnd, buffer.data(), length + 1);
    return std::string(buffer.data());
}

void HuffmanGui::setControlText(HWND controlHwnd, const std::string& text) {
    if (controlHwnd) {
        SetWindowTextA(controlHwnd, text.c_str());
    }
}

#else

int HuffmanGui::run() {
    return 0;
}

#endif
