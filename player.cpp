#include"player.h"



player::player()
{
	ID = 0;
	money = 2000;//total money in account
	position = 0;//position on board
	jailCounter = 0;//any number above 0 indicates the player is in jail and how many turns theyve been in jail
	name = "No Name";
	isBankrupt = false;

}

int player::GetID()
{
	return ID;
}

void player::SetID(int ID)
{
	this->ID = ID;
}

int player::GetMoney()
{
	return money;
}

void player::SetMoney(int Money)
{
	this->money = Money;
}

int player::GetPosition() {
	return position;
}

int player::GetJailCounter()
{
	return jailCounter;
}

void player::SetJailCounter(int jailCounter)
{
	this->jailCounter = jailCounter;
}

void player::SetPosition(int position) {
	this->position = position;
}

std::string player::GetName()
{
	return name;
}

void player::SetName(std::string name)
{
	this->name = name;
}

bool player::GetIsBankrupt()
{
	return isBankrupt;
}

void player::SetIsBankrupt(bool bankrupt)
{
	this->isBankrupt = bankrupt;
}

bool player::doIBuy(spot landedSpot)
{
	//for now, they always buy if they have the money
	return landedSpot.value < money;
}
