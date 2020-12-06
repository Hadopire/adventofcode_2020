#include <string>
#include <fstream>
#include <iostream>

int main()
{
    std::fstream file("d6.txt");
    std::string line;
    size_t n1 = 0;
    size_t n2 = 0;
    while (!file.eof())
    {
        char answers[26] = {0};
        size_t peeps = 0;
        size_t size = 0;
        while (std::getline(file, line))
        {
            if (!line[0])
                break;
            
            ++peeps;
            for (size_t i = 0; i < line.size(); ++i) {
                size += (answers[line[i]-'a']++ == 0);
            }
        }
        n1 += size;
        for (size_t i = 0; i < 26; ++i) {
            n2 += (answers[i] == peeps);
        }
    }

    std::cout << "1: " << n1 << "\n";
    std::cout << "2: " << n2 << "\n";
    return 0;
}