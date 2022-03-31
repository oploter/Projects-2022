#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <cassert>
#include "client.h"
#include "server.h"
#include "declarations.h"
#include "client_map.h"
#include "cl_sr_queue.h"


int ClientGame::gameWindow(){
    map.loadMap("map.txt");
    std::thread srv_thread([this](){
        ServerGame srv;
        srv.runGameServer();
    });
    if(server.connect(ip, 12345) != sf::Socket::Done){
        return 1;
    }
    std::thread receive_thread([this](){
        ReceiveMessages();
    });

    sf::Text txt;
    txt.setFont(*get_or_create_font("basic_font"));
    txt.setString(std::to_string(map.player.x) + ',' + std::to_string(map.player.y));
    txt.setCharacterSize(10);
    txt.setPosition({windowWidth - txt.getGlobalBounds().width - 10, 10});

    bool was_moved_prev = false;
    map.player.x = Player::ps_x;
    map.player.y = Player::ps_y;
    while(window.isOpen()){
        sf::Event ev;
        while(window.pollEvent(ev)){
            if(ev.type == sf::Event::KeyPressed){
                auto& code = ev.key.code;
                if(move_deltas.count(code) > 0){
                    std::pair<float, float> delta = move_deltas.at(code);
                    delta.first *= map.player.speed;
                    delta.second *= map.player.speed;
                    if(ev.key.shift){
                        delta.first *= 1.5;
                        delta.second *= 1.5;
                    }
                    sf::Packet new_msg;
                    new_msg << 1 << delta.first << delta.second;
                    if(server.send(new_msg) != sf::Socket::Done){
                        return 1;
                    }
                }else if(code == sf::Keyboard::K){
                    float delta_x = static_cast<float>(sf::Mouse::getPosition(window).x)/Map::CellSize - map.player.x;
                    float delta_y = static_cast<float>(sf::Mouse::getPosition(window).y)/Map::CellSize - map.player.y;
                    sf::Packet new_msg;
                    new_msg << 2 << delta_x << delta_y;
                    if(server.send(new_msg) != sf::Socket::Done){
                        return 1;
                    }
                }else if(code == sf::Keyboard::T){
                    sf::Packet new_msg;
                    new_msg << 3;
                    if(server.send(new_msg) != sf::Socket::Done){
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
            int type;
            new_msg >> type;
            if(type == 1){
                float delta_x, delta_y;
                new_msg >> delta_x >> delta_y;
                map.player.updatePos(delta_x, delta_y);
                was_moved_curr = true;
            }else if(type == 2){
                map.bullets.push_back(std::vector<float>(5));
                auto& b = map.bullets.back();
                new_msg >> b[0] >> b[1] >> b[2] >> b[3] >> b[4];
            }else if(type == 3){
                int p_x, p_y;
                new_msg >> p_x >> p_y;
                map.plants.push_back({p_x, p_y});
                map.plants_cnts.push_back(0);
                map.field[p_y][p_x].type = BlockType::used;
            }
        }
        if(was_moved_curr || (!new_msgs && was_moved_prev)){
            map.player.state = Player::PlayerState::run;
        }else if(new_msgs || (!new_msgs && !was_moved_prev)){
            map.player.state = Player::PlayerState::still;
        }
        was_moved_prev = was_moved_curr;
        txt.setString(std::to_string(map.player.x) + ',' + std::to_string(map.player.y));

        window.clear();
        map.print(window);
        map.updateBullets(&window);
        map.updatePlants(&window);
        map.player.print(window);
        window.draw(txt);
        window.display();
    }
    receive_thread.join();
    srv_thread.join();
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


int find_first(sf::Image& img, int x_str, bool non_white){
    int y_n = img.getSize().y;
    while(x_str < img.getSize().x){
        bool chc = !non_white;
        for(int y = 0; y < y_n; y++){
            if(non_white){
                chc |= (img.getPixel(x_str, y) != sf::Color(255, 255, 255, 0));
            }else{
                chc &= (img.getPixel(x_str, y) == sf::Color(255, 255, 255, 0));
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

    Player::ps_x = 0;
    Player::ps_y = 0;
    sf::Image player_image;
    player_image.loadFromFile("fonts_textures/player.png");
    player_image.createMaskFromColor(player_image.getPixel(1, 1));
    int y_h = player_image.getSize().y;
    int prev = 0;
    for(int i = 0; i < 9; i++){
        int f_f_n = find_first(player_image, prev, true);
        int l_f_n = find_first(player_image, f_f_n, false);
        assert(f_f_n != -1 && l_f_n != -1);
        prev = l_f_n + 1;
        sf::Texture* texture = get_or_create_texture("player_" + std::to_string(i));
        texture->loadFromImage(player_image, {f_f_n, 0, l_f_n - f_f_n, y_h});
        Player::ps_x = std::max(Player::ps_x, static_cast<float>(texture->getSize().x) / (2 * 2 * Map::CellSize));
        Player::ps_y = std::max(Player::ps_y, static_cast<float>(texture->getSize().y) / (2 * 2 * Map::CellSize));
    }
}
