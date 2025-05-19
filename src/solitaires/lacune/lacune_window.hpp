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

    std::vector<sf::Texture> cardTextures;  // One texture per card
    std::vector<sf::Sprite> cardSprites;    // One sprite per card

    sf::Texture backCardTexture;
    sf::Sprite backCardSprite;

    std::vector<Card> deck;
    size_t currentIndex;

    std::vector<sf::Texture> symbolTextures;  // One texture per symbol
    std::vector<sf::Sprite> symbolSprites;    // One sprite per symbol

    sf::RectangleShape placeholderShadow;

    void handleInput(sf::Event event);
    void handleResize(unsigned int width, unsigned int height);
    void draw();
    //void loadCardImage(const Card& card);
    void resetGame();
};