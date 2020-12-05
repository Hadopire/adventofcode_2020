#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cstring>

bool validate(std::unordered_map<std::string, std::string> &map)
{
    const char *s;
    std::string str;
    int i = std::stoi(map["byr"]);

    if (map["byr"].size() != 4)
        return false;
    if (i < 1920 || i > 2002)
        return false;

    i = std::stoi(map["iyr"]);
    if (map["iyr"].size() != 4)
        return false;
    if (i < 2010 || i > 2020)
        return false;

    i = std::stoi(map["eyr"]);
    if (map["eyr"].size() != 4)
        return false;
    if (i < 2020 || i > 2030)
        return false;

    str = map["hgt"].c_str() + (map["hgt"].size() - 2);
    i = std::stoi(map["hgt"]);
    if (str == "cm") {
        if (i < 150 || i > 193)
            return false;
    }
    else if (str == "in") {
        if (i < 59 || i > 76)
            return false;
    }
    else
        return false;

    if (map["hcl"][0] != '#' || map["hcl"].size() != 7)
        return false;

    s = map["hcl"].c_str() + 1;
    for (;*s;++s) {
        if (!isdigit(*s) && (*s < 'a' || *s > 'f'))
            return false;
    }

    str = map["ecl"];
    if (str != "amb" && str != "blu" && str != "brn" && str != "gry" && str != "grn" && str != "hzl" && str != "oth")
        return false;

    if (map["pid"].size() != 9)
        return false;

    s = map["pid"].c_str();
    for (;*s;++s) {
        if (!isdigit(*s))
            return false;
    }

    return true;
}

int main()
{
    std::fstream file("d4.txt");
    std::string line;
    size_t n1 = 0;
    size_t n2 = 0;
    while (!file.eof())
    {
        std::unordered_map<std::string, std::string> map;
        while (std::getline(file, line))
        {
            if (!line[0])
                break;

            std::string pair;
            size_t delim = 0;
            size_t start = 0;
            size_t end = 0;
            std::string sub;
            while ((end = line.find(' ', start)) != std::string::npos)
            {
                pair = line.substr(start, end - start);
                delim = pair.find(':');
                map[pair.substr(0, delim)] = pair.substr(delim + 1);

                start = end + 1;
            }
            pair = line.substr(start);
            delim = pair.find(':');
            map[pair.substr(0, delim)] = pair.substr(delim + 1);
        }
        if (map.count("byr") && map.count("iyr") && map.count("eyr") && map.count("hgt") && map.count("hcl") && map.count("ecl") && map.count("pid")) {
            ++n1;
            n2 += validate(map);
        }
    }

    std::cout << "1: " << n1 << "\n";
    std::cout << "2: " << n2 << "\n";
    return 0;
}