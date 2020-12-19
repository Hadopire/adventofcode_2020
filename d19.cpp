// SUPER UGLY but it took me so long to solve this one that i'll clean later


#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


bool isblankstr(const std::string& str)
{
    for (int i = 0; i < str.size(); ++i) {
        if (!std::isspace(str[i]))
            return false;
    }
    return true;
}

bool match(std::string& str, const std::string& rule, std::unordered_map<int, std::string>& rules, const std::string& buf = "", size_t pos = 0)
{
    if (pos == str.size() && isblankstr(rule) && isblankstr(buf))
        return true;
    else if (pos >= str.size())
        return false;

    size_t at = 0;
    while (at < rule.size())
    {
        while (rule[at] == ' ' || rule[at] == '|')
            ++at;
        if (at >= rule.size())
            break;


        size_t next = rule.find('|', at);
        std::string branch = rule.substr(at, next - 1);

        if (rule[at] == '"') {
            if (rule[at + 1] == str[pos]) {
                if (match(str, rule.substr(at + 3,next - at) + buf, rules,"",pos + 1))
                    return true;
            }
        } else {
            int n = 0;
            while (std::isdigit(rule[at])) {
                n = n * 10 + (rule[at] - '0');
                ++at;
            }
            std::string nextrule = rules[n];
            if (match(str,nextrule,rules,rule.substr(at,next -  at) + buf,pos))
                return true;
        }

        at = next;
    }

    return false;
}

int main()
{
    std::fstream file("d19.txt");
    std::string line;
    std::unordered_map<int, std::string> rules1, rules2;
    size_t n1 = 0, n2 = 0;
    while (std::getline(file, line)) {
        if (!line[0])
            break;

        const char* str = line.c_str();
        int n = 0;
        while (std::isdigit(*str)) {
            n = n * 10 + (*str++ - '0');
        }
        rules1[n] = str + 2;
    }

    rules2 = rules1;
    rules2[8] = "42 | 42 8";
    rules2[11] = "42 31 | 42 11 31";

    while (std::getline(file, line)) {
        if (match(line, rules1[0], rules1))
            ++n1;
        if (match(line, rules2[0], rules2))
            ++n2;
    }

    std::cout << "1: " << n1 << std::endl;
    std::cout << "2: " << n2 << std::endl;

    return 0;
}