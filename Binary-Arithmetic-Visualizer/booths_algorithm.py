# Booth's Multiplication (n-bit A and Q, with Q-1) with auto bit-checking.

def to_twos_int(x, width):
    """Return integer representing x in two's-complement with 'width' bits."""
    if x < 0:
        x = (1 << width) + x
    return x & ((1 << width) - 1)

def from_twos_int(x, width):
    """Interpret x (0..2^width-1) as signed two's-complement integer."""
    sign = 1 << (width - 1)
    if x & sign:
        return x - (1 << width)
    return x

def to_bin(x, width):
    return format(x & ((1 << width) - 1), f'0{width}b')

def booths_algorithm(multiplicand, multiplier, bits):
    # Ensure bits large enough to represent operands (include sign)
    required = max(multiplicand.bit_length(), multiplier.bit_length()) + 1
    if bits < required:
        print(f"\n⚠️  Number of bits too small! Minimum required bits = {required}")
        print(f"Automatically adjusting bits to {required}.\n")
        bits = required

    n = bits
    mask_n = (1 << n) - 1

    # Represent M and Q as n-bit two's complement integers
    M = to_twos_int(multiplicand, n)
    Q = to_twos_int(multiplier, n)
    A = 0                    # n-bit accumulator register (kept as n-bit pattern)
    Q_1 = 0                  # extra bit

    print("\nBooth’s Multiplication Algorithm")
    print(f"Multiplicand (M): {multiplicand} -> {to_bin(M, n)}")
    print(f"Multiplier  (Q): {multiplier} -> {to_bin(Q, n)}\n")

    print(f"{'Step':<5}{'A (bin)':<12}{'Q (bin)':<12}{'Q-1':<5}{'Op'}")
    print("-" * 60)

    total_bits = n + n + 1   # combined width: A(n) + Q(n) + Q_1(1)

    for step in range(1, n + 1):
        Q0 = Q & 1
        operation = "No Operation"

        # Decide add/sub based on (Q0, Q_1)
        if Q0 == 0 and Q_1 == 1:
            # A = A + M  (mod 2^n)
            A = (A + M) & mask_n
            operation = "A = A + M"
        elif Q0 == 1 and Q_1 == 0:
            # A = A - M  (mod 2^n)
            A = (A - M) & mask_n
            operation = "A = A - M"

        # Build combined register: [A (n bits)] [Q (n bits)] [Q_1 (1 bit)]
        combined = (A << (n + 1)) | (Q << 1) | Q_1

        # Arithmetic right shift by 1: preserve MSB of A (which is combined's top bit)
        msb = (combined >> (total_bits - 1)) & 1
        combined = (combined >> 1) | (msb << (total_bits - 1))

        # Extract new A, Q, Q_1
        A = (combined >> (n + 1)) & mask_n
        Q = (combined >> 1) & mask_n
        Q_1 = combined & 1

        print(f"{step:<5}{to_bin(A, n):<12}{to_bin(Q, n):<12}{Q_1:<5}{operation}")

    # Final product: concatenate A and Q -> 2n bits
    product = (A << n) | Q
    product_signed = from_twos_int(product, 2 * n)

    print("-" * 60)
    print(f"Final A: {to_bin(A, n)}")
    print(f"Final Q: {to_bin(Q, n)}")
    print(f"Final Product (binary, 2n bits): {to_bin(product, 2 * n)}")
    print(f"Final Product (decimal): {product_signed}")
    return product_signed

if __name__ == "__main__":
    m = int(input("Enter multiplicand (decimal): "))
    q = int(input("Enter multiplier (decimal): "))
    bits = int(input("Enter number of bits: "))
    booths_algorithm(m, q, bits)
