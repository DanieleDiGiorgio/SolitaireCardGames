#include "game_window.hpp"
#include <iostream>

OneTwoThreeWindow::OneTwoThreeWindow()
    : window(sf::VideoMode(900, 600), "Carte Napoletane"), currentIndex(0), call(1), cardsPlayed(0),gameState(GameState::WaitingToStart) {
    window.setFramerateLimit(30);
    if (!font.loadFromFile("assets/fonts/Garuda.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("assets/backgrounds/wood_table_scale_32.png")) {
        std::cerr << "Failed to load background image" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    infoText.setFont(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setString("Premi ENTER per pescare");
    // Center it 
    sf::FloatRect infoTextBounds = infoText.getLocalBounds();
    infoText.setPosition(
        450.f - infoTextBounds.width / 2.f - infoTextBounds.left, // Centered X
        100.f
    );

    digitTextures.resize(10);
    for (int i = 0; i < 10; ++i) {
        std::string path = "assets/numbers/" + std::to_string(i) + ".png";
        if (!digitTextures[i].loadFromFile(path)) {
            std::cerr << "Failed to load digit image: " << path << std::endl;
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
    //backButtonSprite.setPosition(10.f, 600.f - backButtonBounds.height - 10.f);
    backButtonSprite.setPosition(1.f,10.f);

    conteggioText.setFont(font);
    conteggioText.setCharacterSize(24);
    conteggioText.setFillColor(sf::Color::White);
    conteggioText.setPosition(377.f, 600.f - 130.f); // In the center under the card
    resetGame();
}

void OneTwoThreeWindow::resetGame() {
    deck = generateDeck();
    shuffleDeck(deck);
    currentIndex = 0;
    call = 1;
    cardsPlayed = 0;
    gameState = GameState::WaitingToStart;
    infoText.setString("Premi ENTER per pescare");
    sf::FloatRect infoTextBounds = infoText.getLocalBounds();
    infoText.setPosition(
        450.f - infoTextBounds.width / 2.f - infoTextBounds.left, // Centered X
        100.f
    );
    
}

void OneTwoThreeWindow::loadCardImage(const Card& card) {
    std::string imagePath = "assets/cards/" + std::to_string(card.number) + "_" + card.suit + ".png";
    if (!cardTexture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load image " << imagePath << std::endl;
        return;
    }
    cardSprite.setTexture(cardTexture);

    // Maintain aspect ratio (scale by height)
    float targetHeight = 300.f;
    float scale = targetHeight / static_cast<float>(cardTexture.getSize().y);
    //std::cout << "scale = " << scale <<std::endl;

    cardSprite.setScale(scale, scale);


    // Center card in logical 800x600 space
    sf::FloatRect bounds = cardSprite.getGlobalBounds();
    float centerX = 450.f - bounds.width / 2.f;
    float centerY = 300.f - bounds.height / 2.f;
    cardSprite.setPosition(centerX, centerY);
}


void OneTwoThreeWindow::handleInput(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();

    } else if (event.type == sf::Event::Resized) {
        handleResize(event.size.width, event.size.height);

    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            resetGame();
        }
    } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Enter) {
            // If the state is "WaitingToStart" then proceed to move into the state "Playing"
            if (gameState == GameState::WaitingToStart) {
                gameState = GameState::Playing;
            }
            
            //If the state is "Playing" and deck is not finished then continue playing
            if (gameState == GameState::Playing && currentIndex < deck.size()) {
                const Card& card = deck[currentIndex];
                loadCardImage(card);
                cardsPlayed++;
                
                currentIndex++;
                
                if (cardsPlayed >= 0 && cardsPlayed <= 40) {
                    int leftDigit = cardsPlayed / 10;
                    int rightDigit = cardsPlayed % 10;

                    if (cardsPlayed >= 10) {
                        digitSpriteLeft.setTexture(digitTextures[leftDigit]);
                    }
                    digitSpriteRight.setTexture(digitTextures[rightDigit]);

                    // Get bounds to help with positioning
                    sf::FloatRect leftBounds, rightBounds;
                    if (cardsPlayed >= 10) leftBounds = digitSpriteLeft.getLocalBounds();
                    rightBounds = digitSpriteRight.getLocalBounds();

                    // Position settings
                    float baseX = 700.f; // center of counter (650)
                    float baseY = 120.f;

                    // Horizontal spacing between digits
                    float spacing = 5.f;

                    if (cardsPlayed >= 10) {
                        float totalWidth = leftBounds.width + spacing + rightBounds.width;

                        digitSpriteLeft.setPosition(baseX - totalWidth / 2.f - leftBounds.left,
                                                    baseY - leftBounds.height / 2.f - leftBounds.top);

                        digitSpriteRight.setPosition(baseX + totalWidth / 2.f - rightBounds.width - rightBounds.left,
                                                    baseY - rightBounds.height / 2.f - rightBounds.top);
                    } else {
                        digitSpriteRight.setPosition(baseX - rightBounds.width / 2.f - rightBounds.left,
                                                    baseY - rightBounds.height / 2.f - rightBounds.top);
                    }
                }

                conteggioText.setString("Conteggio: " + std::to_string(call));
                if (card.number == call) {
                    // If the number of the card correspond to the one of the call then 
                    // you lose the game ==> set state to "Lost"
                    gameState = GameState::Lost;
                    infoText.setString("Hai perso! Premi R per riprovare.");

                    // Center infoText
                    sf::FloatRect infoTextBounds = infoText.getLocalBounds();
                    infoText.setPosition(
                        450.f - infoTextBounds.width / 2.f - infoTextBounds.left,
                        100.f 
                    );

                } else {
                    // If number on the card is different from the call
                    // Update the call
                    call = (call % 3) + 1; 
                    
                    // Check if the deck is finished ==> if so you won ==> set state to "Won"
                    if (currentIndex == deck.size()) {
                        gameState = GameState::Won;
                        infoText.setString("Hai vinto! Premi R per giocare di nuovo.");

                        // Center infoText
                        sf::FloatRect infoTextBounds = infoText.getLocalBounds();
                        infoText.setPosition(
                            450.f - infoTextBounds.width / 2.f - infoTextBounds.left,
                            100.f 
                        );
                    }
                }
            }
        }
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



void OneTwoThreeWindow::handleResize(unsigned int width, unsigned int height) {
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

void OneTwoThreeWindow::draw() {
    window.clear();

    window.draw(backgroundSprite);

    if (gameState == GameState::Playing || gameState == GameState::Lost || gameState == GameState::Won) {
        window.draw(cardSprite);
        if (cardsPlayed >= 10)
            window.draw(digitSpriteLeft);
        window.draw(digitSpriteRight);
        window.draw(conteggioText);
    }
    window.draw(backButtonSprite);
    window.draw(infoText);
    window.display();
}

bool OneTwoThreeWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleInput(event);
        }
        draw();
    }
    return goBackToMenu;
}