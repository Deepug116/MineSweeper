#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

class Tile {

public:
    bool is_revealed = false;
    bool is_mine = false;
    bool is_flag = false;
    bool is_empty = false;
    bool end_game = false;
    bool debugMine = false;
    int row = 0;
    int col = 0;
    vector<Tile*> adjacentTiles;
    int neighbor = 0;
    int totalMines = 0;

    

};
