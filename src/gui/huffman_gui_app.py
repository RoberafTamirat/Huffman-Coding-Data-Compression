"""
Huffman Coding Data Compression — Desktop Graphical User Interface
Author: Roberaf Tamirat (Role 1: GUI Developer & Integration Coordinator)
Project: Huffman Coding for Lossless Data Compression

This application provides a responsive, native-styled desktop interface implementing
all 10 assigned tasks and coordinating the modules of the 6 team members.
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

        # Min-Heap Priority Queue
        heap = [HuffmanNode(ch, freq) for ch, freq in frequencies.items()]
        heapq.heapify(heap)

        # Repeatedly combine lowest two nodes
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

    @staticmethod
    def pack_bits(bit_string):
        """Packs '0'/'1' bits into bytes and tracks valid bits in final byte."""
        if not bit_string:
            return bytearray(), 0
        packed = bytearray()
        cur = 0
        count = 0
        for b in bit_string:
            cur = (cur << 1) | (1 if b == '1' else 0)
            count += 1
            if count == 8:
                packed.append(cur)
                cur = 0
                count = 0
        if count > 0:
            cur <<= (8 - count)
            packed.append(cur)
            valid_last = count
        else:
            valid_last = 8
        return packed, valid_last


# ============================================================================
# MODULE 5: HuffmanDecoder & FileHandler (Member 5: Hermela Tadios)
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

        # Header overhead estimation (storing character frequency table)
        header_overhead = len(frequencies) * 5 + 4
        total_comp_bytes = comp_payload_bytes + header_overhead

        # Official formulas:
        # Compression ratio = Original size / Compressed size
        comp_ratio = (orig_bytes / comp_payload_bytes) if comp_payload_bytes > 0 else 0.0

        # Space saved = ((Original size - Compressed size) / Original size) * 100
        space_saved = (((orig_bytes - comp_payload_bytes) / orig_bytes) * 100.0) if orig_bytes > 0 else 0.0

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
            "header_overhead": header_overhead,
            "total_bytes": total_comp_bytes,
            "comp_ratio": comp_ratio,
            "space_saved": space_saved,
            "avg_code_len": avg_code_len,
            "entropy": entropy,
            "unique_chars": len(frequencies)
        }


# ============================================================================
# MODULE 1: Desktop GUI & Integration Coordinator (Roberaf Tamirat)
# ============================================================================
class HuffmanDesktopApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Huffman Coding Data Compression System — Roberaf Tamirat (Role 1)")
        self.geometry("1120x820")
        self.minsize(980, 720)

        # Style configuration
        self.style = ttk.Style(self)
        self.style.theme_use("clam")
        self.configure(bg="#F1F5F9")

        # Integrated modules
        self.tree_module = HuffmanTree()
        self.last_original_text = ""
        self.last_encoded_bits = ""
        self.current_codes = {}
        self.current_frequencies = {}

        self._build_ui()

    def _build_ui(self):
        # Header banner
        header = tk.Frame(self, bg="#1E3A8A", height=70)
        header.pack(fill=tk.X, side=tk.TOP)
        header.pack_propagate(False)

        title_lbl = tk.Label(header, text="Huffman Coding Data Compression — Desktop Interface",
                             font=("Segoe UI", 15, "bold"), fg="#FFFFFF", bg="#1E3A8A")
        title_lbl.pack(anchor="w", padx=20, pady=(10, 2))

        sub_lbl = tk.Label(header,
                           text="Role 1: GUI Developer & Integration Coordinator | Roberaf Tamirat (Class Project)",
                           font=("Segoe UI", 9), fg="#93C5FD", bg="#1E3A8A")
        sub_lbl.pack(anchor="w", padx=20)

        container = tk.Frame(self, bg="#F1F5F9", padx=16, pady=10)
        container.pack(fill=tk.BOTH, expand=True)

        # 1. Source Text Input
        top_box = tk.LabelFrame(container, text=" 1. Source Text Input (Task 2) ", font=("Segoe UI", 10, "bold"),
                                bg="#FFFFFF", fg="#1E293B", padx=10, pady=6)
        top_box.pack(fill=tk.X, pady=(0, 10))

        bar = tk.Frame(top_box, bg="#FFFFFF")
        bar.pack(fill=tk.X, pady=(0, 4))
        tk.Label(bar, text="Enter text below or load from a text file:", font=("Segoe UI", 9),
                 bg="#FFFFFF", fg="#64748B").pack(side=tk.LEFT)

        tk.Button(bar, text="Clear All", font=("Segoe UI", 9), bg="#EF4444", fg="#FFFFFF",
                  relief=tk.FLAT, padx=10, command=self.on_clear).pack(side=tk.RIGHT, padx=4)
        tk.Button(bar, text="📂 Load Text File...", font=("Segoe UI", 9), bg="#E2E8F0", fg="#1E293B",
                  relief=tk.FLAT, padx=10, command=self.on_load_file).pack(side=tk.RIGHT, padx=4)

        self.txt_input = tk.Text(top_box, height=5, font=("Segoe UI", 10), wrap=tk.WORD,
                                 relief=tk.SOLID, bd=1, padx=6, pady=6)
        self.txt_input.pack(fill=tk.X)
        self.txt_input.insert(tk.END, "huffman coding for data compression")

        # Action Buttons (Task 3 & 4)
        action_bar = tk.Frame(container, bg="#F1F5F9")
        action_bar.pack(fill=tk.X, pady=(0, 10))

        self.btn_compress = tk.Button(action_bar, text="🗜️  Compress Text (Task 3)", font=("Segoe UI", 11, "bold"),
                                      bg="#2563EB", fg="#FFFFFF", activebackground="#1D4ED8", activeforeground="#FFFFFF",
                                      relief=tk.FLAT, padx=20, pady=7, cursor="hand2", command=self.on_compress)
        self.btn_compress.pack(side=tk.LEFT, padx=(0, 10))

        self.btn_decompress = tk.Button(action_bar, text="🔓  Decompress Text (Task 4)", font=("Segoe UI", 11, "bold"),
                                        bg="#10B981", fg="#FFFFFF", activebackground="#059669", activeforeground="#FFFFFF",
                                        relief=tk.FLAT, padx=20, pady=7, cursor="hand2", command=self.on_decompress)
        self.btn_decompress.pack(side=tk.LEFT)

        # Middle Split: Left = Codes Table, Right = Bitstream & Decoded Text
        middle_pane = tk.PanedWindow(container, orient=tk.HORIZONTAL, bg="#CBD5E1", sashwidth=6)
        middle_pane.pack(fill=tk.BOTH, expand=True, pady=(0, 10))

        # Left: Codes Table (Task 5)
        left_box = tk.LabelFrame(middle_pane, text=" 2. Generated Huffman Codes (Task 5) ",
                                 font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=8)
        middle_pane.add(left_box, minsize=420)

        cols = ("Symbol", "ASCII", "Freq", "Huffman Code", "Bits")
        self.tree_codes = ttk.Treeview(left_box, columns=cols, show="headings", height=12)
        for col_name, w in zip(cols, (85, 60, 65, 140, 60)):
            self.tree_codes.heading(col_name, text=col_name)
            self.tree_codes.column(col_name, width=w, anchor="center")

        tree_scroll = ttk.Scrollbar(left_box, orient=tk.VERTICAL, command=self.tree_codes.yview)
        self.tree_codes.configure(yscrollcommand=tree_scroll.set)
        tree_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.tree_codes.pack(fill=tk.BOTH, expand=True)

        # Right: Encoded Output & Decompressed Text (Task 6 & 7)
        right_box = tk.Frame(middle_pane, bg="#F1F5F9")
        middle_pane.add(right_box, minsize=480)

        # Encoded Bitstream (Task 6)
        enc_box = tk.LabelFrame(right_box, text=" 3. Encoded Bitstream Output (Task 6) ",
                                font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        enc_box.pack(fill=tk.BOTH, expand=True, pady=(0, 8))

        self.txt_encoded = tk.Text(enc_box, height=5, font=("Consolas", 10), wrap=tk.CHAR,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=6)
        enc_scroll = ttk.Scrollbar(enc_box, orient=tk.VERTICAL, command=self.txt_encoded.yview)
        self.txt_encoded.configure(yscrollcommand=enc_scroll.set)
        enc_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_encoded.pack(fill=tk.BOTH, expand=True)

        # Decompressed Original Text (Task 7)
        dec_box = tk.LabelFrame(right_box, text=" 4. Reconstructed Decompressed Text (Task 7) ",
                                font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        dec_box.pack(fill=tk.BOTH, expand=True)

        self.txt_decoded = tk.Text(dec_box, height=5, font=("Segoe UI", 10), wrap=tk.WORD,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=6)
        dec_scroll = ttk.Scrollbar(dec_box, orient=tk.VERTICAL, command=self.txt_decoded.yview)
        self.txt_decoded.configure(yscrollcommand=dec_scroll.set)
        dec_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_decoded.pack(fill=tk.BOTH, expand=True)

        # 5. Compression Statistics Panel (Task 8 - Member 6 Integration)
        stats_frame = tk.LabelFrame(container, text=" 5. Compression Statistics & Metrics (Task 8 — Member 6) ",
                                    font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=10, pady=8)
        stats_frame.pack(fill=tk.X, pady=(0, 4))

        self.lbl_stats = tk.Label(stats_frame,
                                  text="Status: Ready. Click [Compress Text] above to evaluate compression efficiency.",
                                  font=("Consolas", 9), bg="#F8FAFC", fg="#0F172A", justify=tk.LEFT,
                                  anchor="w", padx=10, pady=6, relief=tk.SOLID, bd=1)
        self.lbl_stats.pack(fill=tk.X)

        # Status Bar
        self.status_var = tk.StringVar(value="Ready. Designed by Roberaf Tamirat (Role 1: GUI Developer & Integration Coordinator).")
        status_bar = tk.Label(self, textvariable=self.status_var, font=("Segoe UI", 9),
                              bg="#E2E8F0", fg="#334155", anchor="w", padx=12, pady=4)
        status_bar.pack(fill=tk.X, side=tk.BOTTOM)

    # ========================================================================
    # EVENT HANDLERS & INTEGRATION WORKFLOW
    # ========================================================================
    def on_compress(self):
        text = self.txt_input.get("1.0", tk.END).strip("\r\n")

        # Task 9: Input validation
        if not text:
            messagebox.showwarning("Input Validation Notice (Task 9)",
                                   "Input text is empty!\nPlease type text into the input area or load a text file.")
            self.status_var.set("Error: Input text is empty. Compression aborted.")
            return

        self.last_original_text = text

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

        # Step 6: Task 8 Display Member 6 Compression Statistics
        metrics = CompressionAnalyzer.evaluate(text, self.last_encoded_bits,
                                               self.current_frequencies, self.current_codes)
        summary = (
            f"Original Size: {metrics['orig_bytes']} bytes ({metrics['orig_bits']} bits)  |  "
            f"Encoded Payload: {metrics['payload_bytes']} bytes ({metrics['payload_bits']} bits)  |  "
            f"Unique Symbols: {metrics['unique_chars']}\n"
            f"Compression Ratio: {metrics['comp_ratio']:.2f} : 1 (Original / Compressed)  |  "
            f"Space Saved: {metrics['space_saved']:.2f}%  |  "
            f"Avg Code Length: {metrics['avg_code_len']:.2f} bits/char  |  "
            f"Shannon Entropy: {metrics['entropy']:.2f} bits/symbol"
        )
        self.lbl_stats.config(text=summary)

        # Reset decoded display to highlight roundtrip verification step
        self.txt_decoded.delete("1.0", tk.END)

        msg = (f"Compression Successful: {len(text)} characters encoded into "
               f"{len(self.last_encoded_bits)} bits ({metrics['space_saved']:.1f}% space saved).")
        self.status_var.set(msg)

    def on_decompress(self):
        # Task 9: Input validation
        if not self.tree_module.root or not self.last_encoded_bits:
            messagebox.showwarning("Decompression Validation Notice (Task 9)",
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
            messagebox.showinfo("Verification Passed (Task 7)",
                                "Decompression Successful!\nReconstructed text matches the original text 100% losslessly.")
        else:
            self.status_var.set("Warning: Decompressed text does not match original text.")

    def on_clear(self):
        self.txt_input.delete("1.0", tk.END)
        self.txt_encoded.delete("1.0", tk.END)
        self.txt_decoded.delete("1.0", tk.END)
        for item in self.tree_codes.get_children():
            self.tree_codes.delete(item)
        self.lbl_stats.config(text="Status: Ready. Click [Compress Text] above to evaluate compression efficiency.")
        self.tree_module.root = None
        self.last_original_text = ""
        self.last_encoded_bits = ""
        self.current_codes.clear()
        self.current_frequencies.clear()
        self.status_var.set("Cleared all input, outputs, and tables.")

    def on_load_file(self):
        path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")])
        if path:
            try:
                with open(path, "r", encoding="utf-8") as f:
                    content = f.read()
                self.txt_input.delete("1.0", tk.END)
                self.txt_input.insert(tk.END, content)
                self.status_var.set(f"Loaded {len(content)} characters from: {path}")
            except Exception as e:
                messagebox.showerror("File Error", f"Failed to load file: {e}")


if __name__ == "__main__":
    app = HuffmanDesktopApp()
    app.mainloop()
