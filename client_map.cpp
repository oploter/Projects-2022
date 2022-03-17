#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include "client_map.h"
#include "declarations.h"

//Map

const std::unordered_map<BlockType, std::string> ClientMap::textureNames = {{green, "green_block"}, {brick, "brick_block"}, {water, "water_block"}, {noblock, "noblock_block"}, {used, "yellow_block"}};
const std::unordered_map<BlockType, std::string> ClientMap::blockState = {{green, "plant here"}, {brick, "cannot plant here"}, {water, "water"}, {used, "cannot plant here"}};


ClientMap::ClientMap(const std::string& file_name) : Map(file_name){
    loadMap(file_name);
}

void ClientMap::updateField(int row, int col, bool display){
    sf::Texture* texture = get_or_create_texture(textureNames.at(field[row][col].type));
    sf::Sprite s(*texture);
    s.setScale(0.5, 0.5);
    s.setPosition(col * BlockSize, row * BlockSize);
    rTexture.draw(s);
    if(display){
        rTexture.display();
    }
}

void ClientMap::print(sf::RenderTarget& window){
    sf::Sprite s(rTexture.getTexture());
    window.draw(s);
    stateText.setPosition(window.getSize().x - stateText.getLocalBounds().width, Map::BlockSize);
    window.draw(stateText);
}

void ClientMap::loadMap(const std::string& file_name){
    stateText.setFont(*get_or_create_font("basic_font"));
    stateText.setString("None");
    stateText.setPosition(BlockSize * cols - stateText.getLocalBounds().width, BlockSize * 2);

    std::ifstream file(file_name, std::ios_base::in);
    file >> cols >> rows;
    field.resize(rows, std::vector<Block>(cols, noblock));
    if(!rTexture.create(cols * BlockSize, rows * BlockSize)){
        return;
    }
    for(std::size_t row = 0; row < rows; row++){
        for(std::size_t col = 0; col < cols; col++){
            int el;
            file >> el;
            field[row][col] = static_cast<BlockType>(el);
            updateField(row, col);
        }
    }
    stateText.setFillColor(sf::Color::Red);
    stateText.setScale(0.9, 0.9);
    rTexture.display();
}