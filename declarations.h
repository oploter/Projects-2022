#ifndef LOAD_FONTS_H
#define LOAD_FONTS_H


#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

inline sf::Font* get_or_create_font(const std::string& font_name, const std::string& path_to_font = ""){
    static std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    //std::cout << "getting font " << font_name << ' ' << fonts.count(font_name) << std::endl;
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
    //std::cout << "getting texture " << texture_name << ' ' << textures.count(texture_name) << std::endl;
    if(textures.count(texture_name) == 0){
        std::cout << "creating texture " << texture_name << std::endl;
        textures[texture_name] = std::make_unique<sf::Texture>();
        if(path_to_texture != "" && !textures[texture_name]->loadFromFile(path_to_texture)){
            throw std::runtime_error("Could not load " + path_to_texture);
        }
    }
    return textures[texture_name].get();

}

const std::unordered_map<sf::Keyboard::Key, std::pair<int, int>> move_deltas = {{sf::Keyboard::W, {0, -1}}, {sf::Keyboard::A, {-1, 0}}, {sf::Keyboard::S, {0, 1}}, {sf::Keyboard::D, {1, 0}}};



// MAP begin
struct Block;
 
enum BlockType {green = 0, brick, water, noblock, used};

struct Player{
public:
    float x = 0;
    float y = 0;
    friend struct Map;
    Player();
   // void move(const std::pair<int, int>& delta);
    std::pair<int, int> get_map_cords() const;
    void print(sf::RenderTarget& window);
    void updatePos(float delta_x, float delta_y);
    bool isDead() const;
    enum PlayerState{still, run, dead};
private: 
    float speed = 1;
    int img_id = 0;
    std::pair<int, int> dir;
    friend struct ClientGame;
    PlayerState state;
};


struct Block{
public:
    BlockType type;

public:
    Block() = default;
    Block(BlockType type_) : type(type_){}
};

// Map

struct Map{
public:
    static const int BlockSize;
    static const int CellSize;

    Map() : player(){}
    Map(int rows_, int cols_);
    Map(const std::string& file_name);    
    Player player;

protected:
    friend struct ClientGame;
    std::vector<std::vector<Block>> field;
    int rows, cols;
};
// end Map



#endif