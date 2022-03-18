#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "client_map.h"
#include "cl_sr_queue.h"
#include "declarations.h"

struct ClientGame{
private:
    sf::RenderWindow window;
    ClientMap map;
    int windowWidth;
    int windowHeight;
    int cellsWidth;
    int cellsHeight;


    sf::TcpSocket server;
    sf::IpAddress ip;
    threadsafe_queue<sf::Packet> Q;

public:

    ClientGame();
    void runGame();

    void loadTextures();

    void mainWindow();

    int gameWindow();

    void ReceiveMessages();

};


#endif