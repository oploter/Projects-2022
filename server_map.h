#ifndef SERVER_MAP_H
#define SERVER_MAP_H

#include <string>
#include "declarations.h"

struct ServerMap : Map{
public:
    ServerMap();
    ServerMap(const std::string& file_name);

    bool movePlayer(int player_id, float delta_x, float delta_y, int direction_id);
};

#endif

