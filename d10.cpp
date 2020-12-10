#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
    std::fstream file("d10.txt");
    std::vector<int> adapters;
    int n;
    int max = 0;
    adapters.push_back(0);
    adapters.push_back(0);
    while (file >> n) {
        adapters.push_back(n);
        max = std::max(n, max);
    }
    adapters.push_back(max + 3);

    std::sort(adapters.begin(), adapters.end());

    std::vector<size_t> total(adapters.size());
    total[0] = 0;
    total[1] = 1;

    int onejolt = 0;
    int threejolt = 0;
    int prev = 0;
    int pprev = 0;
    for (int i = 2; i < adapters.size(); ++i) {
        int diff = adapters[i] - adapters[i - 1];
        if (diff == 1) {
            ++onejolt;
        } else if (diff == 3) {
            ++threejolt;
        }

        if (pprev == 1 && prev == 1 && diff == 1) {
            total[i] = total[i - 1] + total[i - 2] + total[i - 3];
        } else if (prev == 1 && diff == 1) {
            total[i] = total[i - 1] + total[i - 2];
        } else {
            total[i] = total[i - 1];
        }

        pprev = prev;
        prev = diff;
    }

    std::cout << "1: " << onejolt * threejolt << std::endl;
    std::cout << "2: " << total[total.size() - 1] << std::endl;
    return 0;
}