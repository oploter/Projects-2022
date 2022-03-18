#ifndef SERVER_MAP_H
#define SERVER_MAP_H

#include <string>
#include "declarations.h"

struct ServerMap : Map{
public:
    ServerMap();
    ServerMap(const std::string& file_name);

    bool movePlayer(float delta_x, float delta_y);
};

#endif

