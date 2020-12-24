#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

struct vec2 {
    int x;
    int y;

    bool operator==(const vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    vec2 operator+(const vec2& rhs) const
    {
        return vec2{x + rhs.x, y + rhs.y};
    }
};

struct hash {
    size_t operator()(const vec2& v) const
    {
        size_t h1 = std::hash<int> {}(v.x);
        size_t h2 = std::hash<int> {}(v.y);
        return h1 ^ (h2 << 1);
    }
};

int main()
{
    std::unordered_map<vec2, bool, hash> tiles;
    std::unordered_map<std::string, vec2> directionsmap = {
        { "ne", vec2 { 1, -1 } },
        { "e", vec2 { 1, 0 } },
        { "se", vec2 { 0, 1 } },
        { "sw", vec2 { -1, 1 } },
        { "w", vec2 { -1, 0 } },
        { "nw", vec2 { 0, -1 } },
    };
    std::vector<vec2> directionsarray = {
        vec2 { 1, -1 },
        vec2 { 1, 0 },
        vec2 { 0, 1 },
        vec2 { -1, 1 },
        vec2 { -1, 0 },
        vec2 { 0, -1 },
    };

    std::fstream file("d24.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line[0])
            continue;

        const char* str = line.c_str();
        vec2 position { 0, 0 };
        while (*str) {
            vec2 direction;
            switch (*str) {
            case 's':
            case 'n':
                direction = directionsmap[std::string(str, 2)];
                str += 2;
                break;
            default:
                direction = directionsmap[std::string(str, 1)];
                ++str;
                break;
            }
            position = position + direction;
        }
        if (tiles.count(position))
            tiles.erase(position);
        else
            tiles[position] = true; 
    }

    std::cout << "1: " << tiles.size() << std::endl;

    for (int i = 0; i < 100; ++i) {
        std::unordered_map<vec2, int, hash> neighbours;
        for (const auto& pair : tiles) {
            const vec2& position = pair.first;
            neighbours.emplace(position, 0);
            for (int j = 0; j < directionsarray.size(); ++j) {
                vec2 at = position + directionsarray[j];
                neighbours[at] += 1;
            }
        }

        for (const auto& pair : neighbours){
            const vec2& position = pair.first;
            int n = pair.second;
            if (tiles.count(position)) {
                if (n == 0 || n > 2) {
                    tiles.erase(position);
                }
            } else if (n == 2) {
                tiles[position] = true;
            }
        }
    }

    std::cout << "2: " << tiles.size() << std::endl;

    return 0;
}
