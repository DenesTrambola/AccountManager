#include "tools.h"
#include "ui.h"
#include "algorithms.h"

using namespace std;

char hashKey = '&';
const char filename[] = "accounts.csv";
int userAmount = 0;
extern string userId;

struct Account
{
    string id;
	string name;
	string email;
	string password;
};

vector<Account> accounts;

void createFileIfNeeded()
{
	fstream file(filename, ios::in);

	if (!file.is_open())
	{
		file.close();
		file.open(filename, ios::out);
		file.close();
	}
}

string encryptPassword(const string& passwordStr)
{
    string passwordHash = passwordStr;

    for (char& symbol : passwordHash)
        symbol ^= hashKey;

    return passwordHash;
}

string decryptPassword(const string& passwordHash)
{
    string passwordStr = passwordHash;

    for (char& symbol : passwordStr)
        symbol ^= hashKey;

    return passwordStr;
}

bool nameExists(const string& username)
{
	for (const auto& account : accounts)
    {
        if (account.name == username)
        {
            return true;
        }
    }
	return false;
}

bool emailExists(const string& email)
{
    for (const auto& account : accounts)
    {
        if (account.email == email)
        {
            return true;
        }
    }
    return false;
}

void updateVector()
{
    accounts.clear();
    userAmount = 0;

    Account tempAccount;
    string data = "";
    char symbol;
    short fieldCount = 1;

    createFileIfNeeded();
    ifstream file(filename);

    while (file.get(symbol))
    {
        if (symbol == ',')
        {
            if (fieldCount == 1)
                tempAccount.id = data;
            else if (fieldCount == 2)
                tempAccount.name = data;
            else if (fieldCount == 3)
                tempAccount.email = data;

            data = "";
            fieldCount++;
        }
        else if (symbol == '\n')
        {
            tempAccount.password = decryptPassword(data);
            accounts.push_back(tempAccount);
            ++userAmount;
            data = "";
            fieldCount = 1;
        }
        else
            data += symbol;
    }
    
    file.close();
}

void updateFile()
{
    userAmount = 0;
    ofstream file(filename);
    for (const auto& account : accounts)
        file << ++userAmount << ',' << account.name << ',' << account.email << ',' << encryptPassword(account.password) << '\n';
}

void addAccountToVector(const string& name, const string& email, const string& password)
{
	Account account;
    account.id = to_string(++userAmount);
	account.name = name;
	account.email = email;
	account.password = password;
	accounts.push_back(account);

	ofstream file(filename, ios::app);
	file << account.id << ',' << account.name << ',' << account.email << ',' << encryptPassword(account.password) << '\n';
}

short chooseAndEditAccount()
{
    printf("%sChoose an %saccount%s:%s\n", YELLOW, WHITE, YELLOW, RESET);
    for (const auto& account : accounts)
        printf("%s%s. %s%s%s\n", RED, account.id.c_str(), WHITE, account.name.c_str(), CYAN);

    getline(cin, userId);
    userId.erase(remove(userId.begin(), userId.end(), ' '), userId.end());

    try
    {
        if (1 <= stoi(userId) && stoi(userId) <= userAmount)
            editAccount();
        else
            invalidInput();
    }
    catch (const invalid_argument& error)
    {
        return invalidInput();
    }
    catch (const out_of_range& error)
    {
        return invalidInput();
    }
    catch (...)
    {
        return invalidInput();
    }
}

string* findAccount()
{
    string* editingAccount = new string[3];

    for (const auto& account : accounts)
        if (userId == account.id)
        {
            editingAccount[0] = account.name;
            editingAccount[1] = account.email;
            editingAccount[2] = account.password;
        }

    return editingAccount;
}

string* findAccountByName(const string username)
{
    string* accountData = new string[3];

    accountData[0] = username;

    for (size_t i = 0; i < accounts.size(); ++i)
        if (accounts[i].name == username)
        {
            userId = accounts[i].id;
            accountData[1] = accounts[i].email;
            accountData[2] = accounts[i].password;
        }

    return accountData;
}

bool changeName(string& oldName)
{
    printf("%sEnter the new %sUsername%s: %s", YELLOW, WHITE, YELLOW, CYAN);
    string newName;
    getline(cin, newName);

    for (const auto& account : accounts)
    {
        if (account.name == newName || oldName == "Admin")
        {
            printf("%sYou cannot edit the %sAdmin%s account!%s\n", RED, WHITE, RED, RESET);
            return false;
        }
    }

    oldName = newName;
    return true;
}

bool changeEmail(string& oldEmail)
{
    printf("%sEnter the new %sEmail%s: %s", YELLOW, WHITE, YELLOW, CYAN);
    string newEmail;
    getline(cin, newEmail);

    for (const auto& account : accounts)
    {
        if (account.email == newEmail || oldEmail == "admin@gmail.com")
        {
            printf("%sYou cannot edit the %sAdmin%s account!%s\n", RED, WHITE, RED, RESET);
            return false;
        }
    }

    oldEmail = newEmail;
    return true;
}

void changePassword(string& oldPassword)
{
    printf("%sEnter the new %sPassword%s: %s", YELLOW, WHITE, YELLOW, CYAN);
    string newPassword = getMaskedInput();
    oldPassword = newPassword;
}

void changeData(const string* accountData)
{
    for (auto& account : accounts)
        if (account.id == userId)
        {
            account.name = accountData[0];
            account.email = accountData[1];
            account.password = accountData[2];
        }

    updateFile();
}

short editAccount()
{
    string* accountData = findAccount();

    printf("%s\nChoose what you want to edit:%s\n1. %sUsername%s\n2. %sEmail%s\n3. %sPassword%s\n", YELLOW, RED, WHITE, RED, WHITE, RED, WHITE, CYAN);
    string chosenData;
    getline(cin, chosenData);
    chosenData.erase(remove(chosenData.begin(), chosenData.end(), ' '), chosenData.end());

    if (chosenData == "1")
    {
        if (!changeName(accountData[0]))
        {
            delete[] accountData;
            return invalidInput(1);
        }
    }
    else if (chosenData == "2")
    {
        if (!changeEmail(accountData[1]) || !checkEmailSyntax(accountData[1]))

        {
            delete[] accountData;
            return invalidInput(2);
        }
    }
    else if (chosenData == "3")
    {
        changePassword(accountData[2]);
    }
    else
    {
        return invalidInput();
    }

    changeData(accountData);
    printf("%s\nAccount changes saved successfully!%s\n", YELLOW, RESET);

    delete[] accountData;
    return 1;
}

short chooseAndDeleteAccount()
{
    printf("%sChoose an %saccount%s:%s\n", YELLOW, WHITE, YELLOW, RESET);
    for (const auto& account : accounts)
        printf("%s%s. %s%s%s\n", RED, account.id.c_str(), WHITE, account.name.c_str(), CYAN);

    getline(cin, userId);
    userId.erase(remove(userId.begin(), userId.end(), ' '), userId.end());

    try
    {
        if (1 <= stoi(userId) && stoi(userId) <= userAmount)
            deleteAccount();
        else
            invalidInput();
    }
    catch (const invalid_argument& error)
    {
        return invalidInput();
    }
    catch (const out_of_range& error)
    {
        return invalidInput();
    }
    catch (...)
    {
        return invalidInput();
    }
}

short deleteAccount()
{
    string* accountData = findAccount();
    if (accountData[0] == "Admin")
    {
        printf("%sYou cannot delete the %sAdmin%s account!%s\n", RED, WHITE, RED, RESET);
        return invalidInput();
    }

    printf("%sAccount %s%s%s deleted!%s\n", YELLOW, WHITE, accountData[0].c_str(), YELLOW, RESET);
    accounts.erase(accounts.begin() + stoi(userId) - 1);
    --userAmount;

    updateFile();
    updateVector();
    return 1;
}

void printAccounts()
{
    if (accounts.empty())
    {
        printf("%sThere is no accounts yet!%s\n", YELLOW, RESET);
        return;
    }

    const int idWidth = 10;
    const int nameWidth = 20;
    const int emailWidth = 20;
    const int passwordWidth = 20;

    std::cout << YELLOW << std::setw(idWidth) << std::right << "ID" << " | "
        << std::setw(nameWidth) << std::left << "Name" << " | "
        << std::setw(emailWidth) << std::left << "Email" << " | "
        << std::setw(passwordWidth) << std::left << "Password" << "\n";

    std::cout << std::setfill('-') << std::setw(idWidth) << "" << " | "
        << YELLOW << std::setw(nameWidth) << "" << " | "
        << YELLOW << std::setw(emailWidth) << "" << " | "
        << YELLOW << std::setw(passwordWidth) << "" << "\n";

    std::cout << std::setfill(' ') << RESET;

    for (const auto& account : accounts)
        std::cout << WHITE << std::setw(idWidth) << std::right << account.id << YELLOW << " | "
            << WHITE << std::setw(nameWidth) << std::left << account.name << YELLOW << " | "
            << WHITE << std::setw(emailWidth) << std::left << account.email << YELLOW << " | "
            << WHITE << std::setw(passwordWidth) << std::left << account.password << YELLOW << "\n";

    cout << RESET;
}
