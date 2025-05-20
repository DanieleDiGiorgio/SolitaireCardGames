#include "lacune_window.hpp"
#include <iostream>

LacuneWindow::LacuneWindow() : window(sf::VideoMode(900, 600), "Lacune"), currentRoundIndex(-1), hasCurrentCard(false) {
    // Initialize any assets, cards, or layout here
    if (!backgroundTexture.loadFromFile("assets/backgrounds/green_felt_background_scale.png")) {
        std::cerr << "Failed to load background image" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    symbolTextures.resize(4);
    symbolSprites.resize(4);
    
    float symbTargetHeight = 50.f;
    float symbMargin = 70.f;
    float symbStartX = 20.f;
    float symbStartY = 90.f;
    int symbPerRow = 1;
    std::vector<std::string> suits = {"ori", "coppe", "spade", "bastoni"};

    for (size_t i = 0;i<4; ++i) {
        std::string imagePath = "assets/symbols/" + suits[i]+ "_color.png";
        if (!symbolTextures[i].loadFromFile(imagePath)) {
            std::cerr << "Failed to load " << imagePath << std::endl;
            continue;
        }

        symbolSprites[i].setTexture(symbolTextures[i]);

        float scale = symbTargetHeight / static_cast<float>(symbolTextures[i].getSize().y);
        symbolSprites[i].setScale(scale, scale);

        float scaledWidth = symbolTextures[i].getSize().x * scale;

        int row = i / symbPerRow;
        int col = i % symbPerRow;

        float posX = symbStartX + col * (scaledWidth + symbMargin);
        float posY = symbStartY + row * (symbTargetHeight + symbMargin);

        if(i==0){
            symbolSprites[i].setPosition(posX-3.f, posY);
        }else if(i==2){
            symbolSprites[i].setPosition(posX+2.f, posY);
        }else{
            symbolSprites[i].setPosition(posX, posY);
        }
        
    }

    if (!backButtonTexture.loadFromFile("assets/symbols/back_button.png")) {
        std::cerr << "Failed to load back button image\n";
    }
    backButtonSprite.setTexture(backButtonTexture);
    float backButtonTargetHeight = 50.f;
    float backButtonScale = backButtonTargetHeight / backButtonTexture.getSize().y;
    backButtonSprite.setScale(backButtonScale, backButtonScale);
    sf::FloatRect backButtonBounds = backButtonSprite.getGlobalBounds();
    backButtonSprite.setPosition(1.f, 10.f);

    // Initialize the game state
    resetGame();

}

int LacuneWindow::getSuitIndex(const std::string& suit) {
    if (suit == "Ori") return 0;
    if (suit == "Coppe") return 1;
    if (suit == "Spade") return 2;
    if (suit == "Bastoni") return 3;
    return -1; // error
}


void LacuneWindow::resetGame() {
    deck = generateDeck();
    shuffleDeck(deck);

    cardTextureCache.clear();
    boardCards.clear();
    roundDeck.clear();
    boardState.assign(4, std::vector<Card>(9)); // 4 rows of 9 cards
    revealedCards.assign(4, std::vector<bool>(9, false));
    kingsFound.clear();

    currentRoundIndex = -1;
    currentPhase = GamePhase::WaitingForRoundStart;
    hasCurrentCard = false;

    //place the first 36 cards
    int cardIndex = 0;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 9; ++col) {
            const Card& c = deck[cardIndex++];
            boardState[row][col] = c;        
            boardCards.push_back(c); // Optional: keep also a flat list
        }
    }

    // Remaining 4 go into the round deck
    for (int i = 36; i < 40; ++i) {
        roundDeck.push_back(deck[i]);
    }

    float targetHeight = 110.f;
    float placeHolderWidth=69.0062;

    //load the back of the card for the pile of 4 cards
    std::string imagePath = "assets/cards/small/retro_carte.png";    
    if (!backCardTexture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load " << imagePath << std::endl;
    }
    float scale = targetHeight / static_cast<float>(backCardTexture.getSize().y);
    backCardSprite.setScale(scale, scale);
    backCardSprite.setTexture(backCardTexture);
    backCardSprite.setPosition(800.f, 400.f);

    //create the placeholder for the kings stack
    placeholderShadow.setSize(sf::Vector2f(placeHolderWidth, targetHeight));
    placeholderShadow.setFillColor(sf::Color(0, 0, 0, 50));
    placeholderShadow.setOutlineColor(sf::Color::Black);    
    placeholderShadow.setOutlineThickness(1.f);             
    placeholderShadow.setPosition(800.f, 70.f);
}

void LacuneWindow::flipNextRoundCard() {
    if (currentRoundIndex >= 3) return; // No more cards
    currentRoundIndex++;

    if (currentRoundIndex < roundDeck.size()) {
        currentCard = roundDeck[currentRoundIndex];
        hasCurrentCard = true;

        std::string imagePath = "assets/cards/small/" + std::to_string(currentCard.number) + "_" + currentCard.suit + ".png";

        if (!currentCardTexture.loadFromFile(imagePath)) {
            std::cerr << "Failed to load flipped round card: " << imagePath << std::endl;
        } else {
            currentCardSprite.setTexture(currentCardTexture);
            float scale = 110.f / currentCardTexture.getSize().y;
            currentCardSprite.setScale(scale, scale);
            currentCardSprite.setPosition(800.f, 270.f);

            if (currentCard.number == 10) {
                kingsFound.push_back(currentCard);
                hasCurrentCard = false;
                currentPhase = currentRoundIndex < 3 ? GamePhase::WaitingForRoundStart : GamePhase::GameLost;
            } else {
                currentPhase = GamePhase::Playing;
            }
        }
    }
}


void LacuneWindow::handleInput(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    } else if (event.type == sf::Event::Resized) {
        handleResize(event.size.width, event.size.height);
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
        resetGame();
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i rawMousePos(event.mouseButton.x, event.mouseButton.y);
        sf::FloatRect vp = view.getViewport();

        float vpLeftPx = vp.left * window.getSize().x;
        float vpTopPx = vp.top * window.getSize().y;
        float vpWidthPx = vp.width * window.getSize().x;
        float vpHeightPx = vp.height * window.getSize().y;

        if (rawMousePos.x >= vpLeftPx && rawMousePos.x <= vpLeftPx + vpWidthPx &&
            rawMousePos.y >= vpTopPx && rawMousePos.y <= vpTopPx + vpHeightPx) {
            
            float logicalX = (rawMousePos.x - vpLeftPx) * (900.f / vpWidthPx);
            float logicalY = (rawMousePos.y - vpTopPx) * (600.f / vpHeightPx);

            if (backButtonSprite.getGlobalBounds().contains(sf::Vector2f(logicalX, logicalY))) {
                goBackToMenu = true;
                window.close();  // Return to menu
            } else if (backCardSprite.getGlobalBounds().contains(sf::Vector2f(logicalX, logicalY))) {
                if (!hasCurrentCard && currentRoundIndex < 3 && currentPhase == GamePhase::WaitingForRoundStart) {
                    flipNextRoundCard();
                }
            } else if (hasCurrentCard && currentPhase == GamePhase::Playing) {
                float targetHeight = 110.f;
                float margin = 10.f;
                float startX = 70.f;
                float startY = 60.f;

                // Derive scaled card width from current card texture
                float cardScale = targetHeight / backCardTexture.getSize().y;
                float cardWidth = backCardTexture.getSize().x * cardScale;

                int suitRow = getSuitIndex(currentCard.suit);
                //std::cout << "suitRow= " << suitRow << std::endl;
                int targetCol = currentCard.number - 1;

                float cardX = startX + targetCol * (cardWidth + margin);
                float cardY = startY + suitRow * (targetHeight + margin);

                sf::FloatRect targetBounds(cardX, cardY, cardWidth, targetHeight);

                //std::cout << "Clicked at logical: " << logicalX << "," << logicalY << std::endl;
                //std::cout << "Target Bounds: (" << cardX << "," << cardY << ") size: (" << cardWidth << "," << targetHeight << ")" << std::endl;

                if (targetBounds.contains(logicalX, logicalY)) {
                    Card revealed = boardState[suitRow][targetCol];
                    boardState[suitRow][targetCol] = currentCard;
                    revealedCards[suitRow][targetCol] = true;

                    if (revealed.number == 10) {
                        kingsFound.push_back(revealed);
                        hasCurrentCard = false;
                        currentPhase = kingsFound.size() < 4 ? GamePhase::WaitingForRoundStart : GamePhase::GameLost;
                    } else {
                        currentCard = revealed;
                        std::string imagePath = "assets/cards/small/" + std::to_string(currentCard.number) + "_" + currentCard.suit + ".png";
                        currentCardTexture.loadFromFile(imagePath);
                        currentCardSprite.setTexture(currentCardTexture);
                        float scale = targetHeight / currentCardTexture.getSize().y;
                        currentCardSprite.setScale(scale, scale);
                        currentCardSprite.setPosition(800.f, 270.f);
                    }

                    bool allRevealed = true;
                    for (auto& row : revealedCards) {
                        for (bool b : row) {
                            if (!b) allRevealed = false;
                        }
                    }
                    if (allRevealed && kingsFound.size() < 4) currentPhase = GamePhase::GameWon;
                }
            }
        }
    }
}

void LacuneWindow::handleResize(unsigned int width, unsigned int height) {
    float windowRatio = static_cast<float>(width) / height;
    float viewRatio = 900.f / 600.f;

    sf::FloatRect viewport;

    if (windowRatio > viewRatio) {
        float scale = viewRatio / windowRatio;
        viewport = { (1.f - scale) / 2.f, 0.f, scale, 1.f };
    } else {
        float scale = windowRatio / viewRatio;
        viewport = { 0.f, (1.f - scale) / 2.f, 1.f, scale };
    }

    view.setViewport(viewport);
    view.setSize(900.f, 600.f);
    view.setCenter(450.f, 300.f);
    window.setView(view);
}

sf::Texture& LacuneWindow::getCardTexture(const std::string& imagePath) {
    auto it = cardTextureCache.find(imagePath);
    if (it != cardTextureCache.end()) {
        return it->second;
    }
    
    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load card image: " << imagePath << std::endl;
    }

    cardTextureCache[imagePath] = texture;
    return cardTextureCache[imagePath];
}


void LacuneWindow::draw() {
    window.clear();
    window.draw(backgroundSprite);

    if(kingsFound.size()== 0){
        window.draw(placeholderShadow);
    }
    
    // Draw symbols (suit labels)
    for (const auto& sprite : symbolSprites) {
        window.draw(sprite);
    }

    // Draw board cards
    float targetHeight = 110.f;
    float margin = 10.f;
    float startX = 70.f;
    float startY = 60.f;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 9; ++col) {
            const Card& card = boardState[row][col];

            std::string imagePath;
            if (revealedCards[row][col]) {
                imagePath = "assets/cards/small/" + std::to_string(card.number) + "_" + card.suit + ".png";
            } else {
                imagePath = "assets/cards/small/retro_carte.png";
            }

            sf::Sprite sprite;
            sf::Texture& texture = getCardTexture(imagePath);
            sprite.setTexture(texture);

            float scale = targetHeight / static_cast<float>(texture.getSize().y);
            sprite.setScale(scale, scale);

            float scaledWidth = texture.getSize().x * scale;
            float posX = startX + col * (scaledWidth + margin);
            float posY = startY + row * (targetHeight + margin);
            sprite.setPosition(posX, posY);

            window.draw(sprite);
        }
    }

    // Draw back button and round deck
    window.draw(backButtonSprite);
    if (currentRoundIndex < 3) {
        window.draw(backCardSprite);
    }

    // Draw flipped current round card if any
    if (hasCurrentCard) {
        window.draw(currentCardSprite);
    }

    // Draw found kings (stacked over placeholder)
    float kingStackX = 800.f;
    float kingStackY = 70.f;
    float kingOffsetY = 10.f;
    for (size_t i = 0; i < kingsFound.size(); ++i) {
        const Card& king = kingsFound[i];
        std::string imagePath = "assets/cards/small/" + std::to_string(king.number) + "_" + king.suit + ".png";
        sf::Texture& tex = getCardTexture(imagePath);
        
        sf::Sprite sprite;
        sprite.setTexture(tex);
        float scale = 110.f / tex.getSize().y;
        sprite.setScale(scale, scale);
        sprite.setPosition(kingStackX, kingStackY + i * kingOffsetY); // slight overlap

        window.draw(sprite);
    }

    window.display();
}

    bool LacuneWindow::run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                handleInput(event);
            }
            draw();
        }
    return goBackToMenu;
}