#include "algorithms.h"

using namespace std;

extern bool welcomed;
extern short menuCheckValue;
extern bool logined;
extern bool isAdmin;
extern int userAmount;

extern struct Account
{
	string id;
	string name;
	string email;
	string password;
};

void drawBorderLine()
{
	printf("%s__________________________________________________%s\n\n", YELLOW, RESET);
}

void help(bool needBorderLine)
{
	printf("%sNavigate%s through menus with numbers, %sget back%s to\nprevious menu with command %sback%s. You can also %squit%s\nthe program with command %squit%s."
		" You can also use\ncommand shortcuts, for example %sb%s for %sback%s, %sq%s for %squit%s.%s\n",
		WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, RESET);

	if (needBorderLine)
		drawBorderLine();
}

void welcome()
{
	drawBorderLine();
	printf("%sWelcome to the %sAccount Manager%s console application!\nHere you can %screate%s an account,\n%slog in%s or %sdelete%s and %sedit%s your account.%s\n\n",
		YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, WHITE, YELLOW, RESET);
	help(0);
	welcomed = true;
}

short welcomeMenu()
{
	drawBorderLine();
	printf("%s1. %sLogin%s\n2. %sSign Up%s\n3. %sEdit Account%s\n4. %sDelete Account%s\n5. %sClose%s\n",
		RED, WHITE, RED, WHITE, RED, WHITE, RED, WHITE, RED, WHITE, CYAN);
	if (isAdmin)
		printf("%s6. %sShow Accounts%s\n", RED, WHITE, CYAN);

	string choice;
	getline(cin, choice);
	choice.erase(remove(choice.begin(), choice.end(), ' '), choice.end());

	if (choice == "1")
		return 1;
	else if (choice == "2")
		return 2;
	else if (choice == "3")
		return 3;
	else if (choice == "4")
		return 4;
	else if (choice == "6")
		return 6;
	else if (choice == "5" || choice == "exit" || choice == "e")
	{
		printf("%sClosing %sAccount Manager%s...%s\n", WHITE, RED, WHITE, RESET);
		exit(0);
	}
	else if (choice == "back" || choice == "b")
	{
		printf("%sError: You're not allowed to %sget back%s from this menu!%s\n", RED, WHITE, RED, RESET);
		return welcomeMenu();
	}
	else
	{
		printf("%sInvalid input! Try again!%s\n", RED, RESET);
		return welcomeMenu();
	}
}

string enterUsername()
{
	printf("%sUsername: %s", WHITE, CYAN);
	string username;
	getline(cin, username);
	return username;
}

string enterEmail()
{
	printf("%sEmail: %s", WHITE, CYAN);
	string email;
	getline(cin, email);

	return email;
}

bool checkEmailSyntax(string email)
{
	const regex emailPattern(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b)");

	if (!regex_match(email, emailPattern))
		return false;

	return true;
}

string getMaskedInput()
{
	string input;
	char ch;

	while (true)
	{
		ch = _getch();
		if (ch == '\r' || ch == '\n')
		{
			cout << std::endl;
			break;
		}
		else if (ch == '\b')
		{
			if (!input.empty())
			{
				input.pop_back();
				cout << "\b \b";
			}
		}
		else {
			input.push_back(ch);
			cout << '*';
		}
	}
	return input;
}

string enterPassword()
{
	printf("%sPassword: %s", WHITE, CYAN);
	string password = getMaskedInput();
	return password;
}

short invalidInput(const short& invalidPart)
{
	switch (invalidPart)
	{
	case 1:
		printf("%sInvalid username!%s\n", RED, RESET);
		break;
	case 2:
		printf("%sInvalid email!%s\n", RED, RESET);
		break;
	case 3:
		printf("%sInvalid password! The password must contain from %s5%s to %s15%s symbols!%s\n", RED, WHITE, RED, WHITE, RED, RESET);
		break;
	case 4:
		printf("%sInvalid id!%s\n", RED, RESET);
		break;
	case 5:
		printf("%sName not found!%s\n", RED, RESET);
		break;
	case 6:
		printf("%sEmail not found!%s\n", RED, RESET);
		break;
	case 7:
		printf("%sPassword not found!%s\n", RED, RESET);
		break;
	default:
		printf("%sInvalid enter!%s\n", RED, RESET);
		break;
	}

	printf("%sUse command %sback%s, %sexit%s or enter something else to %stry again%s!%s\n", RED, WHITE, RED, WHITE, RED, WHITE, RED, CYAN);

	string command;
	getline(cin, command);

	if (command == "back" || command == "b")
		return -1;
	else if (command == "exit" || command == "e")
	{
		printf("%sClosing %sAccount Manager%s...%s\n", WHITE, RED, WHITE, RESET);
		exit(0);
	}
	else
		return 0;
}

short login()
{
	Account validAccount;

	string username = enterUsername();
	if (!nameExists(username))
		return invalidInput(1);

	string* validAccountData = findAccountByName(username);
	string validEmail = validAccountData[1];
	string validPassword = validAccountData[2];

	string email = enterEmail();
	if (!emailExists(email) || !checkEmailSyntax(email) || email != validEmail)
	{
		delete[] validAccountData;
		return invalidInput(2);
	}

	string password = enterPassword();
	if (5 > password.length() || password.length() > 15 || password != validPassword)
	{
		delete[] validAccountData;
		return invalidInput(3);
	}

	if (username == "Admin" && email == "admin@gmail.com" && password == "password")
		isAdmin = true;
	else
		isAdmin = false;

	printf("%sWelcome %s%s%s!%s\n", YELLOW, WHITE, username.c_str(), YELLOW, RESET);
	delete[] validAccountData;
	return 1;
}

short signUp()
{
	string username = enterUsername();
	if (nameExists(username))
		return invalidInput(1);
	
	string email = enterEmail();
	if (emailExists(email) || !checkEmailSyntax(email))
		return invalidInput(2);

	string password = enterPassword();
	if (5 > password.length() || password.length() > 15)
		return invalidInput(3);

	addAccountToVector(username, email, password);
	printf("%sAccount %s%s%s created!%s\n", YELLOW, WHITE, username.c_str(), YELLOW, RESET);
	return 1;
}
