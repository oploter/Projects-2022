#include "declarations.h"
#include <string>
#include <iostream>
#include <fstream>

const int Map::BlockSize = 50;
const int Map::CellSize = 10;
namespace{
uint8_t get_rand(){
    int num = std::rand();
    num %= 255;
    return num;
}
}
const std::unordered_map<sf::Keyboard::Key, int> direction_ids = {{sf::Keyboard::W, 0}, {sf::Keyboard::A, 1}, {sf::Keyboard::S, 2}, {sf::Keyboard::D, 3}};
const std::vector<Direction> directions = {{{0, -1}, 0, "up", 3}, {{-1, 0}, 1, "left", 4}, {{0, 1}, 2, "down", 4}, {{1, 0}, 3, "right", 4}};
//Map
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
    players.emplace_back();
    return players.size() - 1;
}  
Player& Map::getPlayer(int player_id){
    return players[player_id];
} 
void Map::addBullet(float curr_x, float curr_y, float dx, float dy){
    float angle = (dx == 0 ? (-1 * (dy < 0))PI / 2 : std::atan(dy/dx));
    if(dx < 0){
        angle += PI;
    }
    bullets.push_back({curr_x, curr_y, dx, dy, angle});
}

// Player
Player::Player() : state(still), x(0), y(0), player_color({get_rand(), get_rand(), get_rand()}){}
std::pair<int, int> Player::get_map_cords() const {
    return {(Map::CellSize * x) / Map::BlockSize, (Map::CellSize * y) / Map::BlockSize};
}
std::pair<float, float> Player::get_cords() const{
    return {x, y};
}
void Player::updatePos(float delta_x, float delta_y, int direction_id_){
    x += delta_x;
    y += delta_y;
    direction = direction_id_;
}
void Player::print(sf::RenderTarget& window){
    sf::Sprite s;
    if(state == still){
        s.setTexture(*get_or_create_texture("player_down_0"));
    }else if(state == run){
        int& curr_img_id = img_id[direction];
        const Direction& dir = directions[direction];
        s.setTexture(*get_or_create_texture("player_" + dir.name + "_" + std::to_string(curr_img_id / 3)));
        curr_img_id++;
        if(curr_img_id >= 3 * dir.max_img_num){
            curr_img_id = 0;
        }
    }
    s.setScale(0.25, 0.25);
    s.setOrigin(s.getLocalBounds().width/2, s.getLocalBounds().height/2);
    s.setPosition(x * Map::CellSize, y * Map::CellSize);
    window.draw(s); 
}
void Player::setState(PlayerState new_state){
    std::cout << "PLAYER::SET_STATE CALLED\n";
    state = new_state;
}
float Player::getSpeed() const {
    return speed;
}