import heapq
from collections import defaultdict, Counter
import math

# Node class for Huffman tree
class Node:
    def __init__(self, char=None, freq=0):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    # For priority queue (min-heap)
    def __lt__(self, other):
        return self.freq < other.freq


# Build Huffman Tree
def build_huffman_tree(frequency):
    heap = [Node(char, freq) for char, freq in frequency.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        node1 = heapq.heappop(heap)
        node2 = heapq.heappop(heap)

        merged = Node(None, node1.freq + node2.freq)
        merged.left = node1
        merged.right = node2

        heapq.heappush(heap, merged)

    return heap[0]


# Generate Huffman Codes
def generate_codes(root, code="", codebook={}):
    if root is None:
        return

    if root.char is not None:
        codebook[root.char] = code
        return

    generate_codes(root.left, code + "0", codebook)
    generate_codes(root.right, code + "1", codebook)

    return codebook


# Encode the text using Huffman codes
def huffman_encode(text, codebook):
    return ''.join(codebook[char] for char in text)


# Decode the binary string using Huffman tree
def huffman_decode(encoded_text, root):
    decoded = ""
    node = root

    for bit in encoded_text:
        node = node.left if bit == "0" else node.right

        if node.char is not None:
            decoded += node.char
            node = root

    return decoded


# Calculate entropy
def calculate_entropy(frequency, total):
    return sum((freq / total) * math.log2(total / freq) for freq in frequency.values())


# Main function
def huffman_coding(text):
    print(f"Original Text:\n{text}\n")

    frequency = Counter(text)
    total_chars = len(text)

    print("Character Frequencies:")
    for char, freq in frequency.items():
        print(f"'{char}': {freq}")

    root = build_huffman_tree(frequency)
    codebook = generate_codes(root)

    print("\nHuffman Codes:")
    for char, code in codebook.items():
        print(f"'{char}': {code}")

    encoded = huffman_encode(text, codebook)
    decoded = huffman_decode(encoded, root)

    original_bits = total_chars * 8  # assuming 8-bit ASCII
    compressed_bits = len(encoded)

    print(f"\nEncoded Binary:\n{encoded}")
    print(f"\nDecoded Text:\n{decoded}")

    print("\n--- Compression Analysis ---")
    print(f"Original Size: {original_bits} bits")
    print(f"Compressed Size: {compressed_bits} bits")
    print(f"Compression Ratio: {original_bits / compressed_bits:.2f}")
    print(f"Entropy: {calculate_entropy(frequency, total_chars):.4f} bits/symbol")

    avg_code_length = sum(len(codebook[c]) * (f / total_chars) for c, f in frequency.items())
    print(f"Average Code Length: {avg_code_length:.4f} bits/symbol")


# Example usage
if __name__ == "__main__":
    sample_text = "This is Digital Communication Project on Source Coding using Huffman Coding done by Ajay Yadav and Manish Sinha."
    huffman_coding(sample_text)