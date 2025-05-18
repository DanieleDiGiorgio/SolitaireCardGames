#include <SFML/Graphics.hpp>
#include "menu/menu.hpp"
#include "solitaires/one_two_three/game_window.hpp"

int main() {
    sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Menu");
    Menu menu(menuWindow);
    std::string selectedGame = menu.run();

    if (selectedGame == "1-2-3 Solitaire") {
        OneTwoThreeWindow game;
        game.run();
    }

    return 0;
}