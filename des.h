#pragma once
 
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>
 
inline std::string bin_to_hex(std::string s) {
	std::string hex = "";
	for (std::size_t i = 0; i < s.size(); i += 4) {
		std::string k = "";
		for (int j = i; j < i + 4; j++)
			k += s[j];
		if (k == "0000") hex += '0';
		else if (k == "0001") hex += '1';
		else if (k == "0010") hex += '2';
		else if (k == "0011") hex += '3';
		else if (k == "0100") hex += '4';
		else if (k == "0101") hex += '5';
		else if (k == "0110") hex += '6';
		else if (k == "0111") hex += '7';
		else if (k == "1000") hex += '8';
		else if (k == "1001") hex += '9';
		else if (k == "1010") hex += 'A';
		else if (k == "1011") hex += 'B';
		else if (k == "1100") hex += 'C';
		else if (k == "1101") hex += 'D';
		else if (k == "1110") hex += 'E';
		else if (k == "1111") hex += 'F';
	}
	return hex;
}
 
inline std::string hex_to_bin(const std::string& s) {
    std::string bin = "";
    for (char digit : s) {
        switch (digit) {
            case '0': bin += "0000"; break;
            case '1': bin += "0001"; break;
            case '2': bin += "0010"; break;
            case '3': bin += "0011"; break;
            case '4': bin += "0100"; break;
            case '5': bin += "0101"; break;
            case '6': bin += "0110"; break;
            case '7': bin += "0111"; break;
            case '8': bin += "1000"; break;
            case '9': bin += "1001"; break;
            case 'A': case 'a': bin += "1010"; break;
            case 'B': case 'b': bin += "1011"; break;
            case 'C': case 'c': bin += "1100"; break;
            case 'D': case 'd': bin += "1101"; break;
            case 'E': case 'e': bin += "1110"; break;
            case 'F': case 'f': bin += "1111"; break;
            default: throw std::invalid_argument("Input harus berupa hexadecimal");
        }
    }
    return bin;
}
 
inline std::string dec_to_bin(int n) {
    std::string bin = "";
    while (n > 0) {
        bin = (char)(n % 2 + '0') + bin;
        n /= 2;
    }
 
    while (bin.size() < 4) bin = '0' + bin;
    return bin;
}
 
 
class DES {
 
public:
    std::string encrypt(const std::string& plaintext, const std::string& key) const;
    std::string decrypt(const std::string& ciphertext, const std::string& key) const;
 
private:
    std::string crypt(const std::string& input, const std::string& key, bool decrypting) const;
    std::string feistel(const std::string& right, const std::string& subkey) const;
    std::array<std::string, 16> make_subkeys(const std::string& key) const;
};
 