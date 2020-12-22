#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

size_t hash(const std::list<int>& list)
{
    size_t hash = 5381;

    for (auto& c : list) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

std::list<int>& combat(std::list<int>& player1, std::list<int>& player2, bool recursive = false)
{
    std::unordered_map<size_t, bool> history1, history2;

    history1[hash(player1)] = true;
    history1[hash(player2)] = true;

    while (player1.size() && player2.size()) {
        int p1card = player1.front();
        int p2card = player2.front();

        player1.pop_front();
        player2.pop_front();

        int wincard, loosecard;
        if (recursive && p1card <= player1.size() && p2card <= player2.size()) {
            std::list<int> p1subdeck(p1card);
            std::copy_n(player1.begin(), p1card, p1subdeck.begin());
            std::list<int> p2subdeck(p2card);
            std::copy_n(player2.begin(), p2card, p2subdeck.begin());

            combat(p1subdeck, p2subdeck, true);
            wincard = p1subdeck.size() ? p1card : p2card;
            loosecard = p1subdeck.size() ? p2card : p1card;
        } else {
            wincard = std::max(p1card, p2card);
            loosecard = std::min(p1card, p2card);
        }

        std::list<int>& winner = wincard == p1card ? player1 : player2;
        winner.push_back(wincard);
        winner.push_back(loosecard);

        size_t hash1 = hash(player1);
        size_t hash2 = hash(player2);
        if (history1.count(hash1) || history2.count(hash2)) {
            return player1;
        }

        history1[hash1] = true;
        history2[hash2] = true;
    }

    return player1.size() ? player1 : player2;
}

size_t score(const std::list<int>& deck)
{
    size_t mult = deck.size();
    size_t ret = 0;
    for (auto value : deck) {
        ret += value * mult--;
    }
    return ret;
}

int main()
{
    std::list<int> player1, player2, deck1, deck2;

    std::fstream file("d22.txt");
    std::string line;

    std::getline(file, line);
    while (std::getline(file, line) && line[0])
        player1.push_back(std::stoi(line));
    std::getline(file, line);
    while (std::getline(file, line))
        player2.push_back(std::stoi(line));

    deck1 = player1;
    deck2 = player2;
    auto& winner = combat(deck1, deck2, false);
    std::cout << "1: " << score(winner) << std::endl;

    deck1 = player1;
    deck2 = player2;
    winner = combat(deck1, deck2, true);
    std::cout << "2: " << score(winner) << std::endl;

    return 0;
}