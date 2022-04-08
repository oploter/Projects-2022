#include "declarations.h"
#include <string>
#include <iostream>
#include <fstream>



const int Map::BlockSize = 50;
const int Map::CellSize = 10;

Map::Map(const std::string& file_name){ 
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
    create_player();
}

int Map::create_player(){
    players.push_back({});
    return players.size() - 1;
}   

uint8_t get_rand(){
    int num = std::rand();
    num %= 255;
    return num;
}


// Player

Player::Player() : state(still), x(0), y(0), dir(move_deltas.at(sf::Keyboard::D)), player_color({get_rand(), get_rand(), get_rand()}){}

std::pair<int, int> Player::get_map_cords() const {
    return {(Map::CellSize * x) / Map::BlockSize, (Map::CellSize * y) / Map::BlockSize};
}

void Player::print(sf::RenderTarget& window){
    //sf::Sprite s;
    //if(state == still){
    //    s.setTexture(*get_or_create_texture("player_0"));
    //}else if(state == run){
    //    s.setTexture(*get_or_create_texture("player_" + std::to_string(img_id / 2)));
    //    img_id++;
    //    if(img_id > 15){
    //        img_id = 0;
    //    }
    //}
    //s.setScale(0.5, 0.5);
    //s.setOrigin(s.getGlobalBounds().width/2, s.getGlobalBounds().height/2);
    //s.setPosition({x * Map::CellSize, y * Map::CellSize});
    sf::RectangleShape rshp({10, 10});
    rshp.setFillColor(player_color);
    rshp.setPosition(x * Map::CellSize, y * Map::CellSize);
    window.draw(rshp);
}


void Player::updatePos(float delta_x, float delta_y){
    x += delta_x;
    y += delta_y;
    dir = {delta_x, delta_y};
}

// End Player
