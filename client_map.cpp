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
        std::cout << "Couldnot create texture\n";
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
    std::cout << "done dis\n";
    rTexture.display();
}

//void Map::checkPlayer(Player& player){
//    std::pair<int, int> player_map_cords = player.get_map_cords();
//    std::cout << "Player is at " << player_map_cords.first << ' ' << player_map_cords.second << std::endl;
//    BlockType pot = field[player_map_cords.second][player_map_cords.first].type;
//    std::cout << "Player is on " << pot << std::endl;
//    if(pot == water){
//        player.dead = true;
//    }
//    stateText.setString(blockState.at(pot));
//    stateText.setPosition(BlockSize * cols - 2 * BlockSize - stateText.getLocalBounds().width, BlockSize);
//}


//bool Map::checkAndPlant(PlantType plantType, const std::pair<int, int>& map_cords){
//    Block& block = field[map_cords.second][map_cords.first];
//    if(block.type != green){ 
//        std::cout << "didnt plant " << map_cords.first << ' ' << map_cords.second << std::endl; 
//        return false;
//    }
//    block = Plant(plantType);
//    std::cout << "planted " << map_cords.first << ' ' << map_cords.second << std::endl;
//    fieldUpdate(map_cords.second, map_cords.first, true);
//    return true;
//}