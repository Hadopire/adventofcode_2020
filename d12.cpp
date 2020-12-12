#include <fstream>
#include <iostream>

struct vec2 {
    int x;
    int y;

    size_t manhattan() const {
        return std::abs(x) + std::abs(y);
    }
};

int main()
{
    std::fstream file("d12.txt");

    vec2 position1 = { 0, 0 };
    vec2 position2 = { 0, 0 };
    vec2 dir = { 1, 0 };
    vec2 waypoint = { 10, 1 };
    while (!file.eof()) {
        char action = 0;
        int value;
        vec2 sign = { 1, -1 };

        file >> action;
        file >> value;

        switch (action) {
        case 'N':
            position1.y += value;
            waypoint.y += value;
            break;
        case 'S':
            position1.y -= value;
            waypoint.y -= value;
            break;
        case 'E':
            position1.x += value;
            waypoint.x += value;
            break;
        case 'W':
            position1.x -= value;
            waypoint.x -= value;
            break;
        case 'L':
            sign = { -1, 1 };
        case 'R':
            value /= 90;
            for (int i = 0; i < value; ++i) {
                int x = dir.x;
                dir.x = sign.x * dir.y;
                dir.y = sign.y * x;
                x = waypoint.x;
                waypoint.x = sign.x * waypoint.y;
                waypoint.y = sign.y * x;
            }
            break;
        case 'F':
            position1.x += dir.x * value;
            position1.y += dir.y * value;
            position2.x += waypoint.x * value;
            position2.y += waypoint.y * value;
            break;
        }
    }

    std::cout << "1: " << position1.manhattan() << std::endl;
    std::cout << "2: " << position2.manhattan() << std::endl;

    return 0;
}