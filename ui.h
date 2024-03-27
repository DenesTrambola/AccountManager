#pragma once
#include "tools.h"

void drawBorderLine();

void help(bool needBorderLine = false);

void welcome();

short welcomeMenu();

std::string enterUsername();

std::string enterEmail();

bool checkEmailSyntax(std::string email);

std::string getMaskedInput();

std::string enterPassword();

short invalidInput(const short& invalidPart = 0);

short login();

short signUp();
