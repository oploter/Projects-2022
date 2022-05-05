#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H
#include <SFML/Network.hpp>
#include "cl_sr_queue.h"
#include <utility>
namespace client{
struct Client_Server_Conn{
private:
    sf::IpAddress ip{"127.0.0.1"};
    threadsafe_queue<sf::Packet> Q;
    int port;
    int player_id = 0;
public:
    int init(bool single_player = true);
    int run(bool single_player = true);
    void receiveMessages();
    void endConnection();
    sf::TcpSocket server_socket;
    sf::Packet getPacket(int msg_type);
    bool getMessage(int& msg_type, sf::Packet& message);
    bool send(sf::Packet&& packet);
    void try_connect();
};
}
#endif