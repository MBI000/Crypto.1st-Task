
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Convert a numeric key or word key into a Caesar shift
int getShift(const string& key) {
    bool isNumber = !key.empty();

    for (char character : key) {
        if (!isdigit(static_cast<unsigned char>(character))) {
            isNumber = false;
            break;
        }
    }

    // If the key contains only digits, use it directly
    if (isNumber) {
        return stoi(key) % 26;
    }

    // If the key is a word, calculate the shift from its letters
    int shift = 0;

    for (char character : key) {
        if (isalpha(static_cast<unsigned char>(character))) {
            char uppercaseCharacter =
                static_cast<char>(toupper(static_cast<unsigned char>(character)));

            // A = 1, B = 2, ..., Z = 26
            shift += uppercaseCharacter - 'A' + 1;
        }
    }

    return shift % 26;
}

// Encrypt or decrypt text using a Caesar shift
string caesarCipher(const string& text, int shift) {
    string result = text;

    for (char& character : result) {
        if (isupper(static_cast<unsigned char>(character))) {
            character = static_cast<char>(
                'A' + (character - 'A' + shift + 26) % 26
            );
        }
        else if (islower(static_cast<unsigned char>(character))) {
            character = static_cast<char>(
                'a' + (character - 'a' + shift + 26) % 26
            );
        }
    }

    return result;
}

int main() {
    string plainText;
    string key;

    cout << "Enter plain text: ";
    getline(cin, plainText);

    cout << "Enter encryption key (number or word): ";
    getline(cin, key);

    int shift = getShift(key);

    cout << "\nCalculated shift: " << shift << endl;

    string encryptedText = caesarCipher(plainText, shift);
    string decryptedText = caesarCipher(encryptedText, -shift);

    cout << "\nEncrypted text: " << encryptedText << endl;
    cout << "Decrypted text: " << decryptedText << endl;

    return 0;
}