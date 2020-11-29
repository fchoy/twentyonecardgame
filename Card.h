#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
private:
	std::string value;
	std::string suit;
	int cardValue;
public:

	Card(){
		value = " ";
		cardValue = 0;
		suit = "";
	}

	void setValue(std::string value) { //A, 2, 3, ...
		this->value = value;
	}

	std::string returnValue() {
		return value;
	}

	void setSuit(std::string suit) { //hearts diamonds
		this->suit = suit;
	}

	std::string returnSuit() {
		return suit;
	}

	void setCardValue(int value) { //actual # value of card
		this->cardValue = value;
	}

	int getCardValue() {
		return cardValue;
	}
};

#endif