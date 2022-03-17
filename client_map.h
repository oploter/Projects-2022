#ifndef CLIENT_MAP_H
#define CLIENT_MAP_H

#include <vector>
#include <unordered_map>
#include <string>
#include "declarations.h"
#include <SFML/Graphics.hpp>


struct ClientMap : Map{
private:
    sf::RenderTexture rTexture;
    sf::Text stateText;

public:
    ClientMap(const std::string& file_name);
    ClientMap(){}

    static const std::unordered_map<BlockType, std::string> textureNames;  
    static const std::unordered_map<BlockType, std::string> blockState;
    void updateField(int row, int col, bool display = false);
    void print(sf::RenderTarget& target);
    void loadMap(const std::string& file_name);

};


#endif