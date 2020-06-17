#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Vending
{
private:
	float balance;
	int coinTypes[8] = {  };	//array for storing coin types put in machine
	vector<string> items;	//vectors for storing stock of the machine
	vector<float> prices;
	vector<int> quantities;
	string returnNotification;

public:

	Vending();

	bool ReadContents(string Filelocation);	//reads the file containing all the items, prices, and quantities

	void SaveChanges(string Filelocation);	//saves any changes made while using the vending machine to the file

	void DisplayContents(bool showQuantity);	//displays all items currently in the machine, along with their prices, showQuantity set to true also shows amount of each item

	string AddMoney(double amount, int type);	//adds the amount of money shown, and a coin type it will keep track of

	string BuyItem(string itemNumStr);	//buys the item that corresponds to the entered number, removes balance based on the price, and updates change for coin return

	string CoinReturn();	//returns remaining balance, informs user what coin types came out

	string AddItem(string newItem);	//adds desired quantity of specified item to the vending machine stock, and updates its price

	string RemoveItem(string newItem);	//removes desired quantity of specified item

	bool CheckPassword(string password);	//checks input against a stored password
};
