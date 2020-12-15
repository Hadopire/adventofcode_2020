#include <iostream>

int main()
{
    const char* input = "1,12,0,20,8,16";
    int* numbers = new int[30000000]();
    int turn = 0, current, last, partone;
    while (*input) {
        current = std::atoi(input);
        while (*input && *input++ != ',');
        if (turn) {
            numbers[last] = turn;
        }
        last = current;
        ++turn;
    }

    while (turn < 30000000) {
        if (numbers[last]) {
            current = turn - numbers[last];
        } else {
            current = 0;
        }
        numbers[last] = turn++;

        last = current;
        if (turn == 2020) {
            partone = current;
        }
    }

    std::cout << "1: " << partone << std::endl;
    std::cout << "2: " << last << std::endl;

    return 0;
}