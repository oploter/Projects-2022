#ifndef LOAD_FONTS_H
#define LOAD_FONTS_H


#include <unordered_map>
#include <string>
#include <memory>
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


int run_game_cycle(sf::RenderWindow& window);

struct Player{
private:
    float x = 0;
    float y = 0; 
    sf::CircleShape circle;
    const float speed = 1.5;
    static const int Radius = 20;
    bool dead = false;

public:
    friend struct Map;
    Player();
    void move(const std::pair<int, int>& delta);
    std::pair<int, int> get_map_cords() const;
    void print(sf::RenderTarget& window) const;

    bool isDead() const;
};

#endif