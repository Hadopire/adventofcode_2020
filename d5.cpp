#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

int sum(int min, int max) {
    return ((max - min + 1) * (min + max)) / 2;
}

int main()
{
    std::fstream file("d5.txt");
    std::string line;
    int min = 0x7FFFFFFF;
    int max = 0;
    int total = 0;
    while (std::getline(file, line)) {
        int id = 0;
        for (size_t i = 0; i <= 9; ++i) {
            if (line[i] == 'B' || line[i] == 'R')
                id |= (1<<(9-i));
        }
        min = std::min(id, min);
        max = std::max(id, max);
        total += id;
    }

    std::cout << "1: " << max << std::endl;
    std::cout << "2: " << sum(min,max)-total << std::endl;
    return 0;
}