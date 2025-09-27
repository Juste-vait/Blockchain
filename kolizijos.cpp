#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <random>
#include <unordered_set>
#include <vector>
#include <sstream>
using namespace std;

const int PAIRS = 100000;

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

    for (int i = 0; i < 10; i++) {
        part0 = (part0 ^ (part1 * 3)) * 0x4e97b2d8f3c1a5e3ULL;
        part1 = (part1 ^ (part2 * 5)) * 0xa3d5f79c482eb16fULL;
        part2 = (part2 ^ (part3 * 7)) * 0x5b18e4c7d92fa06eULL;
        part3 = (part3 ^ (part0 * 9)) * 0xc74a9e21f05bd83cULL;
    }

    stringstream ss;
    ss << hex << setfill('0') << setw(16) << part0 << setw(16) << part1 << setw(16) << part2 << setw(16) << part3;

    return ss.str();
}

string generuoti_random(size_t length) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz";
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(0, chars.size() - 1);

    string result;
    result.reserve(length);
    for (size_t i = 0; i < length; i++) {
        result.push_back(chars[dist(gen)]);
    }
    return result;
}

void testavimas(size_t text_len) {
    cout << "Ilgis: " << text_len << " simbolių." << endl;

    int collisions = 0;

    for (int i = 0; i < PAIRS; i++) {

        string s1 = generuoti_random(text_len);
        string s2 = generuoti_random(text_len);

        while (s1 == s2) {
            s2 = generuoti_random(text_len);
        }

        string h1 = hash_funkcija(s1);
        string h2 = hash_funkcija(s2);

        if (h1 == h2) {
            collisions++;
        }
    }

    cout << "Kolizijų skaičius: " << collisions << " iš " << PAIRS << " porų." << endl;
    cout << "--------------------------------------------" << endl;
}

int main() {
    cout << endl;
    vector<size_t> lengths = {10, 100, 500, 1000};

    for (size_t len : lengths) {
        testavimas(len);
    }

    cout << "Testavimas baigtas." << endl;
    return 0;
}
