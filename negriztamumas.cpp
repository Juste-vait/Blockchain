#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <random>
#include <vector>
#include <chrono>
using namespace std;

string hash_funkcija(const string &input) {
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

string generuoti_salt(size_t length) {
    const string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    string salt;

    srand(time(0));

    for (size_t i = 0; i < length; i++) {
        salt.push_back(chars[rand() % chars.size()]);
    }
    return salt;
}

void test_hiding() {
    cout << "HIDING TESTAS" << endl;

    string original_input = "secret";
    string salt = generuoti_salt(4);
    string combined = original_input + salt;

    string hash_value = hash_funkcija(combined);

    cout << "Salt: " << salt << endl;
    cout << "Tikslinis hash: " << hash_value << endl;

    cout << "Brute force bandymas atspėti originalų inputą (tik 4 mažosios raidės):" << endl;

    const string chars = "abcdefghijklmnopqrstuvwxyz";
    int attempts = 0;
    bool found = false;

    auto start = chrono::high_resolution_clock::now();

    for (char a : chars) {
        for (char b : chars) {
            for (char c : chars) {
                for (char d : chars) {
                    string guess_salt = {a, b, c, d};
                    string guess_combined = original_input + guess_salt;
                    attempts++;

                    if (hash_funkcija(guess_combined) == hash_value) {
                        auto end = chrono::high_resolution_clock::now();
                        chrono::duration<double> duration = end - start;
                        cout << "Rasta! Salt = " << guess_salt << " po " << attempts << " bandymų." << endl;
                        cout << "Laikas: " << duration.count() << " s" << endl;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (found) break;
        }
        if (found) break;
    }

    if (!found) {
        cout << "Salt nerastas, reikia daugiau bandymų." << endl;
    }
}

void test_puzzle_friendliness() {
    cout << "\n PUZZLE-FRIENDLINESS TESTAS" << endl;

    string target_hash;
    cout << "Įveskite tikslinį hash, kurį reikia atspėti: ";
    cin >> target_hash;

    const string chars = "abcdefghijklmnopqrstuvwxyz";
    int attempts = 0;
    bool found = false;

    auto start = chrono::high_resolution_clock::now();


    for (char a : chars) {
        for (char b : chars) {
            for (char c : chars) {
                for (char d : chars) {
                    string guess = {a, b, c, d};
                    attempts++;

                    if (hash_funkcija(guess) == target_hash) {
                        auto end = chrono::high_resolution_clock::now();
                        chrono::duration<double> duration = end - start;
                        cout << "Rasta! Input = " << guess << " po " << attempts << " bandymų." << endl;
                        cout << "Laikas: " << duration.count() << " s" << endl;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (found) break;
        }
        if (found) break;
    }

    if (!found) {
        cout << "Nepavyko rasti input per šią paiešką." << endl;
    }
}

int main() {
    test_hiding();
    test_puzzle_friendliness();
    return 0;
}
