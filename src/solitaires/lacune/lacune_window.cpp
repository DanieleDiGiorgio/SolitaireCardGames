#include "lacune_window.hpp"
#include <iostream>

LacuneWindow::LacuneWindow() : window(sf::VideoMode(900, 600), "Lacune") {
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
    // Optional: scale it down if needed
    float backButtonTargetHeight = 50.f; // logical units
    float backButtonScale = backButtonTargetHeight / backButtonTexture.getSize().y;
    backButtonSprite.setScale(backButtonScale, backButtonScale);
    // Position it in the bottom-left corner of the logical 900x600 view
    sf::FloatRect backButtonBounds = backButtonSprite.getGlobalBounds();
    backButtonSprite.setPosition(1.f, 10.f);

    // Initialize the game state
    resetGame();

}


void LacuneWindow::resetGame() {
    deck = generateDeck();
    shuffleDeck(deck);

    cardSprites.clear();
    cardTextures.clear();
    cardSprites.resize(deck.size());
    cardTextures.resize(deck.size());

    float targetHeight = 110.f;
    float margin = 10.f;
    float startX = 70.f;
    float startY = 60.f;
    int cardsPerRow = 9;
    float placeHolderWidth;

    for (size_t i = 0; i < deck.size()-4; ++i) {
        const Card& card = deck[i];
        std::string imagePath = "assets/cards/small/" + std::to_string(card.number) + "_" + card.suit + ".png";

        if (!cardTextures[i].loadFromFile(imagePath)) {
            std::cerr << "Failed to load " << imagePath << std::endl;
            continue;
        }

        cardSprites[i].setTexture(cardTextures[i]);

        float scale = targetHeight / static_cast<float>(cardTextures[i].getSize().y);
        cardSprites[i].setScale(scale, scale);

        float scaledWidth = cardTextures[i].getSize().x * scale;
        if (i==0){
            placeHolderWidth = scaledWidth;
        }
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float posX = startX + col * (scaledWidth + margin);
        float posY = startY + row * (targetHeight + margin);

        cardSprites[i].setPosition(posX, posY);
    }

    int remainingCardsIdx = 36;

    //load as example one of the remaining (then should be only the back)
    //const Card& card = deck[remainingCardsIdx];
    std::string imagePath = "assets/cards/small/retro_carte.png";    
    if (!backCardTexture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load " << imagePath << std::endl;
    }
    float scale = targetHeight / static_cast<float>(backCardTexture.getSize().y);
    backCardSprite.setScale(scale, scale);

    backCardSprite.setTexture(backCardTexture);
    backCardSprite.setPosition(800.f, 350.f);

    placeholderShadow.setSize(sf::Vector2f(placeHolderWidth, targetHeight));
    placeholderShadow.setFillColor(sf::Color(0, 0, 0, 50)); // semi-transparent black
    placeholderShadow.setOutlineColor(sf::Color::Black);    // optional border
    placeholderShadow.setOutlineThickness(1.f);             // thin border 
    placeholderShadow.setPosition(800.f, 150.f);
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


void LacuneWindow::draw(){
    window.clear();
    window.draw(backgroundSprite);
    window.draw(placeholderShadow);
    for (const auto& sprite : symbolSprites) {
        window.draw(sprite);
    }
    for (const auto& sprite : cardSprites) {
        window.draw(sprite);
    }
    window.draw(backButtonSprite);
    window.draw(backCardSprite);
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