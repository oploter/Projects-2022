#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <string>
#include "game_cycle.h"
#include "load_funcs.h"

int windowWidth = 1400;
int windowHeight = 1200;

sf::Sprite createSprite(sf::Texture& text, const std::string& file_name){
    sf::Sprite new_sprite;
    text.loadFromFile(file_name);
    new_sprite.setTexture(text);
    std::cout << "New sprite created from " << file_name << " and has size of (" << new_sprite.getLocalBounds().width << ", " << new_sprite.getLocalBounds().height << ")\n";
    return std::move(new_sprite);  
}

int main(int argc, char * argv[]){
    std::cout << "got " << argc << " arguments\n";
    if(argc > 3){
        windowWidth = std::stoi(argv[1]);
        windowHeight = std::stoi(argv[2]);
    }
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Main window");



    sf::Font& basic_font = load_new_font("basic", "LEMONMILK-Light.otf");

    sf::Text start_button("Start game", basic_font);
    start_button.setOutlineColor(sf::Color::Green);
    start_button.setOutlineThickness(1);
    start_button.setPosition(sf::Vector2f(windowWidth/2 - (start_button.getLocalBounds().width)/2, windowHeight/2 - (start_button.getLocalBounds().height)/2));

    sf::RectangleShape close_button;
    close_button.setSize(sf::Vector2f(120, 60));
    close_button.setPosition(sf::Vector2f(windowWidth/2 - (start_button.getLocalBounds().width)/2, windowHeight/2 + 100 - (start_button.getLocalBounds().height)/2));
    close_button.setFillColor(sf::Color::Red);

    load_new_texture("returnButton", "return-button-png-34581.png");

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            std::cout << "new event : " << (event.type == sf::Event::KeyPressed) << std::endl;
            if(event.type == sf::Event::Closed){
                window.close();
            }else if(event.type == sf::Event::KeyPressed){
                std::cout << "button pressed " << (event.key.code == sf::Keyboard::A) << std::endl;
            }else if(event.type == sf::Event::MouseButtonPressed){
                std::cout << "mouse button pressed at (" << event.mouseButton.x << ' ' << event.mouseButton.y << std::endl;
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                if(start_button.getGlobalBounds().contains(mouse_x, mouse_y)){
                    std::cout << "START BUTTON PRESSED\n";
                    run_game_cycle(window);
                }
            } 
        }
        window.clear(sf::Color(100, 100, 100));

        window.draw(start_button);
        window.draw(close_button);

        window.display();
    }


}




/*
g++ -c main.cpp game_cycle.cpp load_funcs.cpp
g++ main.o game_cycle.o load_funcs.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system




window.display() -- отобразить все новое
window.clear() -- очистить фон
window.clear(Color) -- очистить фон и заменить на новый цвет // RGB alpha
window.draw() -- рисует переданный объект
RenderTexture === то же самое, что RenderWindow, но рисует в текстуру, в не в окно(RenderTarget -- общий базовый класс)
Rectangular entity + Image == Sprite
Чтобы загрузить картинку, используем sf::Texture(большинтсво методов - загрузить или обновить )
sf::Texture txt;
txt.loadFromFile();
txt.loadFromStream();
txt.loadFromImage(); // load image from sf::Image
texture.loadFromFile("image.png", sf::IntRect(10, 10, 32, 32)) // cut the picture
// create an empty 200x200 texture
texcture.create(200, 200)
//Ti update pixels -- use update function
sf::RenderWindow window;
txt.update(window) // put current contents of screen to texture

sf::Sprite sprite;
sprite.setTexture(texture);
window.clear();
...
window.draw(sprite);
...
window.diplay();

sf::Sprite sprite;
sprite.setTexture(texture);
window.draw(sprite);

sprite.setTetureRect(sf::IntRect(lhs_x, lhs_y, width, height));
sprite.setColor(sf::Color(0, 255, 0));
sprite.setPosition(sf::Vector2f(100, 100)); // absolute position
sprite.move(sf::Vector2f(10, 100)); // move from current position
sprite.setRotation(90); // absolute
sprite.rotate(90); // from current angle
sprite.setScale(1.2); // absolute
sprtie.scale(1.4); // from current scale (1.2 * 2.4)

sf::Sprite load_sprite(){
    sf::Texture textrue;
    texture.loadFromFile("file.name");
    return sf::Sprite(texture);
} // texture is deleted here


Sprite хранит только ссылку на Texture, поэтому необходимо, чтобы lifetime текстуры не был меньше lifetime sprite

*/
