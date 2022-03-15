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
    while(1){
        auto& q_s = queue_for_ser();
        auto& q_c = queue_for_cl();
        while(!q_s.empty()){
            std::pair<int, std::pair<int, int>> pr = q_s.front(); 
            q_s.pop();
            std::cout << "new ev " << pr.first << " {" << pr.second.first << ' ' << pr.second.second << "}\n";
            if(pr.first == 1){
                
                q_c.push({});
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        std::cout << "woke up\n";
    }
}