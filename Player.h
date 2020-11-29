#ifndef PLAYER_H
#define PLAYER_H
#include "Deck.h"
#include <vector>
using namespace std;

class Player {
private:
	vector<Card> hand;
	int handSize;
	int sum;
	int diceRoll;
	bool blowup; //true if over 21
	bool stand; //true if player chooses to stand
	bool dealer;
	bool isCPU;

public:
	Player();
	~Player();
	void drawCard(Card card);
	void discardCard(int index);
	void clearHand();
	
	void setBlowup(bool status);
	bool getblowupStatus() const;
	
	void setStand(bool status);
	bool getstandStatus() const;
	
	void setSum(int sum);
	int getSum() const;
	void resetSum();
	
	void setdealerStatus(bool status);
	bool getdealerStatus() const;
	
	void setwinStatus(bool status);
	bool getwinStatus() const;

	void rollDice();
	int getrollValue() const;

	void setCPUStatus(bool status);
	bool getCPUStatus() const;

	void resetPlayer();

	void printHand();
};

#endif