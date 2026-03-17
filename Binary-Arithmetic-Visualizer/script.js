// --- TAB SWITCHING LOGIC ---
const tabs = document.querySelectorAll('.tab-btn');
const contents = document.querySelectorAll('.tab-content');

tabs.forEach(tab => {
  tab.addEventListener('click', () => {
    tabs.forEach(btn => btn.classList.remove('active'));
    contents.forEach(c => c.classList.remove('active'));
    tab.classList.add('active');
    document.getElementById(tab.dataset.target).classList.add('active');
  });
});

// --- UTILITY FUNCTIONS ---
function toBinary(num, bits) {
  if (num < 0) num = (1 << bits) + num;
  let bin = num.toString(2);
  return bin.padStart(bits, '0').slice(-bits);
}

function fromBinary(bin) {
  if (bin[0] === '1') return parseInt(bin, 2) - (1 << bin.length);
  return parseInt(bin, 2);
}

// --- BOOTH’S MULTIPLICATION ---
function runBooth() {
  let M = parseInt(document.getElementById('boothM').value);
  let Q = parseInt(document.getElementById('boothQ').value);
  let bits = parseInt(document.getElementById('boothBits').value);
  let output = "";

  // auto adjust bit size
  const maxVal = Math.max(Math.abs(M), Math.abs(Q));
  const requiredBits = Math.ceil(Math.log2(maxVal + 1)) + 1;
  if (bits < requiredBits) {
    output += `Number of bits too small! Minimum required bits = ${requiredBits}\nAutomatically adjusting bits to ${requiredBits}.\n\n`;
    bits = requiredBits;
  }

  let A = 0, Qn1 = 0;
  let step = 1;

  output += `Booth's Multiplication Algorithm\n`;
  output += `Multiplicand (M): ${M} -> ${toBinary(M, bits)}\n`;
  output += `Multiplier (Q): ${Q} -> ${toBinary(Q, bits)}\n\n`;
  output += `Step\tA (Binary)\tQ (Binary)\tQ-1\tOperation\n`;

  for (let i = 0; i < bits; i++) {
    const Q0 = Q & 1;
    if (Q0 === 0 && Qn1 === 1) {
      A += M;
      output += `${step++}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\t${Qn1}\tA = A + M\n`;
    } else if (Q0 === 1 && Qn1 === 0) {
      A -= M;
      output += `${step++}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\t${Qn1}\tA = A - M\n`;
    } else {
      output += `${step++}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\t${Qn1}\tNo Operation\n`;
    }

    // Arithmetic Right Shift (A, Q, Qn1)
    Qn1 = Q & 1;
    let combined = ((A << bits) | (Q & ((1 << bits) - 1)));
    combined >>= 1;
    A = (combined >> bits);
    Q = combined & ((1 << bits) - 1);
  }

  const product = A * (1 << bits) + Q;
  output += `\nFinal A: ${toBinary(A, bits)}\nFinal Q: ${toBinary(Q, bits)}\n`;
  output += `Final Product (binary): ${toBinary(product, bits * 2)}\nFinal Product (decimal): ${product}\n`;

  document.getElementById('boothOutput').textContent = output;
}

// --- RESTORING DIVISION ---
function runRestoring() {
  let dividend = parseInt(document.getElementById('resDividend').value);
  let divisor = parseInt(document.getElementById('resDivisor').value);
  let bits = parseInt(document.getElementById('resBits').value);
  let output = "";

  const maxVal = Math.max(Math.abs(dividend), Math.abs(divisor));
  const requiredBits = Math.ceil(Math.log2(maxVal + 1)) + 1;
  if (bits < requiredBits) {
    output += `Number of bits too small! Minimum required bits = ${requiredBits}\nAutomatically adjusting bits to ${requiredBits}.\n\n`;
    bits = requiredBits;
  }

  let A = 0, Q = dividend;
  output += `Restoring Division Algorithm\n`;
  output += `Dividend (Q): ${dividend} -> ${toBinary(dividend, bits)}\n`;
  output += `Divisor (M): ${divisor} -> ${toBinary(divisor, bits)}\n\n`;
  output += `Step\tA (Binary)\tQ (Binary)\tOperation\n`;

  for (let i = 0; i < bits; i++) {
    A = (A << 1) | ((Q >> (bits - 1)) & 1);
    Q = (Q << 1) & ((1 << bits) - 1);
    A -= divisor;
    output += `${i + 1}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\tA = A - M\n`;

    if (A < 0) {
      A += divisor;
      Q = Q & (~1);
      output += `\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\tA < 0 → A = A + M, Q0 = 0\n`;
    } else {
      Q = Q | 1;
      output += `\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\tA >= 0 → Q0 = 1\n`;
    }
  }

  output += `\nFinal Quotient (Q): ${toBinary(Q, bits)} = ${Q}\nFinal Remainder (A): ${toBinary(A, bits)} = ${A}\n`;
  document.getElementById('resOutput').textContent = output;
}

// --- NON-RESTORING DIVISION ---
function runNonRestoring() {
  let dividend = parseInt(document.getElementById('nresDividend').value);
  let divisor = parseInt(document.getElementById('nresDivisor').value);
  let bits = parseInt(document.getElementById('nresBits').value);
  let output = "";

  const maxVal = Math.max(Math.abs(dividend), Math.abs(divisor));
  const requiredBits = Math.ceil(Math.log2(maxVal + 1)) + 1;
  if (bits < requiredBits) {
    output += `Number of bits too small! Minimum required bits = ${requiredBits}\nAutomatically adjusting bits to ${requiredBits}.\n\n`;
    bits = requiredBits;
  }

  let A = 0, Q = dividend;
  output += `Non-Restoring Division Algorithm\n`;
  output += `Dividend (Q): ${dividend} -> ${toBinary(dividend, bits)}\n`;
  output += `Divisor (M): ${divisor} -> ${toBinary(divisor, bits)}\n\n`;
  output += `Step\tA (Binary)\tQ (Binary)\tOperation\n`;

  for (let i = 0; i < bits; i++) {
    A = (A << 1) | ((Q >> (bits - 1)) & 1);
    Q = (Q << 1) & ((1 << bits) - 1);

    if (A >= 0) {
      A -= divisor;
      output += `${i + 1}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\tA >= 0 → A = A - M\n`;
    } else {
      A += divisor;
      output += `${i + 1}\t${toBinary(A, bits)}\t${toBinary(Q, bits)}\tA < 0 → A = A + M\n`;
    }

    if (A >= 0) Q |= 1;
  }

  if (A < 0) {
    A += divisor;
    output += `Final restore: A was negative → A = A + M\n`;
  }

  output += `\nFinal Quotient (Q): ${toBinary(Q, bits)} = ${Q}\nFinal Remainder (A): ${toBinary(A, bits)} = ${A}\n`;
  document.getElementById('nresOutput').textContent = output;
}
