#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>
#include <cassert>
#include "../include/client.h"
#include "../include/server.h"
#include "../include/declarations.h"
#include "../include/client_map.h"
#include "../include/cl_sr_queue.h"
#include "../include/client_functions.h"

namespace{
int find_first(sf::Image& img, std::size_t x_str, bool non_white, const sf::Color& color){
    int y_n = img.getSize().y;
    while(x_str < img.getSize().x){
        bool chc = !non_white;
        for(int y = 0; y < y_n; y++){
            if(non_white){
                chc |= (img.getPixel(x_str, y) != color);
            }else{
                chc &= (img.getPixel(x_str, y) == color);
            }
        }
        if(chc){
            return x_str;
        }
        x_str++;
    }
    return -1;
}
void cut_image(sf::Image& image, const std::string& name, int num_imgs, const sf::Color& color){
    int prev = 0;
    int img_height = image.getSize().y;
    for(int i = 0; i < num_imgs; i++){
        int img_begin = find_first(image, prev, 1, color);
        int img_end = find_first(image, img_begin + 1, 0, color);
        prev = img_end + 1;
        sf::Texture* new_texture = get_or_create_texture("player_" + name + "_" + std::to_string(i));
        new_texture->loadFromImage(image, {img_begin, 0, img_end - img_begin, img_height});
    }
}
}
namespace client{
bool ClientGame::sendMove(int direction,const Player& curr_player, const sf::Event& ev){
    std::cout << "send move called\n";
    std::pair<float, float> delta = directions[direction].delta;
    std::cout << "MOVE directions " << delta.first << ' ' << delta.second;
    delta.first *= curr_player.getSpeed();
    delta.second *= curr_player.getSpeed();
    if(ev.key.shift){
        delta.first *= 1.5;
        delta.second *= 1.5;
    }
    sf::Packet packet  = server.getPacket(1);
    packet << delta.first << delta.second << direction;
    if(server.server_socket.send(packet) != sf::Socket::Done){
        std::cout << "ACACN send\n";
        return 0;
    }
    return true;
}
void ClientGame::readMessages(){
    sf::Packet msg;
    int type;
    was_moved_curr = false;
    new_msgs = false;
    while(server.getMessage(type, msg)){
        new_msgs = true;
        if(type == 1){
            float delta_x, delta_y;
            int player_id_, direction_id;
            msg >> player_id_ >> delta_x >> delta_y >> direction_id;
            map.getPlayer(player_id_).updatePos(delta_x, delta_y, direction_id);
            if(player_id_ == player_id){
                was_moved_curr = true;
            }
        }
    }
}
int ClientGame::gameWindow(bool single_game){
    int port = server.init(single_game);
    if(single_game){
        map.loadMap("../map.txt");
        std::thread server_thread([&port](){
        ServerGame server_game;
        server_game.runGameServer(port);
    });
    server_thread.detach();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    player_id = server.run(single_game);
    Player& curr_player = map.getPlayer(player_id);
    while(window.isOpen()){
        sf::Event ev;
        while(window.pollEvent(ev)){
            if(ev.type == sf::Event::KeyPressed){
                auto& code = ev.key.code;
                if(direction_ids.count(code) > 0){
                    sendMove(direction_ids.at(code), curr_player, ev);
                }
            }else if(ev.type == sf::Event::Closed){
                window.close();
            }
        }
        readMessages();
        std::cout << "was check " << was_moved_curr << ' ' << new_msgs << ' ' << was_moved_prev << "\n";
        if(was_moved_curr || (!new_msgs && was_moved_prev)){
            curr_player.setState(Player::PlayerState::run);
        }else if(new_msgs || (!new_msgs && !was_moved_prev)){
            curr_player.setState(Player::PlayerState::still);
        }
        was_moved_prev = was_moved_curr;
        window.clear();
        map.print(window);
        window.display();
    }
    server.endConnection();
    return 0;
}
void ClientGame::mainWindow(){
    std::cout << "in main\n";
    sf::Text start_button = createButton("Start single game", sf::Color::Green, {windowWidth/2 - 100, windowHeight/2 - 10});
    sf::FloatRect start_button_pos = start_button.getGlobalBounds();
    sf::Text close_button = createButton("Exit", sf::Color::Red, {start_button_pos.left, start_button_pos.top + start_button_pos.height + 10});
    sf::FloatRect close_button_pos = close_button.getGlobalBounds();
    sf::Text join_button = createButton("Join game", sf::Color::Green, {close_button_pos.left, close_button_pos.top + close_button_pos.height + 10});

    std::cout << "buttons created\n";
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }else if(event.type == sf::Event::MouseButtonPressed){
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(start_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    gameWindow(true);
                }else if(close_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    window.close();
                }else if(join_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    gameWindow(false);
                }
            } 
        }
        window.clear(sf::Color(100, 100, 100));
        window.draw(start_button);
        window.draw(close_button);
        window.draw(join_button);
        window.display();

    }
}
void ClientGame::runGame(){
    auto mode = sf::VideoMode::getFullscreenModes().back();
    window.create(sf::VideoMode(mode.width, mode.height), "Main window");
    windowHeight = window.getSize().y;
    windowWidth = window.getSize().x;
    cellsWidth = 1 + windowWidth / Map::CellSize;
    cellsHeight = 1 + windowHeight / Map::CellSize;
    window.setFramerateLimit(30);
    loadTextures();
    std::cout << "textures loadd\n";
    mainWindow();
}
void ClientGame::loadTextures(){
    get_or_create_texture("green_block", "../fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "../fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "../fonts_textures/noblock_block.png");
    get_or_create_texture("yellow_block", "../fonts_textures/yellow_block.png");
    get_or_create_texture("water_block", "../fonts_textures/water_block.png");
    get_or_create_texture("return_button", "../fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "../fonts_textures/basicFont.otf");
    for(auto[img_name, num_imgs] : std::vector<std::pair<std::string, int>>{{"up", 3}, {"down", 4}, {"left", 4}, {"right", 4}}){
        sf::Image new_img;
        new_img.loadFromFile("../fonts_textures/player_" + img_name + ".png");
        sf::Color transparent_color = new_img.getPixel(1, 1);
        new_img.createMaskFromColor(transparent_color);
        transparent_color.a = 0;
        cut_image(new_img, img_name, num_imgs, transparent_color);
    }
    std::cout << "loading doen\n";
}
sf::Text ClientGame::createButton(std::string label, const sf::Color& outline_color, std::pair<float, float> left_up_cords, int thickness, sf::Font* font){
    sf::Text new_button(std::move(label), *font);
    new_button.setOutlineColor(outline_color);
    new_button.setOutlineThickness(thickness);
    new_button.setPosition(left_up_cords.first, left_up_cords.second);
    std::cout << "button created\n";
    return new_button;
}

}

/*
g++ -std=c++17 -c main.cpp client.cpp client_map.cpp server.cpp server_map.cpp declarations.cpp client_functions.cpp
g++ -std=c++17 main.o client.o client_map.o server.o server_map.o declarations.o client_functions.o -o sfml-app -lsfml-window -lsfml-system -lsfml-graphics -lsfml-network

*/