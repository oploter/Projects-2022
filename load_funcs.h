#ifndef LOAD_FONTS_H
#define LOAD_FONTS_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

sf::Font& load_new_font(const std::string& font_name, const std::string& path_to_font);
sf::Font& get_font(const std::string& font_name);
sf::Texture& get_texture(const std::string& texture_name);
sf::Texture& load_new_texture(const std::string& texture_name, const std::string& path_to_texture);


#endif