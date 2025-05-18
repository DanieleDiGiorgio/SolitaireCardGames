#pragma once
#include <SFML/Graphics.hpp>
#include "../../game_logic.hpp"

enum class GameState {
    WaitingToStart,
    Playing,
    Lost,
    Won
};

class OneTwoThreeWindow {
public:
    OneTwoThreeWindow();

    void run();

private:
    sf::RenderWindow window;
    sf::View view;

    std::vector<Card> deck;
    size_t currentIndex;
    int call;
    int cardsPlayed;
    GameState gameState;

    sf::Font font;
    sf::Text infoText;
    //sf::Text counterText;
    sf::Text conteggioText;

    sf::Texture cardTexture;
    sf::Sprite cardSprite;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    //sf::Texture counterBoxTexture;
    //sf::Sprite counterBoxSprite;

    sf::Sprite digitSpriteLeft;
    sf::Sprite digitSpriteRight;
    std::vector<sf::Texture> digitTextures; // For 0–9

    void loadCardImage(const Card& card);
    void draw();
    void handleInput(sf::Event event);
    void resetGame();
    void handleResize(unsigned int width, unsigned int height);

};
