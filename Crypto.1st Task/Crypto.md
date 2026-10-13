# Caesar Cipher Project

## 1. Project overview

This project is a C++ command-line program that encrypts and decrypts text
using a **Caesar cipher**. A Caesar cipher replaces every alphabetic character
with another character a fixed number of positions away in the alphabet.

The program accepts:

1. Plain text from the user.
2. An encryption key, which may be a number or a word.

The key is converted into a shift from `0` to `25`. The program then:

```text
                    +------------------+
                    |   User input     |
                    | text and key     |
                    +--------+---------+
                             |
                             v
                    +------------------+
                    |  Calculate shift |
                    +--------+---------+
                             |
                             v
                    +------------------+
                    | Encrypt plaintext|
                    +--------+---------+
                             |
                             v
                    +------------------+
                    | Decrypt ciphertext|
                    +--------+---------+
                             |
                             v
                    +------------------+
                    | Display results  |
                    +------------------+
```

The implementation is contained in [`Crypto.cpp`](./Crypto.cpp).

> **Important:** This project is useful for learning substitution ciphers, but
> a Caesar cipher is not secure for protecting real or sensitive information.

---

## 2. What is a Caesar cipher?

The English alphabet contains 26 letters:

```text
Index:    0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
Upper:    A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
Lower:    a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
```

For a shift of `3`:

```text
Plain:     A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
Cipher:    D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  A  B  C
```

For example:

```text
HELLO  ->  KHOOR
```

Each letter moves three positions to the right. When the end of the alphabet
is reached, the cipher wraps around to the beginning:

```text
X + 3 = A
Y + 3 = B
Z + 3 = C
```

Spaces, punctuation, and non-alphabetic characters are not changed.

---

## 3. Key handling

The function `getShift()` supports two key formats.

### 3.1 Numeric key

If every character in the key is a digit, the number is used as the shift.
Because the alphabet has 26 letters, the shift is reduced with modulo 26:

$$
s = k \bmod 26
$$

where:

- \(k\) is the numeric key,
- \(s\) is the final Caesar shift.

Example:

```text
Key:       29
Calculation: 29 mod 26 = 3
Final shift: 3
```

Therefore, a key of `29` has the same effect as a key of `3`.

### 3.2 Word key

If the key is not entirely numeric, alphabetic characters are converted using:

```text
A = 1, B = 2, C = 3, ..., Z = 26
```

The values are added and reduced modulo 26:

$$
s = \left(\sum_{i=1}^{n} v(c_i)\right) \bmod 26
$$

where \(v(c_i)\) is the value of the \(i\)-th alphabetic character.

Example using the key `KEY`:

```text
K = 11
E =  5
Y = 25
---------
   41

41 mod 26 = 15
```

The word `KEY` therefore produces a shift of `15`.

Characters in a word key that are not alphabetic are ignored during this
calculation. For example, `A-B` is treated as `A + B = 1 + 2 = 3`.

If the key contains no digits and no alphabetic characters, its calculated
shift is `0`.

---

## 4. Mathematical model

### 4.1 Converting letters to numbers

Each letter is represented by an integer in the range \(0\) through \(25\):

```text
A/a = 0
B/b = 1
C/c = 2
...
Z/z = 25
```

Let:

- \(p\) be the numeric value of a plaintext letter,
- \(c\) be the numeric value of a ciphertext letter,
- \(s\) be the shift.

### 4.2 Encryption equation

Encryption moves a letter forward by the shift:

$$
c = (p + s) \bmod 26
$$

Example with `X` and a shift of `3`:

$$
X = 23
$$

$$
c = (23 + 3) \bmod 26 = 26 \bmod 26 = 0
$$

The numeric value `0` represents `A`, so:

```text
X -> A
```

### 4.3 Decryption equation

Decryption reverses the shift:

$$
p = (c - s) \bmod 26
$$

In the C++ implementation, `+ 26` is added before applying modulo:

```cpp
(character - 'a' + shift + 26) % 26
```

This prevents a negative intermediate value when decrypting letters near the
beginning of the alphabet.

### 4.4 Uppercase and lowercase

Uppercase and lowercase letters are processed independently:

```cpp
'A' + (character - 'A' + shift + 26) % 26
'a' + (character - 'a' + shift + 26) % 26
```

This means:

```text
Hello World -> Khoor Zruog    (shift = 3)
```

The original capitalization is preserved.

---

## 5. Program flow

```text
START
  |
  v
Read plaintext
  |
  v
Read key
  |
  v
Is the key made only of digits?
  |                         |
 Yes                       No
  |                         |
  v                         v
key mod 26          Add A=1...Z=26
  |                  values for letters
  |                         |
  +------------+------------+
               |
               v
       Store calculated shift
               |
               v
  Encrypt each alphabetic character
               |
               v
  Decrypt encrypted text with -shift
               |
               v
       Display both results
               |
               v
              END
```

### Character processing

For every character in the input:

```text
                         +----------------+
                         | Is it uppercase?|
                         +-------+--------+
                                 |
                       +---------+---------+
                       |                   |
                      Yes                 No
                       |                   |
                       v                   v
              Shift within A-Z     Is it lowercase?
                                           |
                                  +--------+--------+
                                  |                 |
                                 Yes               No
                                  |                 |
                                  v                 v
                         Shift within a-z   Copy unchanged
```

---

## 6. Worked example

Suppose the user enters:

```text
Plain text: Hello, World!
Key: 3
```

The numeric key produces:

$$
s = 3 \bmod 26 = 3
$$

Each letter is shifted:

```text
H -> K
e -> h
l -> o
l -> o
o -> r
, -> ,
  -> space
W -> Z
o -> r
r -> u
l -> o
d -> g
! -> !
```

The output is:

```text
Encrypted text: Hello, World! -> Khoor, Zruog!
Decrypted text: Khoor, Zruog! -> Hello, World!
```

The punctuation and space remain unchanged.

---

## 7. Main functions

### `getShift(const string& key)`

This function converts the user-provided key into a shift:

1. Checks whether the key contains only digits.
2. Uses a numeric key directly, reduced modulo 26.
3. Otherwise, adds the values of alphabetic characters in the key.
4. Returns the final shift from `0` to `25`.

### `caesarCipher(const string& text, int shift)`

This function applies the shift:

1. Copies the input text.
2. Processes every character.
3. Shifts uppercase letters within `A`–`Z`.
4. Shifts lowercase letters within `a`–`z`.
5. Copies all other characters unchanged.

The same function performs both operations:

```cpp
string encryptedText = caesarCipher(plainText, shift);
string decryptedText = caesarCipher(encryptedText, -shift);
```

Encryption uses \(+s\), while decryption uses \(-s\).

### `main()`

The `main()` function:

1. Reads the plaintext using `getline()`, so spaces are accepted.
2. Reads the key using `getline()`.
3. Calculates the shift.
4. Encrypts the plaintext.
5. Decrypts the encrypted result.
6. Prints both results.

---

## 8. Correctness property

Encryption and decryption are inverse operations:

$$
D(E(p)) = ((p+s)-s) \bmod 26
$$

Therefore:

$$
D(E(p)) = p \bmod 26
$$

For every alphabetic character, decrypting the encrypted text returns the
original character. Non-alphabetic characters are copied unchanged, so the
complete original text is recovered.

---

## 9. Complexity

Let \(n\) be the number of characters in the text and \(m\) the number of
characters in the key.

### Time complexity

Key conversion takes:

$$
O(m)
$$

Encryption and decryption each take:

$$
O(n)
$$

Overall:

$$
O(m+n)
$$

### Space complexity

The function creates a result string with a copy of the input:

$$
O(n)
$$

---

## 10. Building and running

Compile with a C++17-compatible compiler:

```bash
g++ -std=c++17 Crypto.cpp -o Crypto
```

Run the program:

```bash
./Crypto
```

On Windows, the executable can be run with:

```powershell
Crypto.exe
```

---

## 11. Security limitations

This implementation is intended for education and demonstration. It should
not be used for passwords, private messages, financial information, or other
confidential data.

The Caesar cipher is weak because:

- There are only 26 possible shifts.
- An attacker can try every possible shift quickly.
- Letter-frequency analysis can reveal the plaintext.
- The same letter is always replaced by the same letter.
- The key is not a cryptographically secure key.

For real security, use a well-reviewed cryptographic library and a modern
authenticated encryption algorithm such as AES-GCM or ChaCha20-Poly1305.
