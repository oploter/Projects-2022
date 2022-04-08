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


ClientMap::ClientMap(const std::string& file_name) : Map(){
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
    //stateText.setPosition(window.getSize().x - stateText.getLocalBounds().width, Map::BlockSize);
    //window.draw(stateText);
    for(Player& plr : players){
        plr.print(window);
    }
}

void ClientMap::loadMap(const std::string& file_name){
    stateText.setFont(*get_or_create_font("basic_font"));
    stateText.setString("None");
    stateText.setPosition(BlockSize * block_cols - stateText.getLocalBounds().width, BlockSize * 2);

    std::ifstream file(file_name, std::ios_base::in);
    file >> block_cols >> block_rows;
    cell_cols = block_cols * (BlockSize / CellSize);
    cell_rows = block_rows * (BlockSize / CellSize);
    field.resize(block_rows, std::vector<Block>(block_cols, noblock));
    if(!rTexture.create(block_cols * BlockSize, block_rows * BlockSize)){
        return;
    }
    for(std::size_t row = 0; row < block_rows; row++){
        for(std::size_t col = 0; col < block_cols; col++){
            int el;
            file >> el;
            field[row][col] = static_cast<BlockType>(el);
            updateField(row, col);
        }
    }
    stateText.setFillColor(sf::Color::Red);
    stateText.setScale(0.9, 0.9);
    rTexture.display();
    create_player();
}