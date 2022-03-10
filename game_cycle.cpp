#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include "declarations.h"
#include "map.h"

int widthCells, heightCells;

// Player

Player::Player() : x(Radius / Map::CellSize), y(Radius / Map::CellSize), circle(20){
    circle.setFillColor(sf::Color::Red);
}

void Player::move(const std::pair<int, int>& delta){
    std::cout << "moving\n";
    float new_x = x + speed * delta.first;
    float new_y = y + speed * delta.second;
    std::cout << "new x " << new_x << " new_y " << new_y  << std::endl;
    if(new_x < 0 || widthCells < new_x || new_y < 0 || heightCells < new_y){
        return;
    }
    x = new_x;
    y = new_y;
    std::cout << "moved to " << x << ' ' << y << std::endl;
    circle.move(Map::CellSize * speed * delta.first, Map::CellSize * speed * delta.second);
}

void Player::print(sf::RenderTarget& window) const {
    window.draw(circle);
}

bool Player::isDead() const {
    return dead;
}

std::pair<int, int> Player::get_map_cords() const {
    return {(Map::CellSize * x) / Map::BlockSize, (Map::CellSize * y) / Map::BlockSize};
}

// Player end


std::unordered_map<sf::Keyboard::Key, std::pair<int, int>> deltas_wasd = {{sf::Keyboard::A, {-1, 0}}, {sf::Keyboard::W, {0, -1}}, {sf::Keyboard::D, {1, 0}}, {sf::Keyboard::S, {0, 1}}};
std::unordered_map<sf::Keyboard::Key, std::pair<int, int>> deltas_arrows = {{sf::Keyboard::Left, {-1, 0}}, {sf::Keyboard::Up, {0, -1}}, {sf::Keyboard::Right, {1, 0}}, {sf::Keyboard::Down, {0, 1}}};


int run_game_cycle(sf::RenderWindow &window) {
    std::cout << "windows size " << window.getSize().x << ' ' << window.getSize().y << std::endl;
    widthCells = window.getSize().x / Map::CellSize;
    heightCells = window.getSize().y / Map::CellSize;
    std::cout << "number of cells is " << widthCells << ' ' << heightCells << std::endl;
    window.setTitle("Game cycle");
    sf::Font* basic_font = get_or_create_font("basic_font");
    std::cout << "checking \n";
    get_or_create_texture("green_block", "fonts_textures/green_block.png");
    get_or_create_texture("brick_block", "fonts_textures/brick_block.png");
    get_or_create_texture("noblock_block", "fonts_textures/noblock_block.png");
    get_or_create_texture("water_block", "fonts_textures/water_block.png");
    get_or_create_texture("return_button", "fonts_textures/returnButton.png");
    get_or_create_font("basic_font", "fonts_textures/basicFont.otf");
    std::cout << "done checking\n";

    sf::Text textPlacement("MAP HERE", *basic_font);
    textPlacement.setPosition(sf::Vector2f(
        window.getSize().x / 2 - textPlacement.getLocalBounds().width / 2,
        window.getSize().y / 2 - textPlacement.getLocalBounds().height / 2));

    sf::Texture* returnButtonTexture = get_or_create_texture("return_button");
    sf::Sprite returnButton;
    returnButton.setTexture(*returnButtonTexture);
    returnButton.setScale(0.3, 0.3);


    Map map("map.txt");
    std::cout << "files_loaded\n";

    Player player;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }else if(event.type == sf::Event::KeyPressed){
                auto code = event.key.code;
                std::cout << "pressed at " << code << ' ' << sf::Keyboard::A << std::endl;
                bool d_a = deltas_arrows.count(code);
                bool d_w = deltas_wasd.count(code);
                if(d_a > 0 || d_w > 0){
                    std::cout << "player moving\n";
                    player.move((d_a > 0 ? deltas_arrows[code] : deltas_wasd[code]));
                    map.checkPlayer(player);
                    std::cout << "player moved\n";
                }
            }
        }   
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