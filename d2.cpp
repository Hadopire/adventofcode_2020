#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>

int main()
{
    std::fstream file("d2.txt");
    std::string line;
    int n1 = 0, n2 = 0;
    while (std::getline(file, line))
    {
        int min,max,count = 0;
        char c;
        char str[500];
        sscanf(line.c_str(), "%d-%d %c: %s",&min,&max,&c,str);
        int i = 0;
        for (char *s = str; s[i]; s[i] == c ? ++i : *s++);
        n1 += (i>=min)&&(i<=max);
        n2 += (str[min-1]==c) ^ (str[max-1]==c);
    }
    std::cout << "1: " << n1 << "\n";
    std::cout << "2: " << n2 << "\n";
    return 0;
}