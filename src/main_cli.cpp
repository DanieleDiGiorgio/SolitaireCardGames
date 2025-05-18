#include <iostream>
#include <string>
#include <vector>
#include "game_logic.hpp"

// Print ASCII representation of a card
void printCardAscii(const Card& card) {
    std::string suit = getSuitSymbol(card.suit);
    std::string numStr = std::to_string(card.number);
    if (card.number < 10) numStr = " " + numStr;

    std::cout << "+---------+\n";
    std::cout << "|         |\n";
    std::cout << "|         |\n";
    std::cout << "|  " << numStr << "  " << suit << "  |\n";
    std::cout << "|         |\n";
    std::cout << "|         |\n";
    std::cout << "+---------+\n";
}

// Play one round
bool playGameCLI(std::vector<Card>& deck) {
    int call = 1;
    std::string input;

    std::cout << "\nPremi enter per girare le carte\n";

    for (size_t i = 0; i < deck.size(); ++i) {
        std::getline(std::cin, input); // wait for Enter

        Card& card = deck[i];
        std::cout << "Conteggio: " << call << "\n";
        printCardAscii(card);

        if (card.number == call) {
            std::cout << "You lose! Sfigato, carte giocate: " << i + 1 << "\n";
            return false;
        }

        call = (call % 3) + 1;
    }

    std::cout << "You win! Che bucio di culo.\n";
    return true;
}

int main() {
    std::vector<Card> deck;
    char choice;

    do {
        deck = generateDeck();
        shuffleDeck(deck);
        bool result = playGameCLI(deck);

        std::cout << "\nDai al prossimo vinci(y/n): ";
        choice = std::cin.get();
        std::cin.ignore(); // clean newline

    } while (choice == '\n' || choice == 'y' || choice == 'Y');

    std::cout << "Basta perdere tempo\n";
    return 0;
}
