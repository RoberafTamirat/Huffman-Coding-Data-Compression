"""
Huffman Coding Data Compression — Desktop Graphical User Interface
Project: Huffman Coding for Lossless Data Compression

This application provides a responsive, native-styled desktop interface implementing
all 10 assigned tasks, featuring prominent Before vs. After compression size comparisons.
"""

import math
import heapq
from collections import Counter
import tkinter as tk
from tkinter import ttk, messagebox, filedialog


# ============================================================================
# MODULE 2: FrequencyAnalyzer (Member 2: Marcimoy Assefa)
# ============================================================================
class FrequencyAnalyzer:
    @staticmethod
    def analyze(text):
        """Counts character occurrences (letters, numbers, spaces, punctuation)."""
        return Counter(text)


# ============================================================================
# MODULE 3: HuffmanNode & HuffmanTree (Member 3: Feven Asefa)
# ============================================================================
class HuffmanNode:
    def __init__(self, char, freq, left=None, right=None):
        self.char = char
        self.freq = freq
        self.left = left
        self.right = right

    def __lt__(self, other):
        return self.freq < other.freq

    def is_leaf(self):
        return self.left is None and self.right is None


class HuffmanTree:
    def __init__(self):
        self.root = None

    def build_tree(self, frequencies):
        if not frequencies:
            self.root = None
            return None

        heap = [HuffmanNode(ch, freq) for ch, freq in frequencies.items()]
        heapq.heapify(heap)

        while len(heap) > 1:
            left = heapq.heappop(heap)
            right = heapq.heappop(heap)
            parent = HuffmanNode('\0', left.freq + right.freq, left, right)
            heapq.heappush(heap, parent)

        self.root = heap[0] if heap else None
        return self.root

    def generate_codes(self):
        codes = {}
        if not self.root:
            return codes
        if self.root.is_leaf():
            codes[self.root.char] = "0"
            return codes

        def dfs(node, current_code):
            if not node:
                return
            if node.is_leaf():
                codes[node.char] = current_code
                return
            dfs(node.left, current_code + "0")
            dfs(node.right, current_code + "1")

        dfs(self.root, "")
        return codes


# ============================================================================
# MODULE 4: HuffmanEncoder (Member 4: Mahlet Eyasu)
# ============================================================================
class HuffmanEncoder:
    @staticmethod
    def encode(text, codes):
        """Transforms original text into prefix binary bitstream."""
        return "".join(codes[ch] for ch in text if ch in codes)


# ============================================================================
# MODULE 5: HuffmanDecoder (Member 5: Hermela Tadios)
# ============================================================================
class HuffmanDecoder:
    @staticmethod
    def decode(encoded_bits, root):
        """Traverses the tree to reconstruct original uncompressed text."""
        if not root or not encoded_bits:
            return ""
        if root.is_leaf():
            return root.char * len(encoded_bits)

        decoded = []
        curr = root
        for bit in encoded_bits:
            curr = curr.left if bit == '0' else curr.right
            if curr is None:
                break
            if curr.is_leaf():
                decoded.append(curr.char)
                curr = root
        return "".join(decoded)


# ============================================================================
# MODULE 6: CompressionAnalyzer (Member 6: Bereket Yonatan)
# ============================================================================
class CompressionAnalyzer:
    @staticmethod
    def evaluate(original_text, encoded_bits, frequencies, codes):
        orig_bytes = len(original_text.encode('utf-8'))
        orig_bits = orig_bytes * 8
        comp_payload_bits = len(encoded_bits)
        comp_payload_bytes = (comp_payload_bits + 7) // 8

        # Required official formulas:
        # Compression ratio = Original size / Compressed size
        comp_ratio = (orig_bytes / comp_payload_bytes) if comp_payload_bytes > 0 else 0.0

        # Space saved = ((Original size - Compressed size) / Original size) * 100
        space_saved = (((orig_bytes - comp_payload_bytes) / orig_bytes) * 100.0) if orig_bytes > 0 else 0.0

        bits_saved = orig_bits - comp_payload_bits
        bytes_saved = orig_bytes - comp_payload_bytes

        avg_code_len = (comp_payload_bits / len(original_text)) if original_text else 0.0

        # Shannon entropy
        total = float(len(original_text))
        entropy = 0.0
        if total > 0:
            for count in frequencies.values():
                p = count / total
                if p > 0:
                    entropy -= p * math.log2(p)

        return {
            "orig_bytes": orig_bytes,
            "orig_bits": orig_bits,
            "payload_bits": comp_payload_bits,
            "payload_bytes": comp_payload_bytes,
            "bits_saved": bits_saved,
            "bytes_saved": bytes_saved,
            "comp_ratio": comp_ratio,
            "space_saved": space_saved,
            "avg_code_len": avg_code_len,
            "entropy": entropy,
            "unique_chars": len(frequencies)
        }


# ============================================================================
# MODULE 1: Desktop GUI & Integration Coordinator (Member 1)
# ============================================================================
class HuffmanDesktopApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Huffman Coding Data Compression System")
        self.geometry("1160x860")
        self.minsize(1000, 720)

        self.style = ttk.Style(self)
        self.style.theme_use("clam")
        self.configure(bg="#F1F5F9")

        self.tree_module = HuffmanTree()
        self.last_original_text = ""
        self.last_encoded_bits = ""
        self.current_codes = {}
        self.current_frequencies = {}

        self._build_ui()

    def _build_ui(self):
        # --------------------------------------------------------------------
        # Header banner (Clean, professional without names or role subtitles)
        # --------------------------------------------------------------------
        header = tk.Frame(self, bg="#1E3A8A", height=50)
        header.pack(fill=tk.X, side=tk.TOP)
        header.pack_propagate(False)

        title_lbl = tk.Label(header, text="Huffman Coding Data Compression — Desktop Interface",
                             font=("Segoe UI", 14, "bold"), fg="#FFFFFF", bg="#1E3A8A")
        title_lbl.pack(anchor="w", padx=20, pady=12)

        # Status Bar at very bottom
        self.status_var = tk.StringVar(value="Ready.")
        status_bar = tk.Label(self, textvariable=self.status_var, font=("Segoe UI", 9),
                              bg="#E2E8F0", fg="#334155", anchor="w", padx=12, pady=4)
        status_bar.pack(fill=tk.X, side=tk.BOTTOM)

        # Main padded container
        container = tk.Frame(self, bg="#F1F5F9", padx=16, pady=8)
        container.pack(fill=tk.BOTH, expand=True)

        # --------------------------------------------------------------------
        # 1. Source Text Input - Packed at Top
        # --------------------------------------------------------------------
        top_box = tk.LabelFrame(container, text=" 1. Source Text Input ",
                                font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=10, pady=6)
        top_box.pack(side=tk.TOP, fill=tk.X, pady=(0, 6))

        bar = tk.Frame(top_box, bg="#FFFFFF")
        bar.pack(fill=tk.X, pady=(0, 4))

        tk.Label(bar, text="Type text below or load from file:", font=("Segoe UI", 9),
                 bg="#FFFFFF", fg="#64748B").pack(side=tk.LEFT)

        tk.Button(bar, text="Clear All", font=("Segoe UI", 9), bg="#EF4444", fg="#FFFFFF",
                  relief=tk.FLAT, padx=10, command=self.on_clear).pack(side=tk.RIGHT, padx=4)
        tk.Button(bar, text="📂 Load Text File...", font=("Segoe UI", 9), bg="#E2E8F0", fg="#1E293B",
                  relief=tk.FLAT, padx=10, command=self.on_load_file).pack(side=tk.RIGHT, padx=4)

        self.txt_input = tk.Text(top_box, height=3, font=("Segoe UI", 10), wrap=tk.WORD,
                                 relief=tk.SOLID, bd=1, padx=6, pady=4)
        self.txt_input.pack(fill=tk.X)
        self.txt_input.insert(tk.END, "huffman coding for data compression")
        self.txt_input.bind("<KeyRelease>", self.on_text_change)

        # --------------------------------------------------------------------
        # Action Buttons - Packed below Top Box
        # --------------------------------------------------------------------
        action_bar = tk.Frame(container, bg="#F1F5F9")
        action_bar.pack(side=tk.TOP, fill=tk.X, pady=(0, 6))

        self.btn_compress = tk.Button(action_bar, text="🗜️  Compress Text", font=("Segoe UI", 10, "bold"),
                                      bg="#2563EB", fg="#FFFFFF", activebackground="#1D4ED8", activeforeground="#FFFFFF",
                                      relief=tk.FLAT, padx=18, pady=6, cursor="hand2", command=self.on_compress)
        self.btn_compress.pack(side=tk.LEFT, padx=(0, 10))

        self.btn_decompress = tk.Button(action_bar, text="🔓  Decompress Text", font=("Segoe UI", 10, "bold"),
                                        bg="#10B981", fg="#FFFFFF", activebackground="#059669", activeforeground="#FFFFFF",
                                        relief=tk.FLAT, padx=18, pady=6, cursor="hand2", command=self.on_decompress)
        self.btn_decompress.pack(side=tk.LEFT)

        # --------------------------------------------------------------------
        # 5. BEFORE VS. AFTER COMPARISON & EFFICIENCY METRICS (PACKED AT BOTTOM FIRST!)
        # Packing with side=tk.BOTTOM guarantees it is ALWAYS 100% visible on every screen!
        # --------------------------------------------------------------------
        stats_frame = tk.LabelFrame(container, text=" 5. Before vs. After Compression Size Comparison & Efficiency ",
                                    font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=10, pady=8)
        stats_frame.pack(side=tk.BOTTOM, fill=tk.X, pady=(6, 0))

        cols_frame = tk.Frame(stats_frame, bg="#FFFFFF")
        cols_frame.pack(fill=tk.X)

        # Card 1: BEFORE COMPRESSION
        card1 = tk.Frame(cols_frame, bg="#EFF6FF", padx=12, pady=8, relief=tk.SOLID, bd=1)
        card1.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=4)
        tk.Label(card1, text="📁 BEFORE COMPRESSION", font=("Segoe UI", 9, "bold"),
                 bg="#EFF6FF", fg="#1E40AF").pack(anchor="w")
        self.lbl_card_before = tk.Label(card1, text="Original: 0 bytes (0 bits)\nCharacters: 0 symbols",
                                        font=("Consolas", 10), bg="#EFF6FF", fg="#1E3A8A", justify=tk.LEFT)
        self.lbl_card_before.pack(anchor="w", pady=(4, 0))

        # Card 2: AFTER COMPRESSION
        card2 = tk.Frame(cols_frame, bg="#ECFDF5", padx=12, pady=8, relief=tk.SOLID, bd=1)
        card2.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=4)
        tk.Label(card2, text="🗜️ AFTER COMPRESSION", font=("Segoe UI", 9, "bold"),
                 bg="#ECFDF5", fg="#065F46").pack(anchor="w")
        self.lbl_card_after = tk.Label(card2, text="Compressed: 0 bytes (0 bits)\nReduction: 0 bytes saved",
                                       font=("Consolas", 10), bg="#ECFDF5", fg="#047857", justify=tk.LEFT)
        self.lbl_card_after.pack(anchor="w", pady=(4, 0))

        # Card 3: EFFICIENCY METRICS
        card3 = tk.Frame(cols_frame, bg="#FEF3C7", padx=12, pady=8, relief=tk.SOLID, bd=1)
        card3.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=4)
        tk.Label(card3, text="📊 EFFICIENCY ACHIEVED", font=("Segoe UI", 9, "bold"),
                 bg="#FEF3C7", fg="#92400E").pack(anchor="w")
        self.lbl_card_metrics = tk.Label(card3, text="Ratio: 0.00 : 1 | Space Saved: 0.0%\nEntropy: 0.00 | Avg Code: 0.00",
                                         font=("Consolas", 10), bg="#FEF3C7", fg="#78350F", justify=tk.LEFT)
        self.lbl_card_metrics.pack(anchor="w", pady=(4, 0))

        # --------------------------------------------------------------------
        # Middle Split: Codes Table vs. Encoded Bitstream & Decoded Text
        # Packed in remaining space between Top controls and Bottom Stats Frame
        # --------------------------------------------------------------------
        middle_pane = tk.PanedWindow(container, orient=tk.HORIZONTAL, bg="#CBD5E1", sashwidth=6)
        middle_pane.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        # Left: Codes Table
        left_box = tk.LabelFrame(middle_pane, text=" 2. Generated Prefix Codes Table (Huffman Tree) ",
                                 font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        middle_pane.add(left_box, minsize=420)

        cols = ("Symbol", "ASCII", "Freq", "Huffman Code", "Bits")
        self.tree_codes = ttk.Treeview(left_box, columns=cols, show="headings")
        for col_name, w in zip(cols, (85, 60, 65, 140, 60)):
            self.tree_codes.heading(col_name, text=col_name)
            self.tree_codes.column(col_name, width=w, anchor="center")

        tree_scroll = ttk.Scrollbar(left_box, orient=tk.VERTICAL, command=self.tree_codes.yview)
        self.tree_codes.configure(yscrollcommand=tree_scroll.set)
        tree_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.tree_codes.pack(fill=tk.BOTH, expand=True)

        # Right: Encoded Output & Decompressed Text
        right_box = tk.Frame(middle_pane, bg="#F1F5F9")
        middle_pane.add(right_box, minsize=500)

        # Encoded Bitstream Box
        enc_box = tk.LabelFrame(right_box, text=" 3. Encoded Bitstream Output (0s & 1s) ",
                                font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        enc_box.pack(fill=tk.BOTH, expand=True, pady=(0, 6))

        self.txt_encoded = tk.Text(enc_box, height=4, font=("Consolas", 10), wrap=tk.CHAR,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=4)
        enc_scroll = ttk.Scrollbar(enc_box, orient=tk.VERTICAL, command=self.txt_encoded.yview)
        self.txt_encoded.configure(yscrollcommand=enc_scroll.set)
        enc_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_encoded.pack(fill=tk.BOTH, expand=True)

        # Decompressed Original Text Box
        dec_box = tk.LabelFrame(right_box, text=" 4. Reconstructed Decompressed Text ",
                                font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        dec_box.pack(fill=tk.BOTH, expand=True)

        self.txt_decoded = tk.Text(dec_box, height=4, font=("Segoe UI", 10), wrap=tk.WORD,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=4)
        dec_scroll = ttk.Scrollbar(dec_box, orient=tk.VERTICAL, command=self.txt_decoded.yview)
        self.txt_decoded.configure(yscrollcommand=dec_scroll.set)
        dec_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_decoded.pack(fill=tk.BOTH, expand=True)

        # Initial compression calculation so all cards and tables show numbers on startup!
        self.on_compress(silent=True)

    def on_text_change(self, event=None):
        self.update_before_size_badge()

    def update_before_size_badge(self):
        text = self.txt_input.get("1.0", tk.END).strip("\r\n")
        byte_len = len(text.encode("utf-8"))
        bit_len = byte_len * 8
        self.lbl_card_before.config(text=f"Original Size: {byte_len} bytes ({bit_len} bits)\nCharacter Count: {len(text)} symbols")

    # ========================================================================
    # EVENT HANDLERS
    # ========================================================================
    def on_compress(self, silent=False):
        text = self.txt_input.get("1.0", tk.END).strip("\r\n")

        # Task 9: Input validation
        if not text:
            if not silent:
                messagebox.showwarning("Input Validation Notice",
                                       "Input text is empty!\nPlease type text into the input area or load a text file.")
                self.status_var.set("Error: Input text is empty. Compression aborted.")
            return

        self.last_original_text = text
        self.update_before_size_badge()

        # Step 1: Member 2 (Frequency Analysis)
        self.current_frequencies = FrequencyAnalyzer.analyze(text)

        # Step 2: Member 3 (Tree Construction & Prefix Code Generation)
        self.tree_module.build_tree(self.current_frequencies)
        self.current_codes = self.tree_module.generate_codes()

        # Step 3: Member 4 (Text Encoding)
        self.last_encoded_bits = HuffmanEncoder.encode(text, self.current_codes)

        # Step 4: Task 5 Display Codes Table
        for item in self.tree_codes.get_children():
            self.tree_codes.delete(item)

        for ch, code in sorted(self.current_codes.items(), key=lambda x: (len(x[1]), x[0])):
            if ch == ' ':
                sym = "[SPACE]"
            elif ch == '\n':
                sym = "[LF \\n]"
            elif ch == '\t':
                sym = "[TAB]"
            else:
                sym = ch

            ascii_val = str(ord(ch))
            freq_val = str(self.current_frequencies[ch])
            code_len = str(len(code))
            self.tree_codes.insert("", tk.END, values=(sym, ascii_val, freq_val, code, code_len))

        # Step 5: Task 6 Display Encoded Bitstream
        self.txt_encoded.delete("1.0", tk.END)
        self.txt_encoded.insert(tk.END, self.last_encoded_bits)

        # Step 6: Task 8 Compute Compression Metrics & Part 5 Comparison Cards
        m = CompressionAnalyzer.evaluate(text, self.last_encoded_bits,
                                         self.current_frequencies, self.current_codes)

        self.lbl_card_after.config(
            text=f"Compressed: {m['payload_bytes']} bytes ({m['payload_bits']} bits)\nReduction: {m['bytes_saved']} bytes ({m['bits_saved']} bits saved)"
        )
        self.lbl_card_metrics.config(
            text=f"Compression Ratio: {m['comp_ratio']:.2f} : 1\nSpace Saved: {m['space_saved']:.2f}% | Entropy: {m['entropy']:.2f}"
        )

        # Reset decoded display
        self.txt_decoded.delete("1.0", tk.END)

        msg = (f"Compression Completed: {m['orig_bytes']} bytes compressed into "
               f"{m['payload_bytes']} bytes ({m['space_saved']:.1f}% space saved).")
        self.status_var.set(msg)

    def on_decompress(self):
        # Task 9: Input validation
        if not self.tree_module.root or not self.last_encoded_bits:
            messagebox.showwarning("Decompression Validation Notice",
                                   "No active Huffman tree or encoded bitstream available.\nPlease enter text and click [Compress Text] first.")
            self.status_var.set("Error: Decompression attempted with no encoded data.")
            return

        bits = self.txt_encoded.get("1.0", tk.END).strip()
        if not bits:
            messagebox.showwarning("Validation Notice", "Encoded bitstream field is empty.")
            return

        # Step: Member 5 (Decoding)
        decoded = HuffmanDecoder.decode(bits, self.tree_module.root)

        # Task 7: Display Decompressed Text
        self.txt_decoded.delete("1.0", tk.END)
        self.txt_decoded.insert(tk.END, decoded)

        # Integrity Check
        if decoded == self.last_original_text:
            self.status_var.set("Lossless Verification PASSED: Decoded text matches original input 100%!")
            messagebox.showinfo("Verification Passed",
                                f"Decompression Successful!\nReconstructed text ({len(decoded)} characters) matches the original input 100% losslessly.")
        else:
            self.status_var.set("Warning: Decompressed text does not match original text.")

    def on_clear(self):
        self.txt_input.delete("1.0", tk.END)
        self.txt_encoded.delete("1.0", tk.END)
        self.txt_decoded.delete("1.0", tk.END)
        for item in self.tree_codes.get_children():
            self.tree_codes.delete(item)
        self.tree_module.root = None
        self.last_original_text = ""
        self.last_encoded_bits = ""
        self.current_codes.clear()
        self.current_frequencies.clear()
        self.update_before_size_badge()
        self.lbl_card_after.config(text="Compressed: 0 bytes (0 bits)\nReduction: 0 bytes saved")
        self.lbl_card_metrics.config(text="Ratio: 0.00 : 1 | Space Saved: 0.0%\nEntropy: 0.00 | Avg Code: 0.00")
        self.status_var.set("Cleared all input, outputs, and tables.")

    def on_load_file(self):
        path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")])
        if path:
            try:
                with open(path, "r", encoding="utf-8") as f:
                    content = f.read()
                self.txt_input.delete("1.0", tk.END)
                self.txt_input.insert(tk.END, content)
                self.update_before_size_badge()
                self.status_var.set(f"Loaded {len(content)} characters ({len(content.encode('utf-8'))} bytes) from: {path}")
            except Exception as e:
                messagebox.showerror("File Error", f"Failed to load file: {e}")


if __name__ == "__main__":
    app = HuffmanDesktopApp()
    app.mainloop()
