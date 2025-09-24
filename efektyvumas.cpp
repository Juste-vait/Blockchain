#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdint>
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

    stringstream ss;
    ss << hex << setfill('0') << setw(16) << part0
       << setw(16) << part1
       << setw(16) << part2
       << setw(16) << part3;
    return ss.str();
}

vector<string> read_file_lines(const string &filename) {
    ifstream file(filename);
    vector<string> lines;

    if (!file.is_open()) {
        cerr << "Klaida: nepavyko atidaryti failo '" << filename << "'." << endl;
        return lines;
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

double measure_time(const vector<string> &lines, int count, int repeats) {
    using namespace chrono;
    double total_time = 0.0;

    for (int r = 0; r < repeats; r++) {
        string input;
        for (int i = 0; i < count && i < (int)lines.size(); i++) {
            input += lines[i] + "\n";
        }

        auto start = high_resolution_clock::now();
        string hash = hash_funkcija(input);
        auto end = high_resolution_clock::now();

        duration<double, milli> duration_ms = end - start;
        total_time += duration_ms.count();
    }

    return total_time / repeats;
}

int main() {
    string filename = "konstitucija.txt";
    vector<string> lines = read_file_lines(filename);

    if (lines.empty()) {
        cerr << "Failas tuščias arba nerastas." << endl;
        return 1;
    }

    int line_count;
    cout << "Iveskite kiek eiluciu naudoti testui: ";
    cin >> line_count;

    if (line_count > (int)lines.size()) {
        cerr << "Failas turi tik " << lines.size() << " eilučių, pasirinkta per daug!\n";
        return 1;
    }

    int repeats;
    cout << "Įveskite kiek kartų kartoti matavimą: ";
    cin >> repeats;

    if (repeats <= 0) {
        cerr << "Pakartojimų skaičius turi būti daugiau už 0!\n";
        return 1;
    }

    cout << "\nTestuojama su " << line_count << " eilutėmis, pakartojama " << repeats << " kartų.\n";

    double avg_time = measure_time(lines, line_count, repeats);

    cout << "Rezultatai:\n";
    cout << setw(10) << "Eilutės" << setw(20) << "Vid. laikas (ms)\n";
    cout << string(30, '-') << "\n";
    cout << setw(10) << line_count << setw(20) << fixed << setprecision(4) << avg_time << "\n" << endl;;

    return 0;
}
