#include "vending.h"

int main() {
	Vending machine;
	if (machine.ReadContents("assets/vendingContents.txt")) {	//opens and reads stored contents of the vending machine, exits if it can't
		string mode = "user";	//mode variable allows easy switching between different modes with different available commands
		string command;
		string notification = "---";	//storage for returned notifications from called functions

		while (mode != "exit") {	//loops until user gives the exit command
			system("cls");	//clears window each loop for a clean visual interface
			if (mode == "user") {
				cout << "Vending Machine" << '\n';
				cout << '\n';
				cout << "To insert coins, enter coin type (TWO POUNDS, POUND, 50, 20, 10, 5, 2, 1)" << '\n';	//each mode has available commands listed (SERVICE command is hidden)
				cout << "Enter COIN RETURN to return coins" << '\n';
				cout << "Enter BUY # to purchace, where # is the number item you wish to buy" << '\n';
				cout << "Enter EXIT to exit the vending machine" << '\n';
				cout << '\n';
				machine.DisplayContents(false);	//displays the contents and prices of the vending machine
				cout << '\n';
				cout << notification << '\n';	//current activity notification or error message
				cout << '\n';
				cout << "Enter:";
				getline(cin, command);
				if (!command.empty()) {
					transform(command.begin(), command.end(), command.begin(), ::toupper);	//converts entered command to upper case so inputs are not case sensitive
					if (command == "TWO POUNDS") { notification = machine.AddMoney(2, 0); }
					else if (command == "POUND") { notification = machine.AddMoney(1, 1); }
					else if (command == "50") { notification = machine.AddMoney(0.50, 2); }
					else if (command == "20") { notification = machine.AddMoney(0.20, 3); }
					else if (command == "10") { notification = machine.AddMoney(0.10, 4); }
					else if (command == "5") { notification = machine.AddMoney(0.05, 5); }
					else if (command == "2") { notification = machine.AddMoney(0.02, 6); }
					else if (command == "1") { notification = machine.AddMoney(0.01, 7); }	//adds specified amount and tracks coins entered
					else if (command == "COIN RETURN") { notification = machine.CoinReturn(); }
					else if (command.at(0) == 'B' && command.at(1) == 'U' && command.at(2) == 'Y' && command.at(3) == ' ') { notification = machine.BuyItem(command.substr(4)); } //finds commands the start with BUY, and sends the number after it to the machine
					else if (command == "SERVICE") { mode = "password"; }
					else if (command == "EXIT") { mode = "exit"; }
					else { notification = "Unrecognized command"; }	//only if command does not fit any known inputs
				}
			}

			else if (mode == "password") {
				cout << "Enter Password:";
				getline(cin, command);
				if (machine.CheckPassword(command)) { notification = "---"; mode = "service"; }	//checks input against the stored password and only provides access if correct
				else { notification = "Incorrect Password";  mode = "user"; }
			}

			else if (mode == "service") {
				cout << "Service Mode" << '\n';
				cout << '\n';
				cout << "Enter ADD ITEMS to restock" << '\n';
				cout << "Enter REMOVE ITEMS to remove stock" << '\n';
				cout << "Enter BACK to leave Service Mode" << '\n';
				cout << "Enter EXIT to exit the vending machine" << '\n';
				cout << '\n';
				machine.DisplayContents(true);	//in service mode, quantities of the items in the vending machine are also visible
				cout << '\n';
				cout << notification << '\n';
				cout << '\n';
				cout << "Enter:";
				getline(cin, command);
				transform(command.begin(), command.end(), command.begin(), ::toupper);
				if (command == "ADD ITEMS") { notification = "---"; mode = "adding"; }
				else if (command == "REMOVE ITEMS") { notification = "---"; mode = "removing"; }	//uses different modes to allow seperate instructions and allowed inputs for each option
				else if (command == "BACK") { notification = "---"; mode = "user"; }
				else if (command == "EXIT") { mode = "exit"; }
				else { notification = "Unrecognized command"; }
			}

			else if (mode == "adding") {
				cout << "Adding Items" << '\n';
				cout << '\n';
				cout << "Enter the ITEM NAME, £PRICE, QUANTITY of the item you wish to stock" << '\n';
				cout << "(Example: Dairy Milk, £1.50, 10" << '\n';
				cout << "Enter BACK to return to Service Mode" << '\n';
				cout << "Enter EXIT to exit the vending machine" << '\n';
				cout << '\n';
				machine.DisplayContents(true);
				cout << '\n';
				cout << notification << '\n';
				cout << '\n';
				cout << "Enter:";
				getline(cin, command);
				notification = machine.AddItem(command);
				transform(command.begin(), command.end(), command.begin(), ::toupper);	//after checking if it's a valid input, makes it case insensitive and checks against exit commands
				if (command == "BACK") { notification = "---"; mode = "service"; }
				else if (command == "EXIT") { mode = "exit"; }
			}

			else if (mode == "removing") {
				cout << "Removing Items" << '\n';
				cout << '\n';
				cout << "Enter the ITEM NAME, QUANTITY of the item you wish to remove" << '\n';
				cout << "(Example: Dairy Milk, 5" << '\n';
				cout << "Enter BACK to return to Service Mode" << '\n';
				cout << "Enter EXIT to exit the vending machine" << '\n';
				cout << '\n';
				machine.DisplayContents(true);
				cout << '\n';
				cout << notification << '\n';
				cout << '\n';
				cout << "Enter:";
				getline(cin, command);
				notification = machine.RemoveItem(command);
				transform(command.begin(), command.end(), command.begin(), ::toupper);
				if (command == "BACK") { notification = "---"; mode = "service"; }
				else if (command == "EXIT") { mode = "exit"; }
			}


		}

		machine.SaveChanges("assets/vendingContents.txt");	//save any changes made to the stock before exiting
		cout << "Thank you for using VENDING MACHINE" << '\n';
	}
	else{ cout << "Error, could not read contents file" << '\n'; }
	system("pause");
}