#include "server_map.h"
#include <string>
#include <iostream>
#include <utility>

ServerMap::ServerMap(const std::string& file_name) : Map(file_name){
    std::cout << "in map creating\n";
}

