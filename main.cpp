#include<string>
#include<vector>
#include<iostream>
#include "Board.h"

using namespace std;

int main() {
	//std::cout << "Welcome to Monopoly!" << std::endl;
	int option = 2;
	int numberGames = 1000;
	int numberTurns = 100;
	int numberPlayers = 4;
	string fileString = "newSpots.csv";//file to import monopoly properties and misc spaces from
	vector<int> winningPlayerHistory;
	vector<int> winningTurn;
	Board board(numberPlayers, fileString);//constructor, also calls importSpots 
	board.mainMenu();//menu to decide to change settings or not

	for (int j = 0; j < numberGames; j++) {
		//Game is played until 1 player remains
		bool gameConcluded = false;
		while (gameConcluded == false) {
			for (int i = 0; i < numberPlayers; i++) {
				board.takeTurn(i);
			}
			gameConcluded = board.IsGameConcluded();
			if (board.GetTotalTurns() > 10000) {
				winningPlayerHistory.push_back(-1);
				winningTurn.push_back(board.GetTotalTurns());
				cout << "Game over 10000 turns, exiting early..." << endl;
				break;
			}
		}
		board.printResults();
		winningPlayerHistory.push_back(board.GetWinningPlayerIndex());
		winningTurn.push_back(board.GetTotalTurns());
		cout << "Thanks for playing!" << endl;
		board.Clear(fileString);
	}

	//make binning vector
	vector<int> winTotal;
	for (int i = 0; i < numberPlayers; i++) {
		winTotal.push_back(0);
	}

	//Print results to consol
	for (int i = 0; i < winningTurn.size(); i++) {
		cout << "Player " << winningPlayerHistory.at(i) << " won on turn " << winningTurn.at(i) << endl;
		if (winningPlayerHistory.at(i) != -1) {
			winTotal.at(winningPlayerHistory.at(i)) = winTotal.at(winningPlayerHistory.at(i)) + 1;
		}
	}
	cout << endl;
	cout << "Total games: " << winningPlayerHistory.size() << endl;
	//print binned results
	for (int i = 0; i < winTotal.size(); i++) {
		cout << "Player " << i << " win rate " << (double)winTotal.at(i)/(double)winningPlayerHistory.size() * 100 << " %" << endl;
	}
	
	

	return 0;
}