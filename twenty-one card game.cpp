#include <iostream>
#include <string>
#include "Deck.h"
#include "Player.h"
#include "CPU.h"
#include <algorithm>
#include <limits>
using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	Deck* cardDeck = new Deck();
	Card tempCard; //temp card used to calculate sums

	const int MAX_PLAYERS = 4;
	Player* newPlayer = nullptr;
	Player* newCPU = nullptr;
	int numPlayers = 0, numCPU = 0, cpuRandNum = 0, playerSum = 0, cpuSum = 0; //temp variables to set the sum of each player.
	int dealerIndex; //used to keep track of which index of CPU or Player is the dealer.
	bool gameEnd = false;
	string playerChoice;
	bool endingMenu = false;
	bool MenuClear = false, numPlayersMenu = false, numCPUMenu = false;

	//entire game loop (1 round)
	while (gameEnd == false) {
		//setup menu
		while (MenuClear == false) {
			cout << "Welcome to console Twenty-One, the card game." << endl;
			while (numPlayersMenu == false) {
				cout << "Please enter the number of players playing (1 - 4): ";
				cin >> numPlayers;
				if (numPlayers > 4 || numPlayers < 1 || cin.fail()) {
					cin.clear(); //clear bad input
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); //skip rest of bad input
					cout << "Invalid number of players entered, please try again." << endl;
					cout << endl;
				}
				else {
					numPlayersMenu = true;
					newPlayer = new Player[numPlayers];
				}

			}

			//number of max CPUs allowed is MAX_PLAYERS - numPlayers.
			while (numCPUMenu == false) {
				cout << "Please enter the number of computer players to play against (max " << MAX_PLAYERS - numPlayers << ", min 0" << "): ";
				cin >> numCPU;
				if (numCPU > MAX_PLAYERS - numPlayers || numCPU < 0 || cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); //skip rest of bad input
					cout << "Invalid number of CPU players entered, please try again." << endl;
					cout << endl;
				}
				else {
					numCPUMenu = true;
					newCPU = new Player[numCPU];
					for (int i = 0; i < numCPU; i++) {
						newCPU[i].setCPUStatus(static_cast<bool>(true));
					}
				}

			}
			cout << endl;
			MenuClear = true;
		}

		//choose dealer by seeing who rolls the lowest number
		for (int i = 0; i < numPlayers; i++) {
			newPlayer[i].rollDice();
		}

		for (int j = 0; j < numCPU; j++) {
			newCPU[j].rollDice();
		}

		//compare player rolls
		int minPlayerRoll = newPlayer[0].getrollValue();
		int minCPURoll = newCPU[0].getrollValue();

		for (int i = 1; i < numPlayers; i++) {
			if (newPlayer[i].getrollValue() < minPlayerRoll)
				minPlayerRoll = newPlayer[i].getrollValue();
		}

		for (int j = 1; j < numCPU; j++) {
			if (newCPU[j].getrollValue() < minCPURoll)
				minCPURoll = newCPU[j].getrollValue();
		}

		//compare between lowest player and cpu & set dealer to whoever has lowest value. (If lowest player value = lowest cpu value, then the player will always have the dealer status)
		int lowestRoll = min(minPlayerRoll, minCPURoll);
		for (int i = 0; i < numPlayers; i++) {
			if (newPlayer[i].getrollValue() == lowestRoll) {
				newPlayer[i].setdealerStatus(true);
				dealerIndex = i;
			}
		}

		for (int j = 0; j < numCPU; j++) {
			if (newCPU[j].getrollValue() == lowestRoll) {
				newCPU[j].setdealerStatus(true);
				dealerIndex = j;
			}
		}

		//shuffle the deck, pass out cards for each player
		for (int i = 0; i < 20; i++)
			cardDeck->shuffleDeck(); //shuffle 20 times

		//player draw
		for (int i = 0; i < numPlayers; i++) {
			//player i draw 1
			tempCard = cardDeck->drawfromDeck();
			newPlayer[i].drawCard(tempCard);
			if (tempCard.getCardValue() == 1 && newPlayer[i].getSum() < 11) { //if the player draws an ace & their current sum is less than 11
				playerSum += 11; //add 10 to their total sum.
			}
			else {
				playerSum += tempCard.getCardValue();
			}

			//player i draw 2
			tempCard = cardDeck->drawfromDeck();
			newPlayer[i].drawCard(tempCard);
			if (tempCard.getCardValue() == 1 && newPlayer[i].getSum() < 11) {
				playerSum += 11 ;
			}
			else {
				playerSum += tempCard.getCardValue();
			}
			newPlayer[i].setSum(playerSum);
			playerSum = 0;
		}

		//CPU draw
		for (int j = 0; j < numCPU; j++) { //each player draws 2 cards
			//cpu i draw 1
			tempCard = cardDeck->drawfromDeck();
			newCPU[j].drawCard(tempCard);
			if (tempCard.getCardValue() == 1 && newCPU[j].getSum() < 11) {
				cpuSum += 11;
			}
			else {
				cpuSum += tempCard.getCardValue();
			}

			//cpu i draw 2
			tempCard = cardDeck->drawfromDeck();
			newCPU[j].drawCard(tempCard);
			if (tempCard.getCardValue() == 1 && newCPU[j].getSum() < 11) {
				cpuSum += 11;
			}
			else {
				cpuSum += tempCard.getCardValue();
			}
			newCPU[j].setSum(cpuSum);
			cpuSum = 0;
		}

		//FINISH SETUP OF DECK, HAND, PLAYERS, AND CPU.

		//start player i's turn, choose to hit or stand. 
		//BREAK THIS INTO TWO FOR LOOPS

		for (int i = 0; i < numPlayers; i++) {
			//first check to see if the player is a dealer. 
			if (newPlayer[i].getdealerStatus() == true) {
				//do nothing
			}
			else {
				cout << "It is currently player " << i + 1 << "'s turn:" << endl;
				cout << endl;
				newPlayer[i].printHand(); //prints the current hand of the player.

				//while the player doesn't choose to stand
				while (newPlayer[i].getstandStatus() == false) {
					playerSum = newPlayer[i].getSum(); //before starting, get the sum of the player.
					cout << "Would you like to Hit or Stand? (h/s): ";
					cin >> playerChoice;
					cin.ignore();

					//player chooses to hit
					if (playerChoice == "h" || playerChoice == "H") { //if hit
						cout << "Player " << i + 1 << " has chosen to hit." << endl << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newPlayer[i].drawCard(tempCard); //draw from the deck
						if (tempCard.getCardValue() == 1 && newPlayer[i].getSum() < 11) { //check to see if the drawn card is an ace & if sum is < 11.
							cpuSum += 11;
						}
						else {
							playerSum += tempCard.getCardValue();
						}
						newPlayer[i].setSum(playerSum); //set the new sum.
						//cout << endl;
						newPlayer[i].printHand(); //display the player's hand.

						if (newPlayer[i].getSum() > 21) { //if they go over 21
							newPlayer[i].setBlowup(true); //blow up = true.
							newPlayer[i].setStand(true); //automatically make the player stand.
							cout << "You've blown up! The current sum of your hand is: " << newPlayer[i].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					//player chooses to stand
					else if (playerChoice == "s" || playerChoice == "S") {
						newPlayer[i].setStand(true);
						cout << "Player " << i + 1 << " has chosen to stand." << endl << endl;
						cout << "The final sum of your current hand is: " << newPlayer[i].getSum() << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}

					//if invalid input
					else {
						cout << "You've entered an invalid unput. Please try again." << endl;
						playerChoice.clear(); //clear the string.
						cout << endl;
					}
				}
			}
		}

		//REAL CPU TURN
		for (int j = 0; j < numCPU; j++) {
			if (newCPU[j].getdealerStatus() == true) {
				//do nothing
			}
			else {
				cout << "It is currently CPU " << j + 1 << "'s turn:" << endl;
				cout << endl;
				newCPU[j].printHand(); //print CPU hand.
				cout << "Press Enter to Continue." << flush;
				cin.get();
				while (newCPU[j].getstandStatus() == false) {
					cpuSum = newCPU[j].getSum(); //get the sum of the current CPU.

					//CPU SUM <= 11
					if (newCPU[j].getSum() <= 11) {
						cout << endl;
						cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[j].drawCard(tempCard);
						if (tempCard.getCardValue() == 1 && newCPU[j].getSum() < 11) { //check to see if the drawn card is an ace & if sum is < 11.
							cpuSum += 11;
						}
						else {
							cpuSum += tempCard.getCardValue();
						}
						newCPU[j].setSum(cpuSum); //set CPU sum as new sum
						newCPU[j].printHand(); //print CPU hand.
						cout << "Press Enter to Continue." << flush;
						cin.get();

					}

					//CPU SUM = 12
					else if (newCPU[j].getSum() == 12) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 99) {
							cin.ignore();
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.

							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cin.ignore();
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cin.ignore();
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 13
					else if (newCPU[j].getSum() == 13) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 95) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 14
					else if (newCPU[j].getSum() == 14) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 80) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 15
					else if (newCPU[j].getSum() == 15) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 65) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 16
					else if (newCPU[j].getSum() == 16) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 50) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 17
					else if (newCPU[j].getSum() == 17) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 35) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 18
					else if (newCPU[j].getSum() == 18) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 25) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.

							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 19
					else if (newCPU[j].getSum() == 19) {

						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 15) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.

							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cpuSum = 0;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 20
					else if (newCPU[j].getSum() == 20) {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 5) {
							cout << "CPU #" << j + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[j].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[j].setSum(cpuSum);
							newCPU[j].printHand(); //print CPU hand.
							if (newCPU[j].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[j].setBlowup(true);
								newCPU[j].setStand(true);
								cout << "CPU #" << j + 1 << " has blown up! The current sum of their hand is: " << newCPU[j].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << endl;
							cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
							newCPU[j].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}

					//CPU SUM = 21 (AUTO STAND)
					else if (newCPU[j].getSum() == 21) {
						cout << endl;
						cout << "CPU #" << j + 1 << " has chosen to stand." << endl;
						newCPU[j].setStand(true);
						cout << "The final sum of their current hand is: " << newCPU[j].getSum() << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}
				cpuRandNum = 0; //reset the random number counter.
			}
		} //END REAL CPU TURN

		//(IF PLAYER IS DEALER)
		if (newPlayer[dealerIndex].getdealerStatus() == true && numPlayers > 0) {
			playerSum = newPlayer[dealerIndex].getSum(); //before starting, get the sum of the player.
			cout << "It is currently player " << dealerIndex + 1 << "'s turn:" << endl;
			cout << endl;
			newPlayer[dealerIndex].printHand(); //prints the current hand of the player.

			//while the player doesn't choose to stand
			while (newPlayer[dealerIndex].getstandStatus() == false) {
				cout << "Would you like to Hit or Stand? (h/s): ";
				cin >> playerChoice;
				cin.ignore(); 

				//player chooses to hit
				if (playerChoice == "h" || playerChoice == "H") { //if hit
					cout << "Player " << dealerIndex + 1 << " has chosen to hit." << endl << endl;
					cout << "Press Enter to Continue." << flush;
					cin.get();
					cout << endl;
					tempCard = cardDeck->drawfromDeck();
					newPlayer[dealerIndex].drawCard(tempCard); //draw from the deck
					if (tempCard.getCardValue() == 1 && newPlayer[dealerIndex].getSum() < 11) { //check to see if the drawn card is an ace & if sum is < 11.
						playerSum += 11;
					}
					else {
						playerSum += tempCard.getCardValue();
					}
					newPlayer[dealerIndex].setSum(playerSum); //set the new sum.
					//cout << endl;
					newPlayer[dealerIndex].printHand(); //display the player's hand.

					if (newPlayer[dealerIndex].getSum() > 21) { //if they go over 21
						newPlayer[dealerIndex].setBlowup(true); //blow up = true.
						newPlayer[dealerIndex].setStand(true); //automatically make the player stand.
						cout << "You've blown up! The current sum of your hand is: " << newPlayer[dealerIndex].getSum() << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}

					else if (newPlayer[dealerIndex].getdealerStatus() == true) {
						if (newPlayer[dealerIndex].getSum() >= 17) {
							cout << "You are the dealer. Because the sum of your hand is greater than 16, you must stand." << endl;
							newPlayer[dealerIndex].setStand(true);
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
				}

				//player chooses to stand
				else if (playerChoice == "s" || playerChoice == "S") {
					newPlayer[dealerIndex].setStand(true);
					cout << "Player " << dealerIndex + 1 << " has chosen to stand." << endl << endl;
					cout << "The final sum of your current hand is: " << newPlayer[dealerIndex].getSum() << endl;
					cout << "Press Enter to Continue.";
					cin.get();
					cout << endl;
				}

				//if invalid input
				else {
					cout << "You've entered an invalid unput. Please try again." << endl;
					playerChoice.clear(); //clear the string.
					cout << endl;
				}
			}
		}

		//if CPU is the dealer 
		else if (newCPU[dealerIndex].getdealerStatus() == true && numCPU > 0) {
			cpuSum = newCPU[dealerIndex].getSum(); //before starting, get the sum of the CPU.
			cout << "It is currently CPU " << dealerIndex + 1 << "'s turn:" << endl;
			cout << endl;
			newCPU[dealerIndex].printHand(); //print CPU hand. (Calculates the sum as well).
			cout << "Press Enter to Continue." << flush;
			cin.ignore();
			cin.get();

			while (newCPU[dealerIndex].getstandStatus() == false) {
				//CPU SUM <= 11, 100% HIT RATE
				if (newCPU[dealerIndex].getSum() <= 11) {
					cout << endl;
					cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
					cout << "Press Enter to Continue." << flush;
					cin.get();					
					cout << endl;
					tempCard = cardDeck->drawfromDeck();
					newCPU[dealerIndex].drawCard(tempCard);
					if (tempCard.getCardValue() == 1 && newCPU[dealerIndex].getSum() < 11) { //check to see if the drawn card is an ace & if sum is < 11.
						cpuSum += 11;
					}
					else {
						cpuSum += tempCard.getCardValue();
					}
					newCPU[dealerIndex].setSum(cpuSum);
					newCPU[dealerIndex].printHand(); //print CPU hand.
					cout << "Press Enter to Continue." << flush;
					cin.get();
				}

				//CPU SUM = 12, 99% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 12) {
					cpuRandNum = rand() % 100; //random number between 0 - 100.
					if (cpuRandNum >= 0 && cpuRandNum <= 99) {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[dealerIndex].drawCard(tempCard);
						cpuSum += tempCard.getCardValue();
						newCPU[dealerIndex].setSum(cpuSum);
						newCPU[dealerIndex].printHand(); //print CPU hand.
						if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
							newCPU[dealerIndex].setBlowup(true);
							newCPU[dealerIndex].setStand(true);
							cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}

				//CPU SUM = 13, 95% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 13) {
					cpuRandNum = rand() % 100; //random number between 0 - 100.
					if (cpuRandNum >= 0 && cpuRandNum <= 95) {
						cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[dealerIndex].drawCard(tempCard);
						cpuSum += tempCard.getCardValue();
						newCPU[dealerIndex].setSum(cpuSum);
						newCPU[dealerIndex].printHand(); //print CPU hand.
						if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
							newCPU[dealerIndex].setBlowup(true);
							newCPU[dealerIndex].setStand(true);
							cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}

				//CPU SUM = 14, 80% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 14) {
					cpuRandNum = rand() % 100; //random number between 0 - 100.
					if (cpuRandNum >= 0 && cpuRandNum <= 80) {
						cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[dealerIndex].drawCard(tempCard);
						cpuSum += tempCard.getCardValue();
						newCPU[dealerIndex].setSum(cpuSum);
						newCPU[dealerIndex].printHand(); //print CPU hand.
						if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
							newCPU[dealerIndex].setBlowup(true);
							newCPU[dealerIndex].setStand(true);
							cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}

				//CPU SUM = 15, 65% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 15) {
					cpuRandNum = rand() % 100; //random number between 0 - 100.
					if (cpuRandNum >= 0 && cpuRandNum <= 65) {
						cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[dealerIndex].drawCard(tempCard);
						cpuSum += tempCard.getCardValue();
						newCPU[dealerIndex].setSum(cpuSum);
						newCPU[dealerIndex].printHand(); //print CPU hand.
						if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
							newCPU[dealerIndex].setBlowup(true);
							newCPU[dealerIndex].setStand(true);
							cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}

				//CPU SUM = 16, 50% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 16) {
					cpuRandNum = rand() % 100; //random number between 0 - 100.
					if (cpuRandNum >= 0 && cpuRandNum <= 50) {
						cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();						
						cout << endl;
						tempCard = cardDeck->drawfromDeck();
						newCPU[dealerIndex].drawCard(tempCard);
						cpuSum += tempCard.getCardValue();
						newCPU[dealerIndex].setSum(cpuSum);
						newCPU[dealerIndex].printHand(); //print CPU hand.
						if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
							newCPU[dealerIndex].setBlowup(true);
							newCPU[dealerIndex].setStand(true);
							cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}

				//CPU SUM = 17, 35% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 17) {
					if (newCPU[dealerIndex].getdealerStatus() == true) { //CHECK TO SEE IF CPU IS DEALER. (MUST CHECK IF SUM IS GREATER THAN 17.)
						newCPU[dealerIndex].setStand(true);
						cout << "CPU #" << dealerIndex + 1 << " is the dealer. Because their sum is greater than 16, they must automatically stand." << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cin.get();
					}
					else {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 35) {
							cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[dealerIndex].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[dealerIndex].setSum(cpuSum);
							newCPU[dealerIndex].printHand(); //print CPU hand.
							if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[dealerIndex].setBlowup(true);
								newCPU[dealerIndex].setStand(true);
								cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
							newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
				}

				//CPU SUM = 18, 25% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 18) {
					if (newCPU[dealerIndex].getdealerStatus() == true) { //CHECK TO SEE IF CPU IS DEALER. (MUST CHECK IF SUM IS GREATER THAN 17.)
						newCPU[dealerIndex].setStand(true);
						cout << "CPU #" << dealerIndex + 1 << " is the dealer. Because their sum is greater than 16, they must automatically stand." << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cin.get();
					}
					else {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 25) {
							cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[dealerIndex].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[dealerIndex].setSum(cpuSum);
							newCPU[dealerIndex].printHand(); //print CPU hand.
							if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[dealerIndex].setBlowup(true);
								newCPU[dealerIndex].setStand(true);
								cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
							newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
				}

				//CPU SUM = 19, 15% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 19) {
					if (newCPU[dealerIndex].getdealerStatus() == true) { //CHECK TO SEE IF CPU IS DEALER. (MUST CHECK IF SUM IS GREATER THAN 17.)
						newCPU[dealerIndex].setStand(true);
						cout << "CPU #" << dealerIndex + 1 << " is the dealer. Because their sum is greater than 16, they must automatically stand." << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cin.get();
					}
					else {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 15) {
							cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[dealerIndex].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[dealerIndex].setSum(cpuSum);
							newCPU[dealerIndex].printHand(); //print CPU hand.
							if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[dealerIndex].setBlowup(true);
								newCPU[dealerIndex].setStand(true);
								cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
							newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
				}

				//CPU SUM = 20, 5% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 20) {
					if (newCPU[dealerIndex].getdealerStatus() == true) { //CHECK TO SEE IF CPU IS DEALER. (MUST CHECK IF SUM IS GREATER THAN 17.)
						newCPU[dealerIndex].setStand(true);
						cout << "CPU #" << dealerIndex + 1 << " is the dealer. Because their sum is greater than 16, they must automatically stand." << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cin.get();
					}
					else {
						cpuRandNum = rand() % 100; //random number between 0 - 100.
						if (cpuRandNum >= 0 && cpuRandNum <= 5) {
							cout << "CPU #" << dealerIndex + 1 << " has chosen to hit." << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();							
							cout << endl;
							tempCard = cardDeck->drawfromDeck();
							newCPU[dealerIndex].drawCard(tempCard);
							cpuSum += tempCard.getCardValue();
							newCPU[dealerIndex].setSum(cpuSum);
							newCPU[dealerIndex].printHand(); //print CPU hand.
							if (newCPU[dealerIndex].getSum() > 21) { //check to see if the CPU blows up.
								newCPU[dealerIndex].setBlowup(true);
								newCPU[dealerIndex].setStand(true);
								cout << "CPU #" << dealerIndex + 1 << " has blown up! The current sum of their hand is: " << newCPU[dealerIndex].getSum() << endl;
								cout << "Press Enter to Continue." << flush;
								cin.get();
								cout << endl;
							}
						}
						else {
							cout << endl;
							cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
							newCPU[dealerIndex].setStand(true); //If the random number is not in range, make the CPU stand.
							cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
							cout << endl;
							cout << "Press Enter to Continue." << flush;
							cin.get();
							cout << endl;
						}
					}
				}

				//CPU SUM = 21 (AUTO STAND), 0% HIT RATE
				else if (newCPU[dealerIndex].getSum() == 21) {
					if (newCPU[dealerIndex].getdealerStatus() == true) {
						newCPU[dealerIndex].setStand(true);
						cout << "CPU #" << dealerIndex + 1 << " is the dealer. Because their sum is greater than 16, they must automatically stand." << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cin.get();
					}
					else {
						cout << endl;
						cout << "CPU #" << dealerIndex + 1 << " has chosen to stand." << endl;
						newCPU[dealerIndex].setStand(true);
						cout << "The final sum of their current hand is: " << newCPU[dealerIndex].getSum() << endl;
						cout << endl;
						cout << "Press Enter to Continue." << flush;
						cin.get();
						cout << endl;
					}
				}
			}
			cpuRandNum = 0; //reset the random number counter.
		} //end dealer CPU turn

			//once all players have stood, calculate the sums of each player. Compare to greatest value.
		int greatestPlayerVal = 0, greatestPlayerIndex = 0;
		int greatestCPUVal = 0, greatestCPUIndex = 0;

		for (int i = 0; i < numPlayers; i++) {
			if (newPlayer[i].getblowupStatus() == false) { //if the player hasn't blown up
				if (newPlayer[i].getSum() > greatestPlayerVal) { //calculate their sum.
					greatestPlayerVal = newPlayer[i].getSum();
					greatestPlayerIndex = i;
				}
			}
		}

		for (int j = 0; j < numCPU; j++) {
			if (newCPU[j].getblowupStatus() == false) { //if the CPU hasn't blown up
				if (newCPU[j].getSum() > greatestCPUVal) { //calculate their sum
					greatestCPUVal = newCPU[j].getSum();
					greatestCPUIndex = j;
				}
			}
		}

		cout << endl; 

		if (greatestPlayerVal == 0 && greatestCPUVal == 0) { //if all players blow up
			cout << "All players have blown up. The game is a draw." << endl;
			cout << endl;
		}

		else if (greatestPlayerVal > greatestCPUVal) { //if player wins
			cout << "Player #" << greatestPlayerIndex + 1 << " wins!" << endl;
			cout << "The sum of their hand was: " << newPlayer[greatestPlayerIndex].getSum() << endl;
		}

		else if (greatestPlayerVal < greatestCPUVal) { //if CPU wins
			cout << "CPU #" << greatestPlayerIndex + 1 << " wins!" << endl;
			cout << "The sum of their hand was: " << newCPU[greatestCPUIndex].getSum() << endl;
		}

		else if (greatestPlayerVal == greatestCPUVal) { //if a player and a CPU tie
			cout << "Player #" << greatestPlayerIndex + 1 << " and CPU #" << greatestCPUIndex + 1 << " tied!" << endl;
			cout << "The sum of both of their hands were: " << newCPU[greatestCPUIndex].getSum() << endl;
		}

		cout << "Press Enter to Continue.";
		cin.get();		
		cout << endl;

		//ending menu
		bool endingMenu = false;
		while (endingMenu == false) {
			cout << "Would you like to play again? (y/n): ";
			playerChoice.clear();
			cin >> playerChoice;

			if (playerChoice == "y" || playerChoice == "Y") {
				delete cardDeck; //free up all allocated memory, recreate the objects.
				delete[] newPlayer;
				delete[] newCPU;
				cardDeck = new Deck();
				endingMenu = true;
				system("CLS");
			}

			else if (playerChoice == "n" || playerChoice == "N") {
				gameEnd = true;
				delete[] newPlayer; //free up all allocated memory
				delete[] newCPU;
				delete cardDeck;
				exit(1); //exit the application.
			}

			else {
				cout << "You've entered an invalid input. Please try again." << endl;
			}
		}

		//reset all variables. 
		newPlayer = nullptr;
		newCPU = nullptr;
		numPlayers = 0, numCPU = 0, cpuRandNum = 0;
		playerSum = 0, cpuSum = 0;
		dealerIndex = 0;
		MenuClear = false, numPlayersMenu = false, numCPUMenu = false;
	}
}


/*
* 1. Create a new deck
* 2. Initialize players (Max 4 human players, MIN 1)
* 3. Initialize CPUs (MAX 3, MIN 0).
*	-> Total Number of CPUs allowed is (4 - # human players)
* 4. Distribute 2 cards to each player & CPU
* 5. Order of game : PLAYER 1, CPU 1, PLAYER 2, CPU 2, ... (Will do dice roll system to see who goes first later) 
* 6. Each player can choose to HIT or STAY.
* 7. If one player chooses to hit, draw a card from the deck and add it to the corresponding player's hand.
* 8. If the player chooses to stay, then the player can no longer hit until the round is over.
* 9. If one player hits and exceeds 21, then the player automatically loses the round.
* 10. After each round, prompt the user to see if they want to play again. (Scoring System WIP)
*	-> if so, reset the game.
*	-> else, end the application.
*/

/*
* SPECIAL RULES :
* a. If a player draws a 1 (ace), and if the sum of the players hand is < 11, then the card is automatically converted into an 11.
*/