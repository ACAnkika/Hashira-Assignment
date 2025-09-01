#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

long long convertToDecimal(const string &value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'z') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'Z') digit = 10 + (c - 'A');
        else throw runtime_error("Invalid digit in value");
        if (digit >= base) throw runtime_error("Digit out of base range");
        result = result * base + digit;
    }
    return result;
}

vector<long long> multiplyPoly(const vector<long long> &poly, long long root) {
    vector<long long> result(poly.size() + 1, 0);
    for (size_t i = 0; i < poly.size(); i++) {
        result[i] += poly[i];
        result[i + 1] -= root * poly[i];
    }
    return result;
}

void solvePolynomial(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    json data;
    file >> data;
    int n = data["keys"]["n"];
    int k = data["keys"]["k"];
    int m = k - 1;
    cout << "Processing file: " << filename << endl;
    cout << "n = " << n << ", k = " << k << ", degree = " << m << endl;
    vector<long long> roots;
    for (int i = 1; i <= k; i++) {
        string baseStr = data[to_string(i)]["base"];
        string valueStr = data[to_string(i)]["value"];
        int base = stoi(baseStr);
        long long root = convertToDecimal(valueStr, base);
        roots.push_back(root);
        cout << "Root " << i << " (base " << base << "): " << valueStr 
             << " -> " << root << endl;
    }
    vector<long long> poly = {1};
    for (long long r : roots) {
        poly = multiplyPoly(poly, r);
    }
    cout << "\nPolynomial coefficients (highest degree first):\n";
    for (size_t i = 0; i < poly.size(); i++) {
        cout << poly[i] << (i + 1 < poly.size() ? ", " : "\n");
    }
    cout << "----------------------------------\n";
}

int main() {
    solvePolynomial("test_case_1.json");
    solvePolynomial("test_case_2.json");
    return 0;
}

