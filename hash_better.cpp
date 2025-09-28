#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <fstream>

using namespace std;

// Pagalbinė funkcija bitų rotacijai į kairę
inline uint64_t rotl(uint64_t x, unsigned int r) {
    return (x << r) | (x >> (64 - r));
}

void hash_funkcija(const string& input) {

    uint64_t part0 = 0x1234567890abcdefULL;
    uint64_t part1 = 0xfedcba0987654321ULL;
    uint64_t part2 = 0x0f1e2d3c4b5a6978ULL;
    uint64_t part3 = 0x89abcdef01234567ULL;

    // 1. Pirminis maišymas per kiekvieną įvesties baitą
    for (unsigned char b : input) {
        part0 += (b * 0x21) ^ rotl(part3, 5);
        part1 ^= (part0 + (b * 7)) * 0x9e3779b97f4a7c15ULL; // "golden ratio" konstanta
        part1 = rotl(part1, 13);

        part2 += part1 + (b << 3) + rotl(part0, 7);
        part3 ^= part2 + (part1 << 2) + (b * 11);
        part3 = rotl(part3, (b & 31)); // sukimas pagal baito vertę
    }

    // 2. Stiprus maišymas
    for (int i = 0; i < 12; i++) {
        part0 = rotl(part0 ^ (part1 * 3), 17) * 0x4e97b2d8f3c1a5e3ULL;
        part1 = rotl(part1 ^ (part2 * 5), 23) * 0xa3d5f79c482eb16fULL;
        part2 = rotl(part2 ^ (part3 * 7), 29) * 0x5b18e4c7d92fa06eULL;
        part3 = rotl(part3 ^ (part0 * 9), 31) * 0xc74a9e21f05bd83cULL;

        // Pridėtas papildomas maišymas, kad visos dalys sąveikautų
        uint64_t mix = part0 ^ part1 ^ part2 ^ part3;
        part0 += mix;
        part1 ^= rotl(mix, 11);
        part2 += rotl(mix, 19);
        part3 ^= rotl(mix, 27);
    }

    // 3. Rezultato spausdinimas
    cout << hex << setfill('0') << setw(16) << part0 << setw(16) << part1 << setw(16) << part2 << setw(16) << part3 << endl;
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

        cout << "Failo turinys nuskaitytas, generuojamas hash'as." << endl;
    } else {
        cout << "Įveskite tekstą: ";
        getline(cin, input);
    }

    cout << "Hash'as: ";
    hash_funkcija(input);

    return 0;
}