#pragma once
#include "tools.h"

void createFileIfNeeded();

bool nameExists(const std::string& username);

bool emailExists(const std::string& email);

std::string* findAccountByName(const std::string username);

void updateVector();

void updateFile();

void addAccountToVector(const std::string& name, const std::string& email, const std::string& passwordHash);

short chooseAndEditAccount();

std::string* findAccount();

bool changeName(std::string& oldName);

bool changeEmail(std::string& oldEmail);

void changePassword(std::string& oldPassword);

void changeData(const std::string* accountData);

short editAccount();

short chooseAndDeleteAccount();

short deleteAccount();

void printAccounts();
