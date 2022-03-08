#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include "map.h"
#include "declarations.h"

//Map

const std::unordered_map<Map::BlockType, std::string> Map::block_names = {{Map::green, "green_block"}, {Map::beton, "brick_block"}, {Map::water, "water_block"}, {Map::noblock, "noblock_block"}, {Map::used, "green_block"}};
const std::unordered_map<Map::BlockType, std::string> Map::block_state = {{Map::green, "plant here"}, {Map::beton, "cannot plant here"}, {Map::water, "water"}, {Map::used, "cannot plant here"}};

Map::Map(const std::string& file_name) : state_text("None", *get_or_create_font("basic_font")){
    std::ifstream file(file_name, std::ios_base::in);
    file >> rows >> cols;
    if(!rTexture.create(cols * blockSize, rows * blockSize)){
        std::cout << "Couldnot create texture\n";
    }
    
    field.resize(rows, std::vector<Cell>(cols, Cell(noblock)));
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            int elem;
            file >> elem;
            std::cout << "element at " << row << ' ' << col << " " << elem << std::endl;
            field[row][col] = static_cast<BlockType>(elem);

        }
    }

    state_text.setFillColor(sf::Color::Red);
    state_text.setScale(0.9, 0.9);
    state_text.setPosition(blockSize * cols - 50, blockSize);
    update();
}

void Map::update(){
    rTexture.clear();
    std::cout << "num of rows " << rows << ' ' << cols << std::endl;
    std::cout << field.size() << ' ' << field[0].size() << std::endl;
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            sf::Texture* block_texture = get_or_create_texture(block_names.at(field[row][col].type));
            sf::Sprite s(*block_texture);
            s.setPosition(col * blockSize, row * blockSize);
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
    state_text.setString(block_state.at(pot));
    state_text.setPosition(blockSize * cols - 2 * blockSize - state_text.getLocalBounds().width, blockSize);
}


void Map::print(sf::RenderTarget& window){
    sf::Sprite s(rTexture.getTexture());
    window.draw(s);
    window.draw(state_text);
}

// Map end

// Plant/Cell

Plant::Plant() : Cell(Map::used), hp(hpDefault), timeLimit(timeLimitDefault){
    std::cout << "new plant created\n";
}

int Plant::get_hp() const {
    return hp;
}

int Plant::get_timeLimit() const {
    return timeLimit;
}

// Plant/Cell end