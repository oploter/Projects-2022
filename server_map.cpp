#include "server_map.h"
#include <string>
#include <iostream>
#include <utility>

ServerMap::ServerMap(const std::string& file_name) : Map(file_name){}

ServerMap::ServerMap() : Map() {}

bool ServerMap::movePlayer(float delta_x, float delta_y){
    float new_x = player.x + delta_x;
    float new_y = player.y + delta_y; 
    if(new_x < 0 || new_x > cell_cols || new_y < 0 || new_y > cell_rows){
        return false;
    }
    player.x = new_x;
    player.y = new_y;
    return true;
}
