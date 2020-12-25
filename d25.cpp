#include <fstream>
#include <iostream>

int main()
{
    std::fstream file("d25.txt");
    size_t card, door;
    file >> card;
    file >> door;

    size_t loopsize = 0;
    size_t value = 1;
    while (value != card && value != door) {
        value = value * 7 % 20201227;
        ++loopsize;
    }

    size_t subject = value == card ? door : card;
    value = 1;
    for (size_t i = 0; i < loopsize; ++i) {
        value = value * subject % 20201227;
    }
    
    std::cout << "1: " << value << std::endl;
    return 0;
}