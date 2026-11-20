"""
Huffman Coding Data Compression - Interactive Desktop GUI
Developed by Roberaf Tamirat (Role 1: GUI Developer & Integration Coordinator)

This application provides a full-featured, responsive desktop GUI that implements
all 10 assigned tasks and integrates with the Huffman Coding pipeline.
"""

import math
import heapq
from collections import Counter
import tkinter as tk
from tkinter import ttk, messagebox, filedialog


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


class HuffmanCore:
    """Core Huffman logic mirroring the C++ implementation."""
    @staticmethod
    def build_tree(frequencies):
        if not frequencies:
            return None
        heap = [HuffmanNode(ch, freq) for ch, freq in frequencies.items()]
        heapq.heapify(heap)

        while len(heap) > 1:
            left = heapq.heappop(heap)
            right = heapq.heappop(heap)
            parent = HuffmanNode('\0', left.freq + right.freq, left, right)
            heapq.heappush(heap, parent)

        return heap[0] if heap else None

    @staticmethod
    def generate_codes(root):
        codes = {}
        if not root:
            return codes
        if root.is_leaf():
            codes[root.char] = "0"
            return codes

        def dfs(node, current_code):
            if not node:
                return
            if node.is_leaf():
                codes[node.char] = current_code
                return
            dfs(node.left, current_code + "0")
            dfs(node.right, current_code + "1")

        dfs(root, "")
        return codes

    @staticmethod
    def encode(text, codes):
        return "".join(codes[ch] for ch in text)

    @staticmethod
    def decode(encoded_bits, root):
        if not root or not encoded_bits:
            return ""
        if root.is_leaf():
            return root.char * len(encoded_bits)

        decoded = []
        current = root
        for bit in encoded_bits:
            current = current.left if bit == '0' else current.right
            if current is None:
                break
            if current.is_leaf():
                decoded.append(current.char)
                current = root
        return "".join(decoded)


class CompressionStats:
    """Calculates metrics supplied by Member 6."""
    @staticmethod
    def compute(original_text, encoded_bits, frequencies, codes):
        orig_bytes = len(original_text.encode('utf-8'))
        orig_bits = orig_bytes * 8
        comp_bits = len(encoded_bits)
        comp_bytes = (comp_bits + 7) // 8

        comp_ratio = (comp_bytes / orig_bytes * 100.0) if orig_bytes > 0 else 0.0
        space_saving = (100.0 - comp_ratio) if orig_bytes > 0 else 0.0
        avg_code_len = (comp_bits / len(original_text)) if original_text else 0.0

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
            "comp_bits": comp_bits,
            "comp_bytes": comp_bytes,
            "comp_ratio": comp_ratio,
            "space_saving": space_saving,
            "avg_code_len": avg_code_len,
            "entropy": entropy,
            "unique_chars": len(frequencies)
        }


class HuffmanApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Huffman Coding Data Compression System — Roberaf Tamirat (Role 1)")
        self.geometry("1120x820")
        self.minsize(980, 720)

        # Style configuration
        self.style = ttk.Style(self)
        self.style.theme_use("clam")
        self.configure(bg="#F3F4F6")

        self.last_tree = None
        self.last_original_text = ""
        self.last_encoded_bits = ""

        self._build_ui()

    def _build_ui(self):
        # Header banner
        header = tk.Frame(self, bg="#1E3A8A", height=65)
        header.pack(fill=tk.X, side=tk.TOP)
        header.pack_propagate(False)

        title_lbl = tk.Label(header, text="Huffman Coding Data Compression — Desktop Interface",
                             font=("Segoe UI", 15, "bold"), fg="#FFFFFF", bg="#1E3A8A")
        title_lbl.pack(anchor="w", padx=20, pady=(8, 2))

        sub_lbl = tk.Label(header, text="Role: GUI Developer & Integration Coordinator | Roberaf Tamirat",
                           font=("Segoe UI", 9), fg="#93C5FD", bg="#1E3A8A")
        sub_lbl.pack(anchor="w", padx=20)

        # Main scrollable/padded container
        container = tk.Frame(self, bg="#F3F4F6", padx=16, pady=12)
        container.pack(fill=tk.BOTH, expand=True)

        # Top section: Input
        top_frame = tk.LabelFrame(container, text=" 1. Source Text Input ", font=("Segoe UI", 10, "bold"),
                                  bg="#FFFFFF", fg="#1E293B", padx=10, pady=8)
        top_frame.pack(fill=tk.X, pady=(0, 10))

        btn_row = tk.Frame(top_frame, bg="#FFFFFF")
        btn_row.pack(fill=tk.X, pady=(0, 5))

        tk.Label(btn_row, text="Type or paste uncompressed text below, or load a file:",
                 font=("Segoe UI", 9), bg="#FFFFFF", fg="#475569").pack(side=tk.LEFT)

        tk.Button(btn_row, text="Clear All", font=("Segoe UI", 9), bg="#EF4444", fg="#FFFFFF",
                  relief=tk.FLAT, padx=10, command=self.on_clear).pack(side=tk.RIGHT, padx=4)
        tk.Button(btn_row, text="📂 Load Text File...", font=("Segoe UI", 9), bg="#E2E8F0", fg="#1E293B",
                  relief=tk.FLAT, padx=10, command=self.on_load_file).pack(side=tk.RIGHT, padx=4)

        self.txt_input = tk.Text(top_frame, height=5, font=("Segoe UI", 10), wrap=tk.WORD,
                                 relief=tk.SOLID, bd=1, padx=6, pady=6)
        self.txt_input.pack(fill=tk.X)
        self.txt_input.insert(tk.END, "huffman coding for data compression")

        # Action Buttons
        action_frame = tk.Frame(container, bg="#F3F4F6")
        action_frame.pack(fill=tk.X, pady=(0, 10))

        self.btn_compress = tk.Button(action_frame, text="🗜️  Compress Text", font=("Segoe UI", 11, "bold"),
                                      bg="#2563EB", fg="#FFFFFF", activebackground="#1D4ED8", activeforeground="#FFFFFF",
                                      relief=tk.FLAT, padx=20, pady=8, cursor="hand2", command=self.on_compress)
        self.btn_compress.pack(side=tk.LEFT, padx=(0, 10))

        self.btn_decompress = tk.Button(action_frame, text="🔓  Decompress Text", font=("Segoe UI", 11, "bold"),
                                        bg="#10B981", fg="#FFFFFF", activebackground="#059669", activeforeground="#FFFFFF",
                                        relief=tk.FLAT, padx=20, pady=8, cursor="hand2", command=self.on_decompress)
        self.btn_decompress.pack(side=tk.LEFT)

        # Middle Split: Left = Huffman Codes Table, Right = Encoded Bitstream & Decoded Text
        middle_pane = tk.PanedWindow(container, orient=tk.HORIZONTAL, bg="#E2E8F0", sashwidth=6)
        middle_pane.pack(fill=tk.BOTH, expand=True, pady=(0, 10))

        # Left panel: Codes Table
        left_frame = tk.LabelFrame(middle_pane, text=" 2. Generated Huffman Codes Table ", font=("Segoe UI", 10, "bold"),
                                   bg="#FFFFFF", fg="#1E293B", padx=8, pady=8)
        middle_pane.add(left_frame, minsize=420)

        cols = ("Symbol", "ASCII", "Freq", "Huffman Code", "Bits")
        self.tree_codes = ttk.Treeview(left_frame, columns=cols, show="headings", height=12)
        self.tree_codes.heading("Symbol", text="Symbol")
        self.tree_codes.heading("ASCII", text="ASCII")
        self.tree_codes.heading("Freq", text="Freq")
        self.tree_codes.heading("Huffman Code", text="Huffman Code")
        self.tree_codes.heading("Bits", text="Bits")

        self.tree_codes.column("Symbol", width=90, anchor="center")
        self.tree_codes.column("ASCII", width=65, anchor="center")
        self.tree_codes.column("Freq", width=65, anchor="center")
        self.tree_codes.column("Huffman Code", width=140, anchor="center")
        self.tree_codes.column("Bits", width=60, anchor="center")

        tree_scroll = ttk.Scrollbar(left_frame, orient=tk.VERTICAL, command=self.tree_codes.yview)
        self.tree_codes.configure(yscrollcommand=tree_scroll.set)
        tree_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.tree_codes.pack(fill=tk.BOTH, expand=True)

        # Right panel: Outputs
        right_frame = tk.Frame(middle_pane, bg="#F3F4F6")
        middle_pane.add(right_frame, minsize=480)

        # Encoded Bitstream
        enc_box = tk.LabelFrame(right_frame, text=" 3. Encoded Bitstream Output (0s & 1s) ", font=("Segoe UI", 10, "bold"),
                                bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        enc_box.pack(fill=tk.BOTH, expand=True, pady=(0, 8))

        self.txt_encoded = tk.Text(enc_box, height=6, font=("Consolas", 10), wrap=tk.CHAR,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=6)
        enc_scroll = ttk.Scrollbar(enc_box, orient=tk.VERTICAL, command=self.txt_encoded.yview)
        self.txt_encoded.configure(yscrollcommand=enc_scroll.set)
        enc_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_encoded.pack(fill=tk.BOTH, expand=True)

        # Decoded Original Text
        dec_box = tk.LabelFrame(right_frame, text=" 4. Reconstructed Decompressed Text ", font=("Segoe UI", 10, "bold"),
                                bg="#FFFFFF", fg="#1E293B", padx=8, pady=6)
        dec_box.pack(fill=tk.BOTH, expand=True)

        self.txt_decoded = tk.Text(dec_box, height=5, font=("Segoe UI", 10), wrap=tk.WORD,
                                   bg="#F8FAFC", relief=tk.SOLID, bd=1, padx=6, pady=6)
        dec_scroll = ttk.Scrollbar(dec_box, orient=tk.VERTICAL, command=self.txt_decoded.yview)
        self.txt_decoded.configure(yscrollcommand=dec_scroll.set)
        dec_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.txt_decoded.pack(fill=tk.BOTH, expand=True)

        # Bottom section: Compression Statistics (Member 6 Deliverables)
        stats_frame = tk.LabelFrame(container, text=" 5. Compression Statistics & Metrics (Member 6 Integration) ",
                                    font=("Segoe UI", 10, "bold"), bg="#FFFFFF", fg="#1E293B", padx=10, pady=8)
        stats_frame.pack(fill=tk.X, pady=(0, 4))

        self.lbl_stats = tk.Label(stats_frame, text="Status: Ready. Click [Compress Text] above to compute compression metrics.",
                                  font=("Consolas", 9), bg="#F8FAFC", fg="#0F172A", justify=tk.LEFT,
                                  anchor="w", padx=10, pady=6, relief=tk.SOLID, bd=1)
        self.lbl_stats.pack(fill=tk.X)

        # Status Bar
        self.status_var = tk.StringVar(value="Ready")
        status_bar = tk.Label(self, textvariable=self.status_var, font=("Segoe UI", 9),
                              bg="#E2E8F0", fg="#334155", anchor="w", padx=12, pady=4)
        status_bar.pack(fill=tk.X, side=tk.BOTTOM)

    def on_compress(self):
        text = self.txt_input.get("1.0", tk.END).strip("\r\n")

        # Task 9: Input validation
        if not text:
            messagebox.showwarning("Input Validation Error",
                                   "Input text area is empty!\nPlease type text into the input box or load a file.")
            self.status_var.set("Error: Input text is empty.")
            return

        self.last_original_text = text

        # Tasks 3 & 10: Run encoding pipeline
        freqs = Counter(text)
        self.last_tree = HuffmanCore.build_tree(freqs)
        codes = HuffmanCore.generate_codes(self.last_tree)
        self.last_encoded_bits = HuffmanCore.encode(text, codes)

        # Task 5: Display Huffman codes in table
        for item in self.tree_codes.get_children():
            self.tree_codes.delete(item)

        for ch, code in sorted(codes.items(), key=lambda x: (len(x[1]), x[0])):
            if ch == ' ':
                sym = "[SPACE]"
            elif ch == '\n':
                sym = "[LF \\n]"
            elif ch == '\t':
                sym = "[TAB]"
            else:
                sym = ch

            ascii_val = str(ord(ch))
            freq_val = str(freqs[ch])
            code_len = str(len(code))
            self.tree_codes.insert("", tk.END, values=(sym, ascii_val, freq_val, code, code_len))

        # Task 6: Display encoded bitstream
        self.txt_encoded.delete("1.0", tk.END)
        self.txt_encoded.insert(tk.END, self.last_encoded_bits)

        # Task 8: Display compression statistics
        stats = CompressionStats.compute(text, self.last_encoded_bits, freqs, codes)
        summary = (
            f"Original Size: {stats['orig_bytes']} bytes ({stats['orig_bits']} bits)  |  "
            f"Compressed: {stats['comp_bytes']} bytes ({stats['comp_bits']} bits)  |  "
            f"Unique Symbols: {stats['unique_chars']}\n"
            f"Compression Ratio: {stats['comp_ratio']:.2f}%  |  "
            f"Space Savings: {stats['space_saving']:.2f}%  |  "
            f"Avg Code Length: {stats['avg_code_len']:.2f} bits/char  |  "
            f"Shannon Entropy: {stats['entropy']:.2f} bits/symbol"
        )
        self.lbl_stats.config(text=summary)

        # Clear previous decoded view to highlight next step
        self.txt_decoded.delete("1.0", tk.END)

        msg = f"Compression completed: {len(text)} characters encoded into {len(self.last_encoded_bits)} bits ({stats['space_saving']:.1f}% space saved)."
        self.status_var.set(msg)

    def on_decompress(self):
        # Task 9: Input validation
        if not self.last_tree or not self.last_encoded_bits:
            messagebox.showwarning("Decompression Notice",
                                   "No active Huffman tree or encoded bitstream available.\nPlease enter text and click [Compress Text] first.")
            self.status_var.set("Error: Decompression attempted with no encoded data.")
            return

        bits = self.txt_encoded.get("1.0", tk.END).strip()
        if not bits:
            messagebox.showwarning("Validation Error", "Encoded bitstream field is empty.")
            return

        # Tasks 4, 7 & 10: Decode and display reconstructed text
        decoded = HuffmanCore.decode(bits, self.last_tree)
        self.txt_decoded.delete("1.0", tk.END)
        self.txt_decoded.insert(tk.END, decoded)

        if decoded == self.last_original_text:
            self.status_var.set("Decompression verified: Reconstructed text matches original 100% losslessly!")
            messagebox.showinfo("Verification Passed", "Decompression successful!\nDecoded text matches original text perfectly.")
        else:
            self.status_var.set("Warning: Decompressed text does not match original text.")

    def on_clear(self):
        self.txt_input.delete("1.0", tk.END)
        self.txt_encoded.delete("1.0", tk.END)
        self.txt_decoded.delete("1.0", tk.END)
        for item in self.tree_codes.get_children():
            self.tree_codes.delete(item)
        self.lbl_stats.config(text="Status: Ready. Click [Compress Text] above to compute compression metrics.")
        self.last_tree = None
        self.last_original_text = ""
        self.last_encoded_bits = ""
        self.status_var.set("Cleared all input, outputs, and tables.")

    def on_load_file(self):
        path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")])
        if path:
            try:
                with open(path, "r", encoding="utf-8") as f:
                    content = f.read()
                self.txt_input.delete("1.0", tk.END)
                self.txt_input.insert(tk.END, content)
                self.status_var.set(f"Loaded {len(content)} characters from {path}")
            except Exception as e:
                messagebox.showerror("File Error", f"Failed to load file: {e}")


if __name__ == "__main__":
    app = HuffmanApp()
    app.mainloop()
