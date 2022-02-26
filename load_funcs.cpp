#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <exception>

std::unordered_map<std::string, sf::Font> fonts;
std::unordered_map<std::string, sf::Texture> textures;

sf::Font& get_font(const std::string& font_name){
    return fonts[font_name];
}

sf::Font& load_new_font(const std::string& font_name, const std::string& path_to_font){
    sf::Font& new_font = fonts[font_name];
    if(!new_font.loadFromFile(path_to_font)){
        throw std::runtime_error("Could not load font from '" + path_to_font + "'");
    }
    return new_font;
}

sf::Texture& get_texture(const std::string& texture_name){
    return textures[texture_name];
}

sf::Texture& load_new_texture(const std::string& texture_name, const std::string& path_to_texture){
    sf::Texture& new_texture = textures[texture_name];
    if(!new_texture.loadFromFile(path_to_texture)){
        throw std::runtime_error("Could not load texture from '" + path_to_texture + "'");
    }
    return new_texture;
}

