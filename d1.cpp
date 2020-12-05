#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
    std::fstream file("d1.txt");
    std::vector<int> inputs;
    inputs.reserve(10000);
    int n;
    while (file >> n)
        inputs.push_back(n);
    n = 0;
    for (int i = 0; i < inputs.size(); ++i) {
        for (int j = i+1; j < inputs.size(); ++j) {
            if (inputs[i] + inputs[j] == 2020)  {
                std::cout << "1: " << inputs[i] * inputs[j] << "\n";
                if (n++) return 0;
            }
            for (int k = j+1; k < inputs.size(); ++k) {
                if (inputs[i] + inputs[j] + inputs[k] == 2020) {
                    std::cout << "2: " << inputs[i] * inputs[j] * inputs[k] << "\n";
                    if (n++) return 0;
                }
            }
        }
    }
    return 0;
}