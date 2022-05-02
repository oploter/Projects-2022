#include "server_map.h"
#include <string>
#include <iostream>
#include <utility>

ServerMap::ServerMap(const std::string& file_name) : Map(file_name){}

ServerMap::ServerMap() : Map() {}

bool ServerMap::movePlayer(int player_id, float delta_x, float delta_y, int direction_id){
    Player& player = players[player_id];
    auto [new_x, new_y] = player.get_cords();
    new_x += delta_x;
    new_y += delta_y;
    if(new_x < 0 || new_x > cell_cols || new_y < 0 || new_y > cell_rows){
        return false;
    };
    player.updatePos(delta_x, delta_y, direction_id);
    return true;
}
