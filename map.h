#ifndef MAP_H
#define MAP_H

#include <vector>
#include <unordered_map>
#include <string>
#include "declarations.h"
#include <SFML/Graphics.hpp>

struct Block;

struct Map{
public: 
    friend struct Block;
    enum BlockType {green = 0, brick, water, noblock, used};
    static const int BlockSize;
    static const int CellSize;

    Map(int rows_, int cols_);
    Map(const std::string& file_name);

    void update();
    void print(sf::RenderTarget& window);

    void checkPlayer(Player& player);


private:
    std::vector<std::vector<Block>> field;
    static const std::unordered_map<BlockType, std::string> textureNames;  
    static const std::unordered_map<BlockType, std::string> blockState;
    sf::Text stateText;
    int rows, cols;

    sf::RenderTexture rTexture;

};

struct Block{
    Map::BlockType type;

    Block() = default;
    Block(Map::BlockType type_) : type(type_){}
};

#endif