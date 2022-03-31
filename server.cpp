#include <SFML/Network.hpp> 
#include "server.h" 
#include "cl_sr_queue.h" 
#include <chrono> 
#include <thread> 
#include <iostream> 
#include <utility> 
#include "server_map.h" 
#include "declarations.h"
#include <chrono> 
#include <mutex> 
#include <cmath>

std::mutex m; 
void ServerGame::runGameServer(){
    map = ServerMap("map.txt"); 
    sf::IpAddress ip = sf::IpAddress::getLocalAddress(); 
    listener.listen(12345); 
    if (listener.accept(socket) != sf::Socket::Done) { 
        std::cout << "Error!\n"; 
        return; 
    } 
    socket.setBlocking(false); 
    std::thread send_thread([this](){ 
        SendMessages(); 
    }); 
    std::thread receive_thread([this](){ 
        ReceiveMessages(); 
    }); 
    while(1){
        map.updateBullets();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/30));
    }
    send_thread.join(); 
    receive_thread.join(); 
} 
void ServerGame::SendMessages(){ 
    while(1){ 
        m.lock(); 
        while (!Q.empty()) { 
            packet = Q.pop(); 
            int type;
            packet >> type;
            if(type == 1){
                packet >> x >> y; 
                if (map.movePlayer(x, y)) {
                    packet.clear();
                    packet << 1 << x << y; 
                    socket.send(packet); 
                }
            }else if(type == 2){
                float delta_x, delta_y;
                packet >> delta_x >> delta_y;
                float angle = (delta_x == 0 ? (PI/2 * (delta_y < 0 ? -1 : 1)) : std::atan(delta_y / delta_x));
                if(delta_x < 0){
                    angle += PI;
                }
                std::vector<float> b = {map.player.x, map.player.y, std::cos(angle), std::sin(angle), static_cast<float>(angle * 180 / PI)};
                packet.clear();
                packet << 2 << b[0] << b[1] << b[2] << b[3] << b[4];
                socket.send(packet);
                map.bullets.push_back(std::move(b));
            }else if(type == 3){
                int p_x = map.player.get_map_cords().first;
                int p_y = map.player.get_map_cords().second;
                packet.clear();
                if(map.field[p_y][p_x].type == BlockType::green){
                    map.field[p_y][p_x].type = BlockType::used;
                    map.plants.push_back({p_x, p_y});
                    map.plants_cnts.push_back(0);
                    packet << 3 << p_x << p_y;
                }
                socket.send(packet);

            }
        } 
        m.unlock(); 
    } 
} 
void ServerGame::ReceiveMessages(){ 
    while(1){ 
        m.lock(); 
        if (socket.receive(packet) == sf::Socket::Done) { 
            Q.push(packet); 
        } 
        m.unlock();
    } 
}
