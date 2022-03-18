#include "server_map.h"
#include <string>
#include <iostream>
#include <utility>

ServerMap::ServerMap(const std::string& file_name) : Map(file_name){
    std::cout << "in map creating\n";
}

ServerMap::ServerMap() : Map() {
    std::cout << "empty map\n";
}

bool ServerMap::movePlayer(int delta_x, int delta_y){
    float new_x = player.x + delta_x;
    float new_y = player.y + delta_y;
    if(new_x < 0 || new_x >  || new_y < 0 || new_y > (rows * BlockSize / CellSize)){
        std::cout << "move player err\n";
        return false;
    }
    player.x = new_x;
    player.y = new_y;
    std::cout << "move layer ok\n";
    return true;
}