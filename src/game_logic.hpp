#pragma once
#include <string>
#include <vector>

struct Card {
    std::string suit;
    int number;
};

std::vector<Card> generateDeck();
void shuffleDeck(std::vector<Card>& deck);
std::string getSuitSymbol(const std::string& suit);