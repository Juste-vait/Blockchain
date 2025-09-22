#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <fstream>

using namespace std;

void hash_funkcija(const string& input) {
    uint64_t part0 = 0x1234567890abcdefULL;
    uint64_t part1 = 0xfedcba0987654321ULL;
    uint64_t part2 = 0x0f1e2d3c4b5a6978ULL;
    uint64_t part3 = 0x89abcdef01234567ULL;

    for (unsigned char b : input) {
        part0 = part0 + b;
        part1 = part1 + part0 * 3;
        part2 = part2 + part1 + (b * 7);
        part3 = part3 + part2 + (part0 << 1);
    }

    cout << hex << setfill('0') << setw(16) << part0 << setw(16) << part1 << setw(16) << part2 << setw(16) << part3 << endl;
}

int main(int argc, char* argv[]) {
    string input_text;

    if (argc > 1) {
        
        ifstream file(argv[1]);
        if (!file.is_open()) {
            cerr << "Klaida: nepavyko atidaryti failo '" << argv[1] << "'." << endl;
            return 1;
        }

        string line;
        while (getline(file, line)) {
            input_text += line + '\n';
        }
        file.close();

        cout << "Failo turinys nuskaitytas, generuojamas hash'as..." << endl;
    } else {
        cout << "Iveskite teksta: ";
        getline(cin, input_text);
    }

    cout << "Hash'as: ";
    hash_funkcija(input_text);

    return 0;
}
