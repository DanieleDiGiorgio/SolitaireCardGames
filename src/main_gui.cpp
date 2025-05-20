#include <SFML/Graphics.hpp>
#include "menu/menu.hpp"
#include "solitaires/one_two_three/game_window.hpp"
#include "solitaires/lacune/lacune_window.hpp"

int main() {
    while (true) {
        sf::RenderWindow menuWindow(sf::VideoMode(900, 600), "Menu");
        Menu menu(menuWindow);
        std::string selectedGame = menu.run();

        if (selectedGame == "1-2-3 Solitaire") {
            OneTwoThreeWindow game;
            if (!game.run()) break; 
        } else if( selectedGame == "Lacune") {
            LacuneWindow lacuneGame;
            if (!lacuneGame.run()) break;
        }
        else {
            break;
        }
    }
    return 0;
}