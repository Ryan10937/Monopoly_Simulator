#pragma once
#include<string>
#include"spot.h"
class player {

public:
	player();
	int GetID();
	void SetID(int ID);
	int GetMoney();
	void SetMoney(int Money);
	int GetPosition();
	int GetJailCounter();
	void SetJailCounter(int jailCounter);
	void SetPosition(int position);
	std::string GetName();
	void SetName(std::string name);
	bool GetIsBankrupt();
	void SetIsBankrupt(bool bankrupt);
	//decisions here
	bool doIBuy(spot landedSpot);

private:
	int ID;
	int money;//total money in account
	int position;//position on board
	int jailCounter;//any number above 0 indicates the player is in jail and how many turns theyve been in jail
	std::string name; 
	bool isBankrupt;
};
