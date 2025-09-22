#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>

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

int main() {
    cout << "Iveskite teksta: ";
    string text;
    getline(cin, text);

    cout << "Hash'as: ";
    hash_funkcija(text);

    return 0;
}
