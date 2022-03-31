#include "declarations.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <SFML/Graphics.hpp>


const int Map::BlockSize = 50;
const int Map::CellSize = 10;
const float Map::HalfBlockSize = 20;



Map::Map(const std::string& file_name) : player(), plant_r(50.0 / CellSize){ 
    std::ifstream file(file_name);
    file >> block_cols >> block_rows;
    field.resize(block_rows, std::vector<Block>(block_cols));
    for(std::size_t row = 0; row < block_rows; row++){
        for(std::size_t col = 0; col < block_cols; col++){
            int el;
            file >> el;
            field[row][col] = static_cast<BlockType>(el);
        }
    }
    cell_rows = block_rows * BlockSize / CellSize;
    cell_cols = block_cols * BlockSize / CellSize;
}


void Map::updateBullets(sf::RenderTarget* window){
    for(int i = 0; i < bullets.size(); i++){
        sf::RectangleShape shp({15, 10});
        auto& b = bullets[i];
        b[0] += b[2];
        b[1] += b[3];
        if(b[0] < 0 || b[0] > cell_cols || b[1] < 0 || b[1] > cell_rows){
            bullets.erase(bullets.begin() + i);
            i--;
        }
        float delta_x = (b[0] - player.x);
        float delta_y = (b[1] - player.y);
        if(delta_x * delta_x - delta_y * delta_y < 4){
            player.hp--;
        }
        if(window != nullptr){
            shp.setPosition(b[0] * CellSize, b[1] * CellSize);
            shp.setRotation(b[4]);
            window->draw(shp);
        }
    }   
}

float Map::map_to_pl(int cord){
    return ((cord * BlockSize) + HalfBlockSize) / CellSize;
}

void Map::updatePlants(sf::RenderTarget* window){
    for(int i = 0; i < plants.size(); i++){
        auto& p = plants[i];
        auto& p_cnts = plants_cnts[i];
        float d_x = (player.x - map_to_pl(p.first));
        float d_y = (player.y - map_to_pl(p.second));
        if(d_x * d_x + d_y * d_y <= plant_r * plant_r && p_cnts == 0){
            float angle = (d_x == 0 ? (PI/2 * (d_y < 0 ? -1 : 1)) : std::atan(d_y / d_x));
            if(d_x < 0){
                angle += PI;
            }
            bullets.push_back({(p.first * BlockSize + HalfBlockSize) / CellSize, (p.second * BlockSize + HalfBlockSize) / CellSize, std::cos(angle), std::sin(angle), static_cast<float>(angle * 180 / PI)});
        }
        p_cnts++;
        if(p_cnts > 10){
            p_cnts = 0;
        }
        if(window != nullptr){
            sf::RectangleShape rsh1({12, 12});
            rsh1.setFillColor(sf::Color(100, 255, 0));
            rsh1.setOrigin({6, 6});
            rsh1.setPosition({p.first * BlockSize + HalfBlockSize, p.second * BlockSize + HalfBlockSize});
            window->draw(rsh1);
        }
    }
}



// Player
float Player::ps_x = 0;
float Player::ps_y = 0;

Player::Player() : state(still), x(Player::ps_x), y(Player::ps_y), dir(move_deltas.at(sf::Keyboard::D)){}

std::pair<int, int> Player::get_map_cords() const {
    return {x * Map::CellSize / Map::BlockSize, y * Map::CellSize / Map::BlockSize};
}

void Player::print(sf::RenderTarget& window){
    sf::Sprite s;
    if(state == still){
        s.setTexture(*get_or_create_texture("player_0"));
    }else if(state == run){
        s.setTexture(*get_or_create_texture("player_" + std::to_string(img_id)));
        img_id++;
        if(img_id > 8){
            img_id = 0;
        }   
    }
    s.setScale(0.5, 0.5);
    window.draw(s);
    sf::RectangleShape rsh({15, 15});
    rsh.setFillColor(sf::Color::Blue);
    rsh.setOrigin({7.5, 7.5});
    rsh.setPosition(x * Map::CellSize, y * Map::CellSize);
    //window.draw(rsh);
}

void Player::updatePos(float delta_x, float delta_y){
    x += delta_x;
    y += delta_y;
    dir = {delta_x, delta_y};
}

// End Player
