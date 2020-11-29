#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <vector>
using namespace std;

class Deck { //52 card deck
private:
	vector<Card> deck;
	int size;
public:
	Deck();
	~Deck(); //unused, no memory to deallocate.
	void shuffleDeck(); //shuffles the deck
	void deleteCard(); //deletes a card after it has been drawn
	Card drawfromDeck(); //draws a card from the deck, deletes the card from the deck after.
	void resetDeck(); //resets the deck to original state.
};

#endif