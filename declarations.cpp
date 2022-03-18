#include "declarations.h"
#include <string>
#include <iostream>
#include <fstream>



const int Map::BlockSize = 50;
const int Map::CellSize = 10;

Map::Map(const std::string& file_name) : player(){ 
    std::ifstream file(file_name);
    file >> block_cols >> block_rows;
    field.resize(block_rows, std::vector<Block>(block_cols));
    for(std::size_t row = 0; row < block_rows; row++){
        for(std::size_t col = 0; col < block_cols; col++){
            int el;
            file >> el;
            field[row][col] = static_cast<BlockType>(el);
        }
    }
    cell_cols = block_cols * (BlockSize / CellSize) + 1;
    cell_rows = block_rows * (BlockSize / CellSize) + 1;
}




// Player

Player::Player() : state(still), x(51.0 / Map::CellSize), y(72.0 / Map::CellSize), dir(move_deltas.at(sf::Keyboard::D)){}

std::pair<int, int> Player::get_map_cords() const {
    return {(Map::CellSize * x) / Map::BlockSize, (Map::CellSize * y) / Map::BlockSize};
}

void Player::print(sf::RenderTarget& target){
    static int prev = 0;
    sf::Sprite s;
    if(state == still){
        s.setTexture(*get_or_create_texture("player_0"));
        if(prev != 0){
            prev = 0;
        }
    }else if(state == run){
        s.setTexture(*get_or_create_texture("player_" + std::to_string((img_id) / 3)));
        if(img_id / 3 > prev){
            prev = img_id / 3;
        }
        img_id++;
        if(img_id > 8 * 3 -1){
            img_id = 0;
        }
    }
    std::cout << "st " << (state == PlayerState::run) << std::endl;
    s.setScale(0.5, 0.5);
    float s_width = s.getLocalBounds().width * 0.5;
    float s_height = s.getLocalBounds().height * 0.5;
    s.setOrigin(s_width / 2, s_height / 2);
    s.setPosition(Map::CellSize * x, Map::CellSize * y);
    if(s.getScale().x * dir.first < 0){
        s.scale(-1, 1);
    }
    target.draw(s);
}

void Player::updatePos(float delta_x, float delta_y){
    x += delta_x;
    y += delta_y;
    dir = {delta_x, delta_y};
}

// End Player
