#ifndef LOAD_FONTS_H
#define LOAD_FONTS_H


#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

inline sf::Font* get_or_create_font(const std::string& font_name, const std::string& path_to_font = ""){
    static std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    std::cout << "getting font " << font_name << ' ' << fonts.count(font_name) << std::endl;
    if(fonts.count(font_name) == 0){
        std::cout << "creating font " << font_name << std::endl;
        fonts[font_name] = std::make_unique<sf::Font>();
        if(!fonts[font_name]->loadFromFile(path_to_font)){
            throw std::runtime_error("Could not load font " + path_to_font);
        }
    }
    return fonts[font_name].get(); 
}
inline sf::Texture* get_or_create_texture(const std::string& texture_name, const std::string& path_to_texture = ""){
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::cout << "getting texture " << texture_name << ' ' << textures.count(texture_name) << std::endl;
    if(textures.count(texture_name) == 0){
        std::cout << "creating texture " << texture_name << std::endl;
        textures[texture_name] = std::make_unique<sf::Texture>();
        if(!textures[texture_name]->loadFromFile(path_to_texture)){
            throw std::runtime_error("Could not load " + path_to_texture);
        }
    }
    return textures[texture_name].get();

}

// MAP begin
struct Block;


enum PlantType {off = 0, deff, extra}; 

struct Player{
private:
    float x = 0;
    float y = 0; 
    sf::CircleShape circle;
    float speed = 1.5;
    static int Radius;
    bool dead = false;

public:
    friend struct Map;
    Player();
   // void move(const std::pair<int, int>& delta);
    std::pair<int, int> get_map_cords() const;
    void print(sf::RenderTarget& window) const;

    bool isDead() const;
};

// Map

struct Map{
public:
    enum BlockType {green = 0, brick, water, noblock, used};
    static const int BlockSize;
    static const int CellSize;

    Map(){}
    Map(int rows_, int cols_);
    Map(const std::string& file_name);
    //void loadMap(const std::string& file_name);

    //void checkPlayer(Player& player);
    //bool checkAndPlant(PlantType plantType, const std::pair<int, int>& map_cords);    


protected:
    std::vector<std::vector<Block>> field;
    Player player;
    int rows, cols;
};
// end Map

struct Block{
public:
    Map::BlockType type;

public:
    Block() = default;
    Block(Map::BlockType type_) : type(type_){}
};

struct Plant : Block{
private:
    static const int MaxTimeLimit = 100;
    static const int MaxHP = 100;
    int timeLimit;
    int hp;
    PlantType plantType;
public:
    Plant(PlantType plantType_) : Block(Map::used), hp(MaxHP), timeLimit(MaxTimeLimit), plantType(plantType_){}

};



#endif