#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#define PREAMBLE 25

int main()
{
    std::fstream file("d9.txt");
    int64_t n;
    std::unordered_map<size_t, int> table;
    std::vector<int64_t> preamble;
    std::vector<int64_t> numbers;
    numbers.reserve(10000);
    preamble.reserve(PREAMBLE);
    for (int i = 0; i < PREAMBLE; ++i) {
        file >> n;
        preamble.push_back(n);
        table[n] = i;
        numbers.push_back(n);
    }

    int at = 0;
    while (file >> n) {
        bool found = false;
        numbers.push_back(n);
        for (int i = 0; i < preamble.size(); ++i) {
            if (table.count(n - preamble[i])) {
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "premier romain: " << n << std::endl;
            break;
        }

        table.erase(preamble[at]);
        preamble[at] = n;
        table[n] = at;

        at = (at + 1) % PREAMBLE;
    }

    int i = 0;
    int j = i + 1;
    int64_t sum = numbers[i];
    while (j < numbers.size()) {
        if (sum > n) {
            sum -= numbers[i];
            ++i;
        } else if (sum < n) {
            sum += numbers[j];
            ++j;
        } else
            break;

        if (i == j) {
            i += 1;
            j += 2;
        }
    }

    int64_t min = sum;
    int64_t max = 0;

    for (int k = i; k <= j; ++k) {
        min = std::min(min, numbers[k]);
        max = std::max(max, numbers[k]);
    }

    std::cout << "somme romaine: " << min + max << std::endl;

    return 0;
}