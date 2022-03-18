#ifndef SERVER_MAP_H
#define SERVER_MAP_H

#include <string>
#include "declarations.h"

struct ServerMap : Map{
public:
    ServerMap();
    ServerMap(const std::string& file_name);

    bool movePlayer(int delta_x, int delta_y);
};

#endif

/*
    g++ -std=c++17 main.cpp server.cpp client.cpp server_map.cpp client_map.cpp declarations.cpp -o sfml-app -lsfml-window -lsfml-system -lsfml-graphics
*/