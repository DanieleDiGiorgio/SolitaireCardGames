#include "lacune_window.hpp"
#include <iostream>

LacuneWindow::LacuneWindow() : window(sf::VideoMode(900, 600), "Lacune") {
    // Initialize any assets, cards, or layout here
    if (!backgroundTexture.loadFromFile("assets/backgrounds/green_felt_background_scale.png")) {
        std::cerr << "Failed to load background image" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
}


void LacuneWindow::handleInput(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();

    } else if (event.type == sf::Event::Resized) {
        handleResize(event.size.width, event.size.height);
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
    window.display();
}

void LacuneWindow::run() {
  while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleInput(event);
        }
        draw();
    }
}