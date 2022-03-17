#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include "client.h"
#include "server.h"
#include "declarations.h"
#include "client_map.h"
#include "cl_sr_queue.h"


int ClientGame::gameWindow(){

    map.loadMap("map.txt");

    // connect
    if(server.connect(ip, 12345) != sf::Socket::Done){
        std::cout << "cant servevr!!!!\n";
        return 1;
    }
    std::thread send_thread([this](){
        SendMessages();
    });
    std::thread receive_thread([this](){
        ReceiveMessages();
    });
    // end connect

    while(window.isOpen()){
        sf::Event ev;
        int num_buf;
        while(window.pollEvent(ev)){
            if(ev.type == sf::Event::KeyPressed){
                auto& code = ev.key.code;
                if(move_deltas.count(code) > 0){
                    const std::pair<int, int>& delta = move_deltas.at(code);
                    sf::Packet new_msg;
                    new_msg << 1 << delta.first << delta.second;
                    Q_OUT.push(std::move(new_msg));
                }
            }else if(ev.type == sf::Event::Closed){
                window.close();
            }
        }

        while(!Q_IN.empty()){
            sf::Packet new_msg = std::move(Q_IN.pop());
            int type;
            new_msg >> type;
            if(type == 1){
                int delta_x, delta_y;
                new_msg >> delta_x >> delta_y;
                map.player.updatePos(delta_x, delta_y);
            }
        }

        window.clear();
        map.print(window);
        map.player.print(window);
        window.display();
    }

    send_thread.join();
    receive_thread.join();
    return 0;
}


void ClientGame::mainWindow(){
    sf::Font* basic_font = get_or_create_font("basic_font");

    sf::Text start_button("Start game", *basic_font);
    start_button.setOutlineColor(sf::Color::Green);
    start_button.setOutlineThickness(1);
    start_button.setPosition(sf::Vector2f(windowWidth/2 - (start_button.getLocalBounds().width)/2, windowHeight/2 - (start_button.getLocalBounds().height)/2));

    sf::Text close_button("Exit", *basic_font);
    close_button.setOutlineColor(sf::Color::Red);
    close_button.setOutlineThickness(1);
    sf::FloatRect start_button_pos = start_button.getGlobalBounds();
    close_button.setPosition(start_button_pos.left, start_button_pos.top + start_button_pos.height + 10);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }else if(event.type == sf::Event::MouseButtonPressed){
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(start_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    gameWindow();
                }else if(close_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    window.close();
                }
            } 
        }

        window.clear(sf::Color(100, 100, 100));

        window.draw(start_button);
        window.draw(close_button);

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
    mainWindow();
}

void ClientGame::SendMessages(){
    while(1){
        sf::Packet new_msg = std::move(Q_OUT.pop());
        if(server.send(new_msg) != sf::Socket::Done){
            return;
        }
    }
}

void ClientGame::ReceiveMessages(){
    while(1){
        sf::Packet new_packet; 
        if(server.receive(new_packet) != sf::Socket::Done){
            return;
        }
        Q_IN.push(std::move(new_packet));
    }
}

ClientGame::ClientGame() : ip("127.0.0.1"){}


void ClientGame::loadTextures(){
    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("yellow_block", "fonts_textures/yellow_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");

    sf::Image player_image;
    player_image.loadFromFile("fonts_textures/player.png");
    player_image.createMaskFromColor(player_image.getPixel(2, 2));
    for(int i = 0; i < 8; i++){
        int l_x = i * 102.5 + 20;
        int r_x = l_x + 82.5;
        sf::Texture* img_texture = get_or_create_texture("player_" + std::to_string(i));
        img_texture->loadFromImage(player_image, {l_x, 2, r_x - l_x, 142});
    }
}