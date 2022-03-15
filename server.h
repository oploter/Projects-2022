#ifndef SERVER_H
#define SERVER_H

#include "server_map.h"
#include "declarations.h"
#include <utility>

struct ServerGame{
private:
    ServerMap map;
    
public:

    void runGameServer();
    void movePlayer(const std::pair<int, int>& delta);
    //void checkAndPlant();

};



#endif