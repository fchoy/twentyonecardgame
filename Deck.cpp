#include "Deck.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
using namespace std;

Deck::Deck(){ //sets up all cards
		size = 0;
		std::string allValues[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
		std::string allSuits[] = { "Diamonds", "Clovers", "Hearts", "Spades" };

		Card tempCard;
		for (int i = 0; i < 4; i++) { //set all values and suits
			for (int j = 0; j < 13; j++) {
				tempCard.setSuit(allSuits[i]);
				tempCard.setValue(allValues[j]);
				if (tempCard.returnValue() == "A") {
					tempCard.setCardValue(1);
				}
				else if (tempCard.returnValue() == "2") {
					tempCard.setCardValue(2);
				}
				else if (tempCard.returnValue() == "3") {
					tempCard.setCardValue(3);
				}
				else if (tempCard.returnValue() == "4") {
					tempCard.setCardValue(4);
				}
				else if (tempCard.returnValue() == "5") {
					tempCard.setCardValue(5);
				}
				else if (tempCard.returnValue() == "6") {
					tempCard.setCardValue(6);
				}
				else if (tempCard.returnValue() == "7") {
					tempCard.setCardValue(7);
				}
				else if (tempCard.returnValue() == "8") {
					tempCard.setCardValue(8);
				}
				else if (tempCard.returnValue() == "9") {
					tempCard.setCardValue(9);
				}
				else if (tempCard.returnValue() == "10" || tempCard.returnValue() == "J" || tempCard.returnValue() == "Q" || tempCard.returnValue() == "K") {
					tempCard.setCardValue(10);
				}
				deck.push_back(tempCard);
				size++;
			}
		}
}

Deck::~Deck() { //unused
}

void Deck::shuffleDeck() { //shuffle deck
	random_shuffle(deck.begin(), deck.end());
}

Card Deck::drawfromDeck() {
	Card aCard = deck.back();
	deleteCard();
	return aCard;
}

void Deck::deleteCard() {
	deck.pop_back();
	size--;
}

void Deck::resetDeck() {
	deck.clear(); //clear the entire deck.
	size = 0; //set size to 0.
	
	std::string allValues[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	std::string allSuits[] = { "Diamond", "Clover", "Heart", "Spade" };

	Card tempCard;
	for (int i = 0; i < 4; i++) { //set all values and suits
		for (int j = 0; j < 13; j++) {
			tempCard.setSuit(allSuits[i]);
			tempCard.setValue(allValues[j]);
			if (tempCard.returnValue() == "A") {
				tempCard.setCardValue(1);
			}
			else if (tempCard.returnValue() == "2") {
				tempCard.setCardValue(2);
			}
			else if (tempCard.returnValue() == "3") {
				tempCard.setCardValue(3);
			}
			else if (tempCard.returnValue() == "4") {
				tempCard.setCardValue(4);
			}
			else if (tempCard.returnValue() == "5") {
				tempCard.setCardValue(5);
			}
			else if (tempCard.returnValue() == "6") {
				tempCard.setCardValue(6);
			}
			else if (tempCard.returnValue() == "7") {
				tempCard.setCardValue(7);
			}
			else if (tempCard.returnValue() == "8") {
				tempCard.setCardValue(8);
			}
			else if (tempCard.returnValue() == "9") {
				tempCard.setCardValue(9);
			}
			else if (tempCard.returnValue() == "10" || tempCard.returnValue() == "J" || tempCard.returnValue() == "Q" || tempCard.returnValue() == "K") {
				tempCard.setCardValue(10);
			}
			deck.push_back(tempCard);
			size++;
		}
	}

}
