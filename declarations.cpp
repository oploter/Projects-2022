#include "declarations.h"
#include <string>
#include <iostream>
#include <fstream>



const int Map::BlockSize = 50;
const int Map::CellSize = 10;


Map::Map(int rows_, int cols_) : rows(rows_), cols(cols_), field(rows, std::vector<Block>(cols)), player(){
    std::cout << "map base created2\n";
}

Map::Map(const std::string& file_name) : player(){ 
    std::ifstream file(file_name);
    file >> cols >> rows;
    std::cout << "got " << cols << ' ' << rows << std::endl;
    field.resize(rows, std::vector<Block>(cols));
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            int el;
            file >> el;
            field[row][col] = static_cast<BlockType>(el);
        }
    }
    std::cout << "map base3\n";
}



// Player
int Player::Radius = 20;

Player::Player(): circle(Radius){}

std::pair<int, int> Player::get_map_cords() const {
    return {(Map::CellSize * x) / Map::BlockSize, (Map::CellSize * y) / Map::BlockSize};
}

void Player::print(sf::RenderTarget& target) const {
    //circle.setPosition(Map::CellSize * x, Map::CellSize * y);
    target.draw(circle);
}

bool Player::isDead() const {
    return dead;
}



//End Player