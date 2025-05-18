#include "menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& win) : window(win), selectedIndex(0) {
    if (!font.loadFromFile("assets/fonts/Garuda.ttf")) {
        std::cerr << "Could not load font\n";
    }

    if (!backgroundTexture.loadFromFile("assets/backgrounds/wood_table_scale.png")) {
        std::cerr << "Failed to load background image" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!titleTexture.loadFromFile("assets/titles/title_reduced.png")) {
        std::cerr << "Failed to load title image" << std::endl;
    }
    titleSprite.setTexture(titleTexture);
    // Maintain aspect ratio (scale by height)
    float titleTargetHeight = 100.f;
    float titleScale = titleTargetHeight / static_cast<float>(titleTexture.getSize().y);
    titleSprite.setScale(titleScale, titleScale);
    // Center title in logical 800x600 space
    sf::FloatRect titleBounds = titleSprite.getGlobalBounds();
    float titleCenterX = 400.f - titleBounds.width / 2.f;
    float titleCenterY = 150.f - titleBounds.height / 2.f;
    titleSprite.setPosition(titleCenterX, titleCenterY);

    if (!boxTexture_1.loadFromFile("assets/boxes/menu_text_box.png")) {
        std::cerr << "Failed to load box image" << std::endl;
    }
    boxSprite_1.setTexture(boxTexture_1);
    // Maintain aspect ratio (scale by height)
    float box_1_TargetHeight = 100.f;
    float box_1_scale = box_1_TargetHeight / static_cast<float>(boxTexture_1.getSize().y);
    boxSprite_1.setScale(box_1_scale, box_1_scale);
    // Center box in logical 800x600 space
    sf::FloatRect box_1_bounds = boxSprite_1.getGlobalBounds();
    float box_1_centerX = 400.f - box_1_bounds.width / 2.f;
    float box_1_centerY = 250.f - box_1_bounds.height / 2.f;
    boxSprite_1.setPosition(box_1_centerX, box_1_centerY);

    if (!boxTexture_2.loadFromFile("assets/boxes/menu_text_box.png")) {
        std::cerr << "Failed to load box image" << std::endl;
    }
    boxSprite_2.setTexture(boxTexture_2);
    // Maintain aspect ratio (scale by height)
    float box_2_TargetHeight = 100.f;
    float box_2_scale = box_2_TargetHeight / static_cast<float>(boxTexture_2.getSize().y);
    boxSprite_2.setScale(box_2_scale, box_2_scale);
    // Center box in logical 800x600 space
    sf::FloatRect box_2_bounds = boxSprite_2.getGlobalBounds();
    float box_2_centerX = 400.f - box_2_bounds.width / 2.f;
    float box_2_centerY = 350.f - box_2_bounds.height / 2.f;
    boxSprite_2.setPosition(box_2_centerX, box_2_centerY);

    options = {
        "1-2-3 Solitaire",
        "new incoming"
    };

    std::vector<sf::Sprite*> boxes = { &boxSprite_1, &boxSprite_2 };

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(options[i], font, 30);
        
        // Center the text in the corresponding box
        sf::FloatRect textBounds = text.getLocalBounds();
        sf::Vector2f boxPos = boxes[i]->getPosition();
        sf::Vector2f boxSize(
            boxes[i]->getGlobalBounds().width,
            boxes[i]->getGlobalBounds().height
        );

        float x = boxPos.x + (boxSize.x - textBounds.width) / 2.f - textBounds.left;
        float y = boxPos.y + (boxSize.y - textBounds.height) / 2.f - textBounds.top;
        text.setPosition(x, y);

        textOptions.push_back(text);
    }


    view.setSize(800.f, 600.f);
    view.setCenter(400.f, 300.f);
    window.setView(view);
}


void Menu::handleResize(unsigned int width, unsigned int height) {
    float windowRatio = static_cast<float>(width) / height;
    float viewRatio = 800.f / 600.f;

    sf::FloatRect viewport;

    if (windowRatio > viewRatio) {
        float scale = viewRatio / windowRatio;
        viewport = { (1.f - scale) / 2.f, 0.f, scale, 1.f };
    } else {
        float scale = windowRatio / viewRatio;
        viewport = { 0.f, (1.f - scale) / 2.f, 1.f, scale };
    }

    view.setViewport(viewport);
    view.setSize(800.f, 600.f);
    view.setCenter(400.f, 300.f);
    window.setView(view);
}

std::string Menu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::MouseMoved){
                sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
                sf::FloatRect viewport = view.getViewport();

                float vpLeftPx = viewport.left * window.getSize().x;
                float vpTopPx = viewport.top * window.getSize().y;
                float vpWidthPx = viewport.width * window.getSize().x;
                float vpHeightPx = viewport.height * window.getSize().y;

                if (pixelPos.x >= vpLeftPx && pixelPos.x <= vpLeftPx + vpWidthPx &&
                    pixelPos.y >= vpTopPx && pixelPos.y <= vpTopPx + vpHeightPx) {

                    float vx = (pixelPos.x - vpLeftPx) * (800.f / vpWidthPx);
                    float vy = (pixelPos.y - vpTopPx) * (600.f / vpHeightPx);

                    handleMouseHover(sf::Vector2f(vx, vy));
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i rawMousePos(event.mouseButton.x, event.mouseButton.y);
                sf::FloatRect vp = view.getViewport();

                float vpLeftPixels = vp.left * window.getSize().x;
                float vpTopPixels = vp.top * window.getSize().y;
                float vpWidthPixels = vp.width * window.getSize().x;
                float vpHeightPixels = vp.height * window.getSize().y;

                // Check if mouse is inside the viewport (ignore if outside black bars)
                if (rawMousePos.x >= vpLeftPixels && rawMousePos.x <= vpLeftPixels + vpWidthPixels &&
                    rawMousePos.y >= vpTopPixels && rawMousePos.y <= vpTopPixels + vpHeightPixels) {

                    // Convert mouse coords from window pixels to view logical coords (800x600)
                    float logicalX = (rawMousePos.x - vpLeftPixels) * (800.f / vpWidthPixels);
                    float logicalY = (rawMousePos.y - vpTopPixels) * (600.f / vpHeightPixels);

                    handleMouseClick(sf::Vector2f(logicalX, logicalY));
                }
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up)
                    selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                else if (event.key.code == sf::Keyboard::Down)
                    selectedIndex = (selectedIndex + 1) % options.size();
                else if (event.key.code == sf::Keyboard::Enter) {
                    selectedGame = options[selectedIndex];
                    window.close();  // close menu
                }
            }
            else if (event.type == sf::Event::Resized) {
                handleResize(event.size.width, event.size.height);
            }
        }

        draw();
    }

    return selectedGame;
}


void Menu::draw() {
    window.clear();

    window.draw(backgroundSprite);

    window.draw(titleSprite);
    window.draw(boxSprite_1);
    window.draw(boxSprite_2);
    
    for (size_t i = 0; i < textOptions.size(); ++i) {
        textOptions[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        window.draw(textOptions[i]);
    }

    window.display();
}

void Menu::handleMouseHover(const sf::Vector2f& mousePos) {
    if (boxSprite_1.getGlobalBounds().contains(mousePos))
        selectedIndex = 0;
    else if (boxSprite_2.getGlobalBounds().contains(mousePos))
        selectedIndex = 1;
}

void Menu::handleMouseClick(const sf::Vector2f& mousePos) {
    
    if (boxSprite_1.getGlobalBounds().contains(mousePos)) {
        selectedGame = options[0];
        window.close();
    }
    else if (boxSprite_2.getGlobalBounds().contains(mousePos)) {
        selectedGame = options[1];
        window.close();
    }
}
