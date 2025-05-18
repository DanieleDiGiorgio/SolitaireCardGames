#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(sf::RenderWindow& win);
    std::string run();

private:
    void draw();
    void handleMouseHover(const sf::Vector2f& mousePos);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void handleResize(unsigned int width, unsigned int height);

    sf::RenderWindow& window;
    sf::Font font;
    std::vector<std::string> options;
    std::vector<sf::Text> textOptions;
    int selectedIndex;
    std::string selectedGame = "";

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture titleTexture;
    sf::Sprite titleSprite;

    sf::Texture boxTexture_1;
    sf::Sprite boxSprite_1;

    sf::Texture boxTexture_2;
    sf::Sprite boxSprite_2;

    sf::View view;
};
