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
        part3 = part3 + part2 + (part0 * 2);
    }

    for (int i = 0; i < 10; i++) {
        part0 = (part0 ^ (part1 * 3)) * 0x4e97b2d8f3c1a5e3ULL;
        part1 = (part1 ^ (part2 * 5)) * 0xa3d5f79c482eb16fULL;
        part2 = (part2 ^ (part3 * 7)) * 0x5b18e4c7d92fa06eULL;
        part3 = (part3 ^ (part0 * 9)) * 0xc74a9e21f05bd83cULL;
    }

    cout << hex << setw(16) << part0 << setw(16) << part1 << setw(16) << part2 << setw(16) << part3 << endl;
}

int main(int count, char* arg[]) {
    string input;

    if (count> 1) {
        
        ifstream file(arg[1]);
        if (!file.is_open()) {
            cerr << "Klaida: nepavyko atidaryti failo '" << arg[1] << "'." << endl;
            return 1;
        }

        string line;
        while (getline(file, line)) {
            input += line + '\n';
        }
        file.close();

        cout << "Failo turinys nuskaitytas, generuojamas hash'as..." << endl;
    } else {
        cout << "Iveskite teksta: ";
        getline(cin, input);
    }

    cout << "Hash'as: ";
    hash_funkcija(input);

    return 0;
}
