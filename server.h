#ifndef SERVER_H
#define SERVER_H

#include "server_map.h"
#include "declarations.h"

struct ServerGame{
private:
    ServerMap map;
public:

    void runGameServer();
    //void movePlayer();
    //void checkAndPlant();

};



#endif