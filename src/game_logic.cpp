#include "game_logic.hpp"
#include <algorithm>
#include <random>
#include <ctime>

// Generate a full deck of "carte napoletane"
std::vector<Card> generateDeck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Ori", "Coppe", "Spade", "Bastoni"};

    for (const auto& suit : suits) {
        for (int num = 1; num <= 10; ++num) {
            deck.push_back({suit, num});
        }
    }
    return deck;
}

// Shuffle the deck
void shuffleDeck(std::vector<Card>& deck) {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(deck.begin(), deck.end(), rng);
}

// Get suit symbol (Unicode)
std::string getSuitSymbol(const std::string& suit) {
    if (suit == "Ori") return "♦";
    if (suit == "Coppe") return "♥";
    if (suit == "Spade") return "♠";
    if (suit == "Bastoni") return "♣";
    return "?";
}