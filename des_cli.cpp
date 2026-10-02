// Pemakaian: des_cli enc|dec KEY_HEX   (data hex dibaca dari stdin, kelipatan 16 digit hex)
// Mode ECB: tiap blok 64 bit diproses terpisah memakai kelas DES di des.cpp.
#include "des.h"
#include <iostream>
 
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Pemakaian: des_cli enc|dec KEY_HEX  (data hex dari stdin)\n";
        return 1;
    }
    const std::string mode = argv[1], key = argv[2];
    std::string data;
    std::cin >> data;
 
    if ((mode != "enc" && mode != "dec") || data.empty() || data.size() % 16 != 0) {
        std::cerr << "Mode harus enc/dec dan data harus kelipatan 16 digit hex\n";
        return 1;
    }
    try {
        DES des;
        std::string out;
        for (std::size_t i = 0; i < data.size(); i += 16) {
            const std::string block = data.substr(i, 16);
            out += (mode == "enc") ? des.encrypt(block, key) : des.decrypt(block, key);
        }
        std::cout << out << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}