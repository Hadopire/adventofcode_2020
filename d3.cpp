#include <string>
#include <fstream>
#include <iostream>
#include <vector>

size_t treeslope(const std::vector<std::string> &map, size_t stepx, size_t stepy)
{
    size_t ret = 0;
    size_t x = stepx;
    for (size_t y = stepy; y < map.size(); y += stepy) {
        ret += (map[y][x] == '#');
        x = (x + stepx) % map[0].size();
    }
    return ret;
}

int main()
{
    std::fstream file("d3.txt");
    std::string line;
    std::vector<std::string> map;
    while (std::getline(file, line))
        map.push_back(line);
    
    size_t count = treeslope(map, 3, 1);
    std::cout << count << "\n";
    count *= treeslope(map, 1, 1);
    count *= treeslope(map, 5, 1);
    count *= treeslope(map, 7, 1);
    count *= treeslope(map, 1, 2);
    std::cout << count << "\n";

    return 0;
}