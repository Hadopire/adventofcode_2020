#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

void qmemset(std::unordered_map<size_t, size_t>& memory, size_t addr, size_t value, size_t mask, int i = 35)
{
    size_t bit = 0;
    while (i >= 0 && !bit) {
        bit = mask & (1ull << i--);
    }

    if (bit) {
        qmemset(memory, addr | bit, value, mask, i);
        qmemset(memory, addr & ~bit, value, mask, i);
    } else
        memory[addr] = value;
}

int main()
{
    std::fstream file("d14.txt");
    std::string line;
    size_t mor, mnot, mfloating;
    std::unordered_map<size_t, size_t> memory1;
    std::unordered_map<size_t, size_t> memory2;
    while (std::getline(file, line)) {
        auto it = line.begin();

        if (line.compare(0, 4, "mask") == 0) {
            mor = 0;
            mnot = 0;
            mfloating = 0;
            it += 7;
            for (int i = 0; i < 36; ++i, ++it) {
                switch (*it) {
                case '0':
                    mnot |= (1ull << (35 - i));
                    break;
                case '1':
                    mor |= (1ull << (35 - i));
                    break;
                case 'X':
                    mfloating |= (1ull << (35 - i));
                    break;
                }
            }
        } else if (line.compare(0, 3, "mem") == 0) {
            it += 4;
            size_t idx;
            size_t addr = std::stoull(std::string(it, line.end()), &idx);
            it += idx + 4;
            size_t value = std::stoull(std::string(it, line.end()));

            memory1[addr] = ((value & ~mnot) | mor);
            qmemset(memory2, addr | mor, value, mfloating);
        }
    }

    size_t sum = 0;
    for (auto it = memory1.begin(); it != memory1.end(); ++it) {
        sum += it->second;
    }

    std::cout << "1: " << sum << std::endl;

    sum = 0;
    for (auto it = memory2.begin(); it != memory2.end(); ++it) {
        sum += it->second;
    }

    std::cout << "2: " << sum << std::endl;

    return 0;
}