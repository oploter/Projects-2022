#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "client.h"
#include "declarations.h"
#include "client_map.h"
#include "cl_sr_queue.h"


int ClientGame::gameWindow(){
    std::cout << "GAME WINDOW START\n";
    std::cout << "checking \n";
    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_texture("yellow_block", "fonts_textures/yellow_block.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");
    std::cout << "done checking\n";

    window.setTitle("Game cycle");
    std::cout << "before laosdonf\n";
    map.loadMap("map.txt");
    std::cout << "after loasdnkf\n";
    player = Player();

    const std::unordered_map<sf::Keyboard::Key, std::pair<int, int>> key_codes{{sf::Keyboard::W, {0, -1}}, {sf::Keyboard::A, {-1, 0}}, {sf::Keyboard::S, {0, 1}}, {sf::Keyboard::D, {1, 0}}};
    int msgs = 0;
    while(window.isOpen()){
        sf::Event ev;
        while(window.pollEvent(ev)){
            if(ev.type == sf::Event::Closed){
                window.close();
            }
            if(ev.type == sf::Event::KeyPressed){
                auto code = ev.key.code;
                auto& q = queue_for_ser();
                if(code == sf::Keyboard::K){
                    //plant
                    std::cout << "plant " << q.size() << std::endl;
                    q.push({2, player.get_map_cords()});
                    msgs++;
                    std::cout << "plant\n";
                }else if(key_codes.count(code) > 0){
                    std::cout << "move {" << key_codes.at(code).first << ' ' << key_codes.at(code).second << "}\n";
                    q.push({1, key_codes.at(code)});
                    msgs++;
                }
            }
        }

        auto& from_srvr = queue_for_cl();

        if(player.isDead()){
            return 1;
        }

        window.clear();
        map.print(window);
        player.print(window);
        window.display();
    }
    return 0;
}


void ClientGame::mainWindow(){
    std::cout << "MAIN WINDOW START\n";
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

    sf::Text lose_text("You died", *basic_font);
    lose_text.setFillColor(sf::Color::Red);
    lose_text.setScale(0.4, 0.4);
    lose_text.setPosition(windowWidth/2 - lose_text.getLocalBounds().width/2, windowHeight - lose_text.getLocalBounds().height - 10);

    int state = 0;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }else if(event.type == sf::Event::MouseButtonPressed){
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(start_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    std::cout << "START BUTTON PRESSED\n";
                    state = gameWindow();
                    window.setTitle("Main window");
                }else if(close_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    window.close();
                }
            } 
        }

        window.clear(sf::Color(100, 100, 100));

        if(state == 1){
            window.draw(lose_text);
        }
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
    std::cout << "window " << windowWidth << ' ' << windowHeight << std::endl;
    cellsWidth = 1 + windowWidth / Map::CellSize;
    cellsHeight = 1 + windowHeight / Map::CellSize;
    window.setFramerateLimit(60);
    loadTextures();
    mainWindow();
}


void ClientGame::loadTextures(){
    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("yellow_block", "fonts_textures/yellow_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");
}