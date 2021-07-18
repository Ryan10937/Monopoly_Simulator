#ifndef SPOT_H
#define SPOT_H

#include<string>
#include<vector>


using namespace std;

struct spot{

    int index = -1;//spot on board
    string name = "No Name";//name of property
    int value = -1;//cost to buy property
    vector<int> rent;//array of rent from no houses, a monopoly, to max houses
    int currentRent = 0;//if occupied by a non-owner, they pay this much
    int houseValue = -1;//cost to build a single house
    int numHouses = 0;//number of houses on property
    int ownerID = -1;//owner of property
    vector<int> occupantsID;//IDs players sitting on this spot
    string monopolyColor = "NoColor";//color of set on the board. Used to group properties into monopolies
    int monopolyNumber  = 3;//number of properties needed to make a monopoly
    bool isNormal = true;//false if: railroad, utility, tax, community chest, jail, goto jail, free parking, or GO
    bool isMonopoly = false;
    
};

#endif