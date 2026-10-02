// ATM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "ATM.h"
#include "Help Functions.h"
#include "Clients.h"
using namespace std;

sClient Client;


bool Login();
void StartATM();

int ReadInt(string Message, int Min, int Max)
{
    int Number;
    while (true)
    {
        cout << Message;
        cin >> Number;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Input! Please enter a valid number.\n";
        }
        else if (Number < Min || Number > Max)
        {
            cout << "Please enter a number between " << Min << " and " << Max << ".\n";
        }
        else
            return Number;
    }
}

double ReadPositiveDouble(string Message)
{
    double Amount;
    while (true)
    {
        cout << Message;
        cin >> Amount;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Input! Please enter a number.\n";
        }
        else if (Amount <= 0)
        {
            cout << "Amount Must Be Greater Than Zero.\n";
        }
        else
            return Amount;
    }

   

}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator)
{
    sClient ClientRecord;
    vector<string> vClientData = SplitString(Line, Seperator);
    if (vClientData.size() >= 5)
    {
        ClientRecord.AccountNumber = vClientData[0];
        ClientRecord.PinCode = vClientData[1];
        ClientRecord.Name = vClientData[2];
        ClientRecord.Phone = vClientData[3];
        ClientRecord.AccountBalance = stod(vClientData[4]);
    }
    return ClientRecord;
}


string ConvertRecordToLine(sClient ClientRecord, string Seperator)
{
    string stClientRecord = "";
    stClientRecord += ClientRecord.AccountNumber + Seperator;
    stClientRecord += ClientRecord.PinCode + Seperator;
    stClientRecord += ClientRecord.Name + Seperator;
    stClientRecord += ClientRecord.Phone + Seperator;
    stClientRecord += to_string(ClientRecord.AccountBalance);
    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line != "")
            {
                sClient ClientRecord = ConvertLinetoRecord(Line, "#//#");
                vClients.push_back(ClientRecord);
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    vector<sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient C : vClient)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void SaveClientDataToFile(string FileName, vector<sClient>& vClient)
{
    fstream ClientFile;
    string DataLine;
    ClientFile.open(FileName, ios::out);
    if (ClientFile.is_open())
    {
        for (sClient& s : vClient)
        {
            if (s.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(s, "#//#");
                ClientFile << DataLine << endl;
            }
        }
        ClientFile.close();
    }
}

MainMenu ReadMainMenuChoice()
{
    
    return (MainMenu)ReadInt("\nChoose What Do You Want Do? [1 , 5]:  ", 1, 5);
}

void Deposit(vector<sClient>& vClient)
{
    cout << "----------------------------------------------\n";
    cout << "             Deposit Screen\n";
    cout << "----------------------------------------------\n";

    double Amount = ReadPositiveDouble("Please Enter Deposit Amount? ");
    char Perform;
    cout << "\nAre you sure you want to perform this transaction? (Y/N)? ";
    cin >> Perform;
    if (Perform == 'y' || Perform == 'Y')
    {
        
        for (sClient& C : vClient)
        {
            if (C.AccountNumber == Client.AccountNumber)
            {
                C.AccountBalance += Amount;
                Client.AccountBalance = C.AccountBalance; 
                SaveClientDataToFile(ClientsFileName, vClient);
                cout << "\nDeposit Done Successfully. New balance is: " << C.AccountBalance << endl;
                return;
            }
        }
    }
}

void With_draw(vector<sClient>& vClient)
{
    cout << "----------------------------------------------\n";
    cout << "              Withdraw Screen\n";
    cout << "----------------------------------------------\n";

    int Amount = 0;

    while (true)
    {
        Amount = ReadPositiveDouble("Enter An Amount Multiple Of 5's? ");

    
        if (Amount % 5 != 0)
        {
            cout << "\nInvalid Amount! The amount must be a multiple of 5 (e.g., 5, 10, 15, 20...).\n\n";
            continue;
        }

      
        if (Amount > Client.AccountBalance)
        {
            cout << "\nAmount Exceeds The Balance. You Can Withdraw Up To: " << Client.AccountBalance << endl;
            int Choice = ReadInt("\n[1] Withdraw Again\n[2] Back To Main Menu\nChoice: ", 1, 2);

            if (Choice == 2)
                return;

            cout << "\n";
            continue; 
        }

        break;
    }

   
    char Perform;
    cout << "\nAre you sure you want to perform this transaction? (Y/N)? ";
    cin >> Perform;

    if (Perform == 'y' || Perform == 'Y')
    {
        for (sClient& C : vClient)
        {
            if (C.AccountNumber == Client.AccountNumber)
            {
                C.AccountBalance -= Amount;
                Client.AccountBalance = C.AccountBalance;
                SaveClientDataToFile(ClientsFileName, vClient);
                cout << "\nWithdraw Done Successfully. New balance is: " << C.AccountBalance << endl;
                return;
            }
        }
    }
}

void ShowATMMainMenu() {
    cout << "---------------------------------------------------------------\n";
    cout << "\n\t\t++ ATM Main Menu ++\n\n";
    cout << "---------------------------------------------------------------\n";
    cout << "\t\t[1] Quick Withdraw.\n";
    cout << "\t\t[2] Normal Withdraw.\n";
    cout << "\t\t[3] Deposit.\n";
    cout << "\t\t[4] Check Balance.\n";
    cout << "\t\t[5] Logout.\n";
}

int ReaadQickWithdraw() {
    return ReadInt("Choose What To Withdraw From 1 To 9: ", 1, 9);
}

void ShowQuickWithdrawScreen() {
    cout << "---------------------------------------------------------------\n";
    cout << "\n\t\t++ Quick Withdraw ++\n\n";
    cout << "---------------------------------------------------------------\n";
    cout << "\t\t[1] 20\t\t[2] 50\n";
    cout << "\t\t[3] 100\t\t[4] 200\n";
    cout << "\t\t[5] 400\t\t[6] 600\n";
    cout << "\t\t[7] 800\t\t[8] 1000\n";
    cout << "\t\t[9] Exit\n";
}

void BackToMainMenu()
{
    cout << "\nPress any key to back to main menu...";
    system("pause > nul");
}

short GetQuickWithdrawAmount(int Choice) {
    switch (Choice) {
    case 1: return 20;
    case 2: return 50;
    case 3: return 100;
    case 4: return 200;
    case 5: return 400;
    case 6: return 600;
    case 7: return 800;
    case 8: return 1000;
    default: return 0;
    }
}

void PerformQuickWithdraw(int UserChoice, vector<sClient>& vClients) {
    if (UserChoice == 9) return;

    short Amount = GetQuickWithdrawAmount(UserChoice);

    if (Amount > Client.AccountBalance) {
        cout << "\nAmount Exceeds The Balance. Available Balance: " << Client.AccountBalance << endl;
        return;
    }

    char Perform;
    cout << "\nAre you sure you want to perform this transaction? (Y/N)? ";
    cin >> Perform;
    if (Perform == 'y' || Perform == 'Y') {
        for (sClient& C : vClients) {
            if (C.AccountNumber == Client.AccountNumber) {
                C.AccountBalance -= Amount;
                Client.AccountBalance = C.AccountBalance; 
                SaveClientDataToFile(ClientsFileName, vClients);
                cout << "\nWithdraw Done Successfully. New balance is: " << C.AccountBalance << endl;
                return;
            }
        }
    }
}

void StartQuickWithDraw(vector<sClient>& vclient) {
    while (true) {
        system("cls");
        ShowQuickWithdrawScreen();
        int UserChoice = ReaadQickWithdraw();
        if (UserChoice == 9) {
            break;
        }
        PerformQuickWithdraw(UserChoice, vclient);
        BackToMainMenu();
        break;
    }
}

void ShowCheckBalanceScreen() {
    cout << "---------------------------------------------------------------\n";
    cout << "\n\t\t++ Check Balance ++\n\n";
    cout << "---------------------------------------------------------------\n";
    cout << "Your Balance Is: " << Client.AccountBalance << endl;
}

void PerformMainMenuScreen(MainMenu UserChoice, vector<sClient>& vClient, bool& LoggedOut) {
    switch (UserChoice) {
    case QuickWithdraws:
        system("cls");
        StartQuickWithDraw(vClient);
        break;
    case NormalWithdraw:
        system("cls");
        With_draw(vClient);
        BackToMainMenu();
        break;
    case Depoist:
        system("cls");
        Deposit(vClient);
        BackToMainMenu();
        break;
    case CheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        BackToMainMenu();
        break;
    case Logouts:
        Login();
        break;
    }
}

void StartATM() {
    bool LoggedOut = false;
    while (!LoggedOut) {
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        system("cls");
        ShowATMMainMenu();
        MainMenu UserChoice = ReadMainMenuChoice();
        PerformMainMenuScreen(UserChoice, vClients, LoggedOut);
    }
}

bool Login()
{
    string AccNum;
    string PinCode;
    int LoginAttempts = 0;
    while (LoginAttempts < 3)
    {
        system("cls");
        cout << "----------------------------------------------\n";
        cout << "\t\tLogin Screen\n";
        cout << "----------------------------------------------\n";
        cout << "Enter Account Number: ";
        cin >> AccNum;
        cout << "Enter Account Pin Code: ";
        cin >> PinCode;
        if (LoadClientInfo(AccNum, PinCode))
        {
            cout << "\nLogin Successful!\n";
            cout << "Welcome " << Client.Name << "!\n";
            system("pause");
            StartATM();
            return true;
        }
        LoginAttempts++;
        cout << "\nInvalid Username or Password!\n";
        cout << "Attempts Remaining: " << 3 - LoginAttempts << endl;
        system("pause");
    }
    cout << "\nYou Failed To Login 3 Times.\n";
    cout << "Program Will Exit.\n";
    system("pause");
    return false;
}

int main()
{
    
     Login();
     
    return 0;
}