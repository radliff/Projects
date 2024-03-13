//
// Created by Radliff Jeantinor on 11/19/23.
//

#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#endif //MINESWEEPER_MINESWEEPER_H
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
unordered_map<string, sf::Texture> textures;
sf::Texture& getTexture(const string& textureName) {
    auto result = textures.find(textureName);
    if (result == textures.end()){
        sf::Texture newTexture;
        newTexture.loadFromFile("images/" + textureName + ".png");
        if(!newTexture.loadFromFile("images/" + textureName + ".png")){
            cout << "Texture loading error." << endl;
        }
        textures[textureName] = newTexture;
        return textures[textureName];
    }
    else {
        return result->second;
    }
}
float setCellHeight(const string& filename){
    int width;
    int height;
    int rowCount;
    int colCount;
    float cellHeight;
    string line;
    ifstream file(filename);
    if (!file.is_open()){
        cout << "The board config file is not open.";
        return 0;
    }
    else {
        getline(file, line);
        colCount = stoi(line);

        getline(file, line);
        rowCount = stoi(line);
    }
    width = colCount * 32;
    height = (rowCount * 32) + 100;
    file.close();
    cellHeight = (float)(width / colCount);
    return cellHeight;
}
const float CELLHEIGHT = setCellHeight("files/config.cfg");
class Cell{
    int _numTiles;
public:
    int neighborCount = 0;
    bool _hasFlag = false;
    bool _revealed = false;
    bool hasMine = false;
    Cell(){};
    void DrawCell(float x, float y, sf::RenderWindow &window);
    sf::RectangleShape cellRect;
    sf::RectangleShape numRect;
    sf::Texture hiddenCell = getTexture("tile_hidden");
    sf::Texture mine = getTexture("mine");
    sf::Texture flag = getTexture("flag");
    sf::Texture revealedCell = getTexture("tile_revealed");
};

class Board{
    int _rows = 0;
    int _cols = 0;
    int num_mines = 0;
public:
    vector<vector<Cell>> grid;
    bool pause = false;
    bool debug = false;
    bool win = false;
    bool loss = false;
    Board(){};
    void setDimen(const string& fileName);
    void generateBoard();
    void floodFill(sf::Vector2f &coordinates, sf::RenderWindow &window, vector<sf::Texture> &texturesV);
    void setMines();
    void setNumMines(int mines);
    int checkMines(int targetX, int targetY) const;
    void initMines();
    void drawCellNumber(sf::Vector2f &coordinates, vector<sf::Texture> &texturesV, sf::RenderWindow &window);
    int countFlags();
    int getMines();
    void setFlag(sf::Vector2i &coordinates);
    void drawBoard(sf::RenderWindow &window);
    void revealAllMines();
    void pauseBoard();
    void resetBoard();
    bool checkWin();
    void winState();
    bool checkLoss();
    void lossState();
};

struct LeaderboardEntry{
    sf::Time time;
    string name;
};