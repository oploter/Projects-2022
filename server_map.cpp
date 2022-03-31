#include "server_map.h"
#include <string>
#include <iostream>
#include <utility>

ServerMap::ServerMap(const std::string& file_name) : Map(file_name){}

ServerMap::ServerMap() : Map() {}

bool ServerMap::movePlayer(float delta_x, float delta_y){
    float new_x = player.x + delta_x;
    float new_y = player.y + delta_y; 
    if(new_x < Player::ps_x || (cell_cols - Player::ps_x) < new_x || new_y < Player::ps_y || (cell_rows - Player::ps_y) < new_y){
        return false;
    }
    player.x = new_x;
    player.y = new_y;
    return true;
}
