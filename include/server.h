#ifndef SERVER_H 
#define SERVER_H 
#include "server_map.h" 
#include "declarations.h" 
#include <utility> 
#include "cl_sr_queue.h" 
struct ServerGame{ 
private: 
    ServerMap map; 
    sf::TcpListener listener; 
    sf::TcpSocket socket; 
    sf::Packet packet; 
    threadsafe_queue<sf::Packet> Q; 
    float x, y; 
     
public: 
    void runGameServer(int port); 
    void SendMessages(); 
    void ReceiveMessages(); 
    //void checkAndPlant(); 
}; 
#endif
