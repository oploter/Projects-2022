#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Graphics.hpp>
#include "client_map.h"
#include "declarations.h"

struct ClientGame{
private:
    sf::RenderWindow window;
    ClientMap map;
    Player player;
    int windowWidth;
    int windowHeight;
    int cellsWidth;
    int cellsHeight;
public:

    ClientGame(){};
    void runGame();

    void loadTextures();

    void mainWindow();

    int gameWindow();

};


#endif