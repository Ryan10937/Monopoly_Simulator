#include "Board.h"
#include "iAmHere.h"
#include "ryanRandom.h"
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <random>
#include <map>
using namespace std;
Board::Board(int numPlayers, string spotsFile)
{
	//load spots
	importSpots(spotsFile);

	//make base of players
	for (int i = 0; i < numPlayers; i++) {
		player newPlayer;
		newPlayer.SetName("Player " + to_string(i));
		newPlayer.SetPosition(0);
		newPlayer.SetID(i);
		newPlayer.SetIsBankrupt(false);
		newPlayer.SetJailCounter(0);
		players.push_back(newPlayer);
	}
	ShuffleCards(&chanceDeck);
	ShuffleCards(&ccDeck);

	totalTurns = 0;
	winningPlayerIndex = 0;
	didImportFiles = false;
}

void Board::importSpots(std::string fileName)
{
	ifstream inFile;
	inFile.open(fileName);
	std::smatch m;
	std::regex normalSpots("^([^,]+),(\\d+),(\\d+),(\\d+),(\\d+),(\\d+),(\\d+),(\\w+),(\\d),(\\d+)");
	std::regex RRSpots("^([^,]+),(\\d+),(\\d+),(\\d+),(\\d+),,,,,(\\d+)");
	std::regex taxSpots("^([^,]+),(\\d+),,,,,,,");
	std::regex blankSpots("^([^,]+),,,,,,,,");
	std::regex companySpots("^([^,]+),,,,,,,,,(\\d+)");
	int counter = -1;
	
	
	while (!inFile.eof()) {
		spot tempSpot;
		counter++;
		string importSpot;
		getline(inFile, importSpot);
		//cout << "Trying: " << importSpot << endl;
		//use regex to pull name and rents from string
		if (regex_match(importSpot, m, blankSpots)) {
			//pull out stuff
			tempSpot.name = m[1];
			tempSpot.index = counter;
			tempSpot.isNormal = false;
			tempSpot.monopolyColor = "Board";
			tempSpot.ownerID = -2;



		}
		else if (regex_match(importSpot, m, companySpots)) {
			//pull out stuff
			tempSpot.name = m[1];
			tempSpot.value = stoi(m[2]);
			tempSpot.index = counter;
			tempSpot.isNormal = false;
			tempSpot.monopolyColor = "Company";
			tempSpot.monopolyNumber = 2;


		}
		else if (regex_match(importSpot, m, RRSpots)) {
			//pull out stuff
			tempSpot.name = m[1];
			tempSpot.rent.push_back(stoi(m[2]));
			tempSpot.rent.push_back(stoi(m[3]));
			tempSpot.rent.push_back(stoi(m[4]));
			tempSpot.rent.push_back(stoi(m[5]));
			tempSpot.value = stoi(m[6]);
			tempSpot.index = counter;
			tempSpot.isNormal = false;
			tempSpot.currentRent = tempSpot.rent.at(0);
			tempSpot.monopolyNumber = 4;
			tempSpot.monopolyColor = "Black";



		}
		else if (regex_match(importSpot, m, taxSpots)) {
			//pull out stuff
			tempSpot.name = m[1];
			tempSpot.rent.push_back(stoi(m[2]));
			tempSpot.index = counter;
			tempSpot.isNormal = false;
			tempSpot.monopolyColor = "Board";
			tempSpot.ownerID = -2;

		}
		else if (regex_match(importSpot, m, normalSpots)) {
			//pull out stuff
			tempSpot.name = m[1];
			tempSpot.rent.push_back(stoi(m[2]));
			tempSpot.rent.push_back(stoi(m[3]));
			tempSpot.rent.push_back(stoi(m[4]));
			tempSpot.rent.push_back(stoi(m[5]));
			tempSpot.rent.push_back(stoi(m[6]));
			tempSpot.rent.push_back(stoi(m[7]));
			tempSpot.monopolyColor = m[8];
			tempSpot.monopolyNumber = stoi(m[9]);
			tempSpot.value = stoi(m[10]);
			tempSpot.index = counter;
			tempSpot.isNormal = true;
			tempSpot.currentRent = tempSpot.rent.at(0);
			tempSpot.numHouses = 0;

		}
		spaces.push_back(tempSpot);
	}
	/*for (int i = 0; i < spaces.size(); i++) {
		std::cout << spaces.at(i).name << std::endl;
	}*/
}

int Board::mainMenu()
{
	std::cout << "Welcome to Monopoly" << std::endl;
	std::cout << "Choose an option below: " << std::endl;

	std::cout << "1). Start with default settings\n2). Modify settings\n" << std::endl;
	std::string boardInput;
	std::string input;
	std::string gamesInput;
	int numGames = -1;
	cin >> input;
	switch (stoi(input)) {
	case(1):
		//do nothing and return to main
		return -1;
		break;
	case(2):
		//change stuff
		do {
			cout << "Choose a modification category:\n1).Board\n2).Players\n3).Exit and Start Game\n";
			cin >> input;
			switch (stoi(input)) {
			case(1):
				cout << "Choose a board modification category:\n1). Number of Games\n2). Exit" << endl;
				getline(cin, boardInput);
				getline(cin, boardInput);
				switch (stoi(boardInput)) {
				case(1):
					cout << "Enter number of games:\n";
					getline(cin, gamesInput);
					numGames = stoi(gamesInput);
					cout << "Number of games set to " << numGames << endl;
				break;
				case(2):
					cout << "Exiting" << endl;
				}
				//limit total number of houses?
				//rent multiplier
				//verbose?
				//chance cards
				//add spots?
				//delete spots?
				//turn off GO money?
				//total games?
				//set number of players
				break;
			case(2):

				cout << "Enter the player number followed by the qualities you'd like to change with thier appropriate flags (example below):\n\n" << endl;
				cout << "-player 2 -money 5000 -pos 15 -property {Boardwalk, Park Place, B. & O. Railroad}\n" << endl;
				cout << "To show full list of spots, type '-spots'\n" << endl;
				cout << "To import a series of commands, type 'fileName.txt'\n" << endl;
				string inputString;
				getline(cin, inputString);//get rid of newline from cin
				getline(cin, inputString);
				regex importEdits("(.*.txt)");
				smatch matcher;
				regex_search(inputString, matcher, importEdits);
				if (matcher.size() > 0) {
					didImportFiles = true;
					ifstream inFile;
					inFile.open(matcher[1].str());
					string fileString;
					while (!inFile.eof()) {
						getline(inFile, fileString);
						importedEdits.push_back(fileString);
						ApplyGameEdits(fileString);
					}
				}
				else {
					importedEdits.push_back(inputString);
					ApplyGameEdits(inputString);
				}
				break;
			}
			
			}while (stoi(input) != 3);
			break;
	default:
		std::cout << "Unexpected input" << std::endl;
		break;
	}//end of switch-1
	return numGames;
}//end of mainMenu()

void Board::takeTurn(int playerID)
{
	if (players.at(playerID).GetIsBankrupt()) {
		return;
	}

	//increment turn counter
	totalTurns++;
	if (players.at(playerID).GetJailCounter() > 0) {
		players.at(playerID).SetJailCounter(players.at(playerID).GetJailCounter() + 1);
		if (players.at(playerID).GetJailCounter() < 4) {
			cout << players.at(playerID).GetName() << " is in jail!" << endl;

			return;
		}
		else {
			cout << players.at(playerID).GetName() << " is out of jail!" << endl;
			players.at(playerID).SetJailCounter(0);
		}
	}

	cout << endl;

	//roll and move player on the board
	int roll =  (abs((ryanRandom(time(0)) % 6) + 1) + abs((ryanRandom(time(0)) % 6) + 1));
	players.at(playerID).SetPosition(players.at(playerID).GetPosition() + roll);
	
	
	//if they pass go, mod with length of board and add $200
	if (players.at(playerID).GetPosition() > spaces.size()-1) {
		players.at(playerID).SetPosition(players.at(playerID).GetPosition() % spaces.size());
		cout << players.at(playerID).GetName() << " passed GO! Collect $200!" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);
	}
	cout << players.at(playerID).GetName() << " rolled a " << roll << " and landed on " << spaces.at(players.at(playerID).GetPosition()).name << endl;
	if (players.at(playerID).GetPosition() == 30) {
		cout << players.at(playerID).GetName() << " is in jail! " << endl;
		players.at(playerID).SetPosition(10);
		players.at(playerID).SetJailCounter(1);
	}
	landedOnProperty(playerID,players.at(playerID).GetPosition());
	
	if (players.at(playerID).GetIsBankrupt() == false) {
		//decide to trade
		ProposeTrade(playerID);

		//decide to build
		BuildHouses(playerID);
	}
	


}

void Board::landedOnProperty(int playerID, int spotIndex)
{
	if (spaces.at(spotIndex).ownerID == -2) {
		//player landed on a board spot
		if (spaces.at(spotIndex).name == "Chance") {
			//do chance stuff
			/*
			//for now, a placeyholder 50/50 to get/take $20 will do
			if (ryanRandom(time(0)) % 2 == 1) {
				cout << "Chance was in your favor, collect $20!" << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 20);
			}
			else{
				cout << "Chance was NOT in your favor, pay $20..." << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 20);
			}*/
			Chance(playerID);

		}
		else if(spaces.at(spotIndex).name == "Community Chest") {
			//do CC stuff
			//for now, a placeyholder 50/50 to get/take $20 will do
			/*if (ryanRandom(time(0)) % 2 == 1) {
				cout << "Chance was in your favor, collect $20!" << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 20);
			}
			else {
				cout << "Chance was NOT in your favor, pay $20..." << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 20);
			}*/
			CommunityChest(playerID);

		} 
		else if(spaces.at(spotIndex).name == "GO" || spaces.at(spotIndex).name == "Free Parking" || spaces.at(spotIndex).name == "Jail"){
			//do nothing
			cout << "Take a load off, enjoy the free space" << endl;
		}
		else if (spaces.at(spotIndex).name == "Luxury Tax") {
			//pay $75
			cout << players.at(playerID).GetName() << " paid $75 for Luxury Tax" << endl;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 75);

		}
		else if (spaces.at(spotIndex).name == "Income Tax") {
			//pay $200
			cout << players.at(playerID).GetName() << " paid $200 for Income Tax" << endl;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 200);

		}
	}
	else if (spaces.at(spotIndex).ownerID == -1) {
		//bank owns, can buy/auction
		if (players.at(playerID).doIBuy(spaces.at(spotIndex))) {
			spaces.at(spotIndex).ownerID = playerID;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() - spaces.at(spotIndex).value);
			cout << players.at(playerID).GetName() << " bought " << spaces.at(spotIndex).name <<"!" << endl;
			//adjust rent for player properties if a monopoly is made
			CheckMonopoly(&spaces.at(spotIndex));
		}
		else {
			cout << players.at(playerID).GetName() << " chose not to buy " << spaces.at(spotIndex).name << "!" << endl;
		}
	}

	else {
		//player landed on an owned spot
		//if player owns it, display message and do not pay rent
		if (players.at(playerID).GetID() == spaces.at(spotIndex).ownerID) {
			cout << "Phew! I own this" << endl;
			return;
		}
		else {
			//someone owns it. Pay them the rent on it
			if (spaces.at(spotIndex).monopolyColor != "Company") {
				PayPlayer(playerID, spaces.at(spotIndex).ownerID, spaces.at(spotIndex).currentRent);
			}
			else {
				//player landed on an owned company spot
				int roll = (abs((ryanRandom(time(0)) % 6) + 1) + abs((ryanRandom(time(0)) % 6) + 1));
				int companyCounter = 0;
				//determine how many company spots the owner of the property owns
				for (int i = 0; i < spaces.size(); i++) {
					if (spaces.at(i).ownerID == spaces.at(spotIndex).ownerID && spaces.at(i).monopolyColor == "Company") {
						companyCounter++;
					}
				}
				if(companyCounter == spaces.at(spotIndex).monopolyNumber -1 ){
					PayPlayer(playerID, spaces.at(spotIndex).ownerID, roll * 4);
				}
				else if (companyCounter == spaces.at(spotIndex).monopolyNumber) {
					PayPlayer(playerID, spaces.at(spotIndex).ownerID, roll * 10);
				}
				
			}
		}

	}
}

void Board::printResults()
{
	for (int i = 0; i < players.size(); i++) {
		cout << players.at(i).GetName() << ": " << players.at(i).GetMoney() << " : ";
		for (int j = 0; j < spaces.size(); j++) {
			if (spaces.at(j).ownerID == players.at(i).GetID()) {
				cout << spaces.at(j).name<<":" << spaces.at(j).numHouses <<"\t ";
			}
			
		}
		cout << endl << endl;
	}
}

void Board::CheckMonopoly(spot* checkSpot)
{
	//if the spot is a normal property spot 
	if (checkSpot->isNormal ) {
		int colorCounter = 0;
		int ownerID = checkSpot->ownerID;
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == ownerID && spaces.at(i).monopolyColor == checkSpot->monopolyColor ) {
				colorCounter++;
			}
		}

		if (colorCounter == checkSpot->monopolyNumber) {
			
			cout << checkSpot->name << " makes a monopoly! Let the cash flow" << endl;
			for (int i = 0; i < spaces.size(); i++) {//double all rents of that color that made the monopoly
				if (spaces.at(i).monopolyColor == checkSpot->monopolyColor && checkSpot->numHouses == 0) {
					spaces.at(i).currentRent = spaces.at(i).rent.at(0) * 2;
					spaces.at(i).isMonopoly = true;
				}
			}
		}
	}
	//if railroad
	else if (checkSpot->monopolyColor == "Black") {
		int colorCounter = 0;
		int ownerID = checkSpot->ownerID;
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == ownerID && spaces.at(i).monopolyColor == checkSpot->monopolyColor) {
				colorCounter++;
			}
		}
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == ownerID && spaces.at(i).monopolyColor == checkSpot->monopolyColor) {
				spaces.at(i).currentRent = spaces.at(i).rent.at(colorCounter - 1);
			}
		}
	}
}

void Board::ProposeTrade(int playerID)
{
	map<string, vector<spot>> inventories;
	//look through all spots, find a group of colors that has the player ID and the player ID of another player
		// possible combinations of this find are 1-1,1-2, and the reverses of those
		// be sure to filter out bank as owner

	//bin by color
	for (int i = 0; i < spaces.size(); i++) {
		if (inventories.find(spaces.at(i).monopolyColor) != inventories.end()) {
			//color found. push to vector
			inventories.at(spaces.at(i).monopolyColor).push_back(spaces.at(i));
		}
		else {
			//color not found. Make new vector, then push that color
			vector<spot> tempSpaces;
			tempSpaces.push_back(spaces.at(i));
			inventories.insert(make_pair(spaces.at(i).monopolyColor, tempSpaces));
		}
	}
	
	for (std::map<string, vector<spot>>::iterator it = inventories.begin(); it != inventories.end(); ++it) {

		//we want to find TWO pairs and consolidate

		int player1ColorCounter = 0;
		int player1SpotWanted = -2;
		for (int i = 0; i < it->second.size(); i++) {
			if (it->second.at(i).ownerID == playerID) {
				player1ColorCounter++;
			}
			else {
				//capture the spot the player doesnt have
				player1SpotWanted = it->second.at(i).index;
				
			}
		}
		if (player1SpotWanted == -2 || player1ColorCounter == 0) {
			//player owns a monopoly on that color OR 
			continue;
		}

		if (player1ColorCounter == it->second.at(0).monopolyNumber - 1 && spaces.at(player1SpotWanted).ownerID  != -1 && spaces.at(player1SpotWanted).ownerID != playerID) {
			// then player 1 wants to trade.
			//search the owner of player1SpotWanted to see if they want anything player1 has
			
			for (std::map<string, vector<spot>>::iterator it2 = inventories.begin(); it2 != inventories.end(); ++it2) {
				int player2ColorCounter = 0;
				int player2SpotWanted = -2; 
				for (int i = 0; i < it2->second.size(); i++) {
					if (it2->second.at(i).ownerID == spaces.at(player1SpotWanted).ownerID) {
						player2ColorCounter++;
					}
					else {
						//capture the spot the player doesnt have
						player2SpotWanted = it2->second.at(i).index;

					}
				}
				if (player2SpotWanted == -2 || player2ColorCounter == 0 || spaces.at(player1SpotWanted).monopolyColor == spaces.at(player2SpotWanted).monopolyColor) {
					//player owns a monopoly on that color or wants the same color that player1 wants
					continue;
				}
				//if (player2ColorCounter == it2->second.at(0).monopolyNumber - 1 && spaces.at(player2SpotWanted).ownerID == playerID && spaces.at(player2SpotWanted).ownerID != spaces.at(player1SpotWanted).ownerID) {
				if (player2ColorCounter == it2->second.at(0).monopolyNumber - 1 && spaces.at(player2SpotWanted).ownerID == playerID) {
					//then player1 has something that player2 wants. They can now trade.
					TradeProperty(playerID, spaces.at(player1SpotWanted).ownerID,player2SpotWanted,player1SpotWanted);
					CheckMonopoly(&spaces.at(player1SpotWanted));
					CheckMonopoly(&spaces.at(player2SpotWanted));
					return;

				}
			}

			break;//this limits to one trade per turn
		}
	}
		
}

bool Board::IsGameConcluded()
{
	int playersWithMoney = 0;
	for (int i = 0; i < players.size(); i++) {
		if (!players.at(i).GetIsBankrupt()) {
			playersWithMoney++;
			winningPlayerIndex = i;
		}
	}
	return playersWithMoney == 1;
}

void Board::Chance(int playerID)
{
	int chanceResult = chanceDeck.front();
	chanceDeck.pop();
	int houseCounter = 0;
	switch (chanceResult) {
	case(0):
		cout << "Advance to Boardwalk!" << endl;
		players.at(playerID).SetPosition(39);
		landedOnProperty(playerID,39);
		break;
	case(1):
		cout << "Advance to GO and collect $200!" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);
		landedOnProperty(playerID, 0);
		break;
	case(2):
		cout << "Advance to Illinois Avenue. If you pass Go, collect $200!" << endl;
		if (players.at(playerID).GetPosition() > 24) {
			//collect 200 if in front of this space
			cout << players.at(playerID).GetName() << " collected $200!" << endl;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);

		}
		players.at(playerID).SetPosition(24);
		landedOnProperty(playerID, 24);
		break;
	case(3):
		cout << "Advance to St. Charles Place. If you pass Go, collect $200!" << endl;
		if (players.at(playerID).GetPosition() > 11) {
			//collect 200 if in front of this space
			cout << players.at(playerID).GetName() << " collected $200!" << endl;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);

		}
		players.at(playerID).SetPosition(11);
		landedOnProperty(playerID, 11);
		break;
	case(4):
		cout << "Advance to the nearest Railroad. If unowned, you may buy it from the Bank. If owned, pay wonder twice the rental to which they are otherwise entitled" << endl;
		if (players.at(playerID).GetPosition() > 35 || players.at(playerID).GetPosition() < 5) {
			if (players.at(playerID).GetPosition() > 35) {
				//collect 200 if in front of this space
				cout << players.at(playerID).GetName() << " collected $200!" << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);

			}
			players.at(playerID).SetPosition(5);
			landedOnProperty(playerID, 5);
			landedOnProperty(playerID, 5);
		}
		else if (players.at(playerID).GetPosition() < 15) {
			players.at(playerID).SetPosition(15);
			landedOnProperty(playerID, 15);
			landedOnProperty(playerID, 15);
		}
		else if (players.at(playerID).GetPosition() < 25) {
			players.at(playerID).SetPosition(25);
			landedOnProperty(playerID, 25);
			landedOnProperty(playerID, 25);
		}
		else if (players.at(playerID).GetPosition() < 35) {
			players.at(playerID).SetPosition(35);
			landedOnProperty(playerID, 35);
			landedOnProperty(playerID, 35);
		}
		
		break;
	case(5):
		cout << "Advance to the nearest Railroad. If unowned, you may buy it from the Bank. If owned, pay wonder twice the rental to which they are otherwise entitled" << endl;
		if (players.at(playerID).GetPosition() > 35 || players.at(playerID).GetPosition() < 5) {
			players.at(playerID).SetPosition(5);
			landedOnProperty(playerID, 5);
			landedOnProperty(playerID, 5);
		}
		else if (players.at(playerID).GetPosition() < 15) {
			players.at(playerID).SetPosition(15);
			landedOnProperty(playerID, 15);
			landedOnProperty(playerID, 15);
		}
		else if (players.at(playerID).GetPosition() < 25) {
			players.at(playerID).SetPosition(25);
			landedOnProperty(playerID, 25);
			landedOnProperty(playerID, 25);
		}
		else if (players.at(playerID).GetPosition() < 35) {
			players.at(playerID).SetPosition(35);
			landedOnProperty(playerID, 35);
			landedOnProperty(playerID, 35);
		}
		break;
	case(6):
		cout << "Advance token to nearest Utility. If unowned, you may buy it from the Bank." << endl;
		if (players.at(playerID).GetPosition() > 28) {
			if (players.at(playerID).GetPosition() > 11) {
				//collect 200 if in front of this space
				cout << players.at(playerID).GetName() << " collected $200!" << endl;
				players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);
			}
			players.at(playerID).SetPosition(12);
			landedOnProperty(playerID, 12);
		}
		else if (players.at(playerID).GetPosition() > 12) {
			players.at(playerID).SetPosition(28);
			landedOnProperty(playerID, 28);
		}
		break;
	case(7):
		cout << "Bank pays you dividend of $50" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 50);
		break;
	case(8):
		//should be get out of jail free
		cout << "Bank pays you dividend of $50" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 50);
		break;
	case(9):
		cout << "Go back 3 spaces" << endl;
		players.at(playerID).SetPosition(players.at(playerID).GetPosition() - 3);//thankfully, there are no chance spaces near GO
		break;
	case(10):
		cout << "Go to Jail. Go directly to Jail, do not pass Go, do not collect $200" << endl;
		players.at(playerID).SetPosition(10);
		players.at(playerID).SetJailCounter(1);
		break;
	case(11):
		cout << "Make general repairs on all your property. For each house pay $25." << endl;
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == playerID) {
				houseCounter++;
			}
		}
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 25 * houseCounter);
		cout << players.at(playerID).GetName() << " paid $" << 25 * houseCounter << " to the bank." << endl;
		if (players.at(playerID).GetMoney() < 0) {
			players.at(playerID).SetIsBankrupt(1);
			cout << players.at(playerID).GetName() << " went bankrupt!" << endl;
		}
		break;
	case(12):
		cout << "Speeding fine $15" << endl;
		cout << players.at(playerID).GetName() << " paid $15 to the bank." << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 15);
		if (players.at(playerID).GetMoney() < 0) {
			players.at(playerID).SetIsBankrupt(1);
			cout << players.at(playerID).GetName() << " went bankrupt!" << endl;
		}
		break;
	case(13):
		cout << "Take a trip to Reading Railroad. If you pass Go, collect $200" << endl;
		if (players.at(playerID).GetPosition() > 5) {
			//collect 200 if in front of this space
			cout << players.at(playerID).GetName() << " collected $200!" << endl;
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);

		}
		players.at(playerID).SetPosition(5);
		landedOnProperty(playerID, 5);
		break;
	case(14):
		cout << "You have been elected Chairman of the Board. Pay each player $50" << endl;
		for (int i = 0; i < players.size(); i++) {
			if (players.at(i).GetID() != playerID) {
				PayPlayer(playerID, players.at(i).GetID(), 50);
			}
		}
		break;
	case(15):
		cout << "Your building loan matures. Collect $150" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 150);
		break;
	default:
		cout << "unexpected output" << endl;
		break;
	
	
	};
	
	//if empty, reshuffle
	if (chanceDeck.size() == 0) {
		ShuffleCards(&chanceDeck);
	}


}

void Board::CommunityChest(int playerID)
{
	int chanceResult = ccDeck.front();
	ccDeck.pop();
	int houseCounter = 0;

	switch (chanceResult) {
	case(0):
		cout << "Advance to GO and collect $200!" << endl;
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() + 200);
		landedOnProperty(playerID, 0);
		break;
	case(1):
		cout << "Bank error in your favor. Collect $200!" << endl;
		PayBank(playerID, -200);
		break;
	case(2):
		cout << "Doctor’s fee. Pay $50" << endl;
		PayBank(playerID, 20);
		break;
	case(3):
		cout << "From sale of stock you get $50" << endl;
		PayBank(playerID, -50);
		break;
	case(4):
		//should be get out of jail free
		cout << "From sale of stock you get $50" << endl;
		PayBank(playerID, -50);
		break;
	case(5):
		cout << "Go to Jail. Go directly to Jail, do not pass Go, do not collect $200" << endl;
		players.at(playerID).SetPosition(10);
		players.at(playerID).SetJailCounter(1);
		break;
	case(6):
		cout << "Holiday fund matures. Receive $100" << endl;
		PayBank(playerID, -100);
		break;
	case(7):
		cout << "Income tax refund. Collect $20" << endl;
		PayBank(playerID, -20);
		break;
	case(8):
		cout << "It is your birthday. Collect $10 from every player" << endl;
		for (int i = 0; i < players.size(); i++) {
			if (players.at(i).GetID() != playerID) {
				PayPlayer(players.at(i).GetID(), playerID, 10);
			}
		}
		break;
	case(9):
		cout << "Life insurance matures. Collect $100" << endl;
		PayBank(playerID, -100);
		break;
	case(10):
		cout << "Pay hospital fees of $100" << endl;
		PayBank(playerID, 100);
		break;
	case(11):
		cout << "Pay school fees of $50" << endl;
		PayBank(playerID, 50);
		break;
	case(12):
		cout << "You are assessed for street repair. $40 per house." << endl;
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == playerID) {
				houseCounter++;
			}
		}
		players.at(playerID).SetMoney(players.at(playerID).GetMoney() - 40 * houseCounter);
		cout << players.at(playerID).GetName() << " paid $" << 40 * houseCounter << " to the bank." << endl;
		if (players.at(playerID).GetMoney() < 0) {
			players.at(playerID).SetIsBankrupt(1);
			cout << players.at(playerID).GetName() << " went bankrupt!" << endl;
		}
		break;
	case(13):
		cout << "You have won second prize in a beauty contest. Collect $10" << endl;
		PayBank(playerID, -10);
		break;
	case(14):
		cout << "You inherit $100" << endl;
		PayBank(playerID, -100);
		break;
	case(15):
		cout << "Receive $25 consultancy fee" << endl;
		PayBank(playerID, -25);
		break;
	default:
		cout << "unexpected output" << endl;
		break;


	};

	//if empty, reshuffle
	if (ccDeck.size() == 0) {
		ShuffleCards(&ccDeck);
	}


}

void Board::ShuffleCards(queue<int>* deck)
{
	//pop any extra cards still in deck
	for (int i = 0; i < deck->size(); i++) {
		deck->pop();
	}
	vector<pair<bool, int>> cardNumbers;
	for (int i = 0; i < 16; i++) {
		cardNumbers.push_back(make_pair(true , i));
	}
	for (int j = 0; j < 16; j++) {
		vector<int> trueNumbers;
		for (int i = 0; i < 16; i++) {
			if (cardNumbers.at(i).first) {
				trueNumbers.push_back(i);
			}
		}
		int randomIndex = (ryanRandom(time(0)) % trueNumbers.size());
		deck->push(cardNumbers.at(trueNumbers.at(randomIndex)).second);
		cardNumbers.at(trueNumbers.at(randomIndex)).first = false;
	}

	/*
	//for debug
	for (int i = 0; i < 16; i++) {
		cout << deck.front() << endl;
		deck.pop();
	}*/
}

void Board::PayBank(int playerID, int amount)
{
	players.at(playerID).SetMoney(players.at(playerID).GetMoney() - amount);

	if (amount > 0) {
		cout << players.at(playerID).GetName() << " paid $" << amount << " to the bank." << endl;
		if (players.at(playerID).GetMoney() < 0) {
			players.at(playerID).SetIsBankrupt(1);
			cout << players.at(playerID).GetName() << " went bankrupt!" << endl;
		}
	}
	else {
		cout << players.at(playerID).GetName() << " received $" << amount << " from the bank." << endl;

	}
	

}

void Board::ApplyGameEdits(string inputString)
{
	regex playerEdit("-player (-?\\d+)");
	regex playerEditMoney("-money (-?\\d+)");
	regex playerEditPosition("-pos (-?\\d+)");
	regex playerEditProperty("-property (.*)");
	regex showPropertiesFlag("-spots");
	smatch sm;
	int playerNumber = -1;
	
	//get show properties flag if available
	regex_search(inputString, sm, showPropertiesFlag);
	if (sm.size() > 0) {
		cout << "\nList of Properties: " << endl;
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).isNormal || spaces.at(i).name == "Reading Railroad" || spaces.at(i).name == "B. & O. Railroad" || spaces.at(i).name == "Pennsylvania Railroad" || spaces.at(i).name == "Short Line Railroad")
				cout << spaces.at(i).name << " owned by " << spaces.at(i).ownerID << endl;
		}
		cout << endl;
		return;
	}

	//get player number from string
	regex_search(inputString, sm, playerEdit);
	if (sm.size() > 0) {
		playerNumber = stoi(sm[1]);
		if (playerNumber >= players.size() || playerNumber < 0) {
			cout << "player number out of range (0-" << players.size() << ")" << endl;
			return;
		}
		else {
			cout << "player " << sm[1];
		}
	}
	else {
		cout << "player flag not found" << endl;
		return;
	}

	//get player money from string
	regex_search(inputString, sm, playerEditMoney);
	if (sm.size() > 0) {
		if (stoi(sm[1]) >= 20000000 || stoi(sm[1]) < 0) {
			cout << "money value out of range" << endl;
		}
		else {
			cout << " money set to " << sm[1];
			players.at(playerNumber).SetMoney(stoi(sm[1]));
		}
	}

	//get player position from string
	regex_search(inputString, sm, playerEditPosition);
	if (sm.size() > 0) {
		if (stoi(sm[1]) >= spaces.size() || stoi(sm[1]) < 0) {
			cout << "board index out of range(0-" << spaces.size() << ")" << endl;
		}
		else {
			cout << ", position set to " << sm[1];
			players.at(playerNumber).SetPosition(stoi(sm[1]));
		}

	}

	cout << endl;
	//get player properties from string
	//because this is a list of n length, this will have to work a little differently
	regex_search(inputString, sm, playerEditProperty);
	vector<string> propertiesToAdd;
	if (sm.size() > 0) {
		//property flag was found
		int lastComma = 1;
		int thisComma;
		string match = sm[1];
		for (int i = 0; i < match.length(); i++) {
			if (match.at(i) == ',' || match.at(i) == '}') {
				thisComma = i;
				propertiesToAdd.push_back(match.substr(lastComma, thisComma - lastComma));
				lastComma = thisComma + 2;
			}
		}
	}

	for (int i = 0; i < propertiesToAdd.size(); i++) {
		bool foundProperty = false;
		for (int j = 0; j < spaces.size(); j++) {
			if (spaces.at(j).name == propertiesToAdd.at(i)) {
				spaces.at(j).ownerID = playerNumber;
				cout << propertiesToAdd.at(i) << " added to " << players.at(playerNumber).GetName() << "'s inventory" << endl;
				CheckMonopoly(&spaces.at(j));
				foundProperty = true;
			}
		}
		if (!foundProperty) {
			cout << propertiesToAdd.at(i) << " not found! Property was not added to " << players.at(playerNumber).GetName() << "'s inventory" << endl;
		}
	}


	cout << endl;
}

int Board::GetWinningPlayerIndex()
{
	return winningPlayerIndex;
}

int Board::GetTotalTurns()
{
	return totalTurns;
}

void Board::Clear(string spotsFile)
{
	int numPlayers = players.size();
	spaces.clear();
	players.clear();
	//load spots
	importSpots(spotsFile);

	//make base of players
	for (int i = 0; i < numPlayers; i++) {
		player newPlayer;
		newPlayer.SetName("Player " + to_string(i));
		newPlayer.SetPosition(0);
		newPlayer.SetID(i);
		newPlayer.SetIsBankrupt(false);
		newPlayer.SetJailCounter(0);
		players.push_back(newPlayer);
	}
	ShuffleCards(&chanceDeck);
	ShuffleCards(&ccDeck);

	totalTurns = 0;
	winningPlayerIndex = 0;
	
	for (int i = 0; i < importedEdits.size(); i++) {
		ApplyGameEdits(importedEdits.at(i));
	}

}

void Board::PayPlayer(int payerID, int payeeID, int amount)
{
	if (players.at(payerID).GetMoney() > amount) {
		//if the payer has enough money, pay the amount
		cout << players.at(payerID).GetName() << " paid " << players.at(payeeID).GetName() << " " << amount << " dollars!" << endl;
		players.at(payeeID).SetMoney(players.at(payeeID).GetMoney() + amount);
		players.at(payerID).SetMoney(players.at(payerID).GetMoney() - amount);
	}
	else {
		cout << players.at(payerID).GetName() << " went bankrupt! They paid " << players.at(payeeID).GetName() << " " << players.at(payerID).GetMoney() << " before going out." << endl;
		//if the payer does not have enough money, give remainder and set isBankrupt to true
		players.at(payeeID).SetMoney(players.at(payeeID).GetMoney() + players.at(payerID).GetMoney());
		players.at(payerID).SetMoney(0);
		players.at(payerID).SetIsBankrupt(true);
		
		//give all properties to payee and get rid of houses
		for (int i = 0; i < spaces.size(); i++) {
			if (spaces.at(i).ownerID == payerID) {
				spaces.at(i).ownerID = payeeID;//give property to payee
				if (spaces.at(i).isNormal == true) {
					spaces.at(i).currentRent = spaces.at(i).rent.at(0);//reset rent to base
				}
				CheckMonopoly(&spaces.at(i));
			}
		}
	}
}

void Board::TradeProperty(int playerID1, int playerID2, int spotIndex1, int spotIndex2)
{
	cout << players.at(playerID1).GetName() << " gave " << players.at(playerID2).GetName() << " " << spaces.at(spotIndex1).name << " for " << spaces.at(spotIndex2).name << endl;
	spaces.at(spotIndex1).ownerID = playerID2;
	spaces.at(spotIndex2).ownerID = playerID1;
}

void Board::BuildHouses(int playerID)
{
	//If they have a monopoly, spend all of thier money each turn on houses until they can no longer do so
	//place houses on least-housed property (out of all owned monopolies)
	//find monopoly
	vector<int> monopolySpots;
	for (int i = 0; i < spaces.size(); i++) {
		if (spaces.at(i).isNormal) {
			//if it is normal
			if (spaces.at(i).isMonopoly == true && spaces.at(i).numHouses < 5 && spaces.at(i).ownerID == playerID) {
				//if it is a monopoly and doesnt have max houses and player owns it 
				monopolySpots.push_back(i);
			}
			
			/*if (monopolySpots.size() > 0) {
				//exit early if we find a monopoly-amount of properties
				if (monopolySpots.size() == spaces.at(monopolySpots.at(0)).monopolyNumber) {
					break;
				}
				continue;
			}*/
		}
	}
	if (monopolySpots.size() == 0) {
		//if no monopolies, return
		return;
	}

	int leastHouses = 6;
	int leastHousedProperty = 0;
	int reserveMoney = 300;//player wont spend this amount of money
	while(players.at(playerID).GetMoney() > reserveMoney) {
		for (int j = 0; j < monopolySpots.size(); j++) {
			if (spaces.at(monopolySpots.at(j)).numHouses < leastHouses) {
				leastHouses = spaces.at(monopolySpots.at(j)).numHouses;
				leastHousedProperty = monopolySpots.at(j);
			}
		}
		if (spaces.at(leastHousedProperty).numHouses < 5) {
			//if there can be a house placed, do so
			spaces.at(leastHousedProperty).numHouses = spaces.at(leastHousedProperty).numHouses + 1;//add house
			//increase rent
			spaces.at(leastHousedProperty).currentRent = spaces.at(leastHousedProperty).rent.at(spaces.at(leastHousedProperty).numHouses);
			//subtract house cost from total money
			players.at(playerID).SetMoney(players.at(playerID).GetMoney() - ((spaces.at(leastHousedProperty).index / 10) * 50) + 50);
			leastHouses++;
		}
		else {
			//if all properties are at max capacity, then return instead of building more
			return;
		}
	}
}
