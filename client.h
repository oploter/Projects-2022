#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "client_map.h"
#include "cl_sr_queue.h"
#include "declarations.h"
#include "client_functions.h"
namespace client{
struct ClientGame{
private:
    sf::RenderWindow window;
    ClientMap map;
    Client_Server_Conn server;
    int windowWidth, windowHeight;
    int cellsWidth, cellsHeight;
    int player_id;
    bool new_msgs = false;
    bool was_moved_curr = false;
    bool was_moved_prev = false;
public:
    ClientGame(){}
    void runGame();
    void loadTextures();
    void mainWindow();
    int gameWindow(bool single_game = true);
    void readMessages();
    bool sendMove(int direction, const Player& curr_player, const sf::Event& ev);
    sf::Text createButton(std::string label, const sf::Color& outline_color, std::pair<float, float> left_up_cords, int thickness = 1, sf::Font* font = get_or_create_font("basic_font"));

};
}

#endif