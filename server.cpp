#include "server.h"
#include "cl_sr_queue.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <utility>
#include "server_map.h"
#include <chrono>


void ServerGame::runGameServer(){
    std::cout << "RUNGAMESERVER\n";
    map = ServerMap("map.txt");
    std::cout << "map loaded\n";
}   