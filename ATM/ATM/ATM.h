#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
enum MainMenu { QuickWithdraws = 1, NormalWithdraw, Depoist, CheckBalance , Logouts};

const string ClientsFileName = "ClientInfo.txt";


void Deposit(vector<sClient> &vClient);


void With_draw(vector<sClient> &Client);  

void BackToMainMenu();


bool Login();
void ShowATMMainMenu();
void ShowQuickWithdrawScreen();
void PerformQuickWithdraw(int userChoice,vector<sClient>& vClients);

void ShowCheckBalanceScreen();
