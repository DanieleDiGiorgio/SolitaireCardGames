#pragma once
#include <SFML/Graphics.hpp>
#include "../../game_logic.hpp"

enum class GamePhase {
    WaitingForRoundStart,  // waiting to flip next card from round deck
    Playing,               // currently resolving a round
    GameWon,
    GameLost
};

class LacuneWindow {
public:
    LacuneWindow();

    bool run();

private:
    sf::RenderWindow window;
    sf::View view;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backCardTexture;
    sf::Sprite backCardSprite;

    std::vector<Card> deck;
    size_t currentIndex;

    std::vector<sf::Texture> symbolTextures;  // One texture per symbol
    std::vector<sf::Sprite> symbolSprites;    // One sprite per symbol

    sf::RectangleShape placeholderShadow;

    bool goBackToMenu = false;

    sf::Texture backButtonTexture;
    sf::Sprite backButtonSprite;

    GamePhase currentPhase;

    int currentRoundIndex; 
    Card currentCard; 
    bool hasCurrentCard; 
    std::vector<Card> roundDeck; 
    std::vector<Card> boardCards; 
    std::vector<std::vector<Card>> boardState; 

    std::vector<Card> kingsFound;

    std::vector<std::vector<bool>> revealedCards;

    sf::Texture currentCardTexture;
    sf::Sprite currentCardSprite;

    std::map<std::string, sf::Texture> cardTextureCache;

    void handleInput(sf::Event event);
    void handleResize(unsigned int width, unsigned int height);
    void draw();
    //void loadCardImage(const Card& card);
    void resetGame();
    int getSuitIndex(const std::string& suit);
    void flipNextRoundCard();
    sf::Texture& getCardTexture(const std::string& imagePath);
};