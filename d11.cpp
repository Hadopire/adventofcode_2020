#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef int (*func)(std::vector<std::string>&, int, int);

enum TILE {
    FLOOR = '.',
    EMPTY = 'L',
    OCCUPIED = '#'
};

size_t width;
size_t height;

TILE step(std::vector<std::string>& buffer, int x, int y, int dx, int dy)
{
    TILE ret = FLOOR;
    x += dx;
    y += dy;
    while (x >= 0 && x < width && y >= 0 && y < height) {
        if (buffer[y][x] != FLOOR)
            return (TILE)buffer[y][x];
        x += dx;
        y += dy;
    }
    return ret;
}

int directional(std::vector<std::string>& buffer, int x, int y)
{
    int n = 0;
    n += (step(buffer, x, y, -1, -1) == OCCUPIED);
    n += (step(buffer, x, y, -1, 0) == OCCUPIED);
    n += (step(buffer, x, y, -1, 1) == OCCUPIED);
    n += (step(buffer, x, y, 0, 1) == OCCUPIED);
    n += (step(buffer, x, y, 0, -1) == OCCUPIED);
    n += (step(buffer, x, y, 1, -1) == OCCUPIED);
    n += (step(buffer, x, y, 1, 0) == OCCUPIED);
    n += (step(buffer, x, y, 1, 1) == OCCUPIED);
    return n;
}

int adjacent(std::vector<std::string>& buffer, int x, int y)
{
    int n = 0;
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if (i < 0 || i >= width || j < 0 || j >= height || (i == x && j == y))
                continue;
            n += (buffer[j][i] == OCCUPIED);
        }
    }
    return n;
}

size_t simulate(std::vector<std::string> map, func behaviour, int tolerance)
{
    bool changed = true;
    size_t peeps;
    while (changed) {
        changed = false;
        peeps = 0;
        std::vector<std::string> buffer(map);
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                int n = behaviour(buffer, x, y);
                if (buffer[y][x] == OCCUPIED && n >= tolerance) {
                    map[y][x] = EMPTY;
                    changed = true;
                } else if (buffer[y][x] == EMPTY && n == 0) {
                    map[y][x] = OCCUPIED;
                    changed = true;
                }

                peeps += (map[y][x] == OCCUPIED);
            }
        }
    }

    return peeps;
}

int main()
{
    std::fstream file("d11.txt");
    std::string line;
    std::vector<std::string> map;
    while (std::getline(file, line))
        map.push_back(line);
    width = map[0].size();
    height = map.size();

    std::cout << "1: " << simulate(map, adjacent, 4) << std::endl;
    std::cout << "2: " << simulate(map, directional, 5) << std::endl;

    return 0;
}