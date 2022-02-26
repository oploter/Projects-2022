#include <SFML/Graphics.hpp>
#include <exception>
#include <string>
#include <iostream>
#include "load_funcs.h"


bool run_game_cycle(sf::RenderWindow &window) {
    window.setTitle("Game cycle");
    sf::Font& font = get_font("basic");

    sf::Text textPlacement("MAP HERE", font);
    textPlacement.setPosition(sf::Vector2f(
        window.getSize().x / 2 - textPlacement.getLocalBounds().width / 2,
        window.getSize().y / 2 - textPlacement.getLocalBounds().height / 2));

    sf::Texture& returnButtonTexture = get_texture("returnButton");
    sf::Sprite returnButton;
    returnButton.setTexture(returnButtonTexture);
    returnButton.setScale(0.3, 0.3);

    bool returnButtonPressed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }else if(event.type == sf::Event::MouseButtonPressed){
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(returnButton.getGlobalBounds().contains(mouse_x, mouse_y)){
                    returnButtonPressed = true;
                }
            }
        }   
        if(returnButtonPressed){
            return 0;
        }
        window.clear();
        window.draw(textPlacement);
        window.draw(returnButton);
        window.display();
    }

  return 0;
}