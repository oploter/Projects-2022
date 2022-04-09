#include <SFML/Network.hpp> 
#include "server.h" 
#include "cl_sr_queue.h" 
#include <chrono> 
#include <thread> 
#include <iostream> 
#include <utility> 
#include "server_map.h" 
#include <chrono> 
#include <mutex> 
std::mutex m; 
void ServerGame::runGameServer(int port){ 
    map = ServerMap("map.txt");  
    sf::IpAddress ip = sf::IpAddress::getLocalAddress(); 
    listener.listen(port); 
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
    send_thread.join(); 
    receive_thread.join(); 
} 
void ServerGame::SendMessages(){ 
    while(1){ 
        m.lock(); 
        while (!Q.empty()) { 
            packet = Q.pop(); 
            int player_id;
            packet >> player_id >> x >> y; 
            if (map.movePlayer(player_id, x, y)) {
                packet.clear();
                packet << player_id << x << y; 
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
