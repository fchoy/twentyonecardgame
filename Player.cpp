#include "Player.h"
#include <iostream>
using namespace std;

Player::Player() {
	blowup = false;
	stand = false;
	dealer = false;
	handSize = 0;
	sum = 0;
	diceRoll = 0;
	isCPU = false;
	hand.clear();
}

Player::~Player() { //unused, no pointers or memory to deallocate
}

void Player::drawCard(Card card){
	hand.push_back(card);
	handSize++;
}

void Player::discardCard(int index) { //allows player to choose which card to discard
	hand.erase(hand.begin() + index); //e.x hand.begin() + index 0 = hand.begin(), so deletes the first card
	handSize--;
}

void Player::clearHand() {
	hand.clear();
	handSize = 0;
	sum = 0;
}

void Player::setBlowup(bool status) {
	this->blowup = status;
}

bool Player::getblowupStatus() const {
	return blowup;
}

void Player::setStand(bool status) {
	this->stand = status;
}

bool Player::getstandStatus() const {
	return stand;
}

void Player::setSum(int sum) {
	this->sum = sum;
}

void Player::resetSum() {
	sum = 0;
}

int Player::getSum() const {
	return sum;
}

void Player::setdealerStatus(bool status) {
	this->dealer = status;
}

bool Player::getdealerStatus() const {
	return dealer;
}


void Player::rollDice() {
	diceRoll = (rand() % 10) + 1; //roll a number between 1 and 10.
}

int Player::getrollValue() const {
	return diceRoll;
}

void Player::resetPlayer() {
	blowup = false;
	stand = false;
	dealer = false;
	clearHand();
	diceRoll = 0;
}

void Player::setCPUStatus(bool status) {
	this->isCPU = status;
}

bool Player::getCPUStatus() const {
	return isCPU;
}

void Player::printHand() {
	if (isCPU == true) {
		cout << "Their current hand contains: " << endl;
	}

	else {
		cout << "Your current hand contains: " << endl;
	}

	for (int i = 0; i < handSize; i++) {
		cout << "Card #" << i + 1 << " is " << hand[i].returnValue() << " of " << hand[i].returnSuit() << endl;

	}

	if (isCPU == true) {
		cout << "The current sum of the cards in their hand is: " << getSum() << endl << endl;

	}

	else {  
		cout << "The current sum of the cards in your hand is: " << getSum() << endl << endl;
	}
	
}