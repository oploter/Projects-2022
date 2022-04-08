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


int ClientGame::gameWindow(bool single_game){
    if(single_game){
        map.loadMap("map.txt");
        // connect
        std::thread srv_thread([](){
        ServerGame srv;
        srv.runGameServer();
        });
        srv_thread.detach();
    }
    if(server.connect(ip, 12345) != sf::Socket::Done){
        return 1;
    }
    std::cout << "connected\n";
    std::thread receive_thread([this](){
        ReceiveMessages();
    });

    if(single_game){
        player_id = 0;
    }else{
        msg << 2;
        if(server.send(msg) != sf::Socket::Done){
            return 1;
        }
        sf::Packet new_msg = std::move(Q.pop());
        new_msg >> player_id;
    }
    Player& curr_player = map.players[player_id];
    // end connect

    bool was_moved_prev = false;
    while(window.isOpen()){
        sf::Event ev;
        int num_buf;
        while(window.pollEvent(ev)){
            if(ev.type == sf::Event::KeyPressed){
                auto& code = ev.key.code;
                if(move_deltas.count(code) > 0){
                    std::pair<float, float> delta = move_deltas.at(code);
                    delta.first *= curr_player.speed;
                    delta.second *= curr_player.speed;
                    if(ev.key.shift){
                        delta.first *= 1.5;
                        delta.second *= 1.5;
                    }
                    msg.clear();
                    msg << player_id << delta.first << delta.second;

                    if(server.send(msg) != sf::Socket::Done){
                        return 1;
                    }
                }
            }else if(ev.type == sf::Event::Closed){
                window.close();
            }
        }
        bool new_msgs = false;
        bool was_moved_curr = false;
        while(!Q.empty()){
            new_msgs = true;
            sf::Packet new_msg = std::move(Q.pop());
            float delta_x, delta_y;
            int player_id_;
            new_msg >> player_id_ >> delta_x >> delta_y;
            map.players[player_id_].updatePos(delta_x, delta_y);
            if(player_id_ == player_id){
                was_moved_curr = true;
            }
        }
        if(was_moved_curr || (!new_msgs && was_moved_prev)){
            curr_player.state = Player::PlayerState::run;
        }else if(new_msgs || (!new_msgs && !was_moved_prev)){
            curr_player.state = Player::PlayerState::still;
        }
        was_moved_prev = was_moved_curr;

        window.clear();
        map.print(window);
        window.display();
    }
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
    //mainWindow();
    gameWindow();
}

void ClientGame::ReceiveMessages(){
    while(1){
        sf::Packet new_packet; 
        if(server.receive(new_packet) != sf::Socket::Done){
            return;
        }
        Q.push(std::move(new_packet));
    }
}

ClientGame::ClientGame() : ip("127.0.0.1"){}

int find_first(sf::Image& img, int x_str, bool non_white, const sf::Color& color){
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
void ClientGame::loadTextures(){
    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("yellow_block", "fonts_textures/yellow_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");

    sf::Image player_image;
    player_image.loadFromFile("fonts_textures/player_.png");
    //player_image.createMaskFromColor(player_image.getPixel(2, 2));
    int prev = 0;
    int y_h = player_image.getSize().y;
    sf::Color trnsprnt_color = player_image.getPixel(2, 2);
    for(int i = 0; i < 8; i++){
        int str_i = find_first(player_image, prev, true, trnsprnt_color);
        int end_i = find_first(player_image, str_i + 1, false, trnsprnt_color);
        prev = end_i + 1;
        sf::Texture* texture = get_or_create_texture("player_" + std::to_string(i));
        texture->loadFromImage(player_image, {str_i, 0, end_i - str_i, y_h});
    }
}
