#include <SFML/Network.hpp> 
#include "../include/server.h" 
#include "../include/cl_sr_queue.h"
#include "../include/server_map.h" 
#include <chrono> 
#include <thread> 
#include <iostream> 
#include <utility>  
#include <chrono> 
#include <mutex> 
std::mutex m; 
void ServerGame::runGameServer(int port){ 
    std::cout << "hello from server game " << port << "\n";
    map = ServerMap("../map.txt");  
    //sf::IpAddress ip = sf::IpAddress::getLocalAddress(); 
    sf::IpAddress ip{"127.0.0.1"};
    listener.listen(port); 
    std::cout << "listiengin " << port << ' ' << ip.toString() << "\n";
    if (listener.accept(socket) != sf::Socket::Done) { 
        std::cout << "Error!\n"; 
        return; 
    } 
    std::cout << "accepted inoming connetctino\n";
    socket.setBlocking(false); 
    std::thread send_thread([this](){ 
        SendMessages(); 
    }); 
    std::thread receive_thread([this](){ 
        ReceiveMessages(); 
    }); 
    std::cout << "threds created\n";
    send_thread.join(); 
    receive_thread.join(); 
    std::cout << "ended server game\n";
} 
void ServerGame::SendMessages(){ 
    while(1){ 
        m.lock(); 
        while (!Q.empty()) { 
            packet = Q.pop(); 
            int type;
            packet >> type;
            if(type == 1){
                int player_id, direction_id;
                packet >> player_id >> x >> y >> direction_id;
                if (map.movePlayer(player_id, x, y, direction_id)) {
                    packet.clear();
                    packet << 1 << player_id << x << y << direction_id;
                    if(socket.send(packet) != sf::Socket::Done){
                        std::cout << "Server cant send 1\n";
                    }
                }
            }else if(type == 2){
                packet.clear();
                packet << 2 << map.create_player();
                if(socket.send(packet) != sf::Socket::Done){
                    std::cout << "Server cant send " << 2 << "\n";
                }  
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
