#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include "declarations.h"
#include "map.h"



int windowWidth, windowHeight;

sf::Sprite createSprite(sf::Texture& text, const std::string& file_name){
    sf::Sprite new_sprite;
    text.loadFromFile(file_name);
    new_sprite.setTexture(text);
    std::cout << "New sprite created from " << file_name << " and has size of (" << new_sprite.getLocalBounds().width << ", " << new_sprite.getLocalBounds().height << ")\n";
    return new_sprite;  
}

// void create_map_file(const std::string& file_name){
//     int rows = windowHeight / Map::blockSize + 1;
//     int cols = windowWidth / Map::blockSize + 1;
//     std::ofstream file(file_name, std::ios_base::out);
//     file << rows << ' ' << cols << "\n";
//     for(int row = 0; row < rows; row++){
//         file << 0;
//         for(int col = 1; col < cols; col++){
//             file << ' ' << 0; 
//         }
//         file << "\n";
//     }
//     std::cout << "created file " << rows << ' ' << cols << std::endl;
// }   

int main(int argc, char * argv[]){
    std::cout << "got " << argc << " arguments\n";
    if(argc > 3){
        windowWidth = std::stoi(argv[1]);
        windowHeight = std::stoi(argv[2]);
    }else{
        std::vector<sf::VideoMode> video_modes = sf::VideoMode::getFullscreenModes();
        windowWidth = video_modes.back().width;
        windowHeight = video_modes.back().height;
    }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Main window");
    window.setFramerateLimit(60);
    std::cout << "window size " << windowWidth << ' ' << windowHeight << std::endl;
    //create_map_file("map.txt");


    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");

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
            }else if(event.type == sf::Event::KeyPressed){
                std::cout << "button pressed " << (event.key.code == sf::Keyboard::A) << std::endl;
            }else if(event.type == sf::Event::MouseButtonPressed){
                std::cout << "mouse button pressed at (" << event.mouseButton.x << ' ' << event.mouseButton.y << ")\n";
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(start_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    std::cout << "START BUTTON PRESSED\n";
                    state = run_game_cycle(window);
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