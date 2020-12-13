#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// https://en.wikipedia.org/wiki/Chinese_remainder_theorem

struct bus {
    size_t a;
    size_t n;
};

size_t extended_gcd(size_t a, size_t b, int64_t* u, int64_t* v)
{
    if (b == 0) {
        *u = 1;
        *v = 0;
        return a;
    }

    int64_t ut, vt;
    size_t ret = extended_gcd(b, a % b, &ut, &vt);
    *u = vt;
    *v = ut - (a / b) * vt;
    return ret;
}

size_t mod(int64_t x, int64_t n)
{
    return (x % n + n) % n;
}

int main()
{
    std::fstream file("d13.txt");
    size_t earliest;

    size_t minwait = 0x7ffffff;
    size_t first;
    size_t index = 0;
    std::vector<bus> buses;
    size_t n = 1;

    file >> earliest;
    file.ignore();
    std::string line;
    std::getline(file, line);
    const char* str = line.c_str();
    while (*str) {
        while (*str && !std::isdigit(*str)) {
            if (*str == 'x')
                ++index;
            ++str;
        }
        if (!*str)
            break;

        size_t idx;
        size_t id = std::stol(str, &idx);
        str += idx;

        // part 1
        size_t wait = id - (earliest % id);
        if (wait < minwait) {
            first = id * wait;
            minwait = wait;
        }
        //part 2
        buses.push_back({ id - index, id });
        n *= id;

        ++index;
    }

    size_t second = 0;
    for (int i = 0; i < buses.size(); ++i) {
        int64_t u, v;
        size_t nhat = n / buses[i].n;
        extended_gcd(buses[i].n, nhat, &u, &v);
        size_t e = mod(v, buses[i].n) * nhat;
        second += buses[i].a * e;
    }

    std::cout << "1: " << first << std::endl;
    std::cout << "2: " << second % n << std::endl;

    return 0;
}