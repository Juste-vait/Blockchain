#include <sstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <random>
#include <limits>
#include <iostream>

using namespace std;

string hash_funkcija(const string& input) {
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

    stringstream ss;
    ss << hex << setfill('0')  << setw(16) << part0 << setw(16) << part1 << setw(16) << part2 << setw(16) << part3;

    return ss.str();
}

string generuoti_random(size_t ilgis) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz";
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(0, chars.size() - 1);

    string rezultatas;
    rezultatas.reserve(ilgis);
    for (size_t i = 0; i < ilgis; i++) {
        rezultatas.push_back(chars[dist(gen)]);
    }
    return rezultatas;
}

void pakeisti_viena(string& str) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz";
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist_char(0, chars.size() - 1);
    static uniform_int_distribution<> dist_pos(0, str.size() - 1);

    size_t pos = dist_pos(gen);
    char new_char = chars[dist_char(gen)];
    while (new_char == str[pos]) {
        new_char = chars[dist_char(gen)];
    }
    str[pos] = new_char;
}

int skaiciuoti_bitus(const string& h1, const string& h2) {
    int count = 0;
    for (size_t i = 0; i < h1.size(); i++) {
        uint8_t v1 = stoi(h1.substr(i, 1), nullptr, 16);
        uint8_t v2 = stoi(h2.substr(i, 1), nullptr, 16);
        uint8_t xor_val = v1 ^ v2;
        for (int b = 0; b < 4; b++) {
            count += (xor_val >> b) & 1;
        }
    }
    return count;
}

int skaiciuoti_hex(const string& h1, const string& h2) {
    int count = 0;
    for (size_t i = 0; i < h1.size(); i++) {
        if (h1[i] != h2[i]) count++;
    }
    return count;
}

int main() {
    const int kiekis = 100000;
    const int ilgis = 20;

    int min_bits = numeric_limits<int>::max();
    int max_bits = 0;
    long long total_bits = 0;

    int min_hex = numeric_limits<int>::max();
    int max_hex = 0;
    long long total_hex = 0;

    for (int i = 0; i < kiekis; i++) {

        string s1 = generuoti_random(ilgis);
        string s2 = s1;
        pakeisti_viena(s2);

        string h1 = hash_funkcija(s1);
        string h2 = hash_funkcija(s2);

        int bit_diff = skaiciuoti_bitus(h1, h2);
        int hex_diff = skaiciuoti_hex(h1, h2);

        min_bits = min(min_bits, bit_diff);
        max_bits = max(max_bits, bit_diff);
        total_bits += bit_diff;

        min_hex = min(min_hex, hex_diff);
        max_hex = max(max_hex, hex_diff);
        total_hex += hex_diff;
    }

    cout << "BITAI:" << endl;
    cout << "Min: " << min_bits << " /256 (" << (min_bits / 256.0 * 100) << "%)" << endl;
    cout << "Max: " << max_bits << " /256 (" << (max_bits / 256.0 * 100) << "%)" << endl;
    cout << "Vidurkis: " << (total_bits / (double)kiekis) << " /256 (" << (total_bits / (double)kiekis / 256.0 * 100) << "%)" << endl;

    cout << "\nHEX:" << endl;
    cout << "Min: " << min_hex << " /64 (" << (min_hex / 64.0 * 100) << "%)" << endl;
    cout << "Max: " << max_hex << " /64 (" << (max_hex / 64.0 * 100) << "%)" << endl;
    cout << "Vidurkis: " << (total_hex / (double)kiekis) << " /64 (" << (total_hex / (double)kiekis / 64.0 * 100) << "%)" << endl;

    return 0;
}

