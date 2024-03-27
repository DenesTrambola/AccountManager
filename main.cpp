#include "ui.h"
#include "algorithms.h"

using namespace std;

bool welcomed = false;
short menuValue = -1;
bool logined = false;
bool isAdmin = false;
string userId = "";

int main()
{
	if (!welcomed)
	{
		welcome();
		updateVector();
	}

	short welcomeChoice = welcomeMenu();
	short choiceValue;

	drawBorderLine();
	switch (welcomeChoice)
	{
	case 1:
		if (logined)
			printf("%sLogout...%s\n", YELLOW, RESET);
		printf("%sLogin...%s\n", YELLOW, RESET);
		
		logined = false;

		while (!logined)
		{
			menuValue = login();

			if (menuValue == -1)
				return main();
			else if (menuValue == 0)
				continue;
			else if (menuValue == 1)
			{
				logined = true;
				return main();
			}
		}
		break;

	case 2:
		printf("%sSign in...%s\n", YELLOW, RESET);
		menuValue = signUp();

		if (menuValue == 1 || menuValue == -1)
			return main();

		while (menuValue == 0)
			menuValue = signUp();

		return main();

	case 3:
		if (!logined)
		{
			printf("%sYou need to log in first!%s\n", RED, RESET);
			return main();
		}

		if (isAdmin)
			choiceValue = chooseAndEditAccount();
		else
			choiceValue = editAccount();

		while (choiceValue != 1)
		{
			if (isAdmin)
				choiceValue = chooseAndEditAccount();
			else
				choiceValue = editAccount();

			if (choiceValue == -1)
				return main();
			else if (choiceValue == 0)
				continue;
			else if (choiceValue != 1)
			{
				printf("%sSomething went wrong in code!%s\n", RED, RESET);
				exit(1);
			}
		}

		return main();

	case 4:
		if (!logined)
		{
			printf("%sYou need to log in first!%s\n", RED, RESET);
			return main();
		}

		if (isAdmin)
			choiceValue = chooseAndDeleteAccount();
		else
			choiceValue = deleteAccount();

		while (choiceValue != 1)
		{
			if (isAdmin)
				choiceValue = chooseAndDeleteAccount();
			else
				choiceValue = deleteAccount();

			if (choiceValue == -1)
				return main();
			else if (choiceValue == 0)
				continue;
			else if (choiceValue != 1)
			{
				printf("%sSomething went wrong in code!%s\n", RED, RESET);
				exit(1);
			}
		}

		return main();

	case 6:
		if (isAdmin)
		{
			printAccounts();
			return main();
		}

	default:
		printf_s("%sInvalid input! Try again!%s\n", RED, RESET);
		return main();
	}

	return 0;
}
