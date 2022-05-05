#ifndef LOAD_FONTS_H
#define LOAD_FONTS_H
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#define PI 3.14159265358979323846
inline sf::Font* get_or_create_font(const std::string& font_name, const std::string& path_to_font = ""){
    static std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    if(fonts.count(font_name) == 0){
        fonts[font_name] = std::make_unique<sf::Font>();
        if(!fonts[font_name]->loadFromFile(path_to_font)){
            throw std::runtime_error("Could not load font " + path_to_font);
        }
    }
    return fonts[font_name].get(); 
}
inline sf::Texture* get_or_create_texture(const std::string& texture_name, const std::string& path_to_texture = ""){
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    if(textures.count(texture_name) == 0){
        std::cout << "creating texture " << texture_name << std::endl;
        textures[texture_name] = std::make_unique<sf::Texture>();
        if(path_to_texture != "" && !textures[texture_name]->loadFromFile(path_to_texture)){
            throw std::runtime_error("Could not load " + path_to_texture);
        }
    }
    return textures[texture_name].get();
}
struct Block;
enum BlockType {green = 0, brick, water, noblock, used};
struct Player{
public:
    Player();
    std::pair<int, int> get_map_cords() const;
    std::pair<float, float> get_cords() const;
    float getSpeed() const;
    void updatePos(float delta_x, float delta_y, int direction_id_);
    enum PlayerState{still, run};
    void print(sf::RenderTarget& window);
    void setState(PlayerState new_state);
private: 
    float x, y;
    float speed = 1;
    std::vector<int> img_id = {0, 0, 0, 0};
    friend struct ClientGame;
    int direction = 0;
    PlayerState state;
    sf::Color player_color;
};
struct Block{
public:
     BlockType type;
public:
    Block() = default;
    Block(BlockType type_) : type(type_){}
};
struct Map{
public:
    static const int BlockSize;
    static const int CellSize;
    Map(){}
    Map(const std::string& file_name);    
    int create_player();
    Player& getPlayer(int player_id);

protected:
    friend struct ClientGame;
    friend struct ServerGame;
    std::vector<std::vector<Block>> field;
    int block_rows, block_cols;
    int cell_rows, cell_cols;
    std::vector<Player> players;
};
struct Direction{
    std::pair<int, int> delta;
    int id;
    std::string name;
    int max_img_num;
};
extern const std::vector<Direction> directions;
extern const std::unordered_map<sf::Keyboard::Key, int> direction_ids;


#endif
