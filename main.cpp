#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "board.h"
#include "TextureManager.h"
#include "random.h"

using namespace std;
using namespace sf;

void CountMines(Board& board) {
    for (auto i = 0; i < board.Height; i++) {
        for (auto j = 0; j < board.Width; j++) {
            auto tile = board.tileVector[i][j];
            auto size = tile.adjacentTiles.size();
            for (int k = 0; k < size; k++) {
                if (board.tileVector[i][j].adjacentTiles[k]->is_mine == true)
                    board.tileVector[i][j].totalMines += 1;
            }
        }
    }
}

void Recursion(Board& board, int i, int j) {
    board.tileVector[i][j].is_revealed = true;
    if (board.tileVector[i][j].totalMines == 0)
        for (Tile* tile : board.tileVector[i][j].adjacentTiles) {
            if (!tile->is_revealed && !tile->is_flag)
                Recursion(board, tile->row, tile->col);
        }
}



void TopRight(Tile* tile, Board & board) {
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col-1]);  // left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row+1][tile->col]); // bottom
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col - 1]); // bottom left
}

void TopLeft(Tile* tile, Board& board) {
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col]); // bottom
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col + 1]); // bottom right
}

void BottomLeft(Tile* tile, Board& board) {
    tile->adjacentTiles.push_back(&board.tileVector[tile->row-1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col + 1]); // up right
}

void BottomRight(Tile* tile, Board& board) {
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col - 1]); // left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col - 1]); // up left
}

void LeftAdjacent5(Tile* tile, Board& board) { // 5
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col + 1]); // Up Right 
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // Right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col + 1]); // Bottom Right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col]); // Down
}

void Bottom5(Tile* tile, Board& board) { // 5
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col - 1]); // left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col + 1]); // Up right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col - 1]); // Up left
}

void Up5(Tile* tile, Board& board) { // 5
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col - 1]); // left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col]); // down
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col - 1]); // down left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col + 1]); // down right
}

void RightAdjacent5(Tile* tile, Board& board) { // 5
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col]); // Down
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col - 1]); // Left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col - 1]); // Up left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col - 1]); // Down left
}

void Middle8(Tile* tile, Board& board) { // 8
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col]); // Up
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col]); // Down
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col + 1]); // Right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row][tile->col - 1]); // Left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col - 1]); // Up left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row - 1][tile->col + 1]); // Up Right
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col - 1]); // Bottom Left
    tile->adjacentTiles.push_back(&board.tileVector[tile->row + 1][tile->col + 1]); // Bottom Right
}



void ReadAdjacent(Board& board) {
    for (auto i = 0; i < board.Height; i++) {
        for (auto j = 0; j < board.Width; j++) {
           //Tile* currentTile = &board.tileVector[i][j];
           //    currentTile->adjacentTiles.clear();
            if (i == 0)
                if (j == 0)
                    TopLeft(&board.tileVector[i][j], board);
                else if (j == board.Width - 1)
                    TopRight(&board.tileVector[i][j], board);
                else
                    Up5(&board.tileVector[i][j], board);
            else if (i == board.Height - 1) {
                if (j == 0)
                    BottomLeft(&board.tileVector[i][j], board);
                else if (j == board.Width - 1)
                    BottomRight(&board.tileVector[i][j], board);
                else
                    Bottom5(&board.tileVector[i][j], board);
            }
            else {
                if (j == 0)
                    LeftAdjacent5(&board.tileVector[i][j], board);
                else if (j == board.Width - 1)
                    RightAdjacent5(&board.tileVector[i][j], board);
                else
                    Middle8(&board.tileVector[i][j], board);
            }



        }
    }  
}



void PlaceMines(Board& board) {
    for (int i = 0; i < board.mineCount; ++i) {
        int row = Random::Int(0, board.Height - 1);
        int col = Random::Int(0, board.Width - 1);
        if (!board.tileVector[row][col].is_mine) {
            board.tileVector[row][col].is_mine = true;
        }
        else {
            --i;
        }
    }
}

void TestBoard(string boardTest, Board& board) {
    string path = "boards/testboard";
    path += boardTest + ".brd";
    ifstream inFile(path);
    string lineFromFile;
    

    int current = 0;
    // Clears the board
    for (auto i = 0; i < board.Height; i++) {
        for (auto j = 0; j < board.Width; j++) {
            board.tileVector[i][j].adjacentTiles.clear();
            board.tileVector[i][j].is_revealed = false;
            board.tileVector[i][j].is_mine = false;
            board.tileVector[i][j].debugMine = false;
            board.tileVector[i][j].is_flag = false;
            board.tileVector[i][j].end_game = false;
            board.tileVector[i][j].totalMines = 0;

            
        }
    }
    while (getline(inFile, lineFromFile)) {
        for (auto i = 0; i < lineFromFile.size(); i++) {
            if (lineFromFile[i] == '1') {
                board.tileVector[current][i].is_mine = true;
            }
        }
         current += 1;
    }
}  


int main()
{
    int numCols = 0;
    int numRows = 0;
    int numMines = 0;

    ifstream file("boards/config.cfg");
    if (file.is_open()) {
        string data;
        getline(file, data);
        numCols = stoi(data);

        getline(file, data);
        numRows = stoi(data);

        getline(file, data);
        numMines = stoi(data);
    }
    else {
        cout << "can't open file" << endl;
    }
    Board board;
    board.Height = numRows;
    board.Width = numCols;
    board.mineCount = numMines;
    board.windowHeight = numRows * 32 + 100;
    board.windowWidth = numCols * 32;

    for (auto i = 0; i < numRows; i++) {
        vector<Tile> row;
        for (auto j = 0; j < numCols; j++) {
            Tile tile;
            tile.row = i;
            tile.col = j;
            row.push_back(tile);
        }
        board.tileVector.push_back(row);
    }
    
    PlaceMines(board);
    ReadAdjacent(board);
    CountMines(board);

    // load the sprites and textures
    sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
    sf::Sprite FrownFace(TextureManager::GetTexture("face_lose"));
    sf::Sprite WinFace(TextureManager::GetTexture("face_win"));
    sf::Sprite Mine(TextureManager::GetTexture("mine"));
    sf::Sprite HiddenTiles(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite Debug(TextureManager::GetTexture("debug"));
    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    //sf::Sprite digits(TextureManager::GetTexture("digits"));
    sf::Sprite num1(TextureManager::GetTexture("number_1"));
    sf::Sprite num2(TextureManager::GetTexture("number_2"));
    sf::Sprite num3(TextureManager::GetTexture("number_3"));
    sf::Sprite num4(TextureManager::GetTexture("number_4"));
    sf::Sprite num5(TextureManager::GetTexture("number_5"));
    sf::Sprite num6(TextureManager::GetTexture("number_6"));
    sf::Sprite num7(TextureManager::GetTexture("number_7"));
    sf::Sprite num8(TextureManager::GetTexture("number_8"));

    sf::Texture& digitsImage = TextureManager::GetTexture("digits");
    sf::Sprite digits[11];
    for (int i = 0; i < 11; ++i) {
        digits[i].setTexture(digitsImage);
        digits[i].setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
    }




    sf::RenderWindow window(sf::VideoMode(board.windowWidth, board.windowHeight), "MineSweeper", sf::Style::Close);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::MouseButtonPressed) {
                int row = Mouse::getPosition(window).y / 32;
                int col = Mouse::getPosition(window).x / 32;
                auto mousePosition = sf::Mouse::getPosition(window);

                if (event.mouseButton.button == Mouse::Left) {


                    if (test1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        
                        for (auto i = 0; i < board.Height; i++)
                            for (auto j = 0; j < board.Width; j++)
                                board.tileVector[i][j].totalMines = 0;
                        TestBoard("1", board);
                        ReadAdjacent(board);
                        
                        CountMines(board);
                        cout << board.mineCount << endl;
                        board.lost_game = false;
                        board.win_game = false;
                        
                         }
                    if (test2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        for (auto i = 0; i < board.Height; i++)
                            for (auto j = 0; j < board.Width; j++)
                                board.tileVector[i][j].totalMines = 0;
                        TestBoard("2", board);
                        ReadAdjacent(board);
                        for (auto i = 0; i < board.Height; i++)
                            for (auto j = 0; j < board.Width; j++)
                                board.tileVector[i][j].totalMines = 0;
                        CountMines(board);
                        cout << board.mineCount << endl;
                        board.lost_game = false;
                        board.win_game = false;
                        
                    }
                    if (test3.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        for (auto i = 0; i < board.Height; i++)
                            for (auto j = 0; j < board.Width; j++)
                                board.tileVector[i][j].totalMines = 0;
                        TestBoard("3", board);
                        ReadAdjacent(board);
                        CountMines(board);
                        cout << board.mineCount << endl;
                        board.lost_game = false;
                        board.win_game = false;
                       
                    }
                    if (Debug.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        // Toggle debug mode (set all tiles as mines)
                        if (!board.lost_game && !board.win_game) {
                            for (auto i = 0; i < board.Height; i++) {
                                for (auto j = 0; j < board.Width; j++) {
                                    //board.tileVector[i][j].is_mine = !board.tileVector[i][j].is_mine;
                                    board.tileVector[i][j].debugMine = !board.tileVector[i][j].debugMine;
                                }
                            }
                        }
                    }
                    if (happyFace.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        board.lost_game = false;
                        board.win_game = false;
                        board.flagcount = 0;
                        cout << board.mineCount << endl;
                        // Reset the game board
                        for (auto i = 0; i < numRows; i++) {
                            for (auto j = 0; j < numCols; j++) {
                                board.tileVector[i][j].adjacentTiles.clear(); // Clears the board every time smiley face is pressed
                                board.tileVector[i][j].is_revealed = false;
                                board.tileVector[i][j].is_flag = false;
                                board.tileVector[i][j].is_mine = false;
                                board.tileVector[i][j].neighbor = 0;
                                board.tileVector[i][j].totalMines = 0;
                            }
                        }
                        PlaceMines(board);
                        ReadAdjacent(board);
                        CountMines(board);

                        
                        for (auto i = 0; i < numRows; i++) {
                            for (auto j = 0; j < numCols; j++) {
                                board.tileVector[i][j].neighbor = board.tileVector[i][j].adjacentTiles.size() ;
                                //cout << board.tileVector[i][j].adjacentTiles.size() << endl;
                            }
                        }
                    }

                    if (row < numRows && col < numCols) {
                        if (!board.tileVector[row][col].is_flag && !board.tileVector[row][col].is_mine) {
                            // Left click on a tile
                            if (!board.lost_game && !board.win_game) {
                                board.tileVector[row][col].is_revealed = true;
                                Recursion(board, row, col);

                                int countRevealed = 0;
                                for (auto i = 0; i < board.Height; i++) {
                                    for (auto j = 0; j < board.Width; j++) {
                                        if (board.tileVector[i][j].is_revealed) {
                                            //board.win_game = true;
                                            countRevealed++;
                                        }
                                        
                                    }
                                            
                                    
                                }
                                cout << countRevealed << endl;
                                if (countRevealed == board.Height * board.Width - board.mineCount) {
                                    board.win_game = true;
                                    cout << "GAME WON" << endl;
                                }
                            }
                            

                        }
                        else if (board.tileVector[row][col].is_mine) {
                            if (!board.lost_game&&!board.win_game)
                                board.lost_game = true;
                            for (auto i = 0; i < row; i++) {
                                for (auto j = 0; j < col; j++) {
                                    //board.tileVector[i][j].debugMine = true;
                                    if (board.tileVector[i][j].is_mine)
                                        if (!board.tileVector[i][j].is_revealed)
                                            board.tileVector[i][j].is_revealed = true;
                                }
                            }

                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    if (row < numRows && col < numCols) {
                        if (!board.tileVector[row][col].is_revealed) {
                            if (!board.lost_game && !board.win_game) {
                                board.tileVector[row][col].is_flag = !board.tileVector[row][col].is_flag;
                                if (board.tileVector[row][col].is_flag) {
                                    board.flagcount++;


                                }
                                else {
                                    board.flagcount--;
                                }
                            }
                        }
                    }
                }
                
               
                
            }
        }

             window.clear(sf::Color::White);

        for (auto& row : board.tileVector) {
            for (auto& tile : row) {
                // Set sprite background position
                sf::Sprite tileSprite;
                tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
                tileSprite.setPosition(tile.col * 32.0f, tile.row * 32.0f);

                if (tile.is_revealed) {
                    tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                }
                window.draw(tileSprite);

                if (tile.is_revealed) {
                    if (tile.totalMines == 1) {
                        num1.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num1);
                    }
                    if (tile.totalMines == 2) {
                        num2.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num2);
                    }
                    if (tile.totalMines == 3) {
                        num3.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num3);
                    }
                    if (tile.totalMines == 4) {
                        num4.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num4);
                    }
                    if (tile.totalMines == 5) {
                        num5.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num5);
                    }
                    if (tile.totalMines == 6) {
                        num6.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num6);
                    }
                    if (tile.totalMines == 7) {
                        num7.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num7);
                    }
                    if (tile.totalMines == 8) {
                        num8.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(num8);
                    }
                     
                   
                }
                

                if (tile.is_flag) {
                    sf::Sprite Flag(TextureManager::GetTexture("flag"));
                    Flag.setPosition(tile.col * 32.0f, tile.row * 32.f);
                    window.draw(Flag);
                }
                if (tile.debugMine || board.lost_game) {
                    if (tile.is_mine) {
                        sf::Sprite Mine(TextureManager::GetTexture("mine"));
                        Mine.setPosition(tile.col * 32.0f, tile.row * 32.f);
                        window.draw(Mine);
                    }
                }
            }
        }

        happyFace.setPosition((board.windowWidth / 2) - 32, (numRows * 32));
        FrownFace.setPosition((board.windowWidth / 2) - 32, (numRows * 32));
        WinFace.setPosition((board.windowWidth / 2) - 32, (numRows * 32));
        Debug.setPosition((board.windowWidth / 2) + 96, (numRows * 32));
        test1.setPosition((board.windowWidth / 2) + 160, (numRows * 32));
        test2.setPosition((board.windowWidth / 2) + 224, (numRows * 32));
        test3.setPosition((board.windowWidth / 2) + 288, (numRows * 32));
        //digits.setPosition(32, (numRows * 32));
        
        int scoreRows = board.Height;
        int scoreCols = board.Width * 32 + 1;
        //int score = board.remainingMines - board.flagCount;
        int score = board.mineCount - board.flagcount;
        bool neg = score < 0;
        score = abs(score);

        int ones = score % 10;
        score /= 10;

        int tens = score % 10;
        score /= 10;

        int hundreds = score % 10;
       

   
        digits[abs(ones)].setPosition((board.windowWidth / 2) - 32 - 64, (numRows * 32));
        window.draw(digits[ones]);
        digits[abs(tens)].setPosition((board.windowWidth / 2) - 32 - 85, (numRows * 32));
        window.draw(digits[tens]);
        digits[abs(hundreds)].setPosition((board.windowWidth / 2) - 32 - 106, (numRows * 32));
        window.draw(digits[hundreds]);
        digits[10].setPosition((board.windowWidth / 2) - 32 - 127, (numRows * 32));
        if(neg)
            window.draw(digits[10]);

      
      

        //string number = to_string(abs(score));
        //if (score < 0) {
        //    int negativePosition = 10;
        //    digits[negativePosition].setPosition(scoreRows, scoreCols);
        //    window.draw(digits[negativePosition]);
        //    scoreRows += 21;
        //}
        //for (char digitInput : number) {
        //    int digit = digitInput - '0';
        //    digits[digit].setPosition((board.windowWidth / 2) - 32 -64, (numRows * 32));
        //    window.draw(digits[digit]);
        //    scoreRows += 21;
        //}

        window.draw(happyFace);
        //window.draw(digits);
        window.draw(Debug);//Debug button
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        
        if (board.lost_game == true) {
            window.draw(FrownFace);
        }
        else if (board.win_game == true) {
            window.draw(WinFace);
        }

        
        window.display();
    }
    TextureManager::Clear();
    return 0;
}
