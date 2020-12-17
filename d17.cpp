// super slow because gcc/clang implementation of stl unordered map are shit
// it's fast when compiled with visual studio (how can gcc and clang be worse ???)

// tldr: stl containers suck

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct vec4 {
    int x;
    int y;
    int z;
    int w;

    bool operator==(const vec4& rhs) const
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }

    vec4 operator+(const vec4& rhs) const
    {
        return vec4 {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
            w + rhs.w
        };
    }
};

vec4 neighbours3d[] = {
    { -1, -1, -1 }, { 0, -1, -1 }, { 1, -1, -1 },
    { -1, 0, -1 }, { 0, 0, -1 }, { 1, 0, -1 },
    { -1, 1, -1 }, { 0, 1, -1 }, { 1, 1, -1 },
    { -1, -1, 0 }, { 0, -1, 0 }, { 1, -1, 0 },
    { -1, 0, 0 }, { 1, 0, 0 },
    { -1, 1, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
    { -1, -1, 1 }, { 0, -1, 1 }, { 1, -1, 1 },
    { -1, 0, 1 }, { 0, 0, 1 }, { 1, 0, 1 },
    { -1, 1, 1 }, { 0, 1, 1 }, { 1, 1, 1 }
};

vec4 neighbours4d[] = {
    { -1, -1, -1, -1 }, { 0, -1, -1, -1 }, { 1, -1, -1, -1 },
    { -1, 0, -1, -1 }, { 0, 0, -1, -1 }, { 1, 0, -1, -1 },
    { -1, 1, -1, -1 }, { 0, 1, -1, -1 }, { 1, 1, -1, -1 },
    { -1, -1, 0, -1 }, { 0, -1, 0, -1 }, { 1, -1, 0, -1 },
    { -1, 0, 0, -1 }, { 0, 0, 0, -1 }, { 1, 0, 0, -1 },
    { -1, 1, 0, -1 }, { 0, 1, 0, -1 }, { 1, 1, 0, -1 },
    { -1, -1, 1, -1 }, { 0, -1, 1, -1 }, { 1, -1, 1, -1 },
    { -1, 0, 1, -1 }, { 0, 0, 1, -1 }, { 1, 0, 1, -1 },
    { -1, 1, 1, -1 }, { 0, 1, 1, -1 }, { 1, 1, 1, -1 },
    { -1, -1, -1, 0 }, { 0, -1, -1, 0 }, { 1, -1, -1, 0 },
    { -1, 0, -1, 0 }, { 0, 0, -1, 0 }, { 1, 0, -1, 0 },
    { -1, 1, -1, 0 }, { 0, 1, -1, 0 }, { 1, 1, -1, 0 },
    { -1, -1, 0, 0 }, { 0, -1, 0, 0 }, { 1, -1, 0, 0 },
    { -1, 0, 0, 0 }, { 1, 0, 0, 0 },
    { -1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 },
    { -1, -1, 1, 0 }, { 0, -1, 1, 0 }, { 1, -1, 1, 0 },
    { -1, 0, 1, 0 }, { 0, 0, 1, 0 }, { 1, 0, 1, 0 },
    { -1, 1, 1, 0 }, { 0, 1, 1, 0 }, { 1, 1, 1, 0 },
    { -1, -1, -1, 1 }, { 0, -1, -1, 1 }, { 1, -1, -1, 1 },
    { -1, 0, -1, 1 }, { 0, 0, -1, 1 }, { 1, 0, -1, 1 },
    { -1, 1, -1, 1 }, { 0, 1, -1, 1 }, { 1, 1, -1, 1 },
    { -1, -1, 0, 1 }, { 0, -1, 0, 1 }, { 1, -1, 0, 1 },
    { -1, 0, 0, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 },
    { -1, 1, 0, 1 }, { 0, 1, 0, 1 }, { 1, 1, 0, 1 },
    { -1, -1, 1, 1 }, { 0, -1, 1, 1 }, { 1, -1, 1, 1 },
    { -1, 0, 1, 1 }, { 0, 0, 1, 1 }, { 1, 0, 1, 1 },
    { -1, 1, 1, 1 }, { 0, 1, 1, 1 }, { 1, 1, 1, 1 }
};

namespace std {
template <>
struct hash<vec4> {
    size_t operator()(const vec4& v) const
    {
        size_t h1 = std::hash<int> {}(v.x);
        size_t h2 = std::hash<int> {}(v.y);
        size_t h3 = std::hash<int> {}(v.z);
        size_t h4 = std::hash<int> {}(v.w);
        return ((((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) >> 1) ^ (h4 << 1);
    }
};
}

size_t count(const std::unordered_map<vec4, bool>& grid)
{
    size_t n = 0;
    for (const auto& pair : grid) {
        if (pair.second)
            ++n;
    }
    return n;
}

int main()
{
    std::unordered_map<vec4, bool> grid;
    std::unordered_map<vec4, bool> inputGrid;

    std::fstream file("d17.txt");
    std::string line;
    vec4 p = { 0, 0, 0, 0 };
    while (std::getline(file, line)) {
        const char* str = line.c_str();
        p.x = 0;
        while (*str) {
            if (*str == '#')
                inputGrid[p] = true;
            ++p.x;
            ++str;
        }
        --p.y;
    }

    grid = inputGrid;
    for (int i = 0; i < 6; ++i) {
        std::unordered_map<vec4, int> neighbours;
        for (const auto& pair : grid) {
            const vec4& position = pair.first;
            neighbours.emplace(position, 0);
            for (int j = 0; j < 26; ++j) {
                vec4 at = position + neighbours3d[j];
                neighbours[at] += 1;
            }
        }

        for (const auto& pair : neighbours) {
            const vec4& position = pair.first;
            int n = pair.second;
            if (grid.count(position)) {
                if (n != 3 && n != 2) {
                    grid.erase(position);
                }
            } else if (n == 3) {
                grid[position] = true;
            }
        }
    }

    std::cout << "1: " << count(grid) << std::endl;

    grid = inputGrid;
    for (int i = 0; i < 6; ++i) {
        std::unordered_map<vec4, int> neighbours;
        for (const auto& pair : grid) {
            const vec4& position = pair.first;
            neighbours.emplace(position, 0);
            for (int j = 0; j < 80; ++j) {
                vec4 at = position + neighbours4d[j];
                neighbours[at] += 1;
            }
        }

        for (const auto& pair : neighbours) {
            const vec4& position = pair.first;
            int n = pair.second;
            if (grid.count(position)) {
                if (n != 3 && n != 2) {
                    grid.erase(position);
                }
            } else if (n == 3) {
                grid[position] = true;
            }
        }
    }

    std::cout << "2: " << count(grid) << std::endl;

    return 0;
}