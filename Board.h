#pragma once
#include<vector>
#include<queue>
#include<string>
#include"spot.h"
#include"player.h"

class Board
{
public:
	
	//Functions for board
	Board(int numPlayers, std::string spotsFile);
	void importSpots(std::string fileName);//make this branched to support non-normal spots
	void mainMenu();
	void takeTurn(int playerID);
	void landedOnProperty(int playerID, int spotIndex);//takes care of all outcomes for landing on properties
	void printResults();
	void CheckMonopoly(spot* checkSpot);
	void ProposeTrade(int playerID);
	bool IsGameConcluded();
	void Chance(int playerID);
	void CommunityChest(int playerID);
	void ShuffleCards(queue<int>* deck);
	void PayBank(int playerID, int amount);

	//Functions for players
	void PayPlayer(int payerID, int payeeID, int amount);
	void TradeProperty(int playerID1, int playerID2, int spotIndex1, int spotIndex2);
	void BuildHouses(int playerID);


private:
	std::vector<spot> spaces;
	std::vector<player> players;
	std::queue<int> chanceDeck;
	std::queue<int> ccDeck;

};

