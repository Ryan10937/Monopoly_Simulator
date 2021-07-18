#include<string>
#include<vector>
#include<iostream>
#include "Board.h"

using namespace std;

int main() {
	std::cout << "Welcome to Monopoly!" << std::endl;
	int option = 2;
	int numberGames = 1;
	int numberTurns = 100;
	int numberPlayers = 4;

	Board board(numberPlayers, "newSpots.csv");

	board.mainMenu();
	
	switch (option) {
	//set specific number of turns. Currently not available from the menu
	case(1):
		for (int k = 0; k < numberGames; k++) {
			for (int j = 0; j < numberTurns; j++) {
				for (int i = 0; i < numberPlayers; i++) {
					board.takeTurn(i);

				}
			}
			board.printResults();
			cout << "Thanks for playing!" << endl;
		}
		break;
	//Default, game is played until 1 player remains
	case(2):
		bool gameConcluded = false;
		while (gameConcluded == false) {
			for (int i = 0; i < numberPlayers; i++) {
				board.takeTurn(i);
			}
			gameConcluded = board.IsGameConcluded();
		}
		board.printResults();
		cout << "Thanks for playing!" << endl;
		break;


	}
		
	
	

	return 0;
}