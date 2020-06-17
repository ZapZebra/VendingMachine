#include "vending.h"

Vending::Vending() {
	balance = 0.00;
}

bool Vending::ReadContents(string fileLocation) {
	ifstream vendingContents(fileLocation);	//opens file with stored machine contents
	bool validInput = true;	//a bool to flip to false if any bad inputs are found
	if (vendingContents.is_open())
	{
		string item;	//seperate strings to seperate out each line before converting and storing in their vectors
		string price;
		double fPrice;
		string quantity;
		double iQuantity;
		string skip;	//for storing bits in between important information as it is skipped over
		while (getline(vendingContents, item, ',')) {	//takes item name as all characters before the first comma
			getline(vendingContents, skip, char(156));	//skips to the £ symbol before reading price

			getline(vendingContents, price, ',');
			try {
				if (price.find_first_not_of(" 0123456789.") == string::npos && price.substr(price.find("."), price.size()).length() <= 3) {	//tests for valid price input (only digits, no more than 2 decimal places)
					fPrice = stof(price);	//converts to a float for storage
				}
				else { validInput = false; }	//lets function know if the input cannot be read properly
			}
			catch (...) {
				validInput = false;
			}

			getline(vendingContents, quantity);
			try {
				if (quantity.find_first_not_of(" 0123456789") == string::npos) {
					quantity.erase(remove_if(quantity.begin(), quantity.end(), isspace), quantity.end());	//removes spaces between digits(just in case)
					iQuantity = stoi(quantity);
					if (iQuantity <= 0) { validInput = false; }
				}
				else { validInput = false; }
			}
			catch (...) {
				validInput = false;
			}

			if (validInput) {	//if the line is a valid input, adds the item to the vector
				items.push_back(item);
				prices.push_back(fPrice);
				quantities.push_back(iQuantity);
			}
		}
		vendingContents.close();
	}
	return(validInput);	//returns if it was able to read the file or not
}

void Vending::SaveChanges(string fileLocation) {
	ofstream vendingContents(fileLocation);	//reopens file to write information back in
	if (vendingContents.is_open())
	{
		
		for (int itemNo = 0; itemNo < items.size(); itemNo++) {	//for each item in the array, writes it in to the file in a format that can be read out again next time
			vendingContents << items[itemNo] << ", " << char(156) << setprecision(2) << fixed << prices[itemNo] << ", " << setprecision(0) << quantities[itemNo] << '\n';
		}

		vendingContents.close();
	}
	else cout << "Unable to open file";
}

void Vending::DisplayContents(bool showQuantity) {	//optionally display the quantity of an item as well
	for (int itemNo = 0; itemNo < items.size(); itemNo++) {	//for each item in the machine, assign it a number and display it to the user
		cout << itemNo + 1 << ": " << items[itemNo] << " - " << char(156) << setprecision(2) << fixed << prices[itemNo];
		if (showQuantity) { cout << "    _    current quantity:" << quantities[itemNo]; }
		cout << '\n';
	}

	cout << '\n' << "Current Balance: " << char(156) << setprecision(2) << fixed << balance << '\n';	//also displays the users current balance
}

string Vending::AddMoney(double amount, int type) {
	balance += amount;
	coinTypes[type] ++;	//records the value of the money added as well as the type of coin so the same coins can be returned to the user if they select COIN RETURN
	stringstream deposits;
	deposits << "Added " << char(156) << setprecision(2) << fixed << amount;	//tells the user how much they added
	returnNotification = deposits.str();
	return(returnNotification);
}

string Vending::BuyItem(string itemNumStr) {
	int itemNo;
	try {	//if entry after BUY is invalid, will catch the exception
		itemNo = stoi(itemNumStr);
		if (itemNo > 0 && itemNo <= items.size()) {	//checks the requested value is within the bounds of available items
			itemNo--;
			if (prices[itemNo] <= balance) {	//checks if the user has enough balance to buy the item
				balance -= prices[itemNo];	//if they do, deducts balance
				stringstream purchases;
				purchases << "You have purchased " << items[itemNo] << " for " << char(156) << setprecision(2) << fixed << prices[itemNo];
				quantities[itemNo]--;
				if (quantities[itemNo] <= 0) {
					items.erase(items.begin() + itemNo);
					prices.erase(prices.begin() + itemNo);
					quantities.erase(quantities.begin() + itemNo);
				}
				for (int i = 0; i < 8; i++) { coinTypes[i] = 0; }	//Resets the types of coins held for coin return after purchasing
				double changeCounter = balance;
				while (changeCounter > 1.999) { changeCounter -= 2.00; coinTypes[0]++; }	//Recalculates coin return coin types from remaining change
				while (changeCounter > 0.999) { changeCounter -= 1.00; coinTypes[1]++; }
				while (changeCounter > 0.499) { changeCounter -= 0.50; coinTypes[2]++; }
				while (changeCounter > 0.199) { changeCounter -= 0.20; coinTypes[3]++; }
				while (changeCounter > 0.099) { changeCounter -= 0.10; coinTypes[4]++; }
				while (changeCounter > 0.049) { changeCounter -= 0.05; coinTypes[5]++; }
				while (changeCounter > 0.019) { changeCounter -= 0.02; coinTypes[6]++; }
				while (changeCounter > 0.009) { changeCounter -= 0.01; coinTypes[7]++; }

				returnNotification = purchases.str();
			}
			else {
				returnNotification = "You do not have enough balance for this item";
			}
		}
		else {
			returnNotification = "That item is unavailable or out of stock";
		}
	}
	catch (...) {
		returnNotification = "Sorry, Invalid Input";
	}
	return(returnNotification);
}

string Vending::CoinReturn() {
	returnNotification = "You have been returned";
	if (coinTypes[0] > 0) { returnNotification.append(" " + to_string(coinTypes[0]) + " two pound coins"); }	//returns the exact coins put in, or the change calculated after a purchase, or both
	if (coinTypes[1] > 0) { returnNotification.append(" " + to_string(coinTypes[1]) + " pound coins"); }
	if (coinTypes[2] > 0) { returnNotification.append(" " + to_string(coinTypes[2]) + " fifty pence coins"); }
	if (coinTypes[3] > 0) { returnNotification.append(" " + to_string(coinTypes[3]) + " twenty pence coins"); }
	if (coinTypes[4] > 0) { returnNotification.append(" " + to_string(coinTypes[4]) + " ten pence coins"); }
	if (coinTypes[5] > 0) { returnNotification.append(" " + to_string(coinTypes[5]) + " five pence coins"); }
	if (coinTypes[6] > 0) { returnNotification.append(" " + to_string(coinTypes[6]) + " two pence coins"); }
	if (coinTypes[7] > 0) { returnNotification.append(" " + to_string(coinTypes[7]) + " pennies"); }
	if (balance == 0) { returnNotification = "No coins to return"; }
	balance = 0.00;
	for (int i = 0; i < 8; i++) { coinTypes[i] = 0; }	//resets balance and coin stock after returning
	return(returnNotification);
}

string Vending::AddItem(string newItem) {
	istringstream iss(newItem);
	bool validInput = true;
	string item;
	string price;
	double fPrice;
	string quantity;
	double iQuantity;
	string skip;
	getline(iss, item, ',');	//first seperates the entered string into seperate parts
	getline(iss, skip, char(156));

	getline(iss, price, ',');
	try {
		if (price.find_first_not_of(" 0123456789.") == string::npos && price.substr(price.find("."), price.size()).length() <= 3) {	//checks input is a valid price
			price.erase(remove_if(price.begin(), price.end(), isspace), price.end());
			fPrice = stof(price);
			if (fPrice < 0.00) { validInput = false; returnNotification = "Price cannot be negative"; }	//makes sure price isn't negative
		}
		else { validInput = false; returnNotification = "Please enter a valid price"; }
	}
	catch (...){
		validInput = false;
		returnNotification = "Please enter a valid price";
	}

	getline(iss, quantity);
	try {
		if (quantity.find_first_not_of(" 0123456789") == string::npos) {	//checks quantity is a valid amount
			quantity.erase(remove_if(quantity.begin(), quantity.end(), isspace), quantity.end());
			iQuantity = stoi(quantity);
			if (iQuantity <= 0) { validInput = false; returnNotification = "Cannot add quantities that are zero or negative"; }	//makes sure quantity isn't zero or negative
		}
		else{ validInput = false; returnNotification = "Please enter a valid quantity"; }
	}
	catch (...) {
		validInput = false;
		returnNotification = "Please enter a valid quantity";
	}

	if (validInput) {

		bool addNew = true;
		for (int itemNo = 0; itemNo < items.size(); itemNo++) {	//if the input was valid, first looks for an existing item with the same name (case insensitive)
			string oldItemCaseCheck = items[itemNo];
			transform(oldItemCaseCheck.begin(), oldItemCaseCheck.end(), oldItemCaseCheck.begin(), ::toupper);
			string newItemCaseCheck = item;
			transform(newItemCaseCheck.begin(), newItemCaseCheck.end(), newItemCaseCheck.begin(), ::toupper);
			if (oldItemCaseCheck == newItemCaseCheck) {
				addNew = false;
				prices[itemNo] = fPrice;
				quantities[itemNo] += iQuantity;	//uptades the item if found
			}
		}

		if (addNew) {	//if item does not exist, creates a new one
			items.push_back(item);
			prices.push_back(fPrice);
			quantities.push_back(iQuantity);
		}
		stringstream added;
		added << "Added " << quantity << " of " << item << " for the price of " << char(156) << setprecision(2) << fixed << price;	//notifies user of added item
		returnNotification = added.str();

	}
	return(returnNotification);
}

string Vending::RemoveItem(string newItem) {
	istringstream iss(newItem);
	bool validInput = true;
	string item;
	string quantity;
	double iQuantity;
	getline(iss, item, ',');	//seperates the input string

	getline(iss, quantity);
	try {
		if (quantity.find_first_not_of(" 0123456789") == string::npos) {	//checks quantity is a valid input
			quantity.erase(remove_if(quantity.begin(), quantity.end(), isspace), quantity.end());
			iQuantity = stoi(quantity);
			if (iQuantity <= 0) { validInput = false; returnNotification = "Cannot remove quantities that are zero or negative"; }	//checks quantity is not zero or negative
		}
		else { validInput = false; returnNotification = "Please enter a valid quantity"; }
	}
	catch (...) {
		validInput = false;
		returnNotification = "Please enter a valid quantity";
	}

	if (validInput) {

		bool itemExists = false;
		for (int itemNo = 0; itemNo < items.size(); itemNo++) {	//searches for the existing item (case insensitive)
			string oldItemCaseCheck = items[itemNo];
			transform(oldItemCaseCheck.begin(), oldItemCaseCheck.end(), oldItemCaseCheck.begin(), ::toupper);
			string newItemCaseCheck = item;
			transform(newItemCaseCheck.begin(), newItemCaseCheck.end(), newItemCaseCheck.begin(), ::toupper);
			if (oldItemCaseCheck == newItemCaseCheck) {
				itemExists = true;
				quantities[itemNo] -= iQuantity;	//if found, removes specified quantity
				if (quantities[itemNo] <= 0) {	//if there would now be zero or less, removes it from the list
					items.erase(items.begin() + itemNo);
					prices.erase(prices.begin() + itemNo);
					quantities.erase(quantities.begin() + itemNo);
				}
				stringstream removed;
				removed << "Removed " << quantity << " of " << item;	//notifies the user that the items were removed
				returnNotification = removed.str();
			}
		}

		if (!itemExists) {
			returnNotification = "Could not find specified item";	//notifies the user if item was not found
		}


	}
	return(returnNotification);
}

bool Vending::CheckPassword(string password) {
	ifstream passwordLocation("assets/secure/secretPass.txt");	//gets the stored password and reads the file
	string read;
	if (passwordLocation.is_open()) {
		getline(passwordLocation, read);
	}
	passwordLocation.close();

	if (password == read) {	//checks user entry against the correct password
		return(true);
	}
	else {
		return(false);
	}
}