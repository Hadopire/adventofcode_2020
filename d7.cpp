#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <list>


struct node 
{
    std::string name;
    bool visited = false;
};

struct edge
{
    int weight;
    node *dst;
};

typedef std::unordered_map<std::string, std::list<edge>> graph;

size_t numParents(graph &bags, node& v, size_t count = 0)
{
    if (v.visited)
        return 0;
    
    for (auto &e : bags[v.name]) {
        if (e.weight < 0) {
            count += numParents(bags, *e.dst, 1);
        }
    }
    v.visited = true;
    return count;
}

size_t numChildren(graph &bags, node& v, size_t count = 0)
{
    for (auto &e : bags[v.name]) {
        if (e.weight > 0) {
            count += numChildren(bags, *e.dst, 1) * e.weight;
        }
    }
    return count;
}

int main()
{
    std::fstream file("d7.txt");
    std::string line;
    graph bags;
    std::unordered_map<std::string, node> vertices;

    while (std::getline(file, line)) {
        size_t start = line.find(" bag", 0);
        size_t end = start + 14;
        std::string parent = line.substr(0, start);
        node *vparent = &vertices[parent];
        vparent->name = parent;
        start = end;

        edge e;
        while (sscanf(line.substr(start).c_str(), "%d", &e.weight)) {
            start = line.find(" ", start) + 1;
            end = line.find(" bag", start);
            std::string child = line.substr(start, end-start);
            node *vchild = &vertices[child];
            
            e.dst = vchild;
            bags[parent].push_back(e);
            e.weight = -e.weight;
            e.dst = vparent;
            bags[child].push_back(e);

            start = line.find(",", start) + 2;
        }
    }

    std::cout << numParents(bags, vertices["shiny gold"]) << std::endl;
    std::cout << numChildren(bags, vertices["shiny gold"]) << std::endl;

    return 0;
}