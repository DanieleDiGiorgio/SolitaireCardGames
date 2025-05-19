#pragma once
#include <SFML/Graphics.hpp>
#include "../../game_logic.hpp"


class LacuneWindow {
public:
    LacuneWindow();

    void run();

private:
    sf::RenderWindow window;
    sf::View view;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    void handleInput(sf::Event event);
    void handleResize(unsigned int width, unsigned int height);
    void draw();

};