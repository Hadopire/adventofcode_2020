#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct instruction {
    std::string op;
    int arg;
    bool visited = false;
};

bool exec(std::vector<instruction> program, int* acc)
{
    *acc = 0;
    int i = 0;
    while (i < program.size()) {
        instruction& in = program[i];
        if (in.visited)
            return false;
        in.visited = true;

        if (in.op == "jmp") {
            i += in.arg;
            if (program[i].visited)
                return false;
            else
                continue;
        } else if (in.op == "acc") {
            *acc += in.arg;
        }
        ++i;
    }
    return true;
}

int main()
{
    std::fstream file("d8.txt");
    std::vector<instruction> program;
    instruction line;
    while (file >> line.op && file >> line.arg) {
        program.push_back(std::move(line));
    }

    int acc = 0;
    exec(program, &acc);
    std::cout << "gaulois infini: " << acc << std::endl;

    for (int i = 0; i < program.size(); ++i) {
        instruction& in = program[i];
        std::string b;
        if (in.op == "jmp") {
            b = in.op;
            in.op = "nop";
        } else if (in.op == "nop") {
            b = in.op;
            in.op = "jmp";
        } else
            continue;

        if (exec(program, &acc)) {
            std::cout << "gaulois terminé: " << acc << std::endl;
            break;
        } else {
            in.op = b;
        }
    }
    return 0;
}