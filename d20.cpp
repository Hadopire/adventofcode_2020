#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> square;
typedef std::vector<square> squares;

struct piece {
    size_t id;
    size_t at;
    squares tiles;
};

square rot(const square& sqr)
{
    square ret = sqr;
    int side = sqr.size() - 1;
    for (int y = 0; y < sqr.size(); ++y) {
        for (int x = 0; x < sqr[y].size(); ++x) {
            int newx = -(y - side / 2.f) + side / 2.f;
            ret[y][x] = sqr[x][newx];
        }
    }
    return ret;
}

square flip(const square& sqr)
{
    square ret = sqr;
    int side = sqr.size() - 1;
    for (int y = 0; y < sqr.size(); ++y) {
        for (int x = 0; x < sqr[y].size(); ++x) {
            ret[y][x] = sqr[side - y][x];
        }
    }
    return ret;
}

bool find(std::vector<std::vector<piece>>& board, piece& p, int atx, int aty)
{
    for (int y = 0; y < board.size(); ++y) {
        if (y > aty)
            break;
        for (int x = 0; x < board.size(); ++x) {
            if (y == aty && x > atx)
                break;
            if (board[y][x].id == p.id)
                return true;
        }
    }
    return false;
}

bool solve(std::vector<std::vector<piece>>& board, std::vector<piece>& pieces, int x = 0, int y = 0)
{
    int pieceside = pieces[0].tiles[0].size();
    int boardside = board.size();

    int nextx = x + 1 == boardside ? 0 : x + 1;
    int nexty = nextx ? y : y + 1;

    for (auto& p : pieces) {
        if (find(board, p, x, y))
            continue;

        for (int j = 0; j < p.tiles.size(); ++j) {
            auto& sqr = p.tiles[j];
            bool fit = true;
            if (x) {
                auto& left = board[y][x - 1].tiles[board[y][x - 1].at];
                for (int i = 0; i < pieceside; ++i) {
                    if (sqr[i][0] != left[i][pieceside - 1]) {
                        fit = false;
                        break;
                    }
                }
            }
            if (y && fit) {
                auto& top = board[y - 1][x].tiles[board[y - 1][x].at];
                for (int i = 0; i < pieceside; ++i) {
                    if (sqr[0][i] != top[pieceside - 1][i]) {
                        fit = false;
                        break;
                    }
                }
            }
            if (!fit)
                continue;

            p.at = j;
            board[y][x] = p;
            if (nexty == boardside)
                return true;
            else if (solve(board, pieces, nextx, nexty))
                return true;
        }
    }

    return false;
}

size_t replacepattern(square& img, std::vector<std::string> pattern)
{
    size_t matches = 0;
    for (int iy = 0; iy < img.size() - pattern.size() + 1; ++iy) {
        for (int ix = 0; ix < img.size() - pattern[0].size() + 1; ++ix) {
            bool match = true;
            for (int py = 0; py < pattern.size() && match; ++py) {
                for (int px = 0; px < pattern[py].size() && match; ++px) {
                    if (pattern[py][px] == '#' && img[iy+py][ix+px] != '#') {
                        match = false;
                    }
                }
            }

            if (match) {
                ++matches;
                for (int py = 0; py < pattern.size() && match; ++py) {
                    for (int px = 0; px < pattern[py].size() && match; ++px) {
                        if (pattern[py][px] == '#') {
                            img[iy+py][ix+px] = 'O';
                        }
                    }
                }
            }
        }
    }

    return matches;
}

int main()
{
    std::fstream file("d20.txt");
    std::string line;
    std::vector<piece> pieces;

    while (std::getline(file, line)) {
        int n = std::stoi(std::string(line.substr(4)));
        piece p;
        p.id = n;
        p.tiles.push_back(square());
        while (std::getline(file, line)) {
            if (!line[0])
                break;

            p.tiles[0].push_back(std::move(line));
        }

        square sqr = p.tiles[0];
        p.tiles.push_back(flip(sqr));
        sqr = rot(sqr);
        for (int i = 0; i < 3; ++i) {
            p.tiles.push_back(sqr);
            p.tiles.push_back(flip(sqr));
            sqr = rot(sqr);
        }
        pieces.push_back(std::move(p));
    }

    int boardside = std::sqrt(pieces.size());
    std::vector<std::vector<piece>> board(boardside, std::vector<piece>(boardside));
    if (solve(board, pieces)) {
        std::cout << "1: " << board[0][0].id * board[0][boardside-1].id * board[boardside-1][0].id * board[boardside-1][boardside-1].id << std::endl;
        
        size_t tileside = pieces[0].tiles[0].size() - 2;
        size_t imside = tileside * boardside;
        square image(imside, std::string(imside, '.'));
        for (int y = 0; y < imside; ++y) {
            for (int x = 0; x < imside; ++x) {
                piece& p = board[(y)/tileside][(x)/tileside];
                image[y][x] = p.tiles[p.at][(y)%tileside + 1][x%tileside + 1];
            }
        }

        std::vector<std::string> monster {
            "                  # ",
            "#    ##    ##    ###",
            " #  #  #  #  #  #   "
        };

        for (int i = 0; i < 4; ++i) {
            size_t replaced = replacepattern(image, monster);
            if (replaced)
                break;
            square flipped = flip(image);
            replaced = replacepattern(flipped, monster);
            if (replaced) {
                image = std::move(flipped);
                break;
            }
            image = rot(image);
        }

        std::cout << std::endl << "HD IMAGE: " << std::endl;
        size_t roughness = 0;
        for (auto& row : image) {
            roughness += std::count(row.begin(), row.end(), '#');
            std::cout << row << std::endl;
        }
        std::cout << std::endl << "2: " << roughness << std::endl;


    }

    return 0;
}