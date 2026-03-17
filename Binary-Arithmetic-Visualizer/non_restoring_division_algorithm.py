def to_binary(value, bits):
    """Convert integer to binary string of fixed length (two's complement)."""
    return format(value & ((1 << bits) - 1), f'0{bits}b')


def non_restoring_division(dividend, divisor, bits):
    # Check if the bit size is sufficient
    required_bits = max(dividend.bit_length(), divisor.bit_length()) + 1
    if bits < required_bits:
        print(f"\n⚠️  Number of bits too small! Minimum required bits = {required_bits}")
        print(f"Automatically adjusting bits to {required_bits}.\n")
        bits = required_bits

    A = 0
    Q = dividend
    M = divisor

    print("\nNon-Restoring Division Algorithm")
    print(f"Dividend (Q): {dividend} -> {to_binary(dividend, bits)}")
    print(f"Divisor  (M): {divisor}  -> {to_binary(divisor, bits)}\n")

    print(f"{'Step':<5}{'A (Binary)':<12}{'Q (Binary)':<12}{'Operation'}")
    print("-" * 65)

    for step in range(bits):
        # Left shift A and Q
        A = (A << 1) | ((Q >> (bits - 1)) & 1)
        Q = (Q << 1) & ((1 << bits) - 1)

        if A >= 0:
            A = A - M
            operation = "A = A - M (A≥0 → Q0=1)"
        else:
            A = A + M
            operation = "A = A + M (A<0 → Q0=0)"

        # Set Q0 depending on A
        if A >= 0:
            Q |= 1
        else:
            Q &= ~1

        print(f"{step+1:<5}{to_binary(A, bits+1):<12}{to_binary(Q, bits):<12}{operation}")

    # Final correction
    if A < 0:
        A = A + M
        print(f"Final restore: A was negative → A = A + M")

    print("-" * 65)
    print(f"Final Quotient (Q): {to_binary(Q, bits)} = {Q}")
    print(f"Final Remainder (A): {to_binary(A, bits+1)} = {A}")


# Run Program
if __name__ == "__main__":
    dividend = int(input("Enter Dividend (decimal): "))
    divisor = int(input("Enter Divisor (decimal): "))
    bits = int(input("Enter number of bits: "))
    non_restoring_division(dividend, divisor, bits)
