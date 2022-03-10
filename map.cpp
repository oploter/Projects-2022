#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include "map.h"
#include "declarations.h"

//Map

const int Map::BlockSize = 50;
const int Map::CellSize = 10;

const std::unordered_map<Map::BlockType, std::string> Map::textureNames = {{Map::green, "green_block"}, {Map::brick, "brick_block"}, {Map::water, "water_block"}, {Map::noblock, "noblock_block"}, {Map::used, "green_block"}};
const std::unordered_map<Map::BlockType, std::string> Map::blockState = {{Map::green, "plant here"}, {Map::brick, "cannot plant here"}, {Map::water, "water"}, {Map::used, "cannot plant here"}};

Map::Map(const std::string& file_name) : stateText("None", *get_or_create_font("basic_font")){
    std::ifstream file(file_name, std::ios_base::in);
    file >> cols >> rows;

    if(!rTexture.create(cols * BlockSize, rows * BlockSize)){
        std::cout << "Couldnot create texture\n";
    }
    
    field.resize(rows, std::vector<Block>(cols, Block(noblock)));
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            int elem;
            file >> elem;
            std::cout << "element at " << row << ' ' << col << " " << elem << std::endl;
            field[row][col] = static_cast<BlockType>(elem);

        }
    }

    stateText.setFillColor(sf::Color::Red);
    stateText.setScale(0.9, 0.9);
    stateText.setPosition(BlockSize * cols - 50, BlockSize);
    update();
}

void Map::update(){
    rTexture.clear();
    std::cout << "num of rows " << rows << ' ' << cols << std::endl;
    std::cout << field.size() << ' ' << field[0].size() << std::endl;
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            sf::Texture* block_texture = get_or_create_texture(textureNames.at(field[row][col].type));
            sf::Sprite s(*block_texture);
            s.setPosition(col * BlockSize, row * BlockSize);
            rTexture.draw(s);
        }
    }
    // for(std::size_t row = 0; row <= 7 * rows; row++){
    //     sf::Vertex line[] = {sf::Vertex(sf::Vector2f(0, cellSize * row)), sf::Vertex(sf::Vector2f(blockSize * cols, cellSize * row))};
    //     rTexture.draw(line, 2, sf::Lines);
    // }
    // for(std::size_t col = 0; col <= 7 * cols; col++){
    //     sf::Vertex line[] = {sf::Vertex(sf::Vector2f(cellSize * col, 0)), sf::Vertex(sf::Vector2f(cellSize * col, rows * blockSize))};
    //     rTexture.draw(line, 2, sf::Lines);
    // }
    rTexture.display();
}

void Map::checkPlayer(Player& player){
    std::pair<int, int> player_map_cords = player.get_map_cords();
    std::cout << "Player is at " << player_map_cords.first << ' ' << player_map_cords.second << std::endl;
    BlockType pot = field[player_map_cords.second][player_map_cords.first].type;
    std::cout << "Player is on " << pot << std::endl;
    if(pot == water){
        player.dead = true;
    }
    stateText.setString(blockState.at(pot));
    stateText.setPosition(BlockSize * cols - 2 * BlockSize - stateText.getLocalBounds().width, BlockSize);
}


void Map::print(sf::RenderTarget& window){
    sf::Sprite s(rTexture.getTexture());
    window.draw(s);
    window.draw(stateText);
}

// Map end

// Plant/Cell

//Plant::Plant() : Cell(Map::used), hp(hpDefault), timeLimit(timeLimitDefault){
//    std::cout << "new plant created\n";
//}
//
//int Plant::get_hp() const {
//    return hp;
//}
//
//int Plant::get_timeLimit() const {
//    return timeLimit;
//}

// Plant/Cell end