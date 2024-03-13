#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>
#include <cmath>
#include "minesweeper.h"

using namespace std;
// Welcome Window: 15 PTS
// board changes size & numMines based on values in board_config.cfg: 10 PTS

mt19937 random_mt(random_device{}());

int Random(int min, int max)
{
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);
}

vector<sf::Texture> numberTextures(){
    vector<sf::Texture> v;

    v.push_back(getTexture("tile_revealed"));
    for (int i = 1; i <= 8; ++i) {
        std::string textureName = "number_" + to_string(i);
        sf::Texture texture = getTexture(textureName);
        v.push_back(texture);
    }
    return v;
}


void setDimen(int &width, int &height, const string &filename){
    int rowCount;
    int colCount;
    string line;
    ifstream file(filename);
    if (!file.is_open()){
        cout << "The board config file is not open.";
        return;
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
}


// Texture manager functions


// Making Sprites
sf::Sprite makeSprite(sf::Texture &texture, sf::Vector2f position){
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(position);
    return sprite;
}

// Cell functions
void Cell::DrawCell(float x, float y, sf::RenderWindow &window) {
    this->cellRect.setSize(sf::Vector2f(CELLHEIGHT, CELLHEIGHT));
    this->cellRect.setPosition(x, y);
    if (this->_revealed) {
        this->cellRect.setTexture(&revealedCell);
        window.draw(this->cellRect);
        if (this->numRect.getTexture() != nullptr){
            this->numRect.setPosition(x, y);
            window.draw(numRect);
        } else if(this->hasMine) {
            this->cellRect.setTexture(&hiddenCell);
            sf::RectangleShape mineRect(sf::Vector2f(CELLHEIGHT, CELLHEIGHT));
            mineRect.setTexture(&mine);
            mineRect.setPosition(x, y);
            window.draw(this->cellRect);
            window.draw(mineRect);
        }
        else {
            this->numRect.setPosition(x, y);
            this->numRect.setTexture(&revealedCell);
            window.draw(this->numRect);
        }
    } else {
        this->cellRect.setTexture(&hiddenCell);
        window.draw(this->cellRect);

        if (this->_hasFlag) {
            sf::RectangleShape flagRect(sf::Vector2f(CELLHEIGHT, CELLHEIGHT));
            flagRect.setTexture(&flag);
            flagRect.setPosition(x, y);
            window.draw(flagRect);
        }
    }
}


// Board functions
void Board::setDimen(const string& fileName){
    ifstream file(fileName);
    string line;
    int rowCount;
    int colCount;
    if (!file.is_open()){
        cout << "The board config file is not open.";
        return;
    }
    else {
        getline(file, line);
        colCount = stoi(line);

        getline(file, line);
        rowCount = stoi(line);

        getline(file, line);
        this->num_mines = stoi(line);
    }
   _rows = rowCount;
    _cols = colCount;
}

int Board::getMines() {
    return num_mines;
}

void Board::generateBoard(){
    grid = vector<vector<Cell>>(_rows, vector<Cell>(_cols));
}

void Board::drawBoard(sf::RenderWindow &window) {

    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            float x = j * CELLHEIGHT;
            float y = i * CELLHEIGHT;
            grid[i][j].DrawCell(x, y, window);
        }
    }
}

int Board::countFlags() {
    int count = 0;
    for(int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++){
            if(grid[i][j]._hasFlag){
                count += 1;
            }
        }
    }
    return count;
}

void Board::setMines() {
    int countMines = 0;
    while (countMines < num_mines) {
        int randomX, randomY;
        randomX = Random(0, _rows - 1);
        randomY = Random(0, _cols - 1);
        if (!grid[randomX][randomY].hasMine) {
            grid[randomX][randomY].hasMine = true;
            countMines += 1;
        }
    }
}

void Board::setFlag(sf::Vector2i &coordinates) {
    coordinates.x /= CELLHEIGHT;
    coordinates.y /= CELLHEIGHT;
    if (coordinates.x >= 0 && coordinates.x < _cols && coordinates.y >= 0 && coordinates.y < _rows){
        if (grid[coordinates.y][coordinates.x]._hasFlag || grid[coordinates.y][coordinates.x].hasMine && grid[coordinates.y][coordinates.x]._revealed){
            grid[coordinates.y][coordinates.x]._hasFlag = false;
        } else {
            grid[coordinates.y][coordinates.x]._hasFlag = true;
        }
    }

}

int Board::checkMines(int targetX, int targetY) const{
    int numMines = 0;

    if (grid[targetY][targetX].hasMine) {
        return -1;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int neighborY = targetY + i;
            int neighborX = targetX + j;

            // Boundary checking for neighboring cells
            if (neighborX >= 0 && neighborX < _cols && neighborY >= 0 && neighborY < _rows) {
                if (grid[neighborY][neighborX].hasMine) {
                    numMines += 1;
                }
            }
        }
    }
    return numMines;
}

void Board::initMines() {
    for (int y = 0; y < _rows; y++){
        for (int x = 0; x < _cols; x++){
            grid[y][x].neighborCount = checkMines(x, y);
        }
    }
}


void Board::drawCellNumber(sf::Vector2f &coordinates, vector<sf::Texture> &texturesV, sf::RenderWindow &window) {
    float targetX = coordinates.x / CELLHEIGHT;
    float targetY = coordinates.y / CELLHEIGHT;

    if (targetX < 0 || targetX >= _cols || targetY < 0 || targetY >= _rows) {
        return;
    }

    if (checkLoss()){
        return;
    }

    // Set up the rendering for the current cell
    grid[targetY][targetX].numRect.setSize(sf::Vector2f(CELLHEIGHT, CELLHEIGHT));

    if (grid[targetY][targetX].neighborCount < 0) {
        grid[targetY][targetX]._revealed = true;
        return;  // No texture set for mines
    }

    if (grid[targetY][targetX].neighborCount == 0) {
        // No texture set if no neighboring mines, reveal adjacent non-mines
        floodFill(coordinates, window, texturesV);
        return;
    }

    // Set the texture for cells with neighboring mines
    grid[targetY][targetX]._revealed = true;
    grid[targetY][targetX].numRect.setTexture(&texturesV[grid[targetY][targetX].neighborCount]);
    grid[targetY][targetX].numRect.setPosition(targetX * CELLHEIGHT, targetY * CELLHEIGHT);

}

void Board::setNumMines(int mines) {
    this->num_mines = mines;
}

void Board::floodFill(sf::Vector2f &coordinates, sf::RenderWindow &window, vector<sf::Texture> &texturesV) {
    float targetX = coordinates.x / CELLHEIGHT;
    float targetY = coordinates.y / CELLHEIGHT;

    // Boundary checking
    if (targetX < 0 || targetX >= _cols || targetY < 0 || targetY >= _rows) {
        return;  // Out of bounds
    }

    // If the current cell is a mine or already revealed, stop recursion
    if (grid[targetY][targetX].hasMine || grid[targetY][targetX]._revealed || grid[targetY][targetX]._hasFlag) {
        return;
    }
    sf::Vector2f fC(targetX * CELLHEIGHT, targetY * CELLHEIGHT);
// 
    // Mark the current cell as revealed
    grid[targetY][targetX]._revealed = true;

    // If the current cell has neighbors, reveal it and stop recursion
    if (grid[targetY][targetX].neighborCount > 0) {
        // Draw the cell number and update the window
        drawCellNumber(fC, texturesV, window);
        return;
    }

    // Call floodFill recursively
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            float neighborY = targetY + i;
            float neighborX = targetX + j;

            // Boundary checking for neighboring cells
            if (neighborX >= 0 && neighborX < _cols && neighborY >= 0 && neighborY < _rows) {
                if (!grid[neighborY][neighborX]._revealed) {
                    sf::Vector2f neighborCoords(neighborX * CELLHEIGHT, neighborY * CELLHEIGHT);
                    floodFill(neighborCoords, window, texturesV);
                }
            }
        }
    }
}

void Board::revealAllMines() {
    if (pause){
        return;
    }
    for (int i = 0; i < _rows; i++){
        for (int j = 0; j < _cols; j++){
            if(grid[i][j].hasMine && !grid[i][j]._revealed){
                grid[i][j]._revealed = true;
            } else {
                grid[i][j]._revealed = false;
            }

        }
    }
}

void Board::pauseBoard() {
    if(checkWin()){
        return;
    }
    for (int i = 0; i < _rows; i++){
        for (int j = 0; j < _cols; j++){
            if (!grid[i][j]._revealed){
                grid[i][j]._hasFlag = false;
                grid[i][j]._revealed = true;
            } else {
                grid[i][j].numRect.setTexture(&getTexture("tile_revealed"));
            }
        }
    }
}

void Board::resetBoard() {
    sf::RectangleShape cellRect;
    sf::RectangleShape numRect;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            grid[i][j].neighborCount = 0;
            grid[i][j].hasMine = false;
            grid[i][j]._hasFlag = false;
            grid[i][j]._revealed = false;
            grid[i][j].numRect = numRect;
            grid[i][j].cellRect = cellRect;
        }
    }
    setMines();
    initMines();
}

bool Board::checkWin() {
    sf::Texture &revealedCell = getTexture("tile_revealed");
    if(pause && !win){
        return false;
    }
    for (int i = 0; i < _rows; i++){
        for (int j = 0; j < _cols; j++){
            if (!grid[i][j]._revealed && !grid[i][j].hasMine){
                return false;
            }
        }
    }
    return true;
}

void Board::winState() {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (grid[i][j].hasMine && !grid[i][j]._hasFlag) {
                grid[i][j]._hasFlag = true;
            }
        }
    }
    win = true;
}

bool Board::checkLoss() {
    if (debug){
        return false;
    }
    if (pause && !loss){
        return false;
    }
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (grid[i][j].hasMine && grid[i][j]._revealed) {
                loss = true;
                return true;
            }
        }
    }
    return false;
}

void Board::lossState() {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (grid[i][j].hasMine && !grid[i][j]._revealed) {
                grid[i][j]._revealed = true;
            }
        }
    }
}


sf::Text makeText(const sf::Font& font, const string& input, int charSize, sf::Color color){
    sf::Text text;
    text.setString(input);
    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setFillColor(color);
    return text;
}

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x,y));
}

sf::Time convertTime(const string& timeString){
   int minutes;
   int seconds;
   char colon;

   istringstream iss(timeString);
   if (iss >> minutes >> colon >> seconds){
       return sf::seconds(static_cast<float>(minutes * 60 + seconds));
   } else {
       cout << "Invalid time format: " << timeString << endl;
       return sf::Time::Zero;
   }
}

bool entryEquality(const LeaderboardEntry &e1, const LeaderboardEntry &e2){
    return static_cast<int>(e1.time.asSeconds()) == static_cast<int>(e2.time.asSeconds()) && e1.name == e2.name;
}

void readLeaderboard(vector<LeaderboardEntry> &entries){
    ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.is_open()){
        cout << "File did not open!\n";
    }
    string line;

    while(getline(leaderboardFile, line)){
        istringstream iss(line);;
        string timeString;
        string name;
        LeaderboardEntry entry;
        if(getline(iss, timeString, ',') && getline(iss, name)){
            sf::Time time = convertTime(timeString);
            entry.time = time;
            entry.name = name;
            entries.push_back(entry);
        }
    }
    leaderboardFile.close();
}
//
void drawLeaderboard(sf::Text &boardText, vector<LeaderboardEntry> &entries, sf::RenderWindow &window){
    string displayText;
    string formattedTime;
    int ranking = 1;
    for (int i = 0; i < 5; i++){
        int minutes = entries[i].time.asSeconds() / 60;
        int seconds = static_cast<int>(entries[i].time.asSeconds()) % 60;
        if (minutes >= 10 && seconds < 10){
            formattedTime = to_string(minutes) + ":" + "0" + to_string(seconds);
        } else if (minutes >= 10 && seconds >= 10){
            formattedTime = to_string(minutes) + ":" + to_string(seconds);
        } else if (minutes < 10 && seconds >= 10){
            formattedTime = "0" + to_string(minutes) + ":" + to_string(seconds);
        } else if (minutes < 10 && seconds < 10){
            formattedTime = "0" + to_string(minutes) + ":" + "0" + to_string(seconds);
        }
        displayText += to_string(ranking) + ".\t" +
        formattedTime + "\t" + entries[i].name + "\n\n";
        if (ranking == 5){
            break;
        }
        ++ ranking;
    }
    boardText.setString(displayText);
    window.draw(boardText);
}
//
bool sortTimes(LeaderboardEntry const &entry1, LeaderboardEntry const &entry2){
    return entry1.time.asSeconds() < entry2.time.asSeconds();
}

void updateLeaderboard(vector<LeaderboardEntry> &entries){
    sort(entries.begin(), entries.end(), sortTimes);

    // removing duplicates by checking for exact time & name
    auto uniqueEnd = unique(entries.begin(), entries.end(), entryEquality);

    entries.erase(uniqueEnd, entries.end());

    for (int i = 0; i < entries.size(); i++){
        cout << i + 1 << "." << entries[i].name << "\t" << entries[i].time.asSeconds() << endl;
    }

    // updating the leaderboard file w/ first 5 entries
    ofstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.is_open()){
        cout << "Leaderboard file didnt open :(" << endl;
    } else {
        for (int i = 0; i < 5; i++){
            int minutes = entries[i].time.asSeconds() / 60;
            int seconds = static_cast<int>(entries[i].time.asSeconds()) % 60;

            leaderboardFile << (minutes < 10 ? "0" : "") << minutes << ":"
                            << (seconds < 10 ? "0" : "") << seconds << ","
                            << entries[i].name << endl;
        }
    }
}

void removeAsterisk(vector<LeaderboardEntry> &entries){
    for (auto &entry : entries){
        entry.name.erase(remove(entry.name.begin(), entry.name.end(), '*'), entry.name.end());
    }
}



// creating a savestate for the board to return to when we resume
vector<vector<Cell>> deepCopy(const std::vector<std::vector<Cell>>& original) {
    return vector<vector<Cell>>(original.begin(), original.end());
}



int main() {
    int width;
    int height;

    int letterCount = 0;

    setDimen(width, height, "files/config.cfg");

    float rowCount = (height - 100) / 32;
    float colCount = width / 32;

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    if (!font.loadFromFile("files/font.ttf")) {
        cout << "font didnt load :(" << endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);

    Board board;
    board.setDimen("files/config.cfg");
    board.generateBoard();

    sf::Text welcomeText = makeText(font, "WELCOME TO MINESWEEPER", 24, sf::Color::White);

    setText(welcomeText, width / 2, (height / 2) - 150);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text nameText = makeText(font, "Enter your name:", 20, sf::Color::White);
    setText(nameText, width / 2, (height / 2) - 75);
    nameText.setStyle(sf::Text::Bold);

    sf::Text userName = makeText(font, "", 18, sf::Color::Yellow);
    setText(userName, width / 2, (height / 2) - 45);
    userName.setStyle(sf::Text::Bold);

    sf::RectangleShape cursor;
    cursor.setSize(sf::Vector2f(2.5f, (float) (userName.getCharacterSize())));
    cursor.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::TextEntered) {
                if (isalpha(static_cast<char>(event.text.unicode))) {
                    if (letterCount == 10) {
                        break;
                    }
                    letterCount++;
                    if (userName.getString().isEmpty()) {
                        userName.setString(userName.getString() + static_cast<char>(toupper(event.text.unicode)));
                    } else {
                        userName.setString(userName.getString() + static_cast<char>(tolower(event.text.unicode)));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (letterCount >= 1) {
                        window.close();
                    } else {
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (letterCount >= 1) {
                        userName.setString(userName.getString().substring(0, userName.getString().getSize() - 1));
                        letterCount--;
                    } else {
                        break;
                    }
                }
            }
        }


        window.clear(sf::Color::Blue);

        window.draw(welcomeText);

        window.draw(nameText);

        setText(userName, width / 2, (height / 2) - 45);
        window.draw(userName);

        cursor.setPosition(userName.getPosition().x + userName.getGlobalBounds().width / 2,
                           userName.getPosition().y - 10);
        if (letterCount < 10) {
            window.draw(cursor);
        }
        window.display();
    }
    vector<string> usernames;
    usernames.push_back(userName.getString());

    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    // UI button locations
    sf::Texture happyFace = getTexture("face_happy");
    sf::Texture faceWin = getTexture("face_win");
    sf::Texture faceLose = getTexture("face_lose");

    sf::Sprite hFaceButton = makeSprite(happyFace, sf::Vector2f(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5)));

    sf::Texture debug = getTexture("debug");
    sf::Sprite debugButton = makeSprite(debug, sf::Vector2f((colCount * 32) - 304, 32 * (rowCount + 0.5)));

    sf::Texture pause = getTexture("pause");
    sf::Sprite pauseButton = makeSprite(pause, sf::Vector2f((colCount * 32) - 240, 32 * (rowCount + 0.5)));

    sf::Texture play = getTexture("play");

    sf::Texture leaderboard = getTexture("leaderboard");
    sf::Sprite leaderboardButton = makeSprite(leaderboard, sf::Vector2f((colCount * 32) - 176, 32 * (rowCount + 0.5)));

    sf::Texture digit = getTexture("digits");
    sf::Sprite digitSprite = makeSprite(digit, sf::Vector2f(33, 32 * (rowCount + 0.5) + 16));

    sf::Sprite negativeSprite = makeSprite(digit, sf::Vector2f(12, 32 * (rowCount + 0.5) + 16));

    sf::Sprite timerSprite = makeSprite(digit, sf::Vector2f((colCount * 32) - 97, 32 * (rowCount + 0.5) + 16));
    sf::Sprite secondSprite = makeSprite(digit, sf::Vector2f((colCount * 32) - 54,32 * (rowCount + 0.5) + 16));

    // Number Textures
    vector<sf::Texture> numberTexture = numberTextures();

    // Leaderboard text
    int leaderboardWidth = colCount * 16;
    int leaderboardHeight = (rowCount * 16) + 50;

    sf::Text leaderboardMain = makeText(font, "LEADERBOARD", 20, sf::Color::White);
    setText(leaderboardMain, leaderboardWidth / 2, (leaderboardHeight / 2) - 120);
    leaderboardMain.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text leaderboardInput = makeText(font, "", 18, sf::Color::White);
    setText(leaderboardInput, leaderboardWidth / 2, (leaderboardHeight / 2) + 20);
    leaderboardInput.setStyle(sf::Text::Bold);

    vector<LeaderboardEntry> entries;
    readLeaderboard(entries);

    bool leaderboardPaused = false;

    sf::Clock clock;
    bool clockRunning = true;
    sf::Time startTime = clock.getElapsedTime();
    auto pauseTime = sf::Time::Zero;
    float elapsedPauseTime = 0;

    // This is to stop the leaderboard from updating with slightly different times
    int winCount = 0;
    // Conditional flag to check what the gameWindow should look like with paused board & leaderboardWindow open
    bool lPopup = false;


    board.setMines();
    int numMines = board.getMines();
    board.initMines();
    vector<vector<Cell>> saveState;
    while (gameWindow.isOpen()) {
        sf::Event event;
        int mineCounter = board.getMines() - board.countFlags();
        // 3 digits for counter tracking number of mines
        int digit1 = mineCounter / 10;
        int digit2 = mineCounter % 10;
        int digit0 = mineCounter / 100;
        while (gameWindow.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if( (event.mouseButton.x >= 0 && event.mouseButton.x < gameWindow.getSize().x) && (event.mouseButton.y >= 0 &&
                                                                                                       gameWindow.getSize().y) ) {
                        sf::Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
                        board.setFlag(mousePosition);
                        mineCounter = board.getMines() - board.countFlags();
                        board.drawBoard(gameWindow);
                    }
                } else if (event.mouseButton.button == sf::Mouse::Left) {
                    if( (event.mouseButton.x >= 0 && event.mouseButton.x < gameWindow.getSize().x) && (event.mouseButton.y >= 0 && event.mouseButton.y <
                                                                                                                                   gameWindow.getSize().y) ) {
                        sf::Vector2f mousePosition(event.mouseButton.x, event.mouseButton.y);
                        if(debugButton.getGlobalBounds().contains(mousePosition)){
                            cout << "DEBUGGED!";
                            board.debug = !board.debug;
                            board.revealAllMines();
                        } else if (pauseButton.getGlobalBounds().contains(mousePosition)){
                            if (board.checkLoss() || board.checkWin()){
                                break;
                            }
                            board.pause = !board.pause; // reflects if board is paused or playing
                            if(board.pause){
                                saveState = deepCopy(board.grid);
                                pauseButton.setTexture(play);
                                pauseTime = clock.getElapsedTime();
                            } else {
                                pauseButton.setTexture(pause);
                                auto unpausedTime = clock.getElapsedTime();
                                elapsedPauseTime += (unpausedTime - pauseTime).asSeconds();
                                board.grid = saveState;
                            }
                        } else if (leaderboardButton.getGlobalBounds().contains(mousePosition)) {
                            sf::RenderWindow leaderboardWindow(sf::VideoMode(leaderboardWidth, leaderboardHeight), "Leaderboard", sf::Style::Close);
                            leaderboardPaused = !leaderboardPaused;
                            sf::Clock lClock;
                            while(leaderboardWindow.isOpen()){
                                sf::Event leaderboardEvent;
                                while (leaderboardWindow.pollEvent(leaderboardEvent)){
                                    if (leaderboardEvent.type == sf::Event::Closed){
                                        auto lPause = lClock.getElapsedTime();
                                        // adding up the amount of time for the leaderboard "pause" & actual pause time
                                        if(!board.pause){
                                            elapsedPauseTime += (lPause).asSeconds();
                                        }
                                        leaderboardWindow.close();
                                    }
                                }

                                leaderboardWindow.clear(sf::Color::Blue);
                                leaderboardWindow.draw(leaderboardMain);
                                setText(leaderboardInput, leaderboardWidth / 2, (leaderboardHeight / 2) + 20);

                                drawLeaderboard(leaderboardInput,entries, leaderboardWindow);
                                leaderboardWindow.display();
                            }
                        } else if (hFaceButton.getGlobalBounds().contains(mousePosition)){
                            board.resetBoard();
                            board.setNumMines(numMines);
                            board.initMines();
                            clock.restart();

                            // leaderboard makes this negative, want it to be 0 for reset
                            elapsedPauseTime = 0;
                            board.pause = false;
                            board.win = false;
                            board.loss = false;
                            board.debug = false;
                            winCount = 0;
                        }
                        else {
                            board.drawCellNumber(mousePosition, numberTexture, gameWindow);
                        }
                    }
                }
            }
        }
        if (board.checkWin()){
            hFaceButton.setTexture(faceWin);

            board.winState();
            readLeaderboard(entries);
            cout << "Read Leaderboard" << endl;

            sf::Time winTime = clock.getElapsedTime() - pauseTime;
            LeaderboardEntry e;
            // ensures we only update once, and not while the window is open
            if (winCount < 1){
                lPopup = true;
                removeAsterisk(entries);
                e.time = winTime;
                e.name = userName.getString() + '*';
                entries.push_back(e);
                updateLeaderboard(entries);
            }
            winCount ++;
            pauseTime = clock.getElapsedTime();
            board.pause = true;
        } else if (board.checkLoss()) {
            hFaceButton.setTexture(faceLose);
            board.pause = true;
            board.lossState();
        } else {
            hFaceButton.setTexture(happyFace);
        }
        gameWindow.clear(sf::Color::White);

        digitSprite.setPosition(sf::Vector2f(33, 32 * (rowCount + 0.5) + 16));
        negativeSprite.setPosition(sf::Vector2f(12, 32 * (rowCount + 0.5) + 16));

        timerSprite.setPosition(sf::Vector2f((colCount * 32) - 97, 32 * (rowCount + 0.5) + 16));
        secondSprite.setPosition(sf::Vector2f((colCount * 32) - 54,32 * (rowCount + 0.5) + 16));

        if (mineCounter < 0){
            negativeSprite.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
            gameWindow.draw(negativeSprite);
            digit0 = -digit0;
            digit1 = -digit1;
            digit2 = -digit2;
        }
        digitSprite.setTextureRect(sf::IntRect(digit0 * 21, 0, 21, 32));
        gameWindow.draw(digitSprite);

        digitSprite.move(21, 0);

        digitSprite.setTextureRect(sf::IntRect(digit1 * 21, 0, 21, 32));
        gameWindow.draw(digitSprite);

        digitSprite.move(21, 0);

        digitSprite.setTextureRect(sf::IntRect(digit2 * 21, 0, 21, 32));

        gameWindow.draw(digitSprite);

        auto gameDuration = clock.getElapsedTime().asSeconds() - startTime.asSeconds();
        float totalElapsedTime = gameDuration;

        int minutes;
        int seconds;

        if (!board.pause){
            totalElapsedTime -= elapsedPauseTime;
            seconds = static_cast<int>(lround(totalElapsedTime)) % 60;
            minutes = totalElapsedTime / 60;
        } else {
            if (!board.checkWin() && !board.checkLoss()){
                board.pauseBoard();
            }
        }

        // Now we separate the seconds into ones & tens place
        int second1 = seconds / 10 % 10;
        int second2 = seconds % 10;

        int minute1 = minutes / 10 % 10;
        int minute2 = minutes % 10;

        timerSprite.setTextureRect(sf::IntRect(minute1 * 21, 0, 21, 32));
        gameWindow.draw(timerSprite);

        timerSprite.move(21, 0);

        timerSprite.setTextureRect(sf::IntRect(minute2 * 21, 0, 21, 32));
        gameWindow.draw(timerSprite);

        secondSprite.setTextureRect(sf::IntRect(second1 * 21, 0, 21, 32));
        gameWindow.draw(secondSprite);

        secondSprite.move(21, 0);

        secondSprite.setTextureRect(sf::IntRect(second2 * 21, 0, 21, 32));
        gameWindow.draw(secondSprite);


        gameWindow.draw(hFaceButton);
        gameWindow.draw(debugButton);
        gameWindow.draw(leaderboardButton);
        gameWindow.draw(pauseButton);

        board.drawBoard(gameWindow);
        gameWindow.display();

        if(lPopup){
            sf::RenderWindow leaderboardWindow(sf::VideoMode(leaderboardWidth, leaderboardHeight), "Leaderboard", sf::Style::Close);
            while(leaderboardWindow.isOpen()){
                sf::Event leaderboardEvent;
                while (leaderboardWindow.pollEvent(leaderboardEvent)){
                    if (leaderboardEvent.type == sf::Event::Closed){
                        auto unpausedTime = clock.getElapsedTime();
                        elapsedPauseTime += (unpausedTime - pauseTime).asSeconds();
                        leaderboardWindow.close();
                        lPopup = false;
                        break;
                    }
                }

                leaderboardWindow.clear(sf::Color::Blue);
                leaderboardWindow.draw(leaderboardMain);
                setText(leaderboardInput, leaderboardWidth / 2, (leaderboardHeight / 2) + 20);

                drawLeaderboard(leaderboardInput,entries, leaderboardWindow);
                leaderboardWindow.display();
            }
        }
    }
}

