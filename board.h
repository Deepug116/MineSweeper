#include "Tile.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Board {

public:
    int windowHeight;
    int windowWidth;
    int Height;
    int Width;
    int mineCount;
    int tileCount;
    vector<vector<Tile>> tileVector;
    bool lost_game = false;
    bool win_game = false;
    int flagcount = 0;
    

    Board() {};
    void ResetBoard();
    void PrintBoard() {
    }
    
   void InitializeBoard() {
   }

};
