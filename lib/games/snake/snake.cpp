/*
** EPITECH PROJECT, 2023
** B-OOP-400-MAR-4-1-arcade-adam.elaoumari [WSL : Ubuntu]
** File description:
** snake
*/

#include "snake.hpp"
#include <string>
#include <iostream>
#include <fstream>

std::vector<std::string> fileToArray (std::string filepath)
{
    std::ifstream inputFile(filepath);
    std::string line;
    std::vector<std::string> array;
    while (getline(inputFile, line)) {
        array.push_back(line);
    }
    inputFile.close();
    return array;
}

void Snake::getInfoSnake (std::vector<std::string> map)
{
    _size_snake = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] >= '0' && map[i][j] <= '9' && _size_snake < map[i][j] - '0') {
                _size_snake = map[i][j] - '0';
                _head_x = j;
                _head_y = i;
            }
        }
    }
}

Snake::Snake()
{
    std::vector<std::string> map = fileToArray("lib/games/snake/map1.txt");
    generateGrid(map);
    getInfoSnake(map);
    dataToEntity();
    generateRandomApple();
}

Snake::~Snake()
{
}

bool closeToOne (int a, int b)
{
    if (a == 0 || b == 0)
        return false;
    if (a == b)
        return true;
    if (a + 1 == b)
        return true;
    if (a - 1 == b)
        return true;
    return false;
}

int Snake::chooseSprite (float x, float y, int value)
{
    if (!getGrid().size() || !getGrid()[y].size() || !getGrid()[y][x])
        return 0;
    if (getGrid()[y].size() > x && closeToOne(getGrid()[y][x + 1], value) && x > 0 && closeToOne(getGrid()[y][x - 1], value)) // horizontal
        return 10;
    if (getGrid().size() > y && closeToOne(getGrid()[y + 1][x], value) && y > 0 && closeToOne(getGrid()[y - 1][x], value)) // vertical
        return 1;
    if (value == _size_snake) { // head
        if (getDirection() == DIRECTION::DOWN) {
            return 6;
        } else if (getDirection() == DIRECTION::LEFT) {
            return 7;
        } else if (getDirection() == DIRECTION::RIGHT) {
            return 8;
        } else if (getDirection() == DIRECTION::UP) {
            return 9;
        }
    } else if (value == 1) { // tail
        if (getGrid().size() > y && getGrid()[y + 1][x] == 2) { // move down
            return 11;
        } else if (x > 0 && getGrid()[y][x - 1] == 2) { // move left
            return 12;
        } else if (getGrid()[y].size() > x && getGrid()[y][x + 1] == 2) { // move right
            return 13;
        } else if (y > 0 && getGrid()[y - 1][x] == 2) { // move up
            return 14;
        } else {
            return 10; // ?
        }
    } else {
        if (getGrid().size() > y && closeToOne(getGrid()[y + 1][x], value) && x > 0 && closeToOne(getGrid()[y][x - 1], value)) // down left
            return 2;
        if (getGrid().size() > y && closeToOne(getGrid()[y + 1][x], value) && getGrid()[y].size() > x && closeToOne(getGrid()[y][x + 1], value)) // down right
            return 3;
        if (y > 0 && closeToOne(getGrid()[y - 1][x], value) && x > 0 && closeToOne(getGrid()[y][x - 1], value)) // up left
            return 4;
        if (y > 0 && closeToOne(getGrid()[y - 1][x], value) && getGrid()[y].size() > x && closeToOne(getGrid()[y][x + 1], value)) // up right
            return 5;
    }
    return 10;
}

void Snake::snakePart(std::vector<int> value, float x, float y, int row, int i)
{
    std::vector<std::string> snake = {
        "A", "|", "\\", "/", "/", "\\", "M", "3", "E", "W", "=", "^", ">", "<", "v", "#", " "
    };

    int index_snake = chooseSprite(x, y - 1, value[i]);
    Entity newEntity = {
        index_snake,
        snake[index_snake],
        "",
        x,
        y,
        false,
        false,
        {0, 255, 0},
        // ((index_snake >= 6 && snake[index_snake] <= 9) ? {255, 0, 0} : {0, 255, 0}),
        {0, 0, 0},
    };
    setNewEntity(std::to_string(i) + "snake" + std::to_string(row), newEntity);
}

// TODO :
// size of text in sfml
// size of sprite in sfml

void Snake::dataToEntity(void)
{
    // decale d'un carre pour laisser la place au score
    float y = 1, x = 0;
    int row = 0;
    clearEntities();
    for (std::vector<int> value : getGrid()) {
        for (int i = 0; i < getGrid()[0].size(); i++) {
            if (value[i] == -2) { // walls
                Entity newEntity = {
                    15,
                    "#",
                    "",
                    x,
                    y,
                    false,
                    false,
                    {255, 255, 255},
                    {0, 0, 0},
                };
                setNewEntity(std::to_string(i) + "walls" + std::to_string(row), newEntity);
            } else if (value[i] == 0) { // spaces
                Entity newEntity = {
                    -1,
                    " ",
                    "",
                    x,
                    y,
                    false,
                    false,
                    {0, 0, 0},
                    {0, 0, 0},
                };
                setNewEntity(std::to_string(i) + "spaces" + std::to_string(row), newEntity);
            } else if (value[i] == -1) { // apple
                Entity newEntity = {
                    0,
                    "A",
                    "",
                    x,
                    y,
                    false,
                    false,
                    {255, 0, 0},
                    {0, 0, 0},
                };
                setNewEntity(std::to_string(i) + "apples" + std::to_string(row), newEntity);
            } else if (value[i] > 0) {
                snakePart(value, x, y, row, i);
            } else {
                // std::cout << "Error: " << value[i] << std::endl;
            }
            x += 1;
        }
        x = 0;
        y += 1;
        row++;
    }
    x = 8;
    y = 0;
    Entity newEntity = {
        -1,
        "Time: " + std::to_string(getTime()),
        "E9967A",
        x,
        y,
        true,
        true,
        255,
        255,
        255
    };
    setNewEntity("time", newEntity);
    y = 1;
    newEntity = {
        -1,
        "Score: " + std::to_string(getScore()),
        "E9967A",
        x,
        y,
        true,
        true,
        255,
        255,
        255
    };
    setNewEntity("score", newEntity);
    y = 2;
}

void Snake::moveHead(int x, int y, bool eat, IGameModule::DIRECTION direction)
{
    setDirection(direction);
    setGridValue(_head_y += y, _head_x += x, _size_snake + 1);
    _size_snake += eat;
    if (eat) {
        setScore(10);
        setText("score", "Score: " + std::to_string(getScore()));
        std::cout << "Score: " << getScore() << std::endl;
        generateRandomApple();
    }
}

// return -1 = nop
// return 0 = not decrement
// return 1 = decrement
int Snake::tryMoveHere(IGameModule::DIRECTION direction)
{
    if (direction == UP && _head_y > 0) {
        if (getGrid()[_head_y - 1][_head_x] == -1) {
            moveHead(0, -1, true, direction);
            return 1;
        } else if (getGrid()[_head_y - 1][_head_x] >= 1 || getGrid()[_head_y - 1][_head_x] == -2) {
            setGameStatus(FINISHED);
            return 1; // snake
        }
        moveHead(0, -1, false, direction);
        return 2;
    } else if (direction == DOWN && getGrid().size() > _head_y) {
        if (getGrid()[_head_y + 1][_head_x] == -1) {
            moveHead(0, 1, true, direction);
            return 1;
        } else if (getGrid()[_head_y + 1][_head_x] >= 1 || getGrid()[_head_y + 1][_head_x] == -2) {
            setGameStatus(FINISHED);
            return 1; // snake
        }
        moveHead(0, 1, false, direction);
        return 2;
    } else if (direction == LEFT && _head_x > 0) {
        if (getGrid()[_head_y][_head_x - 1] == -1) {
            moveHead(-1, 0, true, direction);
            return 1;
        } else if (getGrid()[_head_y][_head_x - 1] >= 1 || getGrid()[_head_y][_head_x - 1] == -2) {
            setGameStatus(FINISHED);
            return 1; // snake
        }
        moveHead(-1, 0, false, direction);
        return 2;
    } else if (direction == RIGHT && getGrid()[_head_y].size() > _head_x) {
        if (getGrid()[_head_y][_head_x + 1] == -1) {
            moveHead(1, 0, true, direction);
            return 1;
        } else if (getGrid()[_head_y][_head_x + 1] >= 1 || getGrid()[_head_y][_head_x + 1] == -2) {
            setGameStatus(FINISHED);
            return 1; // snake
        }
        moveHead(1, 0, false, direction);
        return 2;
    } else {
        return 0;
    }
}

void Snake::move(void)
{
    if (getTimeElapsed(start) < std::chrono::milliseconds(int(std::floor(200 - (_score * 0.5))))) {
        return;
    } else {
        setChronoValue(std::chrono::high_resolution_clock::now());
    }

    int decrement = -1;
    if ((decrement = tryMoveHere(_next_direction)) == -1)
        decrement = tryMoveHere(getDirection());
    if (decrement != 2)
        return;
    for (int i = 0; i != getGrid().size(); i++) {
        for (int j = 0; j != getGrid()[i].size(); j++) {
            if (getGrid()[i][j] > 0) {
                setGridValue(i, j, getGrid()[i][j] - 1);
            }
        }
    }
}

void Snake::update(std::string key)
{
    if (_status == IN_GAME) {
        if (getTimeElapsed(scoreClock) >= std::chrono::milliseconds(1000)) {
            setTime(1);
            scoreClock = std::chrono::high_resolution_clock::now();
        }
    }
    if (key == "LeftArrow" && getDirection() != RIGHT)
        _next_direction = LEFT;
    if (key == "RightArrow" && getDirection() != LEFT)
        _next_direction = RIGHT;
    if (key == "UpArrow" && getDirection() != DOWN)
        _next_direction = UP;
    if (key == "DownArrow" && getDirection() != UP)
        _next_direction = DOWN;
    move();
    dataToEntity();
    if (key.empty())
        return;
    if (key == "close") {
        setGameStatus(CLOSED);
    }
}

void Snake::resetGame(void)
{
    _entities.clear();
    _grid.clear();
    start = std::chrono::high_resolution_clock::now();
    scoreClock = std::chrono::high_resolution_clock::now();
    _time = 0;
    _score = 0;
    _next_direction = IGameModule::DIRECTION::RIGHT;
    _head_x = 0;
    _head_y = 0;
    _size_snake = 0;
    std::vector<std::string> map = fileToArray("lib/games/snake/map1.txt");
    generateGrid(map);
    generateRandomApple();
    getInfoSnake(map);
    dataToEntity();

}

extern "C" IGameModule *create(void) {
    return new Snake();
}

extern "C" std::string getType() {
    return "Game";
}

std::vector<std::string> Snake::getTextures() {
    std::vector<std::string> snake = {
        "lib/games/snake/files/snake/apple.png",
        "lib/games/snake/files/snake/body-vertical.png",
        "lib/games/snake/files/snake/curv-down-left.png",
        "lib/games/snake/files/snake/curv-down-right.png",
        "lib/games/snake/files/snake/curv-up-left.png",
        "lib/games/snake/files/snake/curv-up-right.png",
        "lib/games/snake/files/snake/head-down.png",
        "lib/games/snake/files/snake/head-left.png",
        "lib/games/snake/files/snake/head-right.png",
        "lib/games/snake/files/snake/head-up.png",
        "lib/games/snake/files/snake/horizontal-body.png",
        "lib/games/snake/files/snake/tail-down.png",
        "lib/games/snake/files/snake/tail-left.png",
        "lib/games/snake/files/snake/tail-right.png",
        "lib/games/snake/files/snake/tail-up.png",
        "lib/games/snake/files/snake/walls.png"
    };
    return snake;
}

void Snake::generateRandomApple()
{
    int maxX = _grid[0].size();
    int maxY = _grid.size();
    int x = rand() % maxX;
    int y = rand() % maxY;
    if (_grid[y][x] == 0)
        _grid[y][x] = -1;
    else
        generateRandomApple();
}